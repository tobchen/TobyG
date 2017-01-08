/*
 * basic.c
 *
 *  Created on: Nov 16, 2016
 *      Author: tobchen
 */

#include "math.h"

#include <SDL2/SDL.h>
#include <math.h>

#define TOBYG_SRC_GFX_MATH_EPSILON 0.00001f

static double cot(double a);

int TobyG_SetMat4Identity(GLfloat matrix[16]) {
	if (NULL == matrix) {
		return -1;
	}

	matrix[0] = 1;matrix[5] = 1;matrix[10] = 1;matrix[15] = 1;
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = matrix[6] = matrix[7] =
			matrix[8] = matrix[9] = matrix[11] = matrix[12] = matrix[13] =
			matrix[14] = 0;

	return 0;
}

int TobyG_MultMat4Vec4(GLfloat matrix[16], GLfloat vector[4], GLfloat result[4]) {
	size_t i, j;

	if (NULL == matrix || NULL == vector || NULL == result) {
		return -1;
	}

	for (i = 0; i < 4; ++i) {
		result[i] = 0;
		for (j = 0; j < 4; ++j) {
			result[i] += matrix[i + j * 4] * vector[j];
		}
	}

	return 0;
}

int TobyG_MultMat4Mat4(GLfloat factor1[16], GLfloat factor2[16],
		GLfloat product[16]) {
	size_t i;

	if (NULL == factor1 || NULL == factor2 || NULL == product) {
		return -1;
	}

	for (i = 0; i < 4; i++) { /* row */
		size_t j;
		for (j = 0; j < 4; j++) { /* column */
			size_t k;
			product[i+(j*4)] = 0;
			for (k = 0; k < 4; k++) {
				product[i+(j*4)] += (factor1[i+(k*4)] * factor2[k+(j*4)]);
			}
		}
	}

	return 0;
}

int TobyG_InvertMat4(GLfloat matrix[16], GLfloat inverse[16]) {
	GLfloat factor;
	GLfloat determinant;

	if (NULL == matrix || NULL == inverse) {
		return -1;
	}

	/* TODO This must be optimizable... see http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix */

	/*
	 * Algorithm from:
	 * http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
	 * Thanks!
	 */

	/*
	 * Determinant calculation with indices like linked webpage:
     *    matrix[11]*matrix[22]*matrix[33]*matrix[44] + matrix[11]*matrix[23]*matrix[34]*matrix[42] + matrix[11]*matrix[24]*matrix[32]*matrix[43]
     *  + matrix[12]*matrix[21]*matrix[34]*matrix[43] + matrix[12]*matrix[23]*matrix[31]*matrix[44] + matrix[12]*matrix[24]*matrix[33]*matrix[41]
     *  + matrix[13]*matrix[21]*matrix[32]*matrix[44] + matrix[13]*matrix[22]*matrix[34]*matrix[41] + matrix[13]*matrix[24]*matrix[31]*matrix[42]
     *  + matrix[14]*matrix[21]*matrix[33]*matrix[42] + matrix[14]*matrix[22]*matrix[31]*matrix[43] + matrix[14]*matrix[23]*matrix[32]*matrix[41]
     *  - matrix[11]*matrix[22]*matrix[34]*matrix[43] - matrix[11]*matrix[23]*matrix[32]*matrix[44] - matrix[11]*matrix[24]*matrix[33]*matrix[42]
     *  - matrix[12]*matrix[21]*matrix[33]*matrix[44] - matrix[12]*matrix[23]*matrix[34]*matrix[41] - matrix[12]*matrix[24]*matrix[31]*matrix[43]
     *  - matrix[13]*matrix[21]*matrix[34]*matrix[42] - matrix[13]*matrix[22]*matrix[31]*matrix[44] - matrix[13]*matrix[24]*matrix[32]*matrix[41]
     *  - matrix[14]*matrix[21]*matrix[32]*matrix[43] - matrix[14]*matrix[22]*matrix[33]*matrix[41] - matrix[14]*matrix[23]*matrix[31]*matrix[42]
	 */
	determinant =
	        matrix[0]*matrix[5]*matrix[10]*matrix[15] + matrix[0]*matrix[9]*matrix[14]*matrix[7] + matrix[0]*matrix[13]*matrix[6]*matrix[11]
	      + matrix[4]*matrix[1]*matrix[14]*matrix[11] + matrix[4]*matrix[9]*matrix[2]*matrix[15] + matrix[4]*matrix[13]*matrix[10]*matrix[3]
	      + matrix[8]*matrix[1]*matrix[6]*matrix[15] + matrix[8]*matrix[5]*matrix[14]*matrix[3] + matrix[8]*matrix[13]*matrix[2]*matrix[7]
	      + matrix[12]*matrix[1]*matrix[10]*matrix[7] + matrix[12]*matrix[5]*matrix[2]*matrix[11] + matrix[12]*matrix[9]*matrix[6]*matrix[3]
	      - matrix[0]*matrix[5]*matrix[14]*matrix[11] - matrix[0]*matrix[9]*matrix[6]*matrix[15] - matrix[0]*matrix[13]*matrix[10]*matrix[7]
	      - matrix[4]*matrix[1]*matrix[10]*matrix[15] - matrix[4]*matrix[9]*matrix[14]*matrix[3] - matrix[4]*matrix[13]*matrix[2]*matrix[11]
	      - matrix[8]*matrix[1]*matrix[14]*matrix[7] - matrix[8]*matrix[5]*matrix[2]*matrix[15] - matrix[8]*matrix[13]*matrix[6]*matrix[3]
	      - matrix[12]*matrix[1]*matrix[6]*matrix[11] - matrix[12]*matrix[5]*matrix[10]*matrix[3] - matrix[12]*matrix[9]*matrix[2]*matrix[7];

	if (fabs(determinant) < TOBYG_SRC_GFX_MATH_EPSILON) {
		return -1;
	}

	/*
	 * Inverse calculation with indices like linked webpage:
	 * inverse[11] = matrix[22]*matrix[33]*matrix[44] + matrix[23]*matrix[34]*matrix[42] + matrix[24]*matrix[32]*matrix[43] - matrix[22]*matrix[34]*matrix[43] - matrix[23]*matrix[32]*matrix[44] - matrix[24]*matrix[33]*matrix[42]
	 * inverse[12] = matrix[12]*matrix[34]*matrix[43] + matrix[13]*matrix[32]*matrix[44] + matrix[14]*matrix[33]*matrix[42] - matrix[12]*matrix[33]*matrix[44] - matrix[13]*matrix[34]*matrix[42] - matrix[14]*matrix[32]*matrix[43]
	 * inverse[13] = matrix[12]*matrix[23]*matrix[44] + matrix[13]*matrix[24]*matrix[42] + matrix[14]*matrix[22]*matrix[43] - matrix[12]*matrix[24]*matrix[43] - matrix[13]*matrix[22]*matrix[44] - matrix[14]*matrix[23]*matrix[42]
	 * inverse[14] = matrix[12]*matrix[24]*matrix[33] + matrix[13]*matrix[22]*matrix[34] + matrix[14]*matrix[23]*matrix[32] - matrix[12]*matrix[23]*matrix[34] - matrix[13]*matrix[24]*matrix[32] - matrix[14]*matrix[22]*matrix[33]
	 * inverse[21] = matrix[21]*matrix[34]*matrix[43] + matrix[23]*matrix[31]*matrix[44] + matrix[24]*matrix[33]*matrix[41] - matrix[21]*matrix[33]*matrix[44] - matrix[23]*matrix[34]*matrix[41] - matrix[24]*matrix[31]*matrix[43]
	 * inverse[22] = matrix[11]*matrix[33]*matrix[44] + matrix[13]*matrix[34]*matrix[41] + matrix[14]*matrix[31]*matrix[43] - matrix[11]*matrix[34]*matrix[43] - matrix[13]*matrix[31]*matrix[44] - matrix[14]*matrix[33]*matrix[41]
	 * inverse[23] = matrix[11]*matrix[24]*matrix[43] + matrix[13]*matrix[21]*matrix[44] + matrix[14]*matrix[23]*matrix[41] - matrix[11]*matrix[23]*matrix[44] - matrix[13]*matrix[24]*matrix[41] - matrix[14]*matrix[21]*matrix[43]
	 * inverse[24] = matrix[11]*matrix[23]*matrix[34] + matrix[13]*matrix[24]*matrix[31] + matrix[14]*matrix[21]*matrix[33] - matrix[11]*matrix[24]*matrix[33] - matrix[13]*matrix[21]*matrix[34] - matrix[14]*matrix[23]*matrix[31]
	 * inverse[31] = matrix[21]*matrix[32]*matrix[44] + matrix[22]*matrix[34]*matrix[41] + matrix[24]*matrix[31]*matrix[42] - matrix[21]*matrix[34]*matrix[42] - matrix[22]*matrix[31]*matrix[44] - matrix[24]*matrix[32]*matrix[41]
	 * inverse[32] = matrix[11]*matrix[34]*matrix[42] + matrix[12]*matrix[31]*matrix[44] + matrix[14]*matrix[32]*matrix[41] - matrix[11]*matrix[32]*matrix[44] - matrix[12]*matrix[34]*matrix[41] - matrix[14]*matrix[31]*matrix[42]
	 * inverse[33] = matrix[11]*matrix[22]*matrix[44] + matrix[12]*matrix[24]*matrix[41] + matrix[14]*matrix[21]*matrix[42] - matrix[11]*matrix[24]*matrix[42] - matrix[12]*matrix[21]*matrix[44] - matrix[14]*matrix[22]*matrix[41]
	 * inverse[34] = matrix[11]*matrix[24]*matrix[32] + matrix[12]*matrix[21]*matrix[34] + matrix[14]*matrix[22]*matrix[31] - matrix[11]*matrix[22]*matrix[34] - matrix[12]*matrix[24]*matrix[31] - matrix[14]*matrix[21]*matrix[32]
	 * inverse[41] = matrix[21]*matrix[33]*matrix[42] + matrix[22]*matrix[31]*matrix[43] + matrix[23]*matrix[32]*matrix[41] - matrix[21]*matrix[32]*matrix[43] - matrix[22]*matrix[33]*matrix[41] - matrix[23]*matrix[31]*matrix[42]
	 * inverse[42] = matrix[11]*matrix[32]*matrix[43] + matrix[12]*matrix[33]*matrix[41] + matrix[13]*matrix[31]*matrix[42] - matrix[11]*matrix[33]*matrix[42] - matrix[12]*matrix[31]*matrix[43] - matrix[13]*matrix[32]*matrix[41]
	 * inverse[43] = matrix[11]*matrix[23]*matrix[42] + matrix[12]*matrix[21]*matrix[43] + matrix[13]*matrix[22]*matrix[41] - matrix[11]*matrix[22]*matrix[43] - matrix[12]*matrix[23]*matrix[41] - matrix[13]*matrix[21]*matrix[42]
	 * inverse[44] = matrix[11]*matrix[22]*matrix[33] + matrix[12]*matrix[23]*matrix[31] + matrix[13]*matrix[21]*matrix[32] - matrix[11]*matrix[23]*matrix[32] - matrix[12]*matrix[21]*matrix[33] - matrix[13]*matrix[22]*matrix[31]
	 */
	factor = 1.0f / determinant;
	inverse[0] = factor * (matrix[5]*matrix[10]*matrix[15] + matrix[9]*matrix[14]*matrix[7] + matrix[13]*matrix[6]*matrix[11] - matrix[5]*matrix[14]*matrix[11] - matrix[9]*matrix[6]*matrix[15] - matrix[13]*matrix[10]*matrix[7]);
	inverse[4] = factor * (matrix[4]*matrix[14]*matrix[11] + matrix[8]*matrix[6]*matrix[15] + matrix[12]*matrix[10]*matrix[7] - matrix[4]*matrix[10]*matrix[15] - matrix[8]*matrix[14]*matrix[7] - matrix[12]*matrix[6]*matrix[11]);
	inverse[8] = factor * (matrix[4]*matrix[9]*matrix[15] + matrix[8]*matrix[13]*matrix[7] + matrix[12]*matrix[5]*matrix[11] - matrix[4]*matrix[13]*matrix[11] - matrix[8]*matrix[5]*matrix[15] - matrix[12]*matrix[9]*matrix[7]);
	inverse[12] = factor * (matrix[4]*matrix[13]*matrix[10] + matrix[8]*matrix[5]*matrix[14] + matrix[12]*matrix[9]*matrix[6] - matrix[4]*matrix[9]*matrix[14] - matrix[8]*matrix[13]*matrix[6] - matrix[12]*matrix[5]*matrix[10]);
	inverse[1] = factor * (matrix[1]*matrix[14]*matrix[11] + matrix[9]*matrix[2]*matrix[15] + matrix[13]*matrix[10]*matrix[3] - matrix[1]*matrix[10]*matrix[15] - matrix[9]*matrix[14]*matrix[3] - matrix[13]*matrix[2]*matrix[11]);
	inverse[5] = factor * (matrix[0]*matrix[10]*matrix[15] + matrix[8]*matrix[14]*matrix[3] + matrix[12]*matrix[2]*matrix[11] - matrix[0]*matrix[14]*matrix[11] - matrix[8]*matrix[2]*matrix[15] - matrix[12]*matrix[10]*matrix[3]);
	inverse[9] = factor * (matrix[0]*matrix[13]*matrix[11] + matrix[8]*matrix[1]*matrix[15] + matrix[12]*matrix[9]*matrix[3] - matrix[0]*matrix[9]*matrix[15] - matrix[8]*matrix[13]*matrix[3] - matrix[12]*matrix[1]*matrix[11]);
	inverse[13] = factor * (matrix[0]*matrix[9]*matrix[14] + matrix[8]*matrix[13]*matrix[2] + matrix[12]*matrix[1]*matrix[10] - matrix[0]*matrix[13]*matrix[10] - matrix[8]*matrix[1]*matrix[14] - matrix[12]*matrix[9]*matrix[2]);
	inverse[2] = factor * (matrix[1]*matrix[6]*matrix[15] + matrix[5]*matrix[14]*matrix[3] + matrix[13]*matrix[2]*matrix[7] - matrix[1]*matrix[14]*matrix[7] - matrix[5]*matrix[2]*matrix[15] - matrix[13]*matrix[6]*matrix[3]);
	inverse[6] = factor * (matrix[0]*matrix[14]*matrix[7] + matrix[4]*matrix[2]*matrix[15] + matrix[12]*matrix[6]*matrix[3] - matrix[0]*matrix[6]*matrix[15] - matrix[4]*matrix[14]*matrix[3] - matrix[12]*matrix[2]*matrix[7]);
	inverse[10] = factor * (matrix[0]*matrix[5]*matrix[15] + matrix[4]*matrix[13]*matrix[3] + matrix[12]*matrix[1]*matrix[7] - matrix[0]*matrix[13]*matrix[7] - matrix[4]*matrix[1]*matrix[15] - matrix[12]*matrix[5]*matrix[3]);
	inverse[14] = factor * (matrix[0]*matrix[13]*matrix[6] + matrix[4]*matrix[1]*matrix[14] + matrix[12]*matrix[5]*matrix[2] - matrix[0]*matrix[5]*matrix[14] - matrix[4]*matrix[13]*matrix[2] - matrix[12]*matrix[1]*matrix[6]);
	inverse[3] = factor * (matrix[1]*matrix[10]*matrix[7] + matrix[5]*matrix[2]*matrix[11] + matrix[9]*matrix[6]*matrix[3] - matrix[1]*matrix[6]*matrix[11] - matrix[5]*matrix[10]*matrix[3] - matrix[9]*matrix[2]*matrix[7]);
	inverse[7] = factor * (matrix[0]*matrix[6]*matrix[11] + matrix[4]*matrix[10]*matrix[3] + matrix[8]*matrix[2]*matrix[7] - matrix[0]*matrix[10]*matrix[7] - matrix[4]*matrix[2]*matrix[11] - matrix[8]*matrix[6]*matrix[3]);
	inverse[11] = factor * (matrix[0]*matrix[9]*matrix[7] + matrix[4]*matrix[1]*matrix[11] + matrix[8]*matrix[5]*matrix[3] - matrix[0]*matrix[5]*matrix[11] - matrix[4]*matrix[9]*matrix[3] - matrix[8]*matrix[1]*matrix[7]);
	inverse[15] = factor * (matrix[0]*matrix[5]*matrix[10] + matrix[4]*matrix[9]*matrix[2] + matrix[8]*matrix[1]*matrix[6] - matrix[0]*matrix[9]*matrix[6] - matrix[4]*matrix[1]*matrix[10] - matrix[8]*matrix[5]*matrix[2]);

	return 0;
}

int TobyG_InterpolateMat4(GLfloat matrix1[16], GLfloat matrix2[16], GLfloat delta,
		GLfloat result[16]) {
	size_t i;

	if (NULL == matrix1 || NULL == matrix2 || NULL == result) {
		return -1;
	}

	for (i = 0; i < 16; ++i) {
		result[i] = matrix1[i] + delta * (matrix2[i] - matrix1[i]);
	}

	return 0;
}

int TobyG_PrintMat4(GLfloat matrix[16]) {
	if (NULL == matrix) {
		return -1;
	}

	SDL_Log("[%f, %f, %f, %f,", matrix[0], matrix[4], matrix[8], matrix[12]);
	SDL_Log("%f, %f, %f, %f,", matrix[1], matrix[5], matrix[9], matrix[13]);
	SDL_Log("%f, %f, %f, %f,", matrix[2], matrix[6], matrix[10], matrix[14]);
	SDL_Log("%f, %f, %f, %f]", matrix[3], matrix[7], matrix[11], matrix[15]);

	return 0;
}

int TobyG_SetMat4RotationXYZ(double x, double y, double z, GLfloat matrix[16]) {
	GLfloat a, b, c, d, e, f;

	if (NULL == matrix) {
		return -1;
	}

	/*
	 * Calculated by Wolfram Alpha:
	 * http://www.wolframalpha.com/input/?i={{e%2C-f%2C0%2C0}%2C{f%2Ce%2C0%2C0}%2C{0%2C0%2C1%2C0}%2C{0%2C0%2C0%2C1}}+*+{{c%2C0%2Cd%2C0}%2C{0%2C1%2C0%2C0}%2C{-d%2C0%2Cc%2C0}%2C{0%2C0%2C0%2C1}}+*+{{1%2C0%2C0%2C0}%2C{0%2Ca%2C-b%2C0}%2C{0%2Cb%2Ca%2C0}%2C{0%2C0%2C0%2C1}}
	 */

	a = cos(x);
	b = sin(x);
	c = cos(y);
	d = sin(y);
	e = cos(z);
	f = sin(z);

	matrix[0] = c*e;
	matrix[1] = c*f;
	matrix[2] = -d;
	matrix[3] = 0;
	matrix[4] = b*d*e - a*f;
	matrix[5] = a*e + b*d*f;
	matrix[6] = b*c;
	matrix[7] = 0;
	matrix[8] = a*d*e + b*f;
	matrix[9] = a*d*f - b*e;
	matrix[10] = a*c;
	matrix[11] = 0;
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;

	return 0;
}

int TobyG_SetMat4RotationZYX(double x, double y, double z, GLfloat matrix[16]) {
	GLfloat a, b, c, d, e, f;

	if (NULL == matrix) {
		return -1;
	}

	/*
	 * Calculated by Wolfram Alpha:
	 * http://www.wolframalpha.com/input/?i=%7B%7B1,0,0,0%7D,%7B0,a,-b,0%7D,%7B0,b,a,0%7D,%7B0,0,0,1%7D%7D+*+%7B%7Bc,0,d,0%7D,%7B0,1,0,0%7D,%7B-d,0,c,0%7D,%7B0,0,0,1%7D%7D+*+%7B%7Be,-f,0,0%7D,%7Bf,e,0,0%7D,%7B0,0,1,0%7D,%7B0,0,0,1%7D%7D
	 */

	a = cos(x);
	b = sin(x);
	c = cos(y);
	d = sin(y);
	e = cos(z);
	f = sin(z);

	matrix[0] = c*e;
	matrix[1] = b*d*e + a*f;
	matrix[2] = b*f - a*d*e;
	matrix[3] = 0;
	matrix[4] = -c*f;
	matrix[5] = a*e - b*d*f;
	matrix[6] = b*e + a*d*f;
	matrix[7] = 0;
	matrix[8] = d;
	matrix[9] = -b*c;
	matrix[10] = a*c;
	matrix[11] = 0;
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;

	return 0;
}

/* https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml */
int TobyG_SetMat4Perspective(double fovy, GLfloat aspect, GLfloat near,
		GLfloat far, GLfloat matrix[16]) {
	GLfloat f;

	if (NULL == matrix) {
		return -1;
	}

	f = cot(fovy / 2.0);

	matrix[1] = matrix[2] = matrix[3] = matrix[4] = matrix[6] = matrix[7] =
			matrix[8] = matrix[9] = matrix[12] = matrix[13] = matrix[15] = 0;
	matrix[0] = f / aspect;
	matrix[5] = f;
	matrix[10] = (far + near) / (near - far);
	matrix[11] = -1.0f;
	matrix[14] = (2.0f * far * near) / (near - far);

	return 0;
}

/* http://www.wolframalpha.com/input/?i=%7B%7Bc%2Fa,0,0,0%7D,%7B0,c,0,0%7D,%7B0,0,(f%2Bn)%2F(n-f),(2*f*n)%2F(n-f)%7D,%7B0,0,-1,0%7D%7D+*+%7Bx,+y,+z,+1%7D */
int TobyG_SetMat4InversePerspective(double fovy, GLfloat aspect, GLfloat near,
		GLfloat far, GLfloat matrix[16]) {
	GLfloat f;

	if (NULL == matrix) {
		return -1;
	}

	f = cot(fovy / 2.0);

	matrix[1] = matrix[2] = matrix[3] = matrix[4] = matrix[6] = matrix[7] =
			matrix[8] = matrix[9] = matrix[10] = matrix[12] = matrix[13] = 0;
	matrix[0] = aspect / f;
	matrix[5] = 1.0f / f;
	matrix[11] = (near - far) / (2.0f * far * near);
	matrix[14] = -1.0f;
	matrix[15] = (far + near) / (2.0f * far * near);

	return 0;
}

double cot(double a) {
	return cos(a) / sin(a);
}

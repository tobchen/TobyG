/*
 * math.h
 *
 *  Created on: Nov 25, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_SRC_GFX_MATH_H
#define TOBYG_SRC_GFX_MATH_H

#include <GL/glew.h>

/*!
 * \brief Set the identity 4x4 matrix.
 * @param[out] matrix 4x4 matrix
 * @return -1 on error, 0 on success
 */
int TobyG_SetMat4Identity(GLfloat matrix[16]);

/*!
 * \brief Multiply a 4x4 matrix with a 4 vector.
 * @param[in] matrix 4x4 matrix
 * @param[in] vector 4 vector
 * @param[out] result 4 vector, may not reference either input
 * @return -1 on error, 0 on success
 */
int TobyG_MultMat4Vec4(GLfloat matrix[16], GLfloat vector[4], GLfloat result[4]);
/*!
 * \brief Multiplay two 4x4 matrices.
 * @param[in] factor1 first 4x4 matrix
 * @param[in] factor2 second 4x4 matrix
 * @param[out] product 4x4 matrix, may not reference either input
 * @return -1 on error, 0 on success
 */
int TobyG_MultMat4Mat4(GLfloat factor1[16], GLfloat factor2[16],
		GLfloat product[16]);

/*!
 * \brief Invert a 4x4 matrix.
 * @param[in] matrix 4x4 matrix
 * @param[out] inverse inverted 4x4 matrix, may not reference input
 * @return -1 on error, 0 on success
 */
int TobyG_InvertMat4(GLfloat matrix[16], GLfloat inverse[16]);

/*!
 * \brief Linearly interpolate two 4x4 matrices.
 * @param[in] matrix1 4x4 matrix
 * @param[in] matrix2 4x4 matrix
 * @param[in] delta float from 0.0 to 1.0
 * @param[out] result interpolated 4x4 matrix, may not reference either input
 * @return -1 on error, 0 on success
 */
int TobyG_InterpolateMat4(GLfloat matrix1[16], GLfloat matrix2[16], GLfloat delta,
		GLfloat result[16]);

/*!
 * \brief Print a matrix to the console.
 * @param matrix 4x4 matrix
 * @return -1 on error, 0 on success
 */
int TobyG_PrintMat4(GLfloat matrix[16]);

/**
 * \brief Set a rotation matrix with first x, then y and finally z rotation
 * applied.
 * @param[in] x rotation around x axis in radiant
 * @param[in] y rotation around y axis in radiant
 * @param[in] z rotation around z axis in radiant
 * @param[out] matrix 4x4 matrix
 * @return -1 on error, 0 on success
 */
int TobyG_SetMat4RotationXYZ(double x, double y, double z, GLfloat matrix[16]);

/**
 * \brief Set a rotation matrix with first z, then y and finally x rotation
 * applied.
 * @param[in] x rotation around x axis in radiant
 * @param[in] y rotation around y axis in radiant
 * @param[in] z rotation around z axis in radiant
 * @param[out] matrix 4x4 matrix
 * @return -1 on error, 0 on success
 */
int TobyG_SetMat4RotationZYX(double x, double y, double z, GLfloat matrix[16]);

/*!
 * \brief Set a 4x4 perspective matrix.
 * @param[in] fovy field of view in radiant
 * @param[in] aspect aspect ratio
 * @param[in] near nearest clipping distance
 * @param[in] far farthest clipping distance
 * @param[out] matrix 4x4 matrix
 * @return -1 on error, 0 on success
 */
int TobyG_SetMat4Perspective(double fovy, GLfloat aspect, GLfloat near,
		GLfloat far, GLfloat matrix[16]);

/*!
 * \brief Set a 4x4 inverse perspective matrix.
 * @param[in] fovy field of view in radiant
 * @param[in] aspect aspect ratio
 * @param[in] near nearest clipping distance
 * @param[in] far farthest clipping distance
 * @param[out] matrix 4x4 matrix
 * @return -1 on error, 0 on success
 */
int TobyG_SetMat4InversePerspective(double fovy, GLfloat aspect, GLfloat near,
		GLfloat far, GLfloat matrix[16]);

#endif /* TOBYG_SRC_GFX_MATH_H */

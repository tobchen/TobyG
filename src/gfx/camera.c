/*
 * camera3d.c
 *
 *  Created on: Nov 19, 2016
 *      Author: tobchen
 */

#include "camera.h"

#include <SDL2/SDL.h>

#include "../../include/tobyg_const.h"

#include "math.h"
#include "gfx.h"

#define TOBYG_SRC_GFX_CAMERA_DEFAULT_FOV 0.7853981633974483
#define TOBYG_SRC_GFX_CAMERA_DEFAULT_NEAR 0.1f
#define TOBYG_SRC_GFX_CAMERA_DEFAULT_FAR 100.0f

GLfloat posMatrix[16];

double rotX, rotY, rotZ;
GLfloat rotMatrix[16];
SDL_bool rotUpdated;

GLfloat viewMatrix[16];
SDL_bool viewUpdated;

GLfloat projMatrix[16];

GLfloat viewProjMatrix[16];
SDL_bool viewProjUpdated;

int TobyG_SetCameraPerspective(double fov, GLfloat near, GLfloat far) {
	TobyG_SetMat4Perspective(fov,
			(GLfloat) TOBYG_CONST_RES_WIDTH / (GLfloat) TOBYG_CONST_RES_HEIGHT,
			near, far, projMatrix);
	viewProjUpdated = SDL_FALSE;

	return 0;
}

int TobyG_SetCameraPosition(GLfloat x, GLfloat y, GLfloat z) {
	posMatrix[12] = -x;
	posMatrix[13] = -y;
	posMatrix[14] = -z;
	viewUpdated = viewProjUpdated = SDL_FALSE;

	return 0;
}

int TobyG_SetCameraRotation(double x, double y, double z) {
	rotX = x;
	rotY = y;
	rotZ = z;
	viewUpdated = rotUpdated = viewProjUpdated = SDL_FALSE;

	return 0;
}

int _TobyG_InitCamera(void) {
	TobyG_SetCameraPerspective(TOBYG_SRC_GFX_CAMERA_DEFAULT_FOV,
			TOBYG_SRC_GFX_CAMERA_DEFAULT_NEAR, TOBYG_SRC_GFX_CAMERA_DEFAULT_FAR);
	TobyG_SetMat4Identity(posMatrix);
	TobyG_SetCameraRotation(0, 0, 0);

	rotUpdated = viewUpdated = viewProjUpdated = SDL_FALSE;

	return 0;
}

void _TobyG_DestroyCamera(void) {
	/* Well, there's actually zero stuff to destroy */
}

GLfloat* _TobyG_GetCameraMatrix(void) {
	if (!viewProjUpdated) {
		if (!viewUpdated) {
			if (!rotUpdated) {
				TobyG_SetMat4RotationZYX( -rotX, -rotY, -rotZ, rotMatrix);
				rotUpdated = SDL_TRUE;
			}

			TobyG_MultMat4Mat4(rotMatrix, posMatrix, viewMatrix);
			viewUpdated = SDL_TRUE;
		}

		TobyG_MultMat4Mat4(projMatrix, viewMatrix, viewProjMatrix);
		viewProjUpdated = SDL_TRUE;
	}

	return viewProjMatrix;
}

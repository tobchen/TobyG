/*
 * instance3d.c
 *
 *  Created on: Nov 18, 2016
 *      Author: tobchen
 */

#include "instance.h"

#include <SDL2/SDL.h>

#include "math.h"

struct tobyg_instance {
	GLfloat posX, posY, posZ;
	double rotX, rotY, rotZ;
	GLfloat rotMatrix[16];
	SDL_bool rotUpdated;
	GLfloat scaleMatrix[16];

	SDL_bool updated;
	GLfloat matrix[16];
};

TobyG_Instance* TobyG_CreateInstance() {
	TobyG_Instance* result = malloc(sizeof(TobyG_Instance));
	if (NULL == result) {
		return NULL;
	}

	TobyG_SetInstancePosition(result, 0, 0, 0);
	TobyG_SetInstanceRotation(result, 0, 0, 0);
	TobyG_SetMat4Identity(result->scaleMatrix);
	result->updated = SDL_FALSE;

	return result;
}

void TobyG_FreeInstance(TobyG_Instance* instance) {
	if (NULL == instance) {
		return;
	}
	free(instance);
}

int TobyG_SetInstancePosition(TobyG_Instance* instance,
		GLfloat x, GLfloat y, GLfloat z) {
	if (NULL == instance) {
		return -1;
	}

	instance->posX = x;
	instance->posY = y;
	instance->posZ = z;
	instance->updated = SDL_FALSE;

	return 0;
}
int TobyG_SetInstanceRotation(TobyG_Instance* instance,
		double x, double y, double z) {
	if (NULL == instance) {
		return -1;
	}

	instance->rotX = x;
	instance->rotY = y;
	instance->rotZ = z;
	instance->rotUpdated = instance->updated = SDL_FALSE;

	return 0;
}

int TobyG_SetInstanceScale(TobyG_Instance* instance,
		GLfloat x, GLfloat y, GLfloat z) {
	if (NULL == instance) {
		return -1;
	}

	instance->scaleMatrix[0] = x;
	instance->scaleMatrix[5] = y;
	instance->scaleMatrix[10] = z;
	instance->updated = SDL_FALSE;

	return 0;
}

GLfloat* _TobyG_GetInstanceMatrix(TobyG_Instance* instance) {
	if (NULL == instance) {
		return NULL;
	}

	if (!instance->updated) {
		if (!instance->rotUpdated) {
			TobyG_SetMat4RotationXYZ(instance->rotX, instance->rotY,
					instance->rotZ, instance->rotMatrix);
			instance->rotUpdated = SDL_TRUE;
		}

		TobyG_MultMat4Mat4(instance->rotMatrix, instance->scaleMatrix,
				instance->matrix);
		instance->matrix[12] = instance->posX;
		instance->matrix[13] = instance->posY;
		instance->matrix[14] = instance->posZ;
		instance->updated = SDL_TRUE;
	}

	return instance->matrix;
}

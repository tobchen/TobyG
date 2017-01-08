/*
 * mesh.c
 *
 *  Created on: Nov 17, 2016
 *      Author: tobchen
 */

#include "mesh.h"

#include <SDL2/SDL.h>

#include "renderer.h"
#include "gl.h"
#include "instance.h"
#include "camera.h"
#include "math.h"

struct tobyg_mesh {
	GLubyte frameCount;
	GLfloat* frameTimes;

	GLushort vertexCount;
	GLuint vertexBuffer;

	GLuint indexCount;
	GLuint indexBuffer;
};

TobyG_Mesh* currentMesh;

TobyG_Mesh* TobyG_ReadMesh(const char* path) {
	/* TODO Handle zero frameCount */
	TobyG_Mesh* result;
	SDL_RWops* file;
	Uint32 version;
	GLfloat* vertexData;
	GLushort triangleCount;
	GLushort* trianglesRaw;
	size_t i;

	result = malloc(sizeof(TobyG_Mesh));
	if (NULL == result) {
		return NULL;
	}

	file = SDL_RWFromFile(path, "rb");
	if (NULL == file) {
		free(result);
		return NULL;
	}

	version = SDL_ReadBE32(file);
	if (version != 1) {
		/* TODO Yeah, tough shit */
	}

	/* Frames */
	SDL_RWread(file, &(result->frameCount), 1, 1);
	result->frameTimes = malloc(sizeof(GLfloat) * result->frameCount);
	for (i = 0; i < result->frameCount; ++i) {
		Uint32 time = SDL_ReadBE32(file);
		memcpy(result->frameTimes + i, &time, 4);
	}

	/* Vertices */
	result->vertexCount = SDL_ReadBE16(file);
	vertexData = malloc(sizeof(GLfloat) * result->vertexCount * (2 + 3 * result->frameCount));
	for (i = 0; i < (size_t) result->vertexCount * (2 + 3 * result->frameCount); ++i) {
		Uint32 data = SDL_ReadBE32(file);
		memcpy(vertexData + i, &data, 4);
	}

	/* Triangles */
	triangleCount = SDL_ReadBE16(file);
	trianglesRaw = malloc(sizeof(GLushort) * triangleCount * 3);
	for (i = 0; i < (size_t) triangleCount * 3; ++i) {
		trianglesRaw[i] = SDL_ReadBE16(file);
	}

	SDL_RWclose(file);

	/* TODO Handle zero buffers */
	result->vertexBuffer = _TobyG_CreateBuffer(GL_ARRAY_BUFFER,
			sizeof(GLfloat) * result->vertexCount *	(2 + 3 * result->frameCount),
			vertexData, GL_STATIC_DRAW);
	free(vertexData);

	result->indexCount = triangleCount * 3;
	result->indexBuffer = _TobyG_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(GLushort) * result->indexCount, trianglesRaw, GL_STATIC_DRAW);
	free(trianglesRaw);

	return result;
}

void TobyG_FreeMesh(TobyG_Mesh* mesh) {
	if (NULL == mesh) {
		return;
	}

	free(mesh->frameTimes);
	glDeleteBuffers(1, &mesh->vertexBuffer);
	glDeleteBuffers(1, &mesh->indexBuffer);

	free(mesh);
}

int TobyG_SetMesh(TobyG_Mesh* mesh) {
	if (NULL == mesh) {
		return -1;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);

	currentMesh = mesh;

	return 0;
}

int TobyG_DrawMeshAt(TobyG_Instance* instance, GLfloat time) {
	GLint uniformMVP, uniformDelta;
	GLint attribXYZ1, attribXYZ2, attribUV;
	GLfloat* matrixVP;
	GLfloat* matrixM;
	GLfloat matrixMVP[16];
	GLubyte frame1, frame2;
	GLfloat delta;

	if (NULL == instance || NULL == currentMesh
			|| _TobyG_GetCurrentMeshRendererGLSLInfo(&uniformMVP, &uniformDelta,
					&attribXYZ1, &attribXYZ2, &attribUV)) {
		return -1;
	}

	/* Set MVP */
	matrixVP = _TobyG_GetCameraMatrix();
	matrixM = _TobyG_GetInstanceMatrix(instance);
	if (NULL == matrixVP || NULL == matrixM) {
		return -1;
	}
	TobyG_MultMat4Mat4(matrixVP, matrixM, matrixMVP);
	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, matrixMVP);

	/* Set delta */
	if (currentMesh->frameCount == 1 || time <= currentMesh->frameTimes[0]) {
		frame1 = frame2 = 0;
		delta = 0.0f;
	} else {
		for (frame1 = 1; frame1 < currentMesh->frameCount; ++frame1) {
			if (time <= currentMesh->frameTimes[frame1]) {
				frame2 = frame1;
				frame1 = frame2 - 1;
				delta = (time - currentMesh->frameTimes[frame1]) /
						(currentMesh->frameTimes[frame2] - currentMesh->frameTimes[frame1]);
				break;
			}
		}
		if (frame1 >= currentMesh->frameCount) {
			frame1 = frame2 = currentMesh->frameCount - 1;
			delta = 0.0f;
		}
	}
	glUniform1f(uniformDelta, delta);

	/* Set buffers */
	glVertexAttribPointer(attribUV, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(attribXYZ1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) (sizeof(GLfloat) * currentMesh->vertexCount * (2 + 3 * frame1)));
	glVertexAttribPointer(attribXYZ2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) (sizeof(GLfloat) * currentMesh->vertexCount * (2 + 3 * frame2)));

	glDrawElements(GL_TRIANGLES, currentMesh->indexCount, GL_UNSIGNED_SHORT, 0);

	return 0;
}

int TobyG_GetMeshFrameLimits(TobyG_Mesh* mesh, GLfloat* start, GLfloat* end) {
	if (NULL == mesh) {
		return -1;
	}
	if (start != NULL) {
		*start = mesh->frameTimes[0];
	}
	if (end != NULL) {
		*end = mesh->frameTimes[mesh->frameCount - 1];
	}
	return 0;
}

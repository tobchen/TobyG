/*
 * meshbatch.c
 *
 *  Created on: Dec 15, 2016
 *      Author: tobchen
 */

#include "meshbatch.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "renderer.h"
#include "gl.h"
#include "instance.h"
#include "camera.h"
#include "math.h"

typedef struct tobyg_mesh_batch_element {
	GLubyte nameLength;
	char* letters;
	size_t indexStart;
	size_t indexCount;
} TobyG_MeshBatchElement;

struct tobyg_mesh_batch {
	GLuint vertexBuffer;
	GLuint indexBuffer;

	GLubyte elementCount;
	TobyG_MeshBatchElement* elements;
};

TobyG_MeshBatch* currentBatch;

TobyG_MeshBatch* TobyG_ReadMeshBatch(const char* path) {
	TobyG_MeshBatch* batch;
	SDL_RWops* file;
	Uint32 version;
	GLushort vertexCount;
	GLfloat* vertexData;
	GLushort triangleCount;
	GLushort* trianglesRaw;
	size_t indexStart;
	size_t i;
	/* TODO Check maximum values for types */

	batch = malloc(sizeof(TobyG_MeshBatch));
	if (NULL == batch) {
		return NULL;
	}

	file = SDL_RWFromFile(path, "rb");
	if (NULL == file) {
		free(batch);
		return NULL;
	}

	version = SDL_ReadBE32(file);
	if (version != 1) {
		/* TODO Yeah, tough shit! */
	}

	/* Vertices */
	vertexCount = SDL_ReadBE16(file);
	vertexData = malloc(sizeof(GLfloat) * vertexCount * 5);
	for (i = 0; i < (size_t) vertexCount * 5; ++i) {
		Uint32 data = SDL_ReadBE32(file);
		memcpy(vertexData + i, &data, 4);
	}

	/* Triangles */
	triangleCount = SDL_ReadBE16(file);
	trianglesRaw = malloc(sizeof(GLushort) * triangleCount * 3);
	for (i = 0; i < (size_t) triangleCount * 3; ++i) {
		trianglesRaw[i] = SDL_ReadBE16(file);
	}

	/* Mesh elements */
	indexStart = 0;
	SDL_RWread(file, &(batch->elementCount), 1, 1);
	batch->elements = malloc(sizeof(TobyG_MeshBatchElement) * batch->elementCount);
	if (NULL == batch->elements) {
		/* TODO Error handling */
	}
	for (i = 0; i < batch->elementCount; ++i) {
		SDL_RWread(file, &(batch->elements[i].nameLength), 1, 1);
		batch->elements[i].letters = malloc(sizeof(char) * batch->elements[i].nameLength);
		if (NULL == batch->elements[i].letters) {
			/* TODO Error handling */
		}
		SDL_RWread(file, batch->elements[i].letters, 1, batch->elements[i].nameLength);
		batch->elements[i].indexCount = (size_t) SDL_ReadBE16(file) * 3;
		batch->elements[i].indexStart = indexStart;
		indexStart += batch->elements[i].indexCount;
	}

	SDL_RWclose(file);

	/* TODO Handle zero buffers */
	batch->vertexBuffer = _TobyG_CreateBuffer(GL_ARRAY_BUFFER,
			sizeof(GLfloat) * vertexCount * 5, vertexData, GL_STATIC_DRAW);
	free(vertexData);

	batch->indexBuffer = _TobyG_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(GLushort) * indexStart, trianglesRaw, GL_STATIC_DRAW);
	free(trianglesRaw);

	return batch;
}

void TobyG_FreeMeshBatch(TobyG_MeshBatch* batch) {
	GLubyte i;

	if (NULL == batch) {
		return;
	}

	for (i = 0; i < batch->elementCount; ++i) {
		free(batch->elements[i].letters);
	}
	free(batch->elements);
	free(batch);
}

size_t TobyG_GetMeshBatchIndex(TobyG_MeshBatch* batch, const char* name) {
	size_t i;

	if (NULL == batch || NULL == name) {
		return 256;
	}

	for (i = 0; i < batch->elementCount; ++i) {
		GLubyte j;
		SDL_bool potential = SDL_TRUE;
		for (j = 0; j < batch->elements[i].nameLength; ++j) {
			if (0 == name[j] || batch->elements[i].letters[j] != name[j]) {
				potential = SDL_FALSE;
				break;
			}
		}
		if (potential) {
			if (0 == name[batch->elements[i].nameLength]) {
				return i;
			}
		}
	}

	return 256;
}

int TobyG_SetMeshBatch(TobyG_MeshBatch* batch) {
	glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->indexBuffer);

	currentBatch = batch;

	return 0;
}

int TobyG_DrawMeshBatchAt(TobyG_Instance* instance, size_t index) {
	GLint uniformMVP;
	GLint attribXYZ, attribUV;
	GLfloat* matrixVP;
	GLfloat* matrixM;
	GLfloat matrixMVP[16];

	if (NULL == instance || NULL == currentBatch || index >= currentBatch->elementCount
			|| _TobyG_GetCurrentMeshBatchRendererGLSLInfo(&uniformMVP, &attribXYZ, &attribUV)) {
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

	/* Set buffers */
	glVertexAttribPointer(attribXYZ, 3, GL_FLOAT, GL_FALSE, 20, 0);
	glVertexAttribPointer(attribUV, 2, GL_FLOAT, GL_FALSE, 20, (GLvoid*) 12);

	glDrawElements(GL_TRIANGLES, currentBatch->elements[index].indexCount,
			GL_UNSIGNED_SHORT, (GLvoid*) (currentBatch->elements[index].indexStart * sizeof(GLushort)));

	return 0;
}

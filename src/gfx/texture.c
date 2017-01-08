/*
 * texture.c
 *
 *  Created on: Nov 25, 2016
 *      Author: tobchen
 */

#include "texture.h"

#include <webp/decode.h>
#include <stdlib.h>

#include "../io/file.h"

struct tobyg_texture {
	GLuint texture;
};

static TobyG_Texture* defaultTexture;

/* TODO Make texture filter parameter */

TobyG_Texture* TobyG_CreateTextureRGBA(unsigned char* pixel,
		GLsizei width, GLsizei height) {
	TobyG_Texture* texture;

	if (NULL == pixel) {
		return NULL;
	}

	texture = malloc(sizeof(TobyG_Texture));
	if (NULL == texture) {
		return NULL;
	}

	/* TODO Error checks */
	glGenTextures(1, &(texture->texture));
	if (0 == texture->texture) {
		free(texture);
		return NULL;
	}
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, pixel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return texture;
}

TobyG_Texture* TobyG_ReadTextureFromWebP(const char* path) {
	TobyG_Texture* texture;
	unsigned char* rawdata;
	unsigned char* texdata;
	size_t rawsize;
	int width, height;

	if (TobyG_ReadBinaryFile(path, &rawdata, &rawsize)) {
		return NULL;
	}

	texdata = WebPDecodeRGBA(rawdata, rawsize, &width, &height);
	free(rawdata);
	if (NULL == texdata) {
		return NULL;
	}

	texture = TobyG_CreateTextureRGBA(texdata, width, height);
	free(texdata);
	if (NULL == texture) {
		return texture;
	}

	return texture;
}

void TobyG_FreeTexture(TobyG_Texture* texture) {
	if (NULL == texture) {
		return;
	}

	glDeleteTextures(1, &(texture->texture));
	free(texture);
}

int TobyG_SetTexture(TobyG_Texture* texture) {
	if (NULL == texture) {
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, texture->texture);

	return 0;
}

void TobyG_SetDefaultTexture(void) {
	TobyG_SetTexture(defaultTexture);
}

int _TobyG_InitTexture(void) {
	unsigned char defaultTexPixel[] = {255, 255, 255, 255};

	defaultTexture = TobyG_CreateTextureRGBA(defaultTexPixel, 1, 1);
	if (NULL == defaultTexture) {
		return -1;
	}

	glActiveTexture(GL_TEXTURE0);
	TobyG_SetDefaultTexture();

	return 0;
}

void _TobyG_DestroyTexture(void) {
	TobyG_FreeTexture(defaultTexture);
}

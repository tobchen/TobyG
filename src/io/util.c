/*
 * util.c
 *
 *  Created on: Apr 20, 2017
 *      Author: tobchen
 */

#include "util.h"

GLfloat TobyG_ReadFloatBE(SDL_RWops* file) {
	GLfloat result;
	Uint32 raw = SDL_ReadBE32(file);
	memcpy(&result, &raw, 4);
	return result;
}

GLint TobyG_ReadIntBE(SDL_RWops* file) {
	GLint result;
	Uint32 raw = SDL_ReadBE32(file);
	memcpy(&result, &raw, 4);
	return result;
}

GLshort TobyG_ReadShortBE(SDL_RWops* file) {
	GLshort result;
	Uint16 raw = SDL_ReadBE16(file);
	memcpy(&result, &raw, 2);
	return result;
}

GLfloat TobyG_ReadFloatLE(SDL_RWops* file) {
	GLfloat result;
	Uint32 raw = SDL_ReadLE32(file);
	memcpy(&result, &raw, 4);
	return result;
}

GLint TobyG_ReadIntLE(SDL_RWops* file) {
	GLint result;
	Uint32 raw = SDL_ReadLE32(file);
	memcpy(&result, &raw, 4);
	return result;
}

GLshort TobyG_ReadShortLE(SDL_RWops* file) {
	GLshort result;
	Uint16 raw = SDL_ReadLE16(file);
	memcpy(&result, &raw, 2);
	return result;
}

GLbyte TobyG_ReadByte(SDL_RWops* file) {
	GLbyte result;
	SDL_RWread(file, &result, 1, 1);
	return result;
}

GLubyte TobyG_ReadUByte(SDL_RWops* file) {
	GLubyte result;
	SDL_RWread(file, &result, 1, 1);
	return result;
}

char* TobyG_ReadSmallString(SDL_RWops* file) {
	char* result;
	unsigned char length;

	if (SDL_RWread(file, &length, 1, 1) < 1) {
		return NULL;
	}
	result = malloc(sizeof(char) * ((size_t) length + 1));
	if (NULL == result) {
		return NULL;
	}
	if (SDL_RWread(file, result, 1, length) < length) {
		free(result);
		return NULL;
	}
	result[length] = 0;
	SDL_Log("%s\n", result);

	return result;
}

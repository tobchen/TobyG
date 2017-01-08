/*
 * file.c
 *
 *  Created on: Nov 16, 2016
 *      Author: tobchen
 */

#include "file.h"

#include <SDL2/SDL.h>

char* TobyG_ReadTextFile(const char* path) {
	char* result;
	SDL_RWops* rw;
	Sint64 length;

	rw = SDL_RWFromFile(path, "rb");
	if (NULL == rw) {
		return NULL;
	}

	length = SDL_RWseek(rw, 0, RW_SEEK_END);
	if (length < 0) {
		SDL_RWclose(rw);
		return NULL;
	}
	if (SDL_RWseek(rw, 0, RW_SEEK_SET) != 0) {
		SDL_RWclose(rw);
		return NULL;
	}

	result = malloc(length+1);
	if (NULL == result) {
		SDL_RWclose(rw);
		return NULL;
	}

	if (SDL_RWread(rw, result, length, 1) != 1) {
		free(result);
		result = NULL;
	} else {
		result[length] = 0;
	}
	SDL_RWclose(rw);

	return result;
}

int TobyG_ReadBinaryFile(const char* path, unsigned char** data,
		size_t* length) {
	SDL_RWops* rw;
	Sint64 tmpLength;
	unsigned char* tmpData;

	rw = SDL_RWFromFile(path, "rb");
	if (NULL == rw) {
		return -1;
	}

	tmpLength = SDL_RWseek(rw, 0, RW_SEEK_END);
	if (tmpLength < 0) {
		SDL_RWclose(rw);
		return -1;
	}
	if (SDL_RWseek(rw, 0, RW_SEEK_SET) != 0) {
		SDL_RWclose(rw);
		return -1;
	}

	tmpData = malloc(tmpLength);
	if (NULL == tmpData) {
		SDL_RWclose(rw);
		return -1;
	}

	if (SDL_RWread(rw, tmpData, tmpLength, 1) != 1) {
		free(tmpData);
		return -1;
	}
	SDL_RWclose(rw);

	*length = (size_t) tmpLength;
	*data = tmpData;

	return 0;
}

/*
 * path.c
 *
 *  Created on: Dec 10, 2016
 *      Author: tobchen
 */

#include "path.h"

#include <SDL2/SDL.h>

#ifdef _WIN32
	#define TOBYG_PATH_SEPARATOR '\\'
#else
	#define TOBYG_PATH_SEPARATOR '/'
#endif

static char* basepath;
static size_t baselength;

int TobyG_StartPath(void) {
	char* tmppath;
	size_t length;

	tmppath = SDL_GetBasePath();
	if (NULL == tmppath) {
		return -1;
	}
	length = strlen(tmppath);
	basepath = malloc(sizeof(char) * (length + 4));
	if (NULL == basepath) {
		SDL_free(tmppath);
		return -1;
	}
	memcpy(basepath, tmppath, length);
	memcpy(basepath + length, "../", 4);
	SDL_free(tmppath);
	baselength = length + 3;

	return 0;
}

void TobyG_EndPath(void) {
	free(basepath);
}

char* TobyG_GetPathByDirNameExt(const char* directory, const char* name,
		const char* extension) {
	size_t dirlen, namelen, extlen;
	char* merged;

	if (NULL == directory || NULL == name || NULL == extension) {
		return NULL;
	}

	dirlen = strlen(directory);
	namelen = strlen(name);
	extlen = strlen(extension);
	merged = malloc(sizeof(char) * (baselength + dirlen + namelen + extlen + 3));
	if (NULL == merged) {
		return NULL;
	}
	memcpy(merged, basepath, baselength);
	memcpy(merged+baselength, directory, dirlen);
	merged[baselength+dirlen] = TOBYG_PATH_SEPARATOR;
	memcpy(merged+baselength+dirlen+1, name, namelen);
	merged[baselength+dirlen+namelen+1] = '.';
	memcpy(merged+baselength+dirlen+namelen+2, extension, extlen);
	merged[baselength+dirlen+namelen+extlen+2] = 0;

	return merged;
}

char* TobyG_GetPathByDirFile(const char* directory, const char* file) {
	size_t dirlen, filelen;
	char* merged;

	if (NULL == directory || NULL == file) {
		return NULL;
	}

	dirlen = strlen(directory);
	filelen = strlen(file);
	merged = malloc(sizeof(char) * (baselength + dirlen + filelen + 2));
	if (NULL == merged) {
		return NULL;
	}
	memcpy(merged, basepath, baselength);
	memcpy(merged+baselength, directory, dirlen);
	merged[baselength+dirlen] = TOBYG_PATH_SEPARATOR;
	memcpy(merged+baselength+dirlen+1, file, filelen);
	merged[baselength+dirlen+filelen+1] = 0;

	return merged;
}

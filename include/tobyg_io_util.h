/*
 * tobyg_io_util.h
 *
 *  Created on: Apr 20, 2017
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_IO_UTIL_H
#define TOBYG_INCLUDE_TOBYG_IO_UTIL_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

GLfloat TobyG_ReadFloatBE(SDL_RWops* file);
GLint TobyG_ReadIntBE(SDL_RWops* file);
GLshort TobyG_ReadShortBE(SDL_RWops* file);

GLfloat TobyG_ReadFloatLE(SDL_RWops* file);
GLint TobyG_ReadIntLE(SDL_RWops* file);
GLshort TobyG_ReadShortLE(SDL_RWops* file);

GLbyte TobyG_ReadByte(SDL_RWops* file);
GLubyte TobyG_ReadUByte(SDL_RWops* file);

char* TobyG_ReadSmallString(SDL_RWops* file);

#endif /* TOBYG_INCLUDE_TOBYG_IO_UTIL_H */

/*
 * gl.c
 *
 *  Created on: Nov 24, 2016
 *      Author: tobchen
 */

#include "gl.h"

#include <SDL2/SDL.h>

#define TOBYG_SRC_GFX_GL_LOG_LEN 255

GLuint _TobyG_CreateBuffer(GLenum target, GLsizeiptr size, const GLvoid* data,
		GLenum usage) {
	GLuint result;
	glGenBuffers(1, &result);
	glBindBuffer(target, result);
	glBufferData(target, size, data, usage);
	return result;
}

GLuint _TobyG_CreateShader(int logID, const char* source, GLenum type) {
	GLuint shader;
	char log[TOBYG_SRC_GFX_GL_LOG_LEN];
	GLint success;

	if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
		return 0;
	}

	shader = glCreateShader(type);
	if (0 == shader) {
		return 0;
	}

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		glGetShaderInfoLog(shader, TOBYG_SRC_GFX_GL_LOG_LEN, NULL, log);
		SDL_Log("Error with shader %d: %s", logID, log);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

GLuint _TobyG_CreateProgram(int logID, GLuint vertexShader,
		GLuint fragmentShader) {
	GLint success;
	char log[TOBYG_SRC_GFX_GL_LOG_LEN];
	GLuint program;

	program = glCreateProgram();
	if (0 == program) {
		return 0;
	}
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		glGetShaderInfoLog(program, TOBYG_SRC_GFX_GL_LOG_LEN, NULL, log);
		SDL_Log("Error with program %d: %s", logID, log);
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

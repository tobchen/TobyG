/*
 * basic.c
 *
 *  Created on: Nov 16, 2016
 *      Author: tobchen
 */

#include "gfx.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "../../include/tobyg_const.h"

#include "gl.h"
#include "texture.h"
#include "camera.h"

#define TOBYG_SRC_GFX_GFX_TEX_SIZE 256

static SDL_Window* window = NULL;
static int windowWidth, windowHeight;
static SDL_GLContext context = NULL;

static GLuint virtualTexture;
static GLuint virtualDepth, virtualFrame;
static GLuint virtualVertexBuffer, virtualIndexBuffer;
static GLuint virtualProgram;
static GLint virtualUniformTexture;
static GLint virtualAttribXYZ, virtualAttribUV;
static const char* virtualVertexSource =
		"#version 120\n"
		"attribute vec3 xyz;"
		"attribute vec2 uv;"
		"varying vec2 uv_var;"
		"void main() {"
			"uv_var = uv;"
			"gl_Position = vec4(xyz, 1);"
		"}";
static const char* virtualFragmentSource =
		"#version 120\n"
		"uniform sampler2D tex;"
		"varying vec2 uv_var;"
		"void main() {"
			"gl_FragColor = texture2D(tex, uv_var);"
		"}";

static int initVirtualResolution();
static void destroyVirtualResolution(void);
static int initSubSystems(void);
static void destroySubSystems(void);

int TobyG_StartTobyG(const char* title, int width, int height) {
	if (NULL == title) {
		title = "Tobchen.de Advertisement";
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (NULL == window) {
		return -1;
	}
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	context = SDL_GL_CreateContext(window);
	if (NULL == context) {
		SDL_DestroyWindow(window);
		return -1;
	}

	if (glewInit() != GLEW_OK) {
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		return -1;
	}

	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 0.0);

	if (initVirtualResolution()) {
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		return -1;
	}

	if (initSubSystems()) {
		destroyVirtualResolution();
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		return -1;
	}

	return 0;
}

void TobyG_EndTobyG(void) {
	destroySubSystems();
	destroyVirtualResolution();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = context = NULL;
}

void TobyG_StartGraphics() {
	glBindFramebuffer(GL_FRAMEBUFFER, virtualFrame);
	glViewport(0, 0, TOBYG_CONST_RES_WIDTH, TOBYG_CONST_RES_HEIGHT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TobyG_EndGraphics() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight);

	glUseProgram(virtualProgram);

	glBindTexture(GL_TEXTURE_2D, virtualTexture);
	glUniform1i(virtualUniformTexture, 0);

	glEnableVertexAttribArray(virtualAttribXYZ);
	glEnableVertexAttribArray(virtualAttribUV);

	glBindBuffer(GL_ARRAY_BUFFER, virtualVertexBuffer);
	glVertexAttribPointer(virtualAttribUV, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(virtualAttribXYZ, 3, GL_FLOAT, GL_FALSE, 0,
			(GLvoid*) (sizeof(GLfloat) * 8));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, virtualIndexBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(virtualAttribUV);
	glDisableVertexAttribArray(virtualAttribXYZ);

	SDL_GL_SwapWindow(window);
}

int initVirtualResolution() {
	GLfloat x, y;
	GLfloat vertexData[20];
	GLushort indexData[6];
	GLuint vertexShader, fragmentShader;

	/* Shader */
	vertexShader = _TobyG_CreateShader(-5, virtualVertexSource, GL_VERTEX_SHADER);
	if (0 == vertexShader) {
		return -1;
	}
	fragmentShader = _TobyG_CreateShader(-6, virtualFragmentSource, GL_FRAGMENT_SHADER);
	if (0 == fragmentShader) {
		glDeleteShader(vertexShader);
		return -1;
	}
	virtualProgram = _TobyG_CreateProgram(-7, vertexShader, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (0 == virtualProgram) {
		return -1;
	}
	virtualUniformTexture = glGetUniformLocation(virtualProgram, "texture");
	virtualAttribXYZ = glGetAttribLocation(virtualProgram, "xyz");
	virtualAttribUV = glGetAttribLocation(virtualProgram, "uv");

	/* Texture */
	glGenTextures(1, &virtualTexture);
	if (0 == virtualTexture) {
		goto free_program;
	}
	glBindTexture(GL_TEXTURE_2D, virtualTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			TOBYG_SRC_GFX_GFX_TEX_SIZE, TOBYG_SRC_GFX_GFX_TEX_SIZE,
			0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	/* Depth buffer */
	glGenRenderbuffers(1, &virtualDepth);
	if (0 == virtualDepth) {
		goto free_texture;
	}
	glBindRenderbuffer(GL_RENDERBUFFER, virtualDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			TOBYG_SRC_GFX_GFX_TEX_SIZE, TOBYG_SRC_GFX_GFX_TEX_SIZE);
	/*glBindRenderbuffer(GL_RENDERBUFFER, 0);*/

	/* Frame buffer */
	glGenFramebuffers(1, &virtualFrame);
	if (0 == virtualFrame) {
		goto free_depth;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, virtualFrame);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			virtualTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, virtualDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		goto free_frame;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Quad */
	x = 1.0f;
	y = ((float) windowWidth / TOBYG_CONST_RES_WIDTH * TOBYG_CONST_RES_HEIGHT) / windowHeight;
	if (y > x) {
		x = ((float) windowHeight / TOBYG_CONST_RES_HEIGHT * TOBYG_CONST_RES_WIDTH) / windowWidth;
		y = 1.0f;
	}
	vertexData[0] = 0.0f; vertexData[1] = 0.875f;
	vertexData[2] = 1.0f; vertexData[3] = 0.875f;
	vertexData[4] = 1.0f; vertexData[5] = 0.0f;
	vertexData[6] = 0.0f; vertexData[7] = 0.0f;
	vertexData[8] = -x; vertexData[9] = y; vertexData[10] = 0.0f;
	vertexData[11] = x; vertexData[12] = y; vertexData[13] = 0.0f;
	vertexData[14] = x; vertexData[15] = -y; vertexData[16] = 0.0f;
	vertexData[17] = -x; vertexData[18] = -y; vertexData[19] = 0.0f;
	indexData[0] = 2; indexData[1] = 1; indexData[2] = 0;
	indexData[3] = 3; indexData[4] = 2; indexData[5] = 0;
	virtualVertexBuffer = _TobyG_CreateBuffer(GL_ARRAY_BUFFER, sizeof(GLfloat) * 20, vertexData, GL_STATIC_DRAW);
	virtualIndexBuffer = _TobyG_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indexData, GL_STATIC_DRAW);

	return 0;

	free_frame:		glDeleteFramebuffers(1, &virtualFrame); virtualFrame = 0;
	free_depth:		glDeleteRenderbuffers(1, &virtualDepth); virtualDepth = 0;
	free_texture:	glDeleteTextures(1, &virtualTexture); virtualTexture = 0;
	free_program:	glDeleteProgram(virtualProgram);
	return -1;

	return 0;
}

static void destroyVirtualResolution(void) {
	if (0 == virtualTexture) {
		return;
	}
	glDeleteTextures(1, &virtualTexture);
	glDeleteFramebuffers(1, &virtualFrame);
	glDeleteRenderbuffers(1, &virtualDepth);
	glDeleteProgram(virtualProgram);
}

static int initSubSystems(void) {
	if (_TobyG_InitCamera()) {
		return -1;
	}
	if (_TobyG_InitTexture()) {
		_TobyG_DestroyCamera();
		return -1;
	}
	return 0;
}

static void destroySubSystems(void) {
	_TobyG_DestroyCamera();
	_TobyG_DestroyTexture();
}

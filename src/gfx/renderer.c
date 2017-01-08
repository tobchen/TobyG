/*
 * shader.c
 *
 *  Created on: Nov 19, 2016
 *      Author: tobchen
 */

#include "renderer.h"

#include <stdlib.h>
#include <SDL2/SDL.h>

#include "gl.h"

struct tobyg_mesh_renderer {
	GLuint program;
	GLint uniformMVP, uniformDelta, uniformTex;
	GLint attribXYZ1, attribXYZ2, attribUV;
};

struct tobyg_mesh_batch_renderer {
	GLuint program;
	GLint uniformMVP, uniformTex;
	GLint attribXYZ, attribUV;
};

static TobyG_MeshRenderer* currentMeshRenderer = NULL;
static TobyG_MeshBatchRenderer* currentMeshBatchRenderer = NULL;

/*
 * Parts of the shader from https://github.com/keijiro/Retro3D
 */
static const char* retroMeshVertexSrc =
		"#version 120\n"
		"uniform mat4 mvp;"
		"uniform float delta;"
		"attribute vec3 xyz1;"
		"attribute vec3 xyz2;"
		"attribute vec2 uv;"
		/*"varying vec3 uv_var;"*/
		"varying vec2 uv_interpol;"
		"void main() {"
		    "gl_Position = mvp * vec4(mix(xyz1, xyz2, delta), 1);"
			/*"uv_var = vec3(uv * gl_Position.w, gl_Position.w);"*/
			"uv_interpol = uv;"
		"}";
static const char* retroBatchVertexSrc =
		"#version 120\n"
		"uniform mat4 mvp;"
		"attribute vec3 xyz;"
		"attribute vec2 uv;"
		/*"varying vec3 uv_var;"*/
		"varying vec2 uv_interpol;"
		"void main() {"
		    "gl_Position = mvp * vec4(xyz, 1);"
			/*"uv_var = vec3(uv * gl_Position.w, gl_Position.w);"*/
			"uv_interpol = uv;"
		"}";
static const char* retroCommonFragmentSrc =
		"#version 120\n"
		"uniform sampler2D tex;"
		/*"varying vec3 uv_var;"*/
		"varying vec2 uv_interpol;"
		"void main() {"
			/*"vec2 uv = uv_var.xy / uv_var.z;"*/
			/*"gl_FragColor = texture2D(tex, uv);"*/
			"gl_FragColor = texture2D(tex, uv_interpol);"
		"}";

TobyG_MeshRenderer* TobyG_CreateRetroMeshRenderer() {
	GLuint vertexShader;
	GLuint fragmentShader;
	TobyG_MeshRenderer* result;

	vertexShader = _TobyG_CreateShader(-10, retroMeshVertexSrc, GL_VERTEX_SHADER);
	fragmentShader = _TobyG_CreateShader(-11, retroCommonFragmentSrc, GL_FRAGMENT_SHADER);

	result = _TobyG_CreateMeshRenderer(-3, vertexShader, fragmentShader,
			"mvp", "delta", "tex", "xyz1", "xyz2", "uv");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return result;
}

void TobyG_FreeMeshRenderer(TobyG_MeshRenderer* renderer) {
	if (NULL == renderer) {
		return;
	}

	glDeleteProgram(renderer->program);
	free(renderer);
}

int TobyG_StartMeshRenderer(TobyG_MeshRenderer* renderer) {
	if (NULL == renderer) {
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	currentMeshRenderer = renderer;
	glUseProgram(renderer->program);

	glUniform1i(renderer->uniformTex, 0);

	glEnableVertexAttribArray(renderer->attribXYZ1);
	glEnableVertexAttribArray(renderer->attribXYZ2);
	glEnableVertexAttribArray(renderer->attribUV);

	return 0;
}

void TobyG_EndMeshRenderer() {
	glDisableVertexAttribArray(currentMeshRenderer->attribUV);
	glDisableVertexAttribArray(currentMeshRenderer->attribXYZ2);
	glDisableVertexAttribArray(currentMeshRenderer->attribXYZ1);

	currentMeshRenderer = NULL;
}

TobyG_MeshBatchRenderer* TobyG_CreateRetroMeshBatchRenderer() {
	GLuint vertexShader;
	GLuint fragmentShader;
	TobyG_MeshBatchRenderer* result;

	vertexShader = _TobyG_CreateShader(-20, retroBatchVertexSrc, GL_VERTEX_SHADER);
	fragmentShader = _TobyG_CreateShader(-21, retroCommonFragmentSrc, GL_FRAGMENT_SHADER);

	result = _TobyG_CreateMeshBatchRenderer(-4, vertexShader, fragmentShader,
			"mvp", "tex", "xyz", "uv");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return result;
}

void TobyG_FreeMeshBatchRenderer(TobyG_MeshBatchRenderer* renderer) {
	if (NULL == renderer) {
		return;
	}

	glDeleteProgram(renderer->program);
	free(renderer);
}

int TobyG_StartMeshBatchRenderer(TobyG_MeshBatchRenderer* renderer) {
	if (NULL == renderer) {
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	currentMeshBatchRenderer = renderer;
	glUseProgram(renderer->program);

	glUniform1i(renderer->uniformTex, 0);

	glEnableVertexAttribArray(renderer->attribXYZ);
	glEnableVertexAttribArray(renderer->attribUV);

	return 0;
}

void TobyG_EndMeshBatchRenderer() {
	glDisableVertexAttribArray(currentMeshBatchRenderer->attribUV);
	glDisableVertexAttribArray(currentMeshBatchRenderer->attribXYZ);

	currentMeshBatchRenderer = NULL;
}

TobyG_MeshRenderer* _TobyG_CreateMeshRenderer(int logID,
		GLuint vertexShader, GLuint fragmentShader,
		const char* uniformMVP, const char* uniformDelta, const char* uniformTex,
		const char* attribXYZ1, const char* attribXYZ2, const char* attribUV) {
	TobyG_MeshRenderer* renderer;

	if (NULL == uniformMVP || NULL == uniformDelta || NULL == uniformTex
			|| NULL == attribXYZ1 || NULL == attribXYZ2 || NULL == attribUV) {
		return NULL;
	}

	renderer = malloc(sizeof(TobyG_MeshRenderer));
	if (NULL == renderer) {
		return NULL;
	}

	renderer->program = _TobyG_CreateProgram(logID, vertexShader, fragmentShader);
	if (0 == renderer->program) {
		free(renderer);
		return NULL;
	}

	renderer->uniformMVP = glGetUniformLocation(renderer->program, uniformMVP);
	renderer->uniformDelta = glGetUniformLocation(renderer->program, uniformDelta);
	renderer->uniformTex = glGetUniformLocation(renderer->program, uniformTex);
	renderer->attribXYZ1 = glGetAttribLocation(renderer->program, attribXYZ1);
	renderer->attribXYZ2 = glGetAttribLocation(renderer->program, attribXYZ2);
	renderer->attribUV = glGetAttribLocation(renderer->program, attribUV);
	/*SDL_Log("%d, %d, %d, %d, %d", renderer->uniformMVP, renderer->uniformDelta, renderer->attribXYZ1, renderer->attribXYZ2, renderer->attribUV);*/

	return renderer;
}

int _TobyG_GetCurrentMeshRendererGLSLInfo(GLint* uniformMVP, GLint* uniformDelta,
		GLint* attribXYZ1, GLint* attribXYZ2, GLint* attribUV) {
	if (NULL == currentMeshRenderer) {
		return -1;
	}

	*uniformMVP = currentMeshRenderer->uniformMVP;
	*uniformDelta = currentMeshRenderer->uniformDelta;
	*attribXYZ1 = currentMeshRenderer->attribXYZ1;
	*attribXYZ2 = currentMeshRenderer->attribXYZ2;
	*attribUV = currentMeshRenderer->attribUV;

	return 0;
}

TobyG_MeshBatchRenderer* _TobyG_CreateMeshBatchRenderer(int logID,
		GLuint vertexShader, GLuint fragmentShader,
		const char* uniformMVP, const char* uniformTex,
		const char* attribXYZ, const char* attribUV) {
	TobyG_MeshBatchRenderer* renderer;

	if (NULL == uniformMVP || NULL == uniformTex
			|| NULL == attribXYZ || NULL == attribUV) {
		return NULL;
	}

	renderer = malloc(sizeof(TobyG_MeshRenderer));
	if (NULL == renderer) {
		return NULL;
	}

	renderer->program = _TobyG_CreateProgram(logID, vertexShader, fragmentShader);
	if (0 == renderer->program) {
		free(renderer);
		return NULL;
	}

	renderer->uniformMVP = glGetUniformLocation(renderer->program, uniformMVP);
	renderer->uniformTex = glGetUniformLocation(renderer->program, uniformTex);
	renderer->attribXYZ = glGetAttribLocation(renderer->program, attribXYZ);
	renderer->attribUV = glGetAttribLocation(renderer->program, attribUV);

	return renderer;
}

int _TobyG_GetCurrentMeshBatchRendererGLSLInfo(GLint* uniformMVP,
		GLint* attribXYZ, GLint* attribUV) {
	if (NULL == currentMeshBatchRenderer) {
		return -1;
	}

	*uniformMVP = currentMeshBatchRenderer->uniformMVP;
	*attribXYZ = currentMeshBatchRenderer->attribXYZ;
	*attribUV = currentMeshBatchRenderer->attribUV;

	return 0;
}

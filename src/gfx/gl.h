/*
 * gl.h
 *
 *  Created on: Nov 24, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_SRC_GFX_GL_H
#define TOBYG_SRC_GFX_GL_H

#include <GL/glew.h>

/**
 * \brief Create a buffer and fills it with data.
 * @param target buffer target (e.g. GL_ARRAY_BUFFER)
 * @param size data size in bytes
 * @param data data
 * @param usage buffer usage (e.g. GL_STATIC_DRAW)
 * @return buffer
 */
GLuint _TobyG_CreateBuffer(GLenum target, GLsizeiptr size, const GLvoid* data,
		GLenum usage);

/**
 * \brief Create a shader.
 * @param logID log ID for error output
 * @param source null terminated source code
 * @param type shader type (e.g. GL_VERTEX_SHADER)
 * @return shader
 */
GLuint _TobyG_CreateShader(int logID, const char* source, GLenum type);

/**
 * \brief Compile vertex and fragment shader into a program.
 * @param logID log ID for error output
 * @param vertexShader vertex shader
 * @param fragmentShader fragment shader
 * @return program
 */
GLuint _TobyG_CreateProgram(int logID, GLuint vertexShader,
		GLuint fragmentShader);

#endif /* TOBYG_SRC_GFX_GL_H */

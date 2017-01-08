/*
 * basic.h
 *
 *  Created on: Nov 19, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_SRC_GFX_RENDERER_H
#define TOBYG_SRC_GFX_RENDERER_H

#include <GL/glew.h>

#include "../../include/tobyg_gfx_renderer.h"

/*!
 * \brief Create a mesh renderer.
 * @param logID log ID for error output
 * @param vertexShader vertex shader
 * @param fragmentShader fragment shader
 * @param uniformMVP mvp name
 * @param uniformDelta delta name
 * @param uniformTex texture name
 * @param attribXYZ1 coordinates of frame one name
 * @param attribXYZ2 coordinates of frame two name
 * @param attribUV uv name
 * @return mesh renderer
 */
TobyG_MeshRenderer* _TobyG_CreateMeshRenderer(int logID,
		GLuint vertexShader, GLuint fragmentShader,
		const char* uniformMVP, const char* uniformDelta, const char* uniformTex,
		const char* attribXYZ1, const char* attribXYZ2, const char* attribUV);

/*!
 * \brief Get currently set mesh renderer's instance rendering info
 * @param[out] uniformMVP mvp
 * @param[out] uniformDelta delta
 * @param[out] attribXYZ1 vertex coordinates of first frame
 * @param[out] attribXYZ2 vertex coordinates of second frame
 * @param[out] attribUV vertex uv
 * @return -1 on error, 0 on success
 */
int _TobyG_GetCurrentMeshRendererGLSLInfo(GLint* uniformMVP, GLint* uniformDelta,
		GLint* attribXYZ1, GLint* attribXYZ2, GLint* attribUV);

/*!
 * \brief Create a mesh batch renderer.
 * @param logID logID log ID for error output
 * @param vertexShader vertex shader
 * @param fragmentShader fragment shader
 * @param uniformMVP mvp name
 * @param uniformTex texture name
 * @param attribXYZ vertex coordinates name
 * @param attribUV uv name
 * @return -1 on error, 0 on success
 */
TobyG_MeshBatchRenderer* _TobyG_CreateMeshBatchRenderer(int logID,
		GLuint vertexShader, GLuint fragmentShader,
		const char* uniformMVP, const char* uniformTex,
		const char* attribXYZ, const char* attribUV);

/*!
 * \brief Get currently set mesh batch renderer's instance rendering info
 * @param[out] uniformMVP mvp
 * @param[out] attribXYZ vertex coordinates
 * @param[out] attribUV vertex uv
 * @return -1 on error, 0 on success
 */
int _TobyG_GetCurrentMeshBatchRendererGLSLInfo(GLint* uniformMVP,
		GLint* attribXYZ, GLint* attribUV);

#endif /* TOBYG_SRC_GFX_RENDERER_H */

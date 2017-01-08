/*
 * tobyg_gfx_geometry.h
 *
 *  Created on: Nov 17, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_GFX_GEOMETRY_H
#define TOBYG_INCLUDE_TOBYG_GFX_GEOMETRY_H

#include <GL/glew.h>

#include "tobyg_gfx_instance.h"

typedef struct tobyg_mesh TobyG_Mesh;

/*!
 * \brief Read a T3D animated mesh.
 * @param path path to mesh
 * @return mesh
 */
TobyG_Mesh* TobyG_ReadMesh(const char* path);

/*!
 * \brief Free a mesh.
 * @param mesh mesh
 */
void TobyG_FreeMesh(TobyG_Mesh* mesh);

/*!
 * \brief Set mesh to be rendered.
 * @param mesh mesh
 * @return -1 on error, 0 on success
 */
int TobyG_SetMesh(TobyG_Mesh* mesh);

/*!
 * \brief Draw currently set mesh at instance's position.
 * @param instance instance
 * @param time frame time
 * @return -1 on error, 0 on success
 */
int TobyG_DrawMeshAt(TobyG_Instance* instance, GLfloat time);

/*!
 * \brief Get mesh's frame time limits.
 * @param[in] mesh mesh
 * @param[out] start first frame
 * @param[out] end last frame
 * @return -1 on error, 0 on success
 */
int TobyG_GetMeshFrameLimits(TobyG_Mesh* mesh, GLfloat* start, GLfloat* end);

#endif /* TOBYG_INCLUDE_TOBYG_GFX_GEOMETRY_H */

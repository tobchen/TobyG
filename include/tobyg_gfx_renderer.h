/*
 * tobyg_gfx_shader.h
 *
 *  Created on: Nov 19, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_GFX_SHADER_H
#define TOBYG_INCLUDE_TOBYG_GFX_SHADER_H

#include <GL/glew.h>

/* renderer */

typedef struct tobyg_mesh_renderer TobyG_MeshRenderer;
typedef struct tobyg_mesh_batch_renderer TobyG_MeshBatchRenderer;

/*!
 * \brief Free a mesh renderer.
 * @param renderer mesh renderer
 */
void TobyG_FreeMeshRenderer(TobyG_MeshRenderer* renderer);

/*!
 * \brief Start a mesh rendering section.
 * @param renderer mesh renderer
 * @return -1 on error, 0 on success
 */
int TobyG_StartMeshRenderer(TobyG_MeshRenderer* renderer);

/*!
 * \brief End a mesh rendering section.
 */
void TobyG_EndMeshRenderer();

/*!
 * \brief Free a mesh batch renderer.
 * @param renderer mesh batch renderer
 */
void TobyG_FreeMeshBatchRenderer(TobyG_MeshBatchRenderer* renderer);

/*!
 * \brief Start a mesh batch rendering section-
 * @param renderer mesh batch renderer
 * @return -1 on error, 0 on success
 */
int TobyG_StartMeshBatchRenderer(TobyG_MeshBatchRenderer* renderer);

/*!
 * End a mesh batch rendering section.
 */
void TobyG_EndMeshBatchRenderer();

/*!
 * Create a mesh renderer with retro style.
 * @return mesh renderer
 */
TobyG_MeshRenderer* TobyG_CreateRetroMeshRenderer();

/*!
 * Create a mesh batch renderer with retro style.
 * @return mesh batch renderer
 */
TobyG_MeshBatchRenderer* TobyG_CreateRetroMeshBatchRenderer();

#endif /* TOBYG_INCLUDE_TOBYG_GFX_SHADER_H */

/*
 * toby_gfx_meshbatch.h
 *
 *  Created on: Dec 15, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_GFX_MESHBATCH_H
#define TOBYG_INCLUDE_TOBYG_GFX_MESHBATCH_H

#include "tobyg_gfx_instance.h"

typedef struct tobyg_mesh_batch TobyG_MeshBatch;

/*!
 * \brief Start a mesh batch rendering section.
 */
void TobyG_StartMeshBatchRendering(void);

/*!
 * End a mesh batch rendering section.
 */
void TobyG_EndMeshBatchRendering(void);

/*!
 * \brief Read a T3D mesh batch.
 * @param path path to mesh batch
 * @return mesh batch
 */
TobyG_MeshBatch* TobyG_ReadMeshBatch(const char* path);

/*!
 * \brief Free a mesh batch.
 * @param batch mesh batch
 */
void TobyG_FreeMeshBatch(TobyG_MeshBatch* batch);

/*!
 * \brief Get a mesh's index in a mesh batch by name.
 * @param batch mesh batch
 * @param name mesh's name
 * @return index or 256 on error
 */
size_t TobyG_GetMeshBatchIndex(TobyG_MeshBatch* batch, const char* name);

/*!
 * \brief Set mesh batch to be rendered.
 * @param batch mesh batch
 * @return -1 on error, 0 on success
 */
int TobyG_SetMeshBatch(TobyG_MeshBatch* batch);

/*!
 * \brief Draw currently set mesh batch a instance's position.
 * @param instance instance
 * @param index index of mesh in mesh batch
 * @return -1 on error, 0 on success
 */
int TobyG_DrawMeshBatchAt(TobyG_Instance* instance, size_t index);

#endif /* TOBYG_INCLUDE_TOBYG_GFX_MESHBATCH_H */

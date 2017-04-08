/*
 * mesh.h
 *
 *  Created on: Nov 25, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_SRC_GFX_MESH_H
#define TOBYG_SRC_GFX_MESH_H

#include "../../include/tobyg_gfx_mesh.h"

/**
 * \brief Initialize TobyG's mesh renderer.
 * @return -1 on error, 0 on failure
 */
int _TobyG_InitMesh(void);

/**
 * \brief Destroy TobyG's mesh renderer.
 */
void _TobyG_DestroyMesh(void);

#endif /* TOBYG_SRC_GFX_MESH_H */

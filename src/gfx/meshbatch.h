/*
 * meshbatch.h
 *
 *  Created on: Dec 15, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_SRC_GFX_MESHBATCH_H
#define TOBYG_SRC_GFX_MESHBATCH_H

#include "../../include/tobyg_gfx_meshbatch.h"

/**
 * \brief Initialize TobyG's mesh batch renderer.
 * @return -1 on error, 0 on failure
 */
int _TobyG_InitMeshBatch(void);

/**
 * \brief Destroy TobyG's mesh batch renderer.
 */
void _TobyG_DestroyMeshBatch(void);

#endif /* TOBYG_SRC_GFX_MESHBATCH_H */

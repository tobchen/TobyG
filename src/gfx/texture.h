/*
 * texture.h
 *
 *  Created on: Nov 25, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_SRC_GFX_TEXTURE_H
#define TOBYG_SRC_GFX_TEXTURE_H

#include "../../include/tobyg_gfx_texture.h"

/**
 * \brief Initialize texture system.
 * @return -1 on error, 0 on success
 */
int _TobyG_InitTexture(void);

/**
 * \brief Destroy texture system.
 */
void _TobyG_DestroyTexture(void);

#endif /* TOBYG_SRC_GFX_TEXTURE_H */

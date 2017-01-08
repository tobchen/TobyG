/*
 * camera3d.h
 *
 *  Created on: Nov 21, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_SRC_GFX_CAMERA_H
#define TOBYG_SRC_GFX_CAMERA_H

#include "../../include/tobyg_gfx_camera.h"

/**
 * \brief Initialize TobyG's camera.
 * @return -1 on error, 0 on failure
 */
int _TobyG_InitCamera(void);

/**
 * \brief Destroy TobyG's camera.
 */
void _TobyG_DestroyCamera(void);

/**
 * \brief Get TobyG's camera's matrix (projection*view matrix).
 * @return 4x4 matrix
 */
GLfloat* _TobyG_GetCameraMatrix(void);

#endif /* TOBYG_SRC_GFX_CAMERA_H */

/*
 * instance.h
 *
 *  Created on: Nov 21, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_SRC_GFX_INSTANCE_H
#define TOBYG_SRC_GFX_INSTANCE_H

#include "../../include/tobyg_gfx_instance.h"

/**
 * \brief Get an instance's matrix (model matrix).
 * @param instance instance
 * @return 4x4 matrix
 */
GLfloat* _TobyG_GetInstanceMatrix(TobyG_Instance* instance);

#endif /* TOBYG_SRC_GFX_INSTANCE_H */

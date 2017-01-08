/*
 * tobyg_gfx_object.h
 *
 *  Created on: Nov 20, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_GFX_INSTANCE_H
#define TOBYG_INCLUDE_TOBYG_GFX_INSTANCE_H

#include <GL/glew.h>

typedef struct tobyg_instance TobyG_Instance;

/*!
 * \brief Create a 3D instance to be used for mesh (batch) rendering.
 * @return instance
 */
TobyG_Instance* TobyG_CreateInstance();

/*!
 * \brief Free an instance.
 * @param instance instance
 */
void TobyG_FreeInstance(TobyG_Instance* instance);

/*!
 * \brief Set an instance's position.
 * @param instance instance
 * @param x x position
 * @param y y position
 * @param z z position
 * @return -1 on error, 0 on success
 */
int TobyG_SetInstancePosition(TobyG_Instance* instance,
		GLfloat x, GLfloat y, GLfloat z);

/*!
 * \brief Set an instance's rotation, applying first x, then y, finally z.
 * @param instance instance
 * @param x x rotation
 * @param y y rotation
 * @param z z rotation
 * @return -1 on error, 0 on success
 */
int TobyG_SetInstanceRotation(TobyG_Instance* instance,
		double x, double y, double z);

/*!
 * \brief Set an instance's scale.
 * @param instance instance
 * @param x x scale
 * @param y y scale
 * @param z z scale
 * @return -1 on error, 0 on success
 */
int TobyG_SetInstanceScale(TobyG_Instance* instance,
		GLfloat x, GLfloat y, GLfloat z);

#endif /* TOBYG_INCLUDE_TOBYG_GFX_INSTANCE_H */

/*
 * tobyg_gfx_camera.h
 *
 *  Created on: Nov 25, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_GFX_CAMERA_H
#define TOBYG_INCLUDE_TOBYG_GFX_CAMERA_H

#include <GL/glew.h>

/*!
 * \brief Set TobyG's camera's perspective matrix.
 * @param fov field of view in radiant
 * @param near near
 * @param far far
 * @return -1 on error, 0 on success
 */
int TobyG_SetCameraPerspective(double fov, GLfloat near, GLfloat far);

/*!
 * \brief Set TobyG's camera's position.
 * @param x x position
 * @param y y position
 * @param z z position
 * @return -1 on error, 0 on success
 */
int TobyG_SetCameraPosition(GLfloat x, GLfloat y, GLfloat z);

/*!
 * \brief Set TobyG's camera's rotation, applying first x, then y, finally z.
 * @param x x rotation
 * @param y y rotation
 * @param z z rotation
 * @return -1 on error, 0 on success
 */
int TobyG_SetCameraRotation(double x, double y, double z);

#endif /* TOBYG_INCLUDE_TOBYG_GFX_CAMERA_H */

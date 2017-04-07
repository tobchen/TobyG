/*
 * tobyg_gfx.h
 *
 *  Created on: Nov 17, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_GFX_GFX_H
#define TOBYG_INCLUDE_TOBYG_GFX_GFX_H

/* gfx */

/*!
 * \brief Creates an OpenGL 2.1 window.
 *
 * @param[in] title title
 * @param[in] width width
 * @param[in] height height
 * @return -1 on error, 0 on success
 */
int TobyG_StartTobyG(const char* title, int width, int height);

/*!
 * \brief Destroys active window.
 */
void TobyG_EndTobyG(void);

/*!
 * \brief Start main loop's graphics section.
 */
void TobyG_StartGraphics(void);

/*!
 * \brief End main loop's graphics section.
 */
void TobyG_EndGraphics(void);

#endif /* TOBYG_INCLUDE_TOBYG_GFX_GFX_H */

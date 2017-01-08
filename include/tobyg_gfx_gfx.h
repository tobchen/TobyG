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
 * For not making use of virtual resolution set \p vwidth and/or \p vheight to
 * zero or less.
 * @param[in] title title
 * @param[in] width width
 * @param[in] height height
 * @param[in] vwidth virtual width
 * @param[in] vheight virtual height
 * @return -1 on error, 0 on success
 */
int TobyG_StartTobyG(const char* title, int width, int height,
		int vwidth, int vheight);

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

/*!
 * \brief Returns current window resolution or virtual resolution (if set).
 * @param[out] width
 * @param[out] height
 * @return -1 on error, 0 on success
 */
int TobyG_GetResolution(int* width, int* height);

#endif /* TOBYG_INCLUDE_TOBYG_GFX_GFX_H */

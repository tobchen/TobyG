/*
 * tobyg_gfx_texture.h
 *
 *  Created on: Nov 25, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_GFX_TEXTURE_H
#define TOBYG_INCLUDE_TOBYG_GFX_TEXTURE_H

#include <GL/glew.h>

typedef struct tobyg_texture TobyG_Texture;

/*!
 * \brief Create a texture from RGBA data.
 * @param pixel pixel data
 * @param width width
 * @param height height
 * @return texture
 */
TobyG_Texture* TobyG_CreateTextureRGBA(unsigned char* pixel,
		GLsizei width, GLsizei height);

/*!
 * \brief Read a texture from WebP.
 * @param path path to texture
 * @return texture
 */
TobyG_Texture* TobyG_ReadTextureFromWebP(const char* path);

/*!
 * \brief Free a texture.
 * @param texture texture
 */
void TobyG_FreeTexture(TobyG_Texture* texture);

/*!
 * \brief Set texture for future renders.
 * @param texture texture
 * @return -1 on error, 0 on success
 */
int TobyG_SetTexture(TobyG_Texture* texture);

/*!
 * \brief Set the default texture (white).
 */
void TobyG_SetDefaultTexture(void);

#endif /* TOBYG_INCLUDE_TOBYG_GFX_TEXTURE_H */

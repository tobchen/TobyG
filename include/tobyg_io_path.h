/*
 * tobyg_io_path.h
 *
 *  Created on: Dec 10, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_IO_PATH_H
#define TOBYG_INCLUDE_TOBYG_IO_PATH_H

/*!
 * \brief Start path system.
 * @return -1 on error, 0 on success
 */
int TobyG_StartPath(void);

/*!
 * \brief End path system.
 */
void TobyG_EndPath(void);

/*!
 * \brief Get a path by directory, file name (without extension) and extension.
 * The path has to be freed by the user.
 * @param directory directory
 * @param name file name
 * @param extension file extension
 * @return path
 */
char* TobyG_GetPathByDirNameExt(const char* directory, const char* name,
		const char* extension);

/*!
 * \brief Get a path by directory and complete file name.
 * The path has to be freed by the user.
 * @param directory directory
 * @param file file name
 * @return path
 */
char* TobyG_GetPathByDirFile(const char* directory, const char* file);

#endif /* TOBYG_INCLUDE_TOBYG_IO_PATH_H */

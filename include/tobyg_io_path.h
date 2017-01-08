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
 * \brief Get a path.
 * The path has to be freed by the user.
 * @param directory directory
 * @param name file name
 * @param extension file extension
 * @return path
 */
char* TobyG_GetPath(const char* directory, const char* name,
		const char* extension);

#endif /* TOBYG_INCLUDE_TOBYG_IO_PATH_H */

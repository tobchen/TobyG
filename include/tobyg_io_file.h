/*
 * tobyg_io_file.h
 *
 *  Created on: Nov 17, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_IO_FILE_H
#define TOBYG_INCLUDE_TOBYG_IO_FILE_H

#include <stdlib.h>

/* file */

/*!
 * \brief Reads a plain text file.
 *
 * Result must be freed by the user.
 * @param[in] path file path
 * @return NULL on error, c-string on success
 */
char* TobyG_ReadTextFile(const char* path);

/*!
 * \brief Reads a binary file.
 *
 * Result must be freed by the user.
 * @param[in] path file path
 * @param[out] data file's content
 * @param[out] length length of \p data
 * @return -1 on error, 0 on success
 */
int TobyG_ReadBinaryFile(const char* path, unsigned char** data,
		size_t* length);

#endif /* TOBYG_INCLUDE_TOBYG_IO_FILE_H */

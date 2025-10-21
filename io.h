/**
 * @brief     : file declaring the functions necessary to load 
 *              and save the vectors via csv file
 * @file      : io.h
 * 
 * Name       : rostj@msoe.edu <Jesse Rost>
 * Date       : 10/27/25
 * Course     : CPE 2600
 * Assignment : Lab 7
 * Section    : 112
 */

#ifndef IO_H
#define IO_H

#include "vector.h"
#include <stdbool.h>

/**
 * @brief Takes input from a csv file and loads them into vector arrays.
 * @param store Pointer to the VectorStore to initialize.
 * @param filename Filename of the csv file which is being read.
 * @return true if the file was successfully opened and read.
 * @return false if the file could not be opened (e.g., does not exist).
 */
bool load_vectors(VectorStore *store, const char *filename);

/**
 * @brief Takes array of vectors and stores them into a csv file.
 * @param store Pointer to the VectorStore to initialize.
 *Type: uploaded file
 * @param filename Filename of the csv file to which the data is being saved.
 * @return true if the file was successfully opened and saved.
 * @return false if the file could not be opened for writing.
 */
bool save_vectors(const VectorStore *store, const char *filename);

#endif // IO_H
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

#include "vector.h"

/**
 * @brief Takes input from a csv file and loads them into vector arrays
 * @param store Pointer to the VectorStore to initialize.
 * @param filename Filename of the csv file which is being read
 */
bool load_vectors(VectorStore *store, const char *filename);

/**
 * @brief Takes array of vectors and stores them into a csv file 
 * @param store Pointer to the VectorStore to initialize.
 * @param filename Filename of the csv file to which the data is being saved 
 */
bool save_vectors(const VectorStore *store, const char *filename);
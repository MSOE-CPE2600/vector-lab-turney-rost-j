/**
 * @brief     : file defining the functions necessary to load 
 *              and save the vectors via csv file
 * @file      : io.c
 * 
 * Name       : rostj@msoe.edu <Jesse Rost>
 * Date       : 10/27/25
 * Course     : CPE 2600
 * Assignment : Lab 7
 * Section    : 112
 */

#include "io.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Needed to use the bool type, and true/false values
#include <string.h>

#define MAX_LENGTH 1024

/**
 * @brief Takes input from a csv file and loads them into vector arrays.
 *
 * This function will clear any existing vectors in the store before
 * loading the new ones from the file. It will skip any
 * malformed lines in the file and print a warning.
 *
 * @param store Pointer to the VectorStore to load vectors into.
 * @param filename Filename of the csv file which is being read.
 * @return true if the file was successfully opened and read.
 * @return false if the file could not be opened (e.g., does not exist).
 */
bool load_vectors(VectorStore *store, const char *filename){
    // 'r' is used for reading a file
    FILE *file = fopen(filename, "r");
    // creating an array with the max length of a line
    char line[MAX_LENGTH];

    if(file == NULL){
        // fprintf needed for a file
        fprintf(stderr, "Error: could not read the file '%s'\n", filename);
        return false;
    }

    // Optional design choice: clear existing vectors before loading new ones
    clear_vectors(store);

    while (fgets(line, MAX_LENGTH, file) != NULL) {
        // Remove newline at the end if present
        line[strcspn(line, "\n")] = '\0';

        // begins reading at the beginning of the line 
        char *name = strtok(line, ",");
        // NULL tells the strtok command to begin where it left off
        char *x_str = strtok(NULL, ",");
        char *y_str = strtok(NULL, ",");
        char *z_str = strtok(NULL, ",");

        // Check formatting, skip any line with incorrect parameters
        if (!name || !x_str || !y_str || !z_str) {
            fprintf(stderr, "Warning: Skipping malformed line.\n");
            continue; // skip this bad line, move on to next
        }

        // Create a vector and fill in data
        vector v;
        strncpy(v.name, name, sizeof(v.name) - 1);
        v.name[sizeof(v.name) - 1] = '\0'; // ensure null-terminated
        v.x = atof(x_str);
        v.y = atof(y_str);
        v.z = atof(z_str);

        // Add it to the vector store
        add_vector(store, v);
    }
    fclose(file);
    return true;
}

/**
 * @brief Takes array of vectors and stores them into a csv file.
 *
 * Writes all vectors currently in the store to the specified file,
 * overwriting the file if it already exists. Vectors are
 * saved in the format "name,x,y,z".
 *
 * @param store Pointer to the VectorStore containing the vectors to save.
 * @param filename Filename of the csv file to which the data is being saved.
 * @return true if the file was successfully opened for writing and saved.
 * @return false if the file could not be opened for writing.
 */
bool save_vectors(const VectorStore *store, const char *filename){
    FILE *file = fopen(filename, "w");  // open file for writing (overwrite mode)

    if (!file) {
        fprintf(stderr, "Error: could not open file '%s'\n", filename);
        return false;
    }

    // Write each vector as: name,x,y,z
    // loop over the vectors in store
    for (int i = 0; i < store->count; i++) {
        // print accordingly to a csv file
        fprintf(file, "%s,%.4f,%.4f,%.4f\n",
                store->vectors[i].name,
                store->vectors[i].x,
                store->vectors[i].y,
                store->vectors[i].z);
    }

    fclose(file);
    return true;
}

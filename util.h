/**
 * @file      : util.h
 * @brief     : Declares utility string functions for the vector calculator project.
 * 
 * Name       : rostj@msoe.edu <Jesse Rost>
 * Date       : 10/27/25
 * Course     : CPE 2600
 * Assignment : Lab 7
 * Section    : 112
 */

#ifndef UTIL_H
#define UTIL_H

/**
 * @brief Removes leading and trailing whitespace characters from a string.
 * 
 * This function modifies the given C-style string in place by trimming
 * spaces, tabs, and other whitespace from both ends.
 * 
 * @param str Pointer to the string to be trimmed. 
 *            The function updates the original string directly.
 */
void trim(char *str);

#endif /* UTIL_H */

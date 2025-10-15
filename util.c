/**
 * @file      : util.c
 * @brief     : Defines utility string functions for the vector calculator project.
 * 
 * Name       : rostj@msoe.edu <Jesse Rost>
 * Date       : 10/27/25
 * Course     : CPE 2600
 * Assignment : Lab 7
 * Section    : 112
 */

#include <string.h>
#include <ctype.h>
#include "util.h"

/**
 * @brief Removes leading and trailing whitespace characters from a string.
 * 
 * This function modifies the given string in place. It trims all whitespace 
 * characters (spaces, tabs, etc.) from both ends of the string, ensuring 
 * the result contains only meaningful characters. It uses `isspace()` to 
 * detect whitespace and `memmove()` to handle overlapping memory safely.
 * 
 * @param str Pointer to the string to be trimmed. 
 *            The function updates the original string directly.
 */
void trim(char *str)
{
    // Pointer to the first non-whitespace character
    char *start = str;
    // Pointer to the last non-whitespace character
    char *end;

    // Skip leading whitespace characters
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // If the entire string is whitespace, make it empty
    if (*start == '\0') {
        *str = '\0';
        return;
    }

    // Find the last non-whitespace character
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Null-terminate after the last non-whitespace character
    *(end + 1) = '\0';

    // Shift trimmed content to the beginning if needed
    if (start != str) {
        memmove(str, start, end - start + 2);
    }
}

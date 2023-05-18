/**
 * @file my_string.c
 * @author Michelle Namgoong
 * @collaborators NAMES OF PEOPLE THAT YOU COLLABORATED WITH HERE
 * @brief Your implementation of these famous 3 string.h library functions!
 *
 * NOTE: NO ARRAY NOTATION IS ALLOWED IN THIS FILE
 *
 * @date 2023-03-xx
 */

#include <stddef.h>
#include "my_string.h"
/**
 * @brief Calculate the length of a string
 *
 * @param s a constant C string
 * @return size_t the number of characters in the passed in string
 */
size_t my_strlen(const char *s) {
    size_t ret = 0;
    while (*s != '\0') {
        ret++;
        s++;
    }
    return ret;
}

/**
 * @brief Compare two strings
 *
 * @param s1 First string to be compared
 * @param s2 Second string to be compared
 * @param n First (at most) n bytes to be compared
 * @return int "less than, equal to, or greater than zero if s1 (or the first n
 * bytes thereof) is found, respectively, to be less than, to match, or be
 * greater than s2"
 */
int my_strncmp(const char *s1, const char *s2, size_t n) {
    while (n-- > 0 && *s1 && *s2) {
        if (*s1 != *s2) {
            return (*s1 < *s2) ? -1 : 1;
        }
        s1++;
        s2++;
    }
    if (n == (size_t) -1) { // out of bounds
        return 0;
    }
    if (!*s1 && !*s2) { // if both are NUL
        return 0;
    }
    return (*s1 < *s2) ? -1 : 1;
}

/**
 * @brief Copy a string
 *
 * @param dest The destination buffer
 * @param src The source to copy from
 * @param n maximum number of bytes to copy
 * @return char* a pointer same as dest
 */
char *my_strncpy(char *dest, const char *src, size_t n) {
    char *ret = dest;
    size_t i;

    for (i = 0; i < n; i++) {
        if (*src == '\0') {
            break;
        }

        *ret = *src;
        ret++;
        src++;
    }

    for (size_t j = i; j < n; j++) {
        *ret = '\0';
        ret++;
    }

    return dest;
}

/**
 * @brief Concatenates two strings and stores the result
 * in the destination string
 *
 * @param dest The destination string
 * @param src The source string
 * @param n The maximum number of bytes from src to concatenate
 * @return char* a pointer same as dest
 */
char *my_strncat(char *dest, const char *src, size_t n) {
    char *ret = dest;

    while (*ret != '\0') {
        ret++;
    }

    while (n-- > 0 && *src != '\0') {
        *ret++ = *src++;
    }
    *ret = '\0';

    return dest;
}

/**
 * @brief Copies the character c into the first n
 * bytes of memory starting at *str
 *
 * @param str The pointer to the block of memory to fill
 * @param c The character to fill in memory
 * @param n The number of bytes of memory to fill
 * @return char* a pointer same as str
 */
void *my_memset(void *str, int c, size_t n) {
    char *ret = (char *) str;
    
    while (n--) {
        *ret++ = (char) c;
    }

    return str;
}

/**
 * @brief Finds the first instance of c in str
 * and removes it from str in place
 *
 * @param str The pointer to the string
 * @param c The character we are looking to delete
 */
void remove_first_instance(char *str, char c) {
    char *ret = str;

    while (*ret != '\0') {
        if (*ret == c) {
            char *p = ret;
            while (*p != '\0') {
                *p = *(p + 1); // shift to the left
                p++;
            }
            break; 
        }
        ret++;
    }
}

/**
 * @brief Finds the first instance of c in str
 * and replaces it with the contents of replaceStr
 *
 * @param str The pointer to the string
 * @param c The character we are looking to delete
 * @param replaceStr The pointer to the string we are replacing c with
 */
void replace_character_with_string(char *str, char c, char *replaceStr) {
    int replaceStrLength = my_strlen(replaceStr);
    
    if (replaceStrLength == 0) {
        remove_first_instance(str, c);
        return;
    }

    while (*str != '\0') {
        // find first instance of c
        if (*str == c) {
            char *temp = str;
            int length = my_strlen(temp + 1);

            for (int i = length; i >= 0; i--) {
                *(str + i + replaceStrLength) = *(temp + 1 + i);
            }

            for (int j = 0; j < replaceStrLength; j++) {
                *(str + j) = *(replaceStr + j);
            }
            
            break; // replacement is complete
        }
        str++; // move to next character;
    }
}

/**
 * @brief Remove the first character of str (ie. str[0]) IN ONE LINE OF CODE.
 * No loops allowed. Assume non-empty string
 * @param str A pointer to a pointer of the string
 */
void remove_first_character(char **str) {
    (*str)++;
}
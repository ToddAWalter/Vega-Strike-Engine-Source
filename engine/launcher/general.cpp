/*
 * general.cpp
 *
 * Vega Strike - Space Simulation, Combat and Trading
 * Copyright (C) 2001-2025 The Vega Strike Contributors:
 * Project creator: Daniel Horn
 * Original development team: As listed in the AUTHORS file. Specifically: David Ranger
 * Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
 *
 *
 * https://github.com/vegastrike/Vega-Strike-Engine-Source
 *
 * This file is part of Vega Strike.
 *
 * Vega Strike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vega Strike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
 */

/* This include has been designed to act independant of the other modules.
 * This allows it to be used with other programs with minimal changes */

#include "launcher/general.h"
#if defined(__APPLE__) || defined(MACOSX)
#include <sys/param.h> // For MAXPATHLEN
#endif
#ifdef __MINGW32__
#include <dirent.h>
#endif
using std::string;
using std::vector;
#ifdef _G_RANDOM
int RANDOMIZED = 0;
#endif    // _G_RANDOM

// Gets the next parameter from the string, sorted by a space
// Sticks a \0 at the space and returns a pointer to the right side.

#ifdef _G_STRING_PARSE

char *next_parm(char *string) {
    char *next;
    if (string[0] == '\0') {
        return 0;
    }
    next = string;
    while (next[0] != ' ' && next[0] != '\0') {
        next++;
    }
    if (next[0] == '\0') {
        return next;
    }
    next[0] = '\0';
    next++;
    return next;
}

/*
// Splits the string into an array of char pointers

// It's not working. I'll fix it when I need it. For now, use next_parm

char *split_words(char *string, int max_words) {
	char *array[MAX_ELEM];
	int cur = 0;
	char *parm;

	if (max_words >= MAX_ELEM || max_words <= 0) { max_words = MAX_ELEM; }
	while ((parm = next_parm(string)) != 0) {
		if (cur <= MAX_ELEM) { array[cur] = string; }
		cur++;

		string = parm;
	}
//	printf("Debug: %s - %s - %s\n", array[0], array[1], array[2]);
	return *array;
}
*/

// Allocates memory for a string and returns a pointer to it. Includes a \0 at the end
// Maybe eventually this could be used in memory management

// How efficient is malloc at using recently freed space? Would it be better to allocate memory, and instead
// of freeing it, just mark it for reuse and use an array to keep track of it all? (CPU speed vs Memory speed)

// Might be faster to have malloc handle it since keeping track of the memory used would require extra memory allocated
// everytime memory is allocated to keep track of it. It would only be useful if there's lots of stuff coming and going,
// in which case we just leave flags in the linked lists to say wether or not this link is ready for re-use.

// What about variable clean-up? Keeping track of everything allocated so it can free it up at the end.

char *ptr_copy(char *string) {
    char *alloc;
    alloc = (char *) malloc(strlen(string) + 1);
    if (alloc == 0) {
        ShowError("Out of memory", "G01", 1);
        return NULL;
    }
    strncpy(alloc, string, strlen(string));
    alloc[strlen(string)] = '\0';
    return alloc;
}

/* chomp
 * This function replaces trailing \n (carriage return) with the null character
 */
void chomp(char *line) {
    int current;
    for (current = strlen(line) - 1; (line[current] == '\n' || line[current] == ' ' || line[current] == 13);
            current--) {
        line[current] = '\0';
    }
}

/* pre_chomp
 * This function Removes spaces and = from the front of the line and
 * returns a pointer to the new starting character
 */

char *pre_chomp(char *line) {
    while (line[0] == '=' || line[0] == ' ' || line[0] == 13 || line[0] == 9) {
        line++;
    }
    return line;
}

/* replace
 * This function searches the first parameter for the second parameter
 * If the second parameter is found, it is replaced with the third parameter
 */

char *replace(char *line, char *search, char *replace, int LENGTH) {
    int length, dif, calc;
    char *chr_new, *current;
    char *ptr_new, *location;
    calc = strlen(line) - strlen(search) + strlen(replace);
    if (calc > LENGTH) {
        return line;
    }
    chr_new = new char[LENGTH + 1];
    current = new char[LENGTH + 1];
    length = strlen(line);
    strcpy(current, line);
    while ((location = strstr(current, search)) > 0) {
        chr_new[0] = '\0';
        calc = strlen(current) - strlen(search) + strlen(replace);
        if (calc > LENGTH) {
            strcpy(line, current);
            return line;
        }
        dif = location - current;
        strncat(chr_new, current, dif);
        strncat(chr_new, replace, strlen(replace));
        ptr_new = current + dif + strlen(search);
        strncat(chr_new, ptr_new, strlen(ptr_new));
        strcpy(current, chr_new);
    }
    strcpy(line, current);
    delete[] chr_new;
    delete[] current;
    return line;
}

char *strmov(char *to, char *from) {
    char *end;
    strcpy(to, from);
    end = to + strlen(to);
    return end;
}

/* lower
 * This function makes sure all characters are in lower case
 */

void lower(char *line) {
    int current;
    for (current = 0; line[current] != '\0'; current++) {
        if (line[current] >= 65 && line[current] <= 90) {
            line[current] += 32;
        }
    }
}

void strappend(char *dest, char *source, int length) {
    int DoLength;
    DoLength = length - strlen(dest);
    strncat(dest, source, DoLength);
    return;
}

char *StripPath(char *filename) {
    int length, cur;
    char *last = filename;
    length = strlen(filename) - 1;
    if (length <= 0) {
        return filename;
    }
    for (cur = 0; cur < length; cur++) {
        if (filename[cur] == SEPERATOR) {
            last = &filename[cur + 1];
        }
    }
    if (filename[length] == SEPERATOR) {
        filename[length] = '\0';
    }
    return last;
}

char *StripExtension(char *filename) {
    int length, cur;
    char *last = filename;
    length = strlen(filename) - 1;
    if (length <= 0) {
        return const_cast<char *>("");
    }
    for (cur = 0; cur <= length; cur++) {
        if (filename[cur] == '.') {
            last = &filename[cur];
        }
    }
    if (last[0] == '.') {
        last[0] = '\0';
        return last + 1;
    }
    return last;
}

#endif    // _G_STRING_PARSE

#ifdef _G_RANDOM
int randnum(int start, int end) {
    int random, dif, min, max;
    if (RANDOMIZED == 0) { srand(time(NULL)); RANDOMIZED = 1; }
    min = start;
    max = end;
    if (end < start) { min = end; max = start; }
    if (end == start) { return start; }
    dif = max - min + 1;
    random = rand() % dif;
    random += min;
    return random;
}

void randcode(char *line, int length) {
    int current, randomA, randomB, test;
    test = 2;
    for (current = 0; current < length; current++) {
        if (test % 5 == 1 && current > 1) { line[current] = '-'; test++; continue; }
        randomA = randnum(1,2);
        randomB = 60;
        if (randomA == 1) { randomB = randnum(48,57); }
        if (randomA == 2) { randomB = randnum(65,90); }
        line[current] = randomB;
        test++;
    }
    line[length] = '\0';
    return;
}

#endif    // _G_RANDOM

#ifdef _G_NUMBER

void itoa(char *line, int number, int length) {
    int current, cur, multiplier, reduce, base;
    base = number;
    cur = 0;
    line[0] = '\0';
    while (1) {
        multiplier = 0;
        current = base;
        while (current / 10 >= 1) { current /= 10; multiplier++; continue; }
        if (base == 0) { break; }
        current /= 10;
        reduce = pwer(1,multiplier);
        current = base / reduce;
        reduce = pwer(current,multiplier);
        current += 48;
        line[cur] = current;
        cur++;
        base -= reduce;
    }
    line[cur] = '\0';
    return;
}

// pwer is for x * 10^y
// pwr is for x^y
int pwer(int start, int end) {
        return do_power(start, end, 10);
}

int pwr(int start, int end) {
        return do_power(1, end, start);
}

int do_power(int start, int end, int multiply) {
        int current, val_return;
        val_return = start;
        for (current = 2; current <= end; current++) {
                val_return *= multiply;
        }
        return val_return;
}

#ifdef __cplusplus

double pwer(double start, long end) {
    double current, val_return;
    val_return = start;
    for (current = 2; current <= end; current++) {
        val_return *= start;
    }
    return val_return;
}

#endif    // __cplusplus

// Potential buffer overflow in this function. If you need it a lot, create a nbtoa(char *dest, char *string, int length)
// This function converts a binary number stored in a string into real numbers (stored in a string)

void btoa(char *dest, char *string) {
    int max, cur, pos, new_val;
    char *ptr_char, cur_char[1];
    char new_string[strlen(string)+1];
    max = 7;
    cur = 7;
    pos = 0;
    ptr_char = string;
    new_val = 0;
    while (ptr_char[0] != '\0') {
        cur_char[0] = ptr_char[0];
        if (ptr_char[0] == '1') { new_val += pwr(2,cur); }
        cur--;
        if (cur < 0) { cur = max; new_string[pos] = new_val; new_val = 0; pos++; }
        ptr_char++;
    }
    new_string[pos] = '\0';
    strcpy(dest, new_string);
    return;
}

#endif    // _G_NUMBER

#ifdef _G_STRING_MANAGE
#ifdef G_GLIB

// Some handy wrappers for glib that help error handling which prevent segfaults
char *GetString(GString *line) {
    if (line == 0) { return '\0'; }
    return line->str;
}

void SetString(GString **ptr, char *line) {
    if (ptr <= 0) { return; }
    if (*ptr <= 0) {
        *ptr = g_string_new(line);
        return;
    }
    g_string_sprintf(*ptr, "%s", line);
}

#endif    // G_GLIB

#ifndef __cplusplus
char *NewString(char *line) {
    char *new_str;
    new_str = (char *)malloc(strlen(line)+1);
    if (new_str == 0) { ShowError("Out of Memory", "G02", 1); return NULL; }
    strcpy(new_str, line);
    new_str[strlen(line)] = '\0';
    return new_str;
}
#endif  // not __cplusplus

#ifdef __cplusplus

char *GetString(char *line) {
    if (line == 0) {
        return const_cast<char *>("");
    }
    return line;
}

void SetString(char **ptr, char *line) {
    if (*ptr != nullptr) {
        delete *ptr;
        *ptr = nullptr;
    }
    *ptr = strdup(line);
}

char *NewString(char *line) {
    return strdup(line);
}

#endif    // __cplusplus
#endif    // _G_STRING_MANAGE

#ifdef _G_ERROR

// if _G_ERROR is defined, it will print the error message
// if EXIT_ON_FATAL is defined, and is_fatal is greater than 0, the program will exit
void ShowError(char *error_msg, char *error_code, int is_fatal) {
#ifdef _G_ERROR
    if (is_fatal > 0) {
        fprintf(stderr, "Fatal ");
    }
    fprintf(stderr, "Error [%s]: %s\n", error_code, error_msg);
#endif    // _G_ERROR
    if (is_fatal > 0) {
#ifdef _G_ERROR
        fprintf(stderr, "Program will now exit\n");
#endif    // _G_ERROR
#ifdef EXIT_ON_FATAL
        exit(EXIT_ON_FATAL);
#endif    // EXIT_ON_FATAL
    }
    return;
}

#endif    // _G_ERROR

// Checks for a <!-- and returns a pointer to the next character
// A \0 is placed at the < so that anything in front can be kept

#ifdef _G_XML

char *xml_pre_chomp_comment(char *string) {
    int length, cur;
    if (string[0] == '<' && string[1] == '!') { string[0] = '\0'; return string += 5; }
    length = strlen(string) - 5;
    for (cur = 0; cur <= length; cur++) {
        if (string[cur] == '<' && string[cur+1] == '!') { string[cur] = '\0'; return &string[cur+5]; }
    }
    return &string[length+5];
}
char *xml_chomp_comment(char *string) {
    int len, cur;
    if (string[0] == '\0') { return string; }
    len = strlen(string) - 1;
    if (len <= 3) { return &string[len+1]; }
    if (string[len] == '>' && string[len-1] == '-' && string[len-2] == '-') {
        if (string[len-3] == ' ') { string[len-3] = '\0'; }
        string[len-2] = '\0';
        return &string[len-2];
    }
    len -= 3;
    for (cur = 0; cur <= len; cur++) {
        if (string[cur] == '-' && string[cur+2] == '>') {
            if (string[cur-1] == ' ') { string[cur-1] = '\0'; }
            string[cur] = '\0';
            string[cur+2] = '\0';
            return &string[cur+3];
        }
    }
    return &string[len+4];
}

#endif    // _G_XML

#ifdef _G_PATH

int isdir(const char *file) {
    int length = strlen(file);
    if (length == 0) {
        return -1;
    }
    if (file[length - 1] == '.') {
        if (length == 1) {
            return -1;
        }
        if (file[length - 2] == '.' || file[length - 2] == SEPERATOR || file[length - 2] == '\\') {
            return -1;
        }
    }

    if (-1 == chdir(file)) {
        return 0;
    } else {
        chdir("..");
        return 1;
    }
}

// type = 0: file
// type = 1: dirs

glob_t *FindPath(char *path, int type) {
    glob_t *FILES = new glob_t;
    string mypath(path);
#if defined(__APPLE__) || defined(MACOSX)
    char thispath[MAXPATHLEN];
#else
    char thispath[800000];
#endif
    char *curpath = 0;
    DIR *dir;
    vector<string> result;
    vector<string> pathlist;
    dirent *entry;
    unsigned int cur;
    char *newpath = 0;
#if defined(__APPLE__) || defined(MACOSX)
    getcwd(thispath, MAXPATHLEN);
#else
    getcwd(thispath, 790000);
#endif
    pathlist.push_back((string(thispath) + SEPERATOR + mypath).c_str());
    for (cur = 0; cur < pathlist.size(); cur++) {
        curpath = strdup(pathlist[cur].c_str());
        dir = opendir(curpath);
        chdir(curpath);
        if (dir == 0) {
            continue;
        }
        entry = 0;
        while ((entry = readdir(dir)) != NULL) {
            newpath = strdup((string(curpath) + SEPERATOR + entry->d_name).c_str());
            if (isdir(newpath) == 1) {
                pathlist.push_back(newpath);
                if (type == 1) {
                    result.push_back(newpath);
                }
            } else if (type == 0) {
                result.push_back(newpath);
            }
        }
        closedir(dir);
    }
    chdir(thispath);

    FILES->gl_pathc = result.size();

    #ifdef __cplusplus
    FILES->gl_pathv = new char *[FILES->gl_pathc];
    #else
    FILES->gl_pathv = (char *)malloc(sizof(char *) * FILES->gl_pathc);
    if (FILES->gl_pathv == 0) { ShowError("Out of memory", "G04", 1); return NULL; }
    #endif

    for (cur = 0; cur < FILES->gl_pathc; cur++) {
        FILES->gl_pathv[cur] = strdup(result[cur].c_str());
    }

    return FILES;
}

glob_t *FindFiles(char *path, char *extension) {
    return FindPath(path, 0);
}

glob_t *FindDirs(char *path) {
    return FindPath(path, 1);
}

/*
glob_t *FindDirs(char *path) {
	glob_t *DIRS;
	char *pattern;

#ifdef __cplusplus
	DIRS = new glob_t;
	pattern = new char[strlen(path) + 2];
#else
	DIRS = (glob_t *)malloc(sizeof(glob_t));
	pattern = (char *)malloc(strlen(path) + 2);
	if (DIRS == NULL || pattern == NULL) { ShowError("Out of memory", "G04", 1); return NULL; }
#endif    // __cplusplus

	sprintf(pattern, "%s*", path);
	glob(pattern, GLOB_MARK, NULL, DIRS);
	return DIRS;
}
*/

#endif    // _G_PATH

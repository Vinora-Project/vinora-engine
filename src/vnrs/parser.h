/*
parser.h -- Vinora Screenplay scene reader
Copyright (c) 2026 Evgeniy Parfenyuk <parthen@riseup.net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
*/

#ifndef VNRS_PARSER_H
#define VNRS_PARSER_H

#include <stdio.h>

typedef struct {
    FILE *file;
    char *currentChunk;
    int lineNumber;
    int finished;
} Scene;

int SceneOpen(Scene *scene, const char *path);
int SceneReadNext(Scene *scene);
void SceneClose(Scene *scene);

#endif

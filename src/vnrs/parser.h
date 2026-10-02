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

typedef enum {
    chunk_none,
    chunk_dialog,
    chunk_chapter,
    chunk_choice,
    chunk_image,
    chunk_media,
    chunk_comment,
    chunk_directive,
    chunk_literal,
    chunk_empty
} ChunkType;

/* One piece of a scene. The strings belong to Scene and die on
   the next SceneReadNext or on SceneClose.
   speaker is NULL until the first name marker. A bare "::" stores
   "" (narrator, no name). The speaker stays until the next marker.
   path, attr, and marker are for choices, images, and media.
   A run of blank lines is one chunk_empty. */
typedef struct {
    ChunkType type;
    char *text;
    char *speaker;
    char *path;
    char *attr;
    char marker;
    int level;
    int lineNumber;
} Chunk;

typedef struct {
    FILE *file;
    Chunk chunk;
    char *speaker;
    char *pendingLine;
    int lineNumber;
    int finished;
} Scene;

int SceneOpen(Scene *scene, const char *path);
int SceneReadNext(Scene *scene);
void SceneClose(Scene *scene);

#endif

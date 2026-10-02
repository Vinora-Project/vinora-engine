/*
vnrs_parser.c -- print a scene as tagged lines.
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

#include "vnrs/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void PrintUsage(FILE *out, const char *argv0)
{
    fprintf(out, "Usage: %s file.vnrs\n", argv0);
}

static char *Dup(const char *s)
{
    size_t n = 0;
    char *p = NULL;

    n = strlen(s) + 1;
    p = (char *)malloc(n);
    if (!p)
        return NULL;
    memcpy(p, s, n);
    return p;
}

/* Longest tag is DIRECTIVE. Spaces keep the pipe in one column. */
#define TAG_WIDTH 9

static void PrintTag(const char *tag)
{
    printf("%-*s|", TAG_WIDTH, tag);
}

static void PrintText(const char *tag, const char *text)
{
    PrintTag(tag);
    if (text != NULL && text[0] != '\0') {
        fputc(' ', stdout);
        fputs(text, stdout);
    }
    fputc('\n', stdout);
}

static void PrintTitle(const Chunk *chunk)
{
    int i = 0;

    PrintTag("TITLE");
    fputc(' ', stdout);
    for (i = 0; i < chunk->level; i++)
        fputc('#', stdout);
    if (chunk->text != NULL && chunk->text[0] != '\0') {
        fputc(' ', stdout);
        fputs(chunk->text, stdout);
    }
    fputc('\n', stdout);
}

static const char *ChunkTag(ChunkType type)
{
    switch (type) {
    case chunk_dialog:
        return "DIALOG";
    case chunk_chapter:
        return "TITLE";
    case chunk_choice:
        return "CHOICE";
    case chunk_image:
        return "IMAGE";
    case chunk_media:
        return "MEDIA";
    case chunk_comment:
        return "COMMENT";
    case chunk_directive:
        return "DIRECTIVE";
    case chunk_literal:
        return "LITERAL";
    case chunk_empty:
        return "EMPTY";
    default:
        return "NONE";
    }
}

/* 1 on success, 0 if the speaker copy runs out of memory. */
static int PrintChunk(const Chunk *chunk, char **shown)
{
    char *copy = NULL;

    if (chunk->speaker != NULL) {
        if (*shown == NULL || strcmp(*shown, chunk->speaker) != 0) {
            copy = Dup(chunk->speaker);
            if (!copy) {
                fprintf(stderr,
                    "[Vinora] Out of memory reading scene\n");
                return 0;
            }
            free(*shown);
            *shown = copy;
            PrintText("NAME", chunk->speaker);
        }
    }

    if (chunk->type == chunk_chapter)
        PrintTitle(chunk);
    else
        PrintText(ChunkTag(chunk->type), chunk->text);
    return 1;
}

int main(int argc, char **argv)
{
    Scene scene = {0};
    char *shown = NULL;
    int ok = 1;

    if (argc != 2) {
        PrintUsage(stderr, argv[0]);
        return 1;
    }

    if (!SceneOpen(&scene, argv[1]))
        return 1;

    while (SceneReadNext(&scene)) {
        if (!PrintChunk(&scene.chunk, &shown)) {
            ok = 0;
            break;
        }
    }
    if (!scene.finished)
        ok = 0;

    free(shown);
    SceneClose(&scene);
    return ok ? 0 : 1;
}

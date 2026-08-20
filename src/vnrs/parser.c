/*
parser.c -- Vinora Screenplay scene reader
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

#include "parser.h"

#include "raylib.h"

#include <stdlib.h>
#include <string.h>

#define VNRS_LINE_MAX 512
#define VNRS_LINE_WARN 80

static void StripNewline(char *s)
{
    size_t n = strlen(s);

    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}

static int IsWhitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\v' || c == '\f');
}

static int LineIsEmpty(const char *s)
{
    while (*s) {
        if (!IsWhitespace(*s)) return 0;
        s++;
    }
    return 1;
}

static int Utf8CharCount(const char *s)
{
    int n = 0;
    const unsigned char *p = (const unsigned char *)s;

    while (*p) {
        int step = 1;
        int i = 0;

        if (*p < 0x80) step = 1;
        else if (*p < 0xE0) step = 2;
        else if (*p < 0xF0) step = 3;
        else step = 4;

        for (i = 1; i < step; i++) {
            if (p[i] == '\0') {
                step = i;
                break;
            }
        }
        p += step;
        n++;
    }
    return n;
}

static void WarnIfLong(int lineNumber, const char *line)
{
    int chars = Utf8CharCount(line);

    if (chars > VNRS_LINE_WARN) {
        TraceLog(LOG_WARNING,
            "[Vinora] Line %d is %d characters (limit is %d)",
            lineNumber, chars, VNRS_LINE_WARN);
    }
}

static int AppendLine(char **buf, size_t *len, const char *line)
{
    size_t lineLen = strlen(line);
    size_t extra = lineLen;
    int addSpace = (*len > 0);
    char *next = NULL;

    if (addSpace) extra += 1;

    next = (char *)realloc(*buf, *len + extra + 1);
    if (!next) {
        TraceLog(LOG_ERROR, "[Vinora] Out of memory reading scene");
        return 0;
    }

    *buf = next;
    if (addSpace) {
        (*buf)[*len] = ' ';
        *len += 1;
    }
    memcpy(*buf + *len, line, lineLen + 1);
    *len += lineLen;
    return 1;
}

int SceneOpen(Scene *scene, const char *path)
{
    memset(scene, 0, sizeof(*scene));
    scene->file = fopen(path, "r");
    if (!scene->file) {
        TraceLog(LOG_ERROR, "[Vinora] Cannot open scene: %s", path);
        scene->finished = 1;
        return 0;
    }
    return 1;
}

int SceneReadNext(Scene *scene)
{
    char line[VNRS_LINE_MAX];
    char *buf = NULL;
    size_t bufLen = 0;
    int gotLine = 0;

    if (scene->currentChunk) {
        free(scene->currentChunk);
        scene->currentChunk = NULL;
    }

    if (!scene->file || scene->finished) return 0;

    while (fgets(line, (int)sizeof(line), scene->file) != NULL)
    {
        scene->lineNumber++;
        StripNewline(line);
        WarnIfLong(scene->lineNumber, line);
        if (!LineIsEmpty(line)) {
            gotLine = 1;
            break;
        }
    }

    if (!gotLine) {
        scene->finished = 1;
        return 0;
    }

    if (!AppendLine(&buf, &bufLen, line)) {
        free(buf);
        return 0;
    }

    while (fgets(line, (int)sizeof(line), scene->file) != NULL)
    {
        scene->lineNumber++;
        StripNewline(line);
        WarnIfLong(scene->lineNumber, line);
        if (LineIsEmpty(line)) break;
        if (!AppendLine(&buf, &bufLen, line)) {
            free(buf);
            return 0;
        }
    }

    scene->currentChunk = buf;
    return 1;
}

void SceneClose(Scene *scene)
{
    if (scene->file) {
        fclose(scene->file);
        scene->file = NULL;
    }
    if (scene->currentChunk) {
        free(scene->currentChunk);
        scene->currentChunk = NULL;
    }
    scene->finished = 1;
}

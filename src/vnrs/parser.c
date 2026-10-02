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

#include <stdio.h>
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
        if (!IsWhitespace(*s))
            return 0;
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

        if (*p < 0x80)
            step = 1;
        else if (*p < 0xE0)
            step = 2;
        else if (*p < 0xF0)
            step = 3;
        else
            step = 4;

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
        fprintf(stderr,
            "[Vinora] Line %d is %d characters (limit is %d)\n",
            lineNumber, chars, VNRS_LINE_WARN);
    }
}

static void TrimTrailing(char *s)
{
    size_t n = strlen(s);

    while (n > 0 && IsWhitespace(s[n - 1])) {
        s[n - 1] = '\0';
        n--;
    }
}

static char *Dup(const char *s)
{
    size_t n = 0;
    char *p = NULL;

    if (!s)
        return NULL;
    n = strlen(s) + 1;
    p = (char *)malloc(n);
    if (!p) {
        fprintf(stderr, "[Vinora] Out of memory reading scene\n");
        return NULL;
    }
    memcpy(p, s, n);
    return p;
}

static void ClearChunk(Chunk *chunk)
{
    free(chunk->text);
    free(chunk->speaker);
    free(chunk->path);
    free(chunk->attr);
    memset(chunk, 0, sizeof(*chunk));
}

static int TakeLine(Scene *scene, char *line, int cap)
{
    size_t n = 0;

    if (scene->pendingLine) {
        n = strlen(scene->pendingLine);
        if ((int)n >= cap)
            n = (size_t)cap - 1;
        memcpy(line, scene->pendingLine, n);
        line[n] = '\0';
        free(scene->pendingLine);
        scene->pendingLine = NULL;
        return 1;
    }

    if (!scene->file)
        return 0;
    if (fgets(line, cap, scene->file) == NULL)
        return 0;

    scene->lineNumber++;
    StripNewline(line);
    WarnIfLong(scene->lineNumber, line);
    return 1;
}

static int PushLine(Scene *scene, const char *line)
{
    char *copy = NULL;

    copy = Dup(line);
    if (!copy)
        return 0;
    free(scene->pendingLine);
    scene->pendingLine = copy;
    return 1;
}

static int ChapterLevel(const char *line)
{
    int n = 0;

    if (line[0] != '#')
        return 0;

    while (line[n] == '#' && n < 7)
        n++;
    if (n > 6)
        return 0;
    if (line[n] != '\0' && line[n] != ' ' && line[n] != '\t')
        return 0;
    return n;
}

static const char *ChapterTitle(const char *line, int level)
{
    const char *p = line + level;

    while (*p == ' ' || *p == '\t')
        p++;
    return p;
}

static int SpeakerName(const char *line, char *name, int nameCap)
{
    const char *start = line;
    const char *end = NULL;
    int nameLen = 0;

    while (IsWhitespace(*start))
        start++;

    end = start + strlen(start);
    while (end > start && IsWhitespace(*(end - 1)))
        end--;

    if ((end - start) < 2)
        return 0;
    if (*(end - 1) != ':' || *(end - 2) != ':')
        return 0;

    nameLen = (int)(end - start - 2);
    while (nameLen > 0 && IsWhitespace(start[nameLen - 1]))
        nameLen--;

    if (nameLen >= nameCap)
        nameLen = nameCap - 1;
    memcpy(name, start, (size_t)nameLen);
    name[nameLen] = '\0';
    return 1;
}

static int SetSpeaker(Scene *scene, const char *name)
{
    char *copy = NULL;

    copy = Dup(name);
    if (!copy)
        return 0;
    free(scene->speaker);
    scene->speaker = copy;
    return 1;
}

static int AppendLine(char **buf, size_t *len, const char *line)
{
    size_t lineLen = strlen(line);
    size_t extra = lineLen;
    int addSpace = (*len > 0);
    char *next = NULL;

    if (addSpace)
        extra += 1;

    next = (char *)realloc(*buf, *len + extra + 1);
    if (!next) {
        fprintf(stderr, "[Vinora] Out of memory reading scene\n");
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

static int ReadBody(Scene *scene, const char *first, char **out,
    int *startLine)
{
    char line[VNRS_LINE_MAX];
    char name[VNRS_LINE_MAX];
    char *buf = NULL;
    size_t len = 0;

    *startLine = scene->lineNumber;
    if (!AppendLine(&buf, &len, first)) {
        free(buf);
        return 0;
    }

    for (;;) {
        if (!TakeLine(scene, line, (int)sizeof(line)))
            break;
        if (LineIsEmpty(line)) {
            if (!PushLine(scene, line)) {
                free(buf);
                return 0;
            }
            break;
        }
        if (ChapterLevel(line) != 0 ||
            SpeakerName(line, name, (int)sizeof(name))) {
            if (!PushLine(scene, line)) {
                free(buf);
                return 0;
            }
            break;
        }
        if (!AppendLine(&buf, &len, line)) {
            free(buf);
            return 0;
        }
    }

    *out = buf;
    return 1;
}

static int FillChunk(Scene *scene, ChunkType type, char *text,
    int level, int lineNumber)
{
    char *speaker = NULL;

    if (scene->speaker) {
        speaker = Dup(scene->speaker);
        if (!speaker) {
            free(text);
            return 0;
        }
    }

    scene->chunk.type = type;
    scene->chunk.text = text;
    scene->chunk.speaker = speaker;
    scene->chunk.level = level;
    scene->chunk.lineNumber = lineNumber;
    return 1;
}

int SceneOpen(Scene *scene, const char *path)
{
    memset(scene, 0, sizeof(*scene));
    scene->file = fopen(path, "r");
    if (!scene->file) {
        fprintf(stderr, "[Vinora] Cannot open scene: %s\n", path);
        scene->finished = 1;
        return 0;
    }
    return 1;
}

int SceneReadNext(Scene *scene)
{
    char line[VNRS_LINE_MAX];
    char name[VNRS_LINE_MAX];
    char *text = NULL;
    int level = 0;
    int startLine = 0;

    ClearChunk(&scene->chunk);

    if (!scene->file || scene->finished)
        return 0;

    for (;;) {
        text = NULL;
        level = 0;
        startLine = 0;

        if (!TakeLine(scene, line, (int)sizeof(line))) {
            scene->finished = 1;
            return 0;
        }
        if (LineIsEmpty(line)) {
            int emptyLine = scene->lineNumber;

            for (;;) {
                if (!TakeLine(scene, line, (int)sizeof(line)))
                    break;
                if (!LineIsEmpty(line)) {
                    if (!PushLine(scene, line))
                        return 0;
                    break;
                }
            }
            if (!FillChunk(scene, chunk_empty, NULL, 0, emptyLine))
                return 0;
            return 1;
        }

        level = ChapterLevel(line);
        if (level > 0) {
            text = Dup(ChapterTitle(line, level));
            if (!text)
                return 0;
            TrimTrailing(text);
            if (!FillChunk(scene, chunk_chapter, text, level,
                scene->lineNumber)) {
                return 0;
            }
            return 1;
        }

        if (SpeakerName(line, name, (int)sizeof(name))) {
            if (!SetSpeaker(scene, name))
                return 0;
            if (!TakeLine(scene, line, (int)sizeof(line)))
                continue;
            if (LineIsEmpty(line)) {
                if (!PushLine(scene, line))
                    return 0;
                continue;
            }
            if (ChapterLevel(line) != 0 ||
                SpeakerName(line, name, (int)sizeof(name))) {
                if (!PushLine(scene, line))
                    return 0;
                continue;
            }
            if (!ReadBody(scene, line, &text, &startLine))
                return 0;
            if (!FillChunk(scene, chunk_dialog, text, 0, startLine))
                return 0;
            return 1;
        }

        if (!ReadBody(scene, line, &text, &startLine))
            return 0;
        if (!FillChunk(scene, chunk_dialog, text, 0, startLine))
            return 0;
        return 1;
    }
}

void SceneClose(Scene *scene)
{
    if (scene->file) {
        fclose(scene->file);
        scene->file = NULL;
    }
    ClearChunk(&scene->chunk);
    free(scene->speaker);
    scene->speaker = NULL;
    free(scene->pendingLine);
    scene->pendingLine = NULL;
    scene->finished = 1;
}

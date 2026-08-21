/*
font_manager.c -- font manage functions for Vinora Engine
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

#include "font_manager.h"

#include <stdlib.h>

typedef struct {
    int start;
    int end;
    const char *name;
} UnicodeBlock;

static int *GenerateFontCodepoints(int *outCount)
{
    UnicodeBlock blocks[] = {
        {0, 0xFFFF, "Unicode Basic Multilingual Plane"},
        // TODO: Probably should be dynamically calculated all of this based
        // on text.
    };
    int blockCount = (int)(sizeof(blocks) / sizeof(blocks[0]));
    int totalCapacity = 0;
    int *codepoints = NULL;
    int index = 0;
    int i = 0;

    for (i = 0; i < blockCount; i++) {
        totalCapacity += (blocks[i].end - blocks[i].start + 1);
    }

    codepoints = (int *)malloc(sizeof(int) * (size_t)totalCapacity);
    if (codepoints == NULL) {
        TraceLog(LOG_ERROR, "[Vinora] Language font loading error!");
        *outCount = 0;
        return NULL;
    }

    for (i = 0; i < blockCount; i++) {
        int cp = 0;
        TraceLog(LOG_INFO,
            "[Vinora] Adding codepoint range: %s (0x%04X - 0x%04X)",
            blocks[i].name, blocks[i].start, blocks[i].end);

        for (cp = blocks[i].start; cp <= blocks[i].end; cp++) {
            codepoints[index++] = cp;
        }
    }

    *outCount = index;
    return codepoints;
}

Font LoadGameFont(const char *path, int fontSize)
{
    int codepointCount = 0;
    int *codepoints = GenerateFontCodepoints(&codepointCount);
    Font font = LoadFontEx(path, fontSize, codepoints, codepointCount);

    free(codepoints);
    if (font.texture.id != 0) {
        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    }
    return font;
}

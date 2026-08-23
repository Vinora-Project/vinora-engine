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
#include "bits.h"

#include <stdlib.h>

#define BMP_LAST_CODEPOINT 0xFFFF
#define BMP_CODEPOINT_COUNT (BMP_LAST_CODEPOINT + 1)
#define BMP_BYTE_COUNT (BMP_CODEPOINT_COUNT / 8)

static void MarkUtf8Codepoints(unsigned char *used, int usedSize,
    const char *text)
{
    int i = 0;

    if (!text)
        return;

    while (text[i] != '\0') {
        int size = 0;
        int codepoint = GetCodepoint(&text[i], &size);

        if (size <= 0)
            size = 1;
        if (codepoint >= 32)
            SetBit(used, usedSize, codepoint);
        i += size;
    }
}

static int *GenerateFontCodepoints(const char *scenePath, int *outCount)
{
    unsigned char used[BMP_BYTE_COUNT] = {0};
    char *text = NULL;
    int *codepoints = NULL;
    int count = 0;
    int index = 0;
    int cp = 0;

    *outCount = 0;

    if (scenePath)
        text = LoadFileText(scenePath);
    if (text) {
        MarkUtf8Codepoints(used, BMP_BYTE_COUNT, text);
        UnloadFileText(text);
    } else {
        TraceLog(LOG_WARNING,
            "[Vinora] Cannot read scene for font glyphs: %s",
            scenePath ? scenePath : "(null)");
    }

    for (cp = 0; cp < BMP_CODEPOINT_COUNT; cp++) {
        if (IsBitSet(used, BMP_BYTE_COUNT, cp))
            count++;
    }

    if (count <= 0) {
        for (cp = 32; cp <= 126; cp++)
            SetBit(used, BMP_BYTE_COUNT, cp);
        count = 95;
    }

    codepoints = (int *)malloc(sizeof(int) * (size_t)count);
    if (codepoints == NULL) {
        TraceLog(LOG_ERROR, "[Vinora] Language font loading error!");
        return NULL;
    }

    for (cp = 0; cp < BMP_CODEPOINT_COUNT; cp++) {
        if (IsBitSet(used, BMP_BYTE_COUNT, cp))
            codepoints[index++] = cp;
    }

    TraceLog(LOG_INFO, "[Vinora] Font glyphs: %d codepoints", index);
    *outCount = index;
    return codepoints;
}

Font LoadGameFont(const char *path, int fontSize, const char *scenePath)
{
    int codepointCount = 0;
    int *codepoints = GenerateFontCodepoints(scenePath, &codepointCount);
    Font font = LoadFontEx(path, fontSize, codepoints, codepointCount);

    free(codepoints);
    if (font.texture.id != 0) {
        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    }
    return font;
}

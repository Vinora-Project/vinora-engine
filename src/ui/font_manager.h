/*
font_manager.h -- font manage functions for Vinora Engine
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

#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <stdlib.h>
#include "raylib.h"

typedef struct {
    int start;
    int end;
    const char *name;
} UnicodeBlock;

int *GenerateFontCodepoints(int *outCount) 
{
    UnicodeBlock blocks[] = {
        { 32, 126, "ASCII (English, Numbers, Punctuation)" },
        { 0x0400, 0x04FF, "Cyrillic (Russian)" },
        { 0x0590, 0x05FF, "Hebrew" },
        { 0x3000, 0x303F, "Japanese CJK Symbols and Punctuation" },
        { 0x3040, 0x309F, "Japanese Hiragana" },
        { 0x30A0, 0x30FF, "Japanese Katakana" },
        { 0x4E00, 0x9FFF, "CJK Unified Ideographs (Japanese Kanji / Chinese)" }
    };
    int blockCount = sizeof(blocks) / sizeof(blocks[0]);

    int totalCapacity = 0;
    for (int i = 0; i < blockCount; i++) {
        totalCapacity += (blocks[i].end - blocks[i].start + 1);
    }

    int *codepoints = (int *)malloc(sizeof(int) * totalCapacity);
    if (codepoints == NULL) {
        TraceLog(LOG_ERROR, "[Vinora] Language font loading error!");
        *outCount = 0;
        return NULL;
    }

    int index = 0;
    for (int i = 0; i < blockCount; i++) {
        TraceLog(LOG_INFO, 
                "[Vinora] Adding codepoint range: %s (0x%04X - 0x%04X)", 
                blocks[i].name, blocks[i].start, blocks[i].end);
        
        for (int cp = blocks[i].start; cp <= blocks[i].end; cp++) {
            codepoints[index++] = cp;
        }
    }

    *outCount = index; 
    return codepoints;
}

#endif

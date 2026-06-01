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

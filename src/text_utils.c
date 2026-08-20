/*
text_utils.c -- text utils functions for Vinora Engine.
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

#include "text_utils.h"

#include <stdlib.h>
#include <string.h>

char *StrDup(const char *s)
{
    size_t n = 0;
    char *p = NULL;

    if (!s) return NULL;
    n = strlen(s) + 1;
    p = malloc(n);
    if (!p) return NULL;
    memcpy(p, s, n);
    return p;
}

char *WrapText(Font font, const char *text, float fontSize,
               float spacing, float maxWidth)
{
    int origLen = 0;
    char *result = NULL;
    int i = 0;
    int resultLen = 0;
    int lastSpaceIdx = -1;
    int lineStart = 0;

    if (!text || text[0] == '\0') return StrDup("");

    origLen = (int)strlen(text);
    result = malloc((size_t)origLen*2 + 8);
    if (!result) return NULL;
    memcpy(result, text, (size_t)origLen + 1);

    resultLen = origLen;

    while (i < resultLen)
    {
        unsigned char c = (unsigned char)result[i];
        int cpLen = 0;
        char saved = 0;
        Vector2 size = { 0 };

        if (c == '\n') {
            lineStart = i + 1;
            lastSpaceIdx = -1;
            i++;
            continue;
        }

        GetCodepoint(&result[i], &cpLen);
        if (cpLen <= 0) {
            i++;
            continue;
        }

        if (c == ' ') lastSpaceIdx = i;

        saved = result[i + cpLen];
        result[i + cpLen] = '\0';
        size = MeasureTextEx(font, &result[lineStart], fontSize, spacing);
        result[i + cpLen] = saved;

        if (size.x > maxWidth) {
            if (lastSpaceIdx >= lineStart) {
                result[lastSpaceIdx] = '\n';
                lineStart = lastSpaceIdx + 1;
                lastSpaceIdx = -1;
                i = lineStart;
            } else if (i > lineStart) {
                memmove(&result[i + 1], &result[i],
                        (size_t)(resultLen - i + 1));
                result[i] = '\n';
                resultLen++;
                lineStart = i + 1;
                lastSpaceIdx = -1;
                i = lineStart;
            } else {
                // Even a single glyph does not fit.
                i += cpLen;
            }
        } else {
            i += cpLen;
        }
    }

    return result;
}

int GetUtf8ByteLength(const char *text, int count)
{
    int byteLen = 0;
    int glyphCount = 0;

    if (!text || count <= 0) return 0;

    while ((text[byteLen] != '\0') && (glyphCount < count))
    {
        int currentCharLen = 1;
        int codepoint = GetCodepoint(&text[byteLen], &currentCharLen);

        if (currentCharLen <= 0) currentCharLen = 1;

        // Characters beyond BMP (0xFFFF) are unsupported by spec.
        if (codepoint > 0xFFFF) {
            byteLen += currentCharLen;
            continue;
        }

        byteLen += currentCharLen;
        glyphCount++;
    }

    return byteLen;
}

int CountUtf8Glyphs(const char *text)
{
    int i = 0;
    int count = 0;

    if (!text) return 0;

    while (text[i] != '\0')
    {
        int size = 0;
        int codepoint = GetCodepoint(&text[i], &size);

        if (size <= 0) size = 1;
        if (codepoint <= 0xFFFF) count++;
        i += size;
    }

    return count;
}

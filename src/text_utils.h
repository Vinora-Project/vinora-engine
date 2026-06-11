/*
text_utils.h -- text utils functions for Vinora Engine.
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

#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H
#include <string.h>
#include <stdlib.h>


static int utfCharLength(unsigned char first_byte)
{
    if (first_byte < 0x80)        return 1;   // 0xxxxxxx
    if (first_byte < 0xE0)        return 2;   // 110xxxxx
    if (first_byte < 0xF0)        return 3;   // 1110xxxx
    if (first_byte < 0xF5)        return 4;   // 11110xxx

    return 0;
}

static char *my_strdup(const char *s)
{
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (!p) return NULL;
    memcpy(p, s, n);
    return p;
}

static char *WrapText(Font font, const char *text, float fontSize,
                                 float spacing,    float maxWidth)
{
    if (!text || text[0] == '\0') return my_strdup("");

    int orig_len = (int)strlen(text);
    char *result = malloc(orig_len * 2 + 8);
    if (!result) return NULL;
    memcpy(result, text, orig_len + 1);

    int i               = 0;
    int resultLen       = orig_len;
    int lastSpaceIdx    = -1;
    int lineStart       = 0;

    while (i < resultLen) {
        unsigned char c = (unsigned char)result[i];

        if (c == '\n') {
            lineStart    = i + 1;
            lastSpaceIdx = -1;
            i++;
            continue;
        }

        int cpLen = utfCharLength(c);
        if (cpLen <= 0) { i++; continue; }

        if (c == ' ')
            lastSpaceIdx = i;

        char saved = result[i + cpLen];
        result[i + cpLen] = '\0';
        Vector2 size = MeasureTextEx(font, &result[lineStart], fontSize,
                                     spacing);
        result[i + cpLen] = saved;

        if (size.x > maxWidth) {
            if (lastSpaceIdx >= lineStart) {
                result[lastSpaceIdx] = '\n';
                lineStart    = lastSpaceIdx + 1;
                lastSpaceIdx = -1;
                i = lineStart;
            }
            else if (i > lineStart) {
                memmove(&result[i + 1], &result[i], resultLen - i + 1);
                result[i] = '\n';
                resultLen++;
                lineStart    = i + 1;
                lastSpaceIdx = -1;
                i = lineStart;
            }
            else // Even a single symbol can't be displayed
                i += cpLen;
        }
        else
            i += cpLen;
    }

    return result;
}

static int GetUtf8ByteLength(const char *text, int count)
{
    if (!text || count <= 0) return 0;

    int byte_len = 0;
    int glyph_count = 0;

    while (text[byte_len] != '\0' && glyph_count < count) {
        int current_char_len = 1;
        int codepoint = GetCodepoint(&text[byte_len], &current_char_len);

        // Broken byte
        if (current_char_len <= 0) current_char_len = 1;

        // Characters beyond BMP (0xFFFF) are unsupported by spec.
        if (codepoint > 0xFFFF) {
            byte_len += current_char_len; 
            continue; 
        }

        byte_len += current_char_len;
        glyph_count++;
    }

    return byte_len;
}

#endif


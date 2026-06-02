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

    char *result = my_strdup(text);
    if (!result) return NULL;

    int length = strlen(result);
    int lastSpaceIdx = -1;
    int currentLineStartIdx = 0;

    for (int i = 0; i < length; i++)
    {
        if (result[i] == ' ') {
            lastSpaceIdx = i;
        }
        else if (result[i] == '\n') {
            currentLineStartIdx = i + 1;
            lastSpaceIdx = -1;
            continue;
        }

        char savedChar = result[i + 1];
        result[i + 1] = '\0';

        Vector2 size = MeasureTextEx(font, &result[currentLineStartIdx],
                                     fontSize, spacing);
        
        result[i + 1] = savedChar;

        if (size.x > maxWidth)
        {
            if (lastSpaceIdx != -1 && lastSpaceIdx >= currentLineStartIdx)
            {
                result[lastSpaceIdx] = '\n';
                currentLineStartIdx = lastSpaceIdx + 1;
                lastSpaceIdx = -1;
                i = currentLineStartIdx - 1; 
            }
            else
            {
                if (i > currentLineStartIdx) 
                {
                    result[i] = '\n';
                    currentLineStartIdx = i;
                    lastSpaceIdx = -1;
                }
            }
        }
    }

    return result;
}

static int GetUtf8ByteLength(const char *text, int count)
{
    if (!text || count <= 0) return 0;

    int byte_len = 0;
    int glyph_count = 0;

    while (text[byte_len] != '\0' && glyph_count < count)
    {
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


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
        unsigned char c = (unsigned char)text[byte_len];
        int current_char_len = 1;

        if (c >= 0xF0)      current_char_len = 4; // Emoji (unsupported) 
        else if (c >= 0xE0) current_char_len = 3; // CJK
        else if (c >= 0xC0) current_char_len = 2; // Cyryllic
        else                current_char_len = 1; // ASCII

        for (int i = 0; i < current_char_len; i++) {
            if (text[byte_len + i] == '\0') {
                return byte_len;
            }
        }

        // Skipping emojis...
        if (current_char_len == 4) {
            byte_len += 4; 
            continue; 
        }

        byte_len += current_char_len;
        glyph_count++;
    }

    return byte_len;
}

#endif


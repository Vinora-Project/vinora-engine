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

        Vector2 size = MeasureTextEx(font, &result[currentLineStartIdx], fontSize, spacing);
        
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


#endif


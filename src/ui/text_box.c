/*
text_box.c -- functions for text boxes of Vinora Engine
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

#include "text_box.h"
#include "text_utils.h"

#include <stdlib.h>
#include <string.h>

void TextBoxInit(TextBox *tb, Rectangle rect, Font gameFont)
{
    memset(tb, 0, sizeof(TextBox));
    tb->rect = rect;
    tb->padding = 20.0f;

    tb->bgColor = (Color){ 23, 28, 37, 220 };
    tb->borderColor = (Color){ 25, 31, 40, 180 };
    tb->borderThickness = 3;

    tb->font = gameFont;
    tb->fontSize = 24.0f;
    tb->baseFontSize = 24.0f;
    tb->fontSpacing = 1.0f;
    tb->textColor = WHITE;

    tb->typingSpeed = 45.0f;
    tb->timer = 0.0f;
}

void TextBoxCleanup(TextBox *tb)
{
    if (tb->text) free(tb->text);
    if (tb->wrappedText) free(tb->wrappedText);
    tb->text = NULL;
    tb->wrappedText = NULL;
    tb->glyphCount = 0;
}

void TextBoxReflow(TextBox *tb)
{
    float minSize = 16.0f;
    float currentSize = 0.0f;
    float maxWidth = 0.0f;
    float maxHeight = 0.0f;
    char *tempWrapped = NULL;

    if (!tb->text || tb->text[0] == '\0') {
        if (tb->wrappedText) {
            free(tb->wrappedText);
            tb->wrappedText = NULL;
        }
        tb->glyphCount = 0;
        return;
    }

    currentSize = tb->baseFontSize;
    maxWidth = tb->rect.width - (tb->padding*2);
    maxHeight = tb->rect.height - (tb->padding*2);

    while (1)
    {
        Vector2 textSize = { 0 };

        if (tempWrapped) {
            free(tempWrapped);
            tempWrapped = NULL;
        }
        tempWrapped = WrapText(tb->font, tb->text, currentSize,
                               tb->fontSpacing, maxWidth);
        if (!tempWrapped) break;

        textSize = MeasureTextEx(tb->font, tempWrapped,
                                 currentSize, tb->fontSpacing);
        if (textSize.y <= maxHeight) break;
        if (currentSize <= minSize) {
            TraceLog(LOG_WARNING,
                "[Vinora] Text too long! Even with %f fontSize!",
                currentSize);
            break;
        }
        currentSize -= 1.0f;
    }

    tb->fontSize = currentSize;
    if (tb->wrappedText) free(tb->wrappedText);
    tb->wrappedText = tempWrapped;
    tb->glyphCount = CountUtf8Glyphs(tb->wrappedText);
}

void TextBoxUpdate(TextBox *tb, float dt)
{
    tb->timer += dt;
}

void TextBoxSetText(TextBox *tb, const char *text)
{
    if (tb->text) free(tb->text);
    tb->text = StrDup(text);
    tb->timer = 0.0f;
    TextBoxReflow(tb);
}

void TextBoxSkip(TextBox *tb)
{
    if (tb->typingSpeed > 0.0f) {
        tb->timer = (float)tb->glyphCount/tb->typingSpeed;
    }
}

int TextBoxIsComplete(const TextBox *tb)
{
    int shown = 0;

    if (!tb->wrappedText || tb->wrappedText[0] == '\0') return 1;
    if (tb->typingSpeed <= 0.0f) return 1;

    shown = (int)(tb->timer*tb->typingSpeed);
    return (shown >= tb->glyphCount);
}

void TextBoxDraw(const TextBox *tb)
{
    DrawRectangleRec(tb->rect, tb->bgColor);
    if (tb->borderThickness > 0) {
        DrawRectangleLinesEx(tb->rect, tb->borderThickness, tb->borderColor);
    }

    if (tb->wrappedText && tb->wrappedText[0] != '\0')
    {
        int maxGlyphs = 0;
        int byteLen = 0;
        const char *visibleText = NULL;
        Vector2 textPos = { 0 };

        if (tb->typingSpeed > 0.0f) {
            maxGlyphs = (int)(tb->timer*tb->typingSpeed);
        } else {
            maxGlyphs = tb->glyphCount;
        }
        byteLen = GetUtf8ByteLength(tb->wrappedText, maxGlyphs);
        visibleText = TextSubtext(tb->wrappedText, 0, byteLen);

        textPos.x = tb->rect.x + tb->padding;
        textPos.y = tb->rect.y + tb->padding;

        DrawTextEx(tb->font, visibleText, textPos,
                   tb->fontSize, tb->fontSpacing, tb->textColor);
    }
}

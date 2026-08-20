/*
text_box.h -- header file for text boxes of Vinora Engine
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

#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "raylib.h"

typedef struct
{
    Rectangle rect;
    float padding;

    Color bgColor;
    Color borderColor;
    int borderThickness;

    Font font;
    float fontSize;
    float baseFontSize;
    float fontSpacing;
    Color textColor;

    char *text;
    char *wrappedText;
    int glyphCount;
    float typingSpeed; // Characters per second
    float timer;       // Seconds since the current text was set
} TextBox;

void TextBoxInit(TextBox *tb, Rectangle rect, Font gameFont);
void TextBoxCleanup(TextBox *tb);
void TextBoxReflow(TextBox *tb);
void TextBoxUpdate(TextBox *tb, float dt);
void TextBoxSetText(TextBox *tb, const char *text);
void TextBoxSkip(TextBox *tb);
int TextBoxIsComplete(const TextBox *tb);
void TextBoxDraw(const TextBox *tb);

#endif

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

    Color bg_color;
    Color border_color;
    int   border_thickness;
    
    Font  font;
    float font_size;
    float base_font_size;
    float font_spacing;
    Color text_color;
    
    char *text;
    char *wrapped_text;
    float typing_speed; // Characters per second
    float timer;        // How much seconds passed
} TextBox;

void TextBoxReflow(TextBox *tb);

void TextBoxInit(TextBox *tb, Rectangle rect, Font gameFont);
void TextBoxCleanup(TextBox *tb);

void TextBoxUpdate(TextBox *tb, float dt);
void TextBoxSetText(TextBox *tb, const char *text);
void TextBoxDraw(const TextBox *tb);
#endif

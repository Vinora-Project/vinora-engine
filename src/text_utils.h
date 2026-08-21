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

#include "raylib.h"

char *StrDup(const char *s);
char *WrapText(Font font, const char *text, float fontSize,
    float spacing, float maxWidth);
int GetUtf8ByteLength(const char *text, int count);
int CountUtf8Glyphs(const char *text);
bool EndsWith(const char *str, const char *suffix);

#endif

#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "raylib.h"

typedef struct
{
    Rectangle rect;
    Color bg_color;
    Color border_color;
    int border_thickness;

    Font font;
    int font_size;
    Color text_color;

    char name[256];
    Color name_color;

    char *text;
    int max_text_length;
} TextBox;

void TextBoxInit(TextBox *tb, int screenWidth, int screenHeight);
void TextBoxSetText(TextBox *tb, const char *name, const char *text);
void TextBoxDraw(TextBox *tb);
void TextBoxCleanup(TextBox *tb);

#endif

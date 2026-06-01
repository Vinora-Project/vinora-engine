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

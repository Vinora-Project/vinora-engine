#include "text_box.h"
#include <stdlib.h>
#include <string.h>

static char *strdup(const char *s)
{
    if (!s)
        return NULL;
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (!p)
        return NULL;
    memcpy(p, s, n);
    return p;
}

void TextBoxInit(TextBox *tb, int screenWidth, int screenHeight)
{
    tb->rect = (Rectangle){
        .x = 40,
        .y = screenHeight - 180,
        .width = screenWidth - 80,
        .height = 160};

    tb->bg_color = (Color){0, 0, 0, 220};
    tb->border_color = (Color){255, 255, 255, 180};
    tb->border_thickness = 3;

    tb->font = GetFontDefault();
    tb->font_size = 24;
    tb->text_color = WHITE;
    tb->name_color = YELLOW;

    memset(tb->name, 0, sizeof(tb->name));
    tb->text = NULL;
    tb->max_text_length = 1024;
}

void TextBoxSetText(TextBox *tb, const char *name, const char *text)
{
    if (name)
        strncpy(tb->name, name, 63);
    else
        tb->name[0] = '\0';

    if (tb->text)
        free(tb->text);
    tb->text = strdup(text ? text : "");
}

void TextBoxDraw(TextBox *tb)
{
    DrawRectangleRec(tb->rect, tb->bg_color);
    DrawRectangleLinesEx(tb->rect, tb->border_thickness, tb->border_color);

    if (tb->name[0] != '\0')
    {
        Vector2 nameSize = MeasureTextEx(tb->font, tb->name,
                                         tb->font_size + 2, 1);
        Rectangle nameRect = {
            tb->rect.x + 20,
            tb->rect.y - 30,
            nameSize.x + 24,
            32};

        DrawRectangleRec(nameRect, tb->bg_color);
        DrawRectangleLinesEx(nameRect, 3, tb->border_color);

        DrawTextEx(tb->font, tb->name,
                   (Vector2){nameRect.x + 12, nameRect.y + 6},
                   tb->font_size + 2, 1, tb->name_color);
    }

    if (tb->text)
    {
        DrawTextEx(tb->font, tb->text,
                   (Vector2){tb->rect.x + 25, tb->rect.y + 25},
                   tb->font_size, 1.2f, tb->text_color);
    }
}

void TextBoxCleanup(TextBox *tb)
{
    if (tb->text)
    {
        free(tb->text);
        tb->text = NULL;
    }
}

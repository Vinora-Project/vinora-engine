#include "text_box.h"
#include "text_utils.h"
#include <stdlib.h>
#include <string.h>

void TextBoxInit(TextBox *tb, int screenWidth, int screenHeight)
{
    tb->rect = (Rectangle){
        .x = 40,
        .y = screenHeight - 180,
        .width = screenWidth - 80,
        .height = 160};

    tb->bg_color = (Color){23, 28, 37, 220};
    tb->border_color = (Color){25, 31, 40, 180};
    tb->border_thickness = 3;
    // TODO: remake to use charset instead of 0-2048 range
    tb->font = LoadFontEx("assets/fonts/NotoSans-Regular.ttf", 32, NULL, 2048);
    SetTextureFilter(tb->font.texture, TEXTURE_FILTER_BILINEAR);
    tb->font_size = 24;
    tb->text_color = WHITE;
    tb->name_color = RED;

    tb->typing_speed = 25.0f;
    tb->timer = 0.0f;

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
    tb->text = my_strdup(text ? text : "");
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
                   (Vector2){nameRect.x + 12, nameRect.y + 3},
                   tb->font_size + 2, 1, tb->name_color);
    }

    if (tb->text)
    {
        DrawTextEx(tb->font, TextSubtext(tb->text, 0, tb->timer * tb->typing_speed),
                   (Vector2){tb->rect.x + 25, tb->rect.y + 25},
                   tb->font_size, 1.2f, tb->text_color);
    }
}

void TextBoxCleanup(TextBox *tb)
{
    UnloadFont(tb->font);
    if (tb->text)
    {
        free(tb->text);
        tb->text = NULL;
    }
}

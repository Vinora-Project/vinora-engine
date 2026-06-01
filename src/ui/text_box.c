#include "text_box.h"
#include "text_utils.h"
#include <stdlib.h>
#include <string.h>

void TextBoxInit(TextBox *tb, Rectangle rect, Font gameFont)
{
    memset(tb, 0, sizeof(TextBox));
    tb->rect = rect;
    tb->padding = 20.0f;

    tb->bg_color = (Color){23, 28, 37, 220};
    tb->border_color = (Color){25, 31, 40, 180};
    tb->border_thickness = 3;
    
    tb->font = gameFont;
    tb->font_size = 24;
    tb->font_spacing = 1.0f;
    tb->text_color = WHITE;

    tb->typing_speed = 45.0f;
    tb->timer = 0.0f;
    tb->text = NULL;
}

void TextBoxCleanup(TextBox *tb)
{
    if (tb->text) free(tb->text);
    if (tb->wrapped_text) free(tb->wrapped_text);
    tb->text = NULL;
    tb->wrapped_text = NULL;
}

void TextBoxReflow(TextBox *tb)
{
    if (tb->wrapped_text) {
        free(tb->wrapped_text);
        tb->wrapped_text = NULL;
    }

    if (tb->text) {
        float max_text_width = tb->rect.width - (tb->padding * 2);
        tb->wrapped_text = WrapText(tb->font, tb->text, tb->font_size,
                                    tb->font_spacing, max_text_width);
    }
}

void TextBoxUpdate(TextBox *tb, float dt)
{
    tb->timer += dt;
}


void TextBoxSetText(TextBox *tb, const char *text)
{
    if (tb->text)
        free(tb->text);
    if (text) {
        size_t len = strlen(text) + 1;
        tb->text = malloc(len);
        if (tb->text) {
            memcpy(tb->text, text, len);
        }
    } else
        tb->text = NULL;
    tb->timer = 0.0f;
    TextBoxReflow(tb);
}

void TextBoxDraw(const TextBox *tb)
{
    DrawRectangleRec(tb->rect, tb->bg_color);
    if (tb->border_thickness > 0)
        DrawRectangleLinesEx(tb->rect, tb->border_thickness, tb->border_color);
    if (tb->wrapped_text && tb->wrapped_text[0] != '\0')
    {
        int max_chars = (tb->typing_speed > 0) 
            ? (int)(tb->timer * tb->typing_speed)
            : (int)strlen(tb->wrapped_text);
        const char *visible_text = TextSubtext(tb->wrapped_text, 0, max_chars);

        Vector2 text_pos = {
            tb->rect.x + tb->padding,
            tb->rect.y + tb->padding
        };

        DrawTextEx(tb->font, visible_text, text_pos, 
                   tb->font_size, tb->font_spacing, tb->text_color);

    }
}


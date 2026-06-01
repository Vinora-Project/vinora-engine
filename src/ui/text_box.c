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
    tb->font_size = 24.0f;
    tb->base_font_size = 24.0f;
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
    if (!tb->text || tb->text[0] == '\0') {
        if (tb->wrapped_text) {
            free(tb->wrapped_text);
            tb->wrapped_text = NULL;
        }
        return;
    }
    float min_size = 16.0f;
    float current_size = tb->base_font_size;
    
    float max_width = tb->rect.width - (tb->padding * 2);
    float max_height = tb->rect.height - (tb->padding * 2);
    
    char *temp_wrapped = NULL;
    
    while (current_size > min_size)
    {
        if (temp_wrapped) {
            free(temp_wrapped);
            temp_wrapped = NULL;
        }
        temp_wrapped = WrapText(tb->font, tb->text, current_size,
                                tb->font_spacing, max_width);
        Vector2 text_size = MeasureTextEx(tb->font, temp_wrapped,
                                          current_size, tb->font_spacing);
        if (text_size.y <= max_height)
            break;
        current_size -= 1.0f;
    }
    tb->font_size = current_size;

    if (tb->wrapped_text)
        free(tb->wrapped_text);
    tb->wrapped_text = temp_wrapped;

    if (current_size < min_size)
        TraceLog(LOG_WARNING, 
            "[Vinora] Text too long! Even with %f font_size!", current_size);
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
        int max_glyphs = (tb->typing_speed > 0)
            ? (int)(tb->timer * tb->typing_speed)
            : (int)strlen(tb->wrapped_text);
        int byte_len = GetUtf8ByteLength(tb->wrapped_text, max_glyphs);
        const char *visible_text = TextSubtext(tb->wrapped_text, 0, byte_len);

        Vector2 text_pos = {
            tb->rect.x + tb->padding,
            tb->rect.y + tb->padding
        };

        DrawTextEx(tb->font, visible_text, text_pos, 
                   tb->font_size, tb->font_spacing, tb->text_color);

    }
}


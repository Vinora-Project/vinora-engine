/*
Vinora Engine -- simple and portable visual novels engine.
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

#include "raylib.h"
#include "ui/text_box.h"
#include "ui/font_manager.h"
#include <stdio.h>

int main(void)
{
    int screenWidth = 1280;
    int screenHeight = 720;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Vinora Engine");
    SetWindowMinSize(800, 600);
    SetTargetFPS(60);

    int codepointCount = 0;
    int *myCodepoints = GenerateFontCodepoints(&codepointCount);
    Font gameFont = LoadFontEx("assets/fonts/unifont.otf", 32, 
                                myCodepoints, codepointCount);
    free(myCodepoints);
    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);
    
    Rectangle box_rect = { 
        ((float)screenWidth - (float)screenWidth * 5.0f / 6.0f) / 2.0f, 
        (float)screenHeight - 180, 
        (float)screenWidth * 5.0f / 6.0f, 
        160 
    };
    TextBox dialogueBox;
    TextBoxInit(&dialogueBox, box_rect, gameFont);

    TextBoxSetText(&dialogueBox,
    // LINE 0: Digits
    "123456789 123456789 123456789 123456789 "
    "123456789 123456789 123456789 123456789_\n"
    // LINE 1: English (Latin)
    "\x4E\x65\x76\x65\x72\x20\x67\x6F\x6E\x6E\x61\x20\x67\x69\x76\x65"
    "\x20\x79\x6F\x75\x20\x75\x70\x2E\x0A"

    // LINE 2: Russian (Cyrillic)
    "\xD0\x92\xD1\x8B\x20\xD0\xBF\xD1\x80\xD0\xBE\xD0\xB4\xD0\xB0\xD1"
    "\x91\xD1\x82\xD0\xB5\x20\xD1\x80\xD1\x8B\xD0\xB1\xD0\xBE\xD0\xB2"
    "\x3F\x20\xD0\x9D\xD0\xB5\xD1\x82\x2C\x20\xD1\x82\xD0\xBE\xD0\xBB"
    "\xD1\x8C\xD0\xBA\xD0\xBE\x20\xD0\xBF\xD0\xBE\xD0\xBA\xD0\xB0\xD0"
    "\xB7\xD1\x8B\xD0\xB2\xD0\xB0\xD1\x8E\x2E\x20\xD0\x9A\xD1\x80\xD0"
    "\xB0\xD1\x81\xD0\xB8\xD0\xB2\xD0\xBE\xD0\xB5\x2E\x2E\x2E\x0A"

    // LINE 3: Korean (Hangul)
    "\xEB\xAC\xB4\xEA\xB5\x80\xED\x99\x94\x20\xEA\xBD\x83\xEC\x9D"
    "\xB4\x20\xED\x94\xBC\xEC\x97\x85\xEC\x8A\xB5\xEB\x8B\x88\xEB"
    "\x8B\xA4\x2E\x0A"

    // LINE 4: Japanese (CJK)
    "\xE9\xAD\x94\xE7\x8E\x8B\xE5\xAD\xA6\xE9\x99\xA2\xE3\x81\xAE"
    "\xE4\xB8\x8D\xE9\x81\xA9\xE5\x90\x88\xE8\x80\x85\x20\xE3\x80"
    "\x9C\xE5\x8F\xB2\xE4\xB8\x8A\xE6\x9C\x80\xE5\xBC\xB3\xE3\x81"
    "\xAE\xE9\xAD\x94\xE7\x8E\x8B\xE3\x81\xAE\xE5\xA7\x8B\xE7\xA5"
    "\x96\xE3\x80\x81\xE8\xBB\xA2\xE7\x94\x9F\xE3\x81\x97\xE3\x81"
    "\xA6\xE5\xAD\x90\xE5\xAD\x93\xE3\x81\x9F\xE3\x81\xA1\xE3\x81"
    "\xAE\xE5\xAD\xA6\xE6\xA0\xA1\xE3\x81\xB8\xE9\x80\x9A\xE3\x81"
    "\x86\xE3\x80\x9C\x0A");
    
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        TextBoxUpdate(&dialogueBox, dt);
        
        if (IsWindowResized()) {
            screenWidth  = GetScreenWidth();
            screenHeight = GetScreenHeight();
            dialogueBox.rect = 
                (Rectangle){ 
                    ((float)screenWidth - (float)screenWidth*5.0f/6.0f)/2.0f, 
                    (float)screenHeight - 180, 
                    (float)screenWidth * 5.0f / 6.0f, 
                    165 
                };
            TextBoxReflow(&dialogueBox);
        }

        BeginDrawing();
            ClearBackground(BLACK);
            TextBoxDraw(&dialogueBox);
        EndDrawing();

    }

    TextBoxCleanup(&dialogueBox);
    UnloadFont(gameFont);
    CloseWindow();
    return 0;
}

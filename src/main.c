/*
Vinora Engine -- simple and portable visual novels engine.

Copyright (c) 2026 
Evgeniy "Parthen" Parfenyuk <parthen [at] riseup.net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "raylib.h"
#include <stdio.h>
#include "ui/text_box.h"

int main(void)
{
    int screenWidth = 1280;
    int screenHeight = 720;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Vinora Engine");
    SetWindowMinSize(800, 600);
    SetTargetFPS(60);

    Font gameFont = LoadFontEx("assets/fonts/NotoSans-Regular.ttf", 32, 
                                NULL, 1240);
    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);

    Rectangle box_rect = { 40, (float)screenHeight - 180, 
                               (float)screenWidth  - 80, 160 };
    TextBox dialogueBox;
    TextBoxInit(&dialogueBox, box_rect, gameFont);

    TextBoxSetText(&dialogueBox,
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit,"
    "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
    "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
    "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in"
    "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
    "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa "
    "qui officia deserunt mollit anim id est laborum.");

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        TextBoxUpdate(&dialogueBox, dt);
        
        if (IsWindowResized()) {
            screenWidth  = GetScreenWidth();
            screenHeight = GetScreenHeight();
            dialogueBox.rect = (Rectangle){40, (float)screenHeight - 180, 
                                               (float)screenWidth  - 80, 160};
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

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
#include "ui/text_box.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Vinora Engine");
    SetTargetFPS(60);

    TextBox dialogueBox;
    TextBoxInit(&dialogueBox, screenWidth, screenHeight);

    TextBoxSetText(&dialogueBox, "\xD0\x90\xD0\xBB\xD0\xB8\xD1\x81\xD0\xB0",
                   "Hello from Vinora Engine textbox!\n\n"
                   "D-d-do you like it or something? :3");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground((Color){20, 25, 40, 255});
            TextBoxDraw(&dialogueBox);
        EndDrawing();
    }

    TextBoxCleanup(&dialogueBox);
    CloseWindow();
    return 0;
}

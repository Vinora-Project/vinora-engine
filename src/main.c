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
#include "ui/font_manager.h"
#include "game.h"
#include "text_utils.h"

#include <string.h>
#include <stdio.h>

static void PrintUsage(FILE *out, const char *argv0)
{
    fprintf(out, "Usage: %s [file.vnrs]\n", argv0);
    fprintf(out, "       %s -h | --help\n", argv0);
    fprintf(out, "\n");
    fprintf(out, "If no file is given, assets/i18n.vnrs is loaded.\n");
}

int main(int argc, char **argv)
{
    const char *scenePath = "assets/i18n.vnrs";
    int screenWidth = 1280;
    int screenHeight = 720;
    Font gameFont = {0};
    GameState game = {0};

    if (argc > 1) {
        if (0 == strcmp("--help", argv[1]) ||
            0 == strcmp("-h", argv[1])) {
            PrintUsage(stdout, argv[0]);
            return 0;
        } else if (EndsWith(argv[1], ".vnrs")) {
            scenePath = argv[1];
        } else {
            fprintf(stderr, "Bad argument: %s\n", argv[1]);
            PrintUsage(stderr, argv[0]);
            return 1;
        }
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Vinora Engine");
    SetWindowMinSize(800, 600);
    SetTargetFPS(60);

    gameFont = LoadGameFont("assets/fonts/unifont.otf", 64,
        scenePath);
    GameStateInit(&game, gameFont, scenePath);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        GameStateUpdate(&game, dt);

        if (IsWindowResized())
            GameStateResize(&game);

        BeginDrawing();
        ClearBackground(BLACK);
        GameStateDraw(&game);
        EndDrawing();
    }

    GameStateCleanup(&game);
    UnloadFont(gameFont);
    CloseWindow();

    return 0;
}

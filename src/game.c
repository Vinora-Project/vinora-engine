/*
game.c -- game state for Vinora Engine
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

#include "game.h"

#define DIALOGUE_WIDTH_RATIO (5.0f/6.0f)
#define DIALOGUE_HEIGHT 160.0f
#define DIALOGUE_BOTTOM_GAP 20.0f

static Rectangle DialogueBoxRect(int screenWidth, int screenHeight)
{
    float width = (float)screenWidth*DIALOGUE_WIDTH_RATIO;
    float x = ((float)screenWidth - width)/2.0f;
    float y = (float)screenHeight - DIALOGUE_HEIGHT - DIALOGUE_BOTTOM_GAP;

    return (Rectangle){ x, y, width, DIALOGUE_HEIGHT };
}

static int AdvancePressed(void)
{
    return (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
            IsKeyPressed(KEY_SPACE) ||
            IsKeyPressed(KEY_ENTER));
}

static void ShowNextChunk(GameState *gs)
{
    if (SceneReadNext(&gs->scene)) {
        gs->currentState = state_dialogue;
        TextBoxSetText(&gs->dialogueBox, gs->scene.currentChunk);
    } else {
        gs->currentState = state_end;
    }
}

void GameStateInit(GameState *gs, Font font, const char *scenePath)
{
    Rectangle boxRect = { 0 };

    *gs = (GameState){ 0 };
    gs->currentState = state_loading;

    boxRect = DialogueBoxRect(GetScreenWidth(), GetScreenHeight());
    TextBoxInit(&gs->dialogueBox, boxRect, font);

    if (!SceneOpen(&gs->scene, scenePath)) {
        gs->currentState = state_end;
        return;
    }
    ShowNextChunk(gs);
}

void GameStateUpdate(GameState *gs, float dt)
{
    TextBoxUpdate(&gs->dialogueBox, dt);

    if (gs->currentState != state_dialogue) return;
    if (!AdvancePressed()) return;

    if (!TextBoxIsComplete(&gs->dialogueBox)) {
        TextBoxSkip(&gs->dialogueBox);
        return;
    }

    ShowNextChunk(gs);
}

void GameStateDraw(const GameState *gs)
{
    TextBoxDraw(&gs->dialogueBox);
}

void GameStateResize(GameState *gs)
{
    gs->dialogueBox.rect = DialogueBoxRect(GetScreenWidth(),
                                           GetScreenHeight());
    TextBoxReflow(&gs->dialogueBox);
}

void GameStateCleanup(GameState *gs)
{
    TextBoxCleanup(&gs->dialogueBox);
    SceneClose(&gs->scene);
}

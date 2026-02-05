#pragma once
#include "raylib.h"

void DrawTextCenteredAtPoint(Font font, const char* text,
                             float centerX, float centerY,
                             float fontSize, float spacing, Color color);

void DrawTextCenteredInRect(Font font, const char* text,
                            Rectangle rect,
                            float fontSize, float spacing, Color color);
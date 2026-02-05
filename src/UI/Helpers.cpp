#include "Helpers.h"

void DrawTextCenteredAtPoint(Font font, const char* text,
                             float centerX, float centerY,
                             float fontSize, float spacing, Color color)
{
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
    float x = centerX - size.x * 0.5f;
    float y = centerY - size.y * 0.5f;
    DrawTextEx(font, text, { x, y }, fontSize, spacing, color);
}

void DrawTextCenteredInRect(Font font, const char* text,
                            Rectangle rect,
                            float fontSize, float spacing, Color color)
{
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
    float x = rect.x + (rect.width  - size.x) * 0.5f;
    float y = rect.y + (rect.height - size.y) * 0.5f;
    DrawTextEx(font, text, { x, y }, fontSize, spacing, color);
}
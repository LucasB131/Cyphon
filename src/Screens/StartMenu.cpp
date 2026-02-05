#include "StartMenu.h"
#include "CreateAvatar.h"
#include "../GameState.h"
#include "../SaveFunctions/SaveSystem.h"
#include "../Types/SaveData.h"
#include "../Colors/Colors.h"

// Suppressor
static bool suppressNextClick = false;

// Popup for the three-dot menu
static int activePopupSlot = -1;

// Popup for delete confirmation
static int activeConfirm = -1;

// Which slot is being deleted
static int deleteSlot = -1;

// Rename popup
static int activeRename = -1;
static std::string renameBuffer = "";

// Unified mouse getter
static inline Vector2 Mouse() {
    return GetMousePosition();
}

// Check hover
static inline bool Hover(Rectangle r) {
    return CheckCollisionPointRec(Mouse(), r);
}

// Draw text with float-safe Vector2
static inline void DrawTextF(const char* txt, float x, float y, float size, float spacing, Color c) {
    DrawTextEx(gGame.cinzel, txt, Vector2{ x, y }, size, spacing, c);
}

// Draw save slot box
static void DrawSaveSlot(int slot, float y, bool exists, const SaveData& preview) {

    Rectangle box = { 400.0f, y, 1120.0f, 100.0f };
    DrawRectangleRounded(box, 0.25f, 16, CustomGold);

    bool hover = Hover({ 550.0f, (float)(160 + 200 * slot), 500.0f, 80.0f });

    if (!exists) {
        DrawTextF("NEW GAME", 600.0f, y + 25.0f, 60.0f, 4.0f, hover ? DARKGRAY : BLACK);

        if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            gGame.currentScreen = SCREEN_NEWAVATAR;
            return;
        }
    }
    else {
        std::string text = "Level " + std::to_string(preview.level) + " - " + preview.name;
        DrawTextF(text.c_str(), 600.0f, y + 25.0f, 60.0f, 4.0f, hover ? DARKGRAY : BLACK);

        DrawCircle(1470, 180 + (200 * slot), 4.0f, BLACK);
        DrawCircle(1470, 200 + (200 * slot), 4.0f, BLACK);
        DrawCircle(1470, 220 + (200 * slot), 4.0f, BLACK);

        Rectangle dotsButton = { 1450.0f, (float)(150 + 200 * slot), 40.0f, 100.0f };

        if (Hover(dotsButton)) {
            Color highlight = { 255, 212, 122, 155 };
            DrawRectangleRec(dotsButton, highlight);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                activePopupSlot = slot;
            }
        }
    }
}

// Draw popup
static void DrawPopup(int slot) {

    Rectangle popup = {1500.0f, (float)(180 + 200 * slot), 250.0f, 100.0f};

    DrawRectangleRec(popup, DARKGRAY);
    DrawRectangle(1500, 230 + (200 * slot), 250, 4, WHITE);

    DrawTextF("DELETE SAVE", 1528.0f, (float)(190 + 200 * slot), 30.0f, 2.0f, BLACK);
    DrawTextF("RENAME SAVE", 1528.0f, (float)(240 + 200 * slot), 30.0f, 2.0f, BLACK);
}

// Handle popup input
static void HandlePopupInput(int slot) {

    Rectangle deleteBtn = { 1500.0f, (float)(150 + 200 * slot), 250.0f, 50.0f };
    Rectangle renameBtn = { 1500.0f, (float)(200 + 200 * slot), 250.0f, 50.0f };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = Mouse();

        if (CheckCollisionPointRec(m, deleteBtn)) {
            deleteSlot = slot;
            activePopupSlot = -1;
            activeConfirm = slot;
            suppressNextClick = true;
        }
        else if (CheckCollisionPointRec(m, renameBtn)) {
            activePopupSlot = -1;
            activeRename = slot;

            SaveData preview = LoadPreview(slot);
            renameBuffer = preview.name;
        }
    }
}

// Close popup when clicking outside
static void HandlePopupClose(int slot) {

    Rectangle popup = { 1500.0f, (float)(180 + 200 * slot), 250.0f, 100.0f };
    Rectangle dotsButton = { 1450.0f, (float)(150 + 200 * slot), 40.0f, 100.0f };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = Mouse();

        if (!CheckCollisionPointRec(m, popup) && !CheckCollisionPointRec(m, dotsButton)) {
            activePopupSlot = -1;
        }
    }
}

// Draw confirm delete
static void DrawConfirm(int slot, const SaveData& preview) {

    Rectangle confirm = {500.0f, 400.0f, 920.0f, 400.0f};
    DrawRectangleRounded(confirm, 0.25f, 16, WHITE);

    std::string confirmdeleteplayer = preview.name;
    Vector2 size = MeasureTextEx(gGame.cinzel, confirmdeleteplayer.c_str(), 50.0f, 3.0f);
    float playernamex = confirm.x + (confirm.width - size.x) * 0.5f;

    DrawTextEx(gGame.cinzel, confirmdeleteplayer.c_str(), {playernamex, 475.0f}, 50.0f, 3.0f, RED);
    DrawTextF("ARE YOU SURE YOU WANT TO DELETE", 540.0f, 425.0f, 50.0f, 3.0f, RED);
    DrawTextF("THIS CANNOT BE UNDONE!", 650.0f, 525.0f, 50.0f, 3.0f, RED);

    Rectangle deletebox = {600.0f, 700.0f, 200.0f, 50.0f};
    Rectangle cancelbox = {1120.0f, 700.0f, 200.0f, 50.0f};

    DrawRectangleRounded(deletebox, 0.25f, 16, CustomLavender);
    DrawRectangleRounded(cancelbox, 0.25f, 16, CustomLavender);

    DrawTextF("DELETE", 635.0f, 710.0f, 40.0f, 3.0f, BLACK);
    DrawTextF("CANCEL", 1145.0f, 710.0f, 40.0f, 3.0f, BLACK);
}

// Confirm delete input
static void HandleConfirmInput(int& activeConfirm, int slot) {

    Rectangle deletebox = {600.0f, 670.0f, 200.0f, 55.0f};
    Rectangle cancelbox = {1120.0f, 670.0f, 200.0f, 55.0f};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = Mouse();

        if (CheckCollisionPointRec(m, deletebox)) {
            DeleteSave(slot);
            activeConfirm = -1;
        }
        else if (CheckCollisionPointRec(m, cancelbox)) {
            activeConfirm = -1;
        }
    }
}

// Close confirm popup
static void HandleConfirmClose(int& activeConfirm) {
    if (suppressNextClick) {
        suppressNextClick = false;
        return;
    }

    Rectangle confirm = {500.0f, 400.0f, 920.0f, 400.0f};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = Mouse();

        if (!CheckCollisionPointRec(m, confirm)) {
            activeConfirm = -1;
        }
    }
}

// Draw rename popup
static void DrawRenamePopup(int slot) {

    Rectangle box = {500.0f, 400.0f, 920.0f, 400.0f};
    DrawRectangleRounded(box, 0.25f, 16, WHITE);

    DrawTextF("RENAME SAVE", 750.0f, 430.0f, 50.0f, 3.0f, RED);

    Rectangle textBox = {600.0f, 550.0f, 720.0f, 70.0f};
    DrawRectangleRounded(textBox, 0.25f, 16, CustomLavender);

    bool cursorVisible = ((int)(GetTime() * 2) % 2) == 0;
    std::string display = renameBuffer;
    if (cursorVisible) display += "|";

    DrawTextF(display.c_str(), 620.0f, 565.0f, 40.0f, 3.0f, BLACK);

    Rectangle okBtn = {600.0f, 700.0f, 200.0f, 50.0f};
    Rectangle cancelBtn = {1120.0f, 700.0f, 200.0f, 50.0f};

    DrawRectangleRounded(okBtn, 0.25f, 16, CustomLavender);
    DrawRectangleRounded(cancelBtn, 0.25f, 16, CustomLavender);

    DrawTextF("OK", 675.0f, 710.0f, 40.0f, 3.0f, BLACK);
    DrawTextF("CANCEL", 1145.0f, 710.0f, 40.0f, 3.0f, BLACK);
}

// Rename input handler
static void HandleRenameInput(int slot) {

    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 125 && renameBuffer.length() < 16) {
            renameBuffer.push_back((char)key);
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !renameBuffer.empty()) {
        renameBuffer.pop_back();
    }

    Vector2 m = Mouse();

    Rectangle okBtn = {600.0f, 700.0f, 200.0f, 50.0f};
    Rectangle cancelBtn = {1120.0f, 700.0f, 200.0f, 50.0f};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        if (CheckCollisionPointRec(m, okBtn)) {

            if (renameBuffer.length() == 0)
                return;

            SaveData data;
            LoadFromSlot(data, slot);
            data.name = renameBuffer;
            SaveToSlot(data, slot);
            activeRename = -1;
        }
        else if (CheckCollisionPointRec(m, cancelBtn)) {
            activeRename = -1;
        }
    }
}

// Main start menu
void StartMenu() {

    static double CurrentTime = GetTime();
    double NetTime = GetTime() - CurrentTime;

    if (gGame.currentScreen == SCREEN_MENU && NetTime < 5 && IsKeyPressed(KEY_SPACE)) {
        CurrentTime = GetTime() - 5.0;
    }

    if (NetTime > 0 && NetTime < 1) {
        float TriBase = (float)(NetTime * 3840.0);
        float TriHeight = (float)(NetTime * 2400.0);
        DrawTriangle(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, TriHeight }, Vector2{ TriBase, 0.0f }, CustomPurple);
    }
    if (NetTime > 1) {
        DrawRectangle(0, 0, 1920, 1200, CustomPurple);
    }

    if (NetTime > 1 && NetTime < 3) {
        float fontsize = (float)(60.0 * (NetTime - 1));
        float xpos = (float)(950.0 - (275.0 * (NetTime - 1)));
        float ypos = (float)(200.0 - (25.0 * (NetTime - 1)));

        DrawTextF("WORLDS OF ENCHANT", xpos, ypos, fontsize, 3.0f, CustomGold);
    }
    if (NetTime > 3) {
        DrawTextF("WORLDS OF ENCHANT", 400.0f, 150.0f, 120.0f, 3.0f, CustomGold);
    }

    if (NetTime > 2 && NetTime < 4) {
        float recheight = (float)(400.0 * (NetTime - 2));
        Rectangle lavender = { 300.0f, 300.0f, 1320.0f, recheight };
        DrawRectangleRounded(lavender, 0.25f, 16, CustomLavender);
    }
    if (NetTime > 4) {
        Rectangle lavender = { 300.0f, 300.0f, 1320.0f, 800.0f };
        DrawRectangleRounded(lavender, 0.25f, 16, CustomLavender);
    }

    if (NetTime > 4 && NetTime < 5) {
        float opacity = (float)(255.0 * (NetTime - 4));
        Color CustomGold2 = { 255, 212, 122, (unsigned char)opacity };

        Rectangle gold1 = { 400.0f, 350.0f, 1120.0f, 100.0f };
        Rectangle gold2 = { 400.0f, 550.0f, 1120.0f, 100.0f };
        Rectangle gold3 = { 400.0f, 750.0f, 1120.0f, 100.0f };
        Rectangle gold4 = { 400.0f, 950.0f, 1120.0f, 100.0f };

        DrawRectangleRounded(gold1, 0.25f, 16, CustomGold2);
        DrawRectangleRounded(gold2, 0.25f, 16, CustomGold2);
        DrawRectangleRounded(gold3, 0.25f, 16, CustomGold2);
        DrawRectangleRounded(gold4, 0.25f, 16, CustomGold2);
    }

    if (NetTime > 5) {

        for (int slot = 1; slot <= 4; slot++) {

            float y = 350.0f + (float)(slot - 1) * 200.0f;

            bool exists = SaveExists(slot);
            SaveData preview = exists ? LoadPreview(slot) : SaveData();

            DrawSaveSlot(slot, y, exists, preview);

            Rectangle slotBox = {400.0f, y, 1120.0f, 100.0f};

            if (activePopupSlot == -1 && activeConfirm == -1 && activeRename == -1) {
                if (Hover(slotBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                    gGame.slot = slot;

                    if (!exists) {
                        gGame.save = SaveData();
                        SaveToSlot(gGame.save, slot);

                        gGame.currentScreen = SCREEN_NEWAVATAR; 
                    } 
                    
                    else {
                        LoadFromSlot(gGame.save, slot);

                        gGame.currentScreen = SCREEN_SANDBOX;
                    }

                    return;
                }
            }
        }

        if (activePopupSlot != -1) {
            DrawPopup(activePopupSlot);
            HandlePopupInput(activePopupSlot);
            HandlePopupClose(activePopupSlot);
        }

        if (activeConfirm != -1) {
            SaveData preview = LoadPreview(activeConfirm);
            DrawConfirm(activeConfirm, preview);
            HandleConfirmInput(activeConfirm, deleteSlot);
            HandleConfirmClose(activeConfirm);
        }

        if (activeRename != -1) {
            DrawRenamePopup(activeRename);
            HandleRenameInput(activeRename);
            return;
        }
    }
}
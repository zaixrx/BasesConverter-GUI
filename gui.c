#include <stdio.h>
#include "gui.h"
#include "raygui.h"

bool is_valid_character(char keycode) {
    return ('(' <= keycode && keycode <= '/') || ('0' <= keycode && keycode <= '9') || ('A' <= keycode && keycode <= 'F') || ('a' <= keycode && keycode <= 'f' || keycode == '.');
}

void DrawInputField(InputField* state) {
    int border_color = M_BORDER_COLOR_DEFUALT;

    state->is_hovering = CheckCollisionPointRec(GetMousePosition(), state->bounds);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        state->is_selected = state->is_hovering;

    DrawRectangleRec(state->bounds, GetColor(M_BG_COLOR));

    if (state->is_selected) {
        // --- LOGIC PART --- //
        // returns a queue containing pressed keys after last frame
        // the function will return 0 in case the queue is empty
        int key = GetKeyPressed();

        while (key != 0) {
            if (is_valid_character((char)key) && state->characters_count < state->characters_capacity) {
                state->content_buffer[state->characters_count + 1] = '\0';
                state->content_buffer[state->characters_count++] = (char)key;
            } else if (IsKeyDown(KEY_BACKSPACE)) {
                state->content_buffer[state->characters_count - 1] = '\0';
                state->characters_count--;
                if (state->characters_count < 0) {
                    state->characters_count = 0; break;
                }
            }

            key = GetKeyPressed();
        }

        border_color = M_BORDER_COLOR_FOCUSED;
        bool is_time = (++(state->frames_passed_since_selected) / 20) % 2 == 0;
        if (state->characters_count < state->characters_capacity && is_time) {
            int x = (int)state->bounds.x + 8 + MeasureText(state->content_buffer, M_FONT_SIZE);
            int y = (int)state->bounds.y + 12;
            DrawText("_", x, y, M_FONT_SIZE, GetColor(M_TEXT_COLOR));
        }
    } else state->frames_passed_since_selected = 0;

    if (state->is_hovering) {
        border_color = M_BORDER_COLOR_FOCUSED;
    }

    DrawRectangleLinesEx(state->bounds, M_BORDER_WIDTH, GetColor(border_color));
    DrawText(state->content_buffer, (int)state->bounds.x + 5, (int)state->bounds.y + 8, M_FONT_SIZE, GetColor(M_TEXT_COLOR));
    SetMouseCursor(state->is_hovering ? MOUSE_CURSOR_IBEAM : MOUSE_CURSOR_DEFAULT);
}

int DrawButton(Rectangle bounds, const char *text) {
    int result = 0, cursor_state = MOUSE_CURSOR_DEFAULT;
    int border_color = M_BORDER_COLOR_DEFUALT;

    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        border_color = M_BORDER_COLOR_FOCUSED;
        cursor_state = MOUSE_CURSOR_POINTING_HAND;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
    }

    DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, GetColor(M_BG_PRIMARY));
    DrawText(text, bounds.x + 0.5 * (bounds.width - MeasureText(text, M_FONT_SIZE)), bounds.y + bounds.height / 2 - M_FONT_SIZE / 2, M_FONT_SIZE, GetColor(M_TEXT_COLOR));
    DrawRectangleLinesEx(bounds, M_BORDER_WIDTH, GetColor(border_color));

    SetMouseCursor(cursor_state);

    return result;
}

int DrawDropdown(Rectangle bounds, const char* text, int* active, bool is_open) {
    int result = 0;

    int temp = 0;
    if (active == NULL) active = &temp;

    int itemSelected = *active;
    int itemFocused = -1;

    bool mouseHovering = CheckCollisionPointRec(GetMousePosition(), bounds);

    // Get substrings items from text (items pointers, lengths and count)
    int itemCount = 0;
    const char **items = TextSplit(text, ';', &itemCount);

    Rectangle boundsOpen = bounds;
    // (itemscount + 1) * (dropdown.height + spacing?)
    boundsOpen.height = (itemCount + 1) * bounds.height;
    //if (direction == 1) boundsOpen.y -= itemCount*(bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING)) + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING);

    Rectangle itemBounds = bounds;

    if (itemCount > 1) {
        Vector2 mousePoint = GetMousePosition();

        if (is_open) {
            // Check if mouse has been pressed or released outside limits
            if (!CheckCollisionPointRec(mousePoint, boundsOpen)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
            }

            // Check if already selected item has been pressed again
            if (CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = 1;

            // Check focused and selected item
            for (int i = 0; i < itemCount; i++) {
                // Update item rectangle y position for next item with spacing?
                itemBounds.y += bounds.height;

                if (CheckCollisionPointRec(mousePoint, itemBounds)) {
                    itemFocused = i;
                    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                        itemSelected = i;
                        result = 1;
                    }
                    break;
                }
            }

            itemBounds = bounds;
        } else {
            if (CheckCollisionPointRec(mousePoint, bounds)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    result = 1;
                }
            }
        }
    }

    DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, GetColor(M_BG_COLOR));
    DrawText(items[itemSelected], bounds.x + 0.5 * (bounds.width - MeasureText(items[itemSelected], M_FONT_SIZE)), bounds.y + 0.5 * (bounds.height - M_FONT_SIZE), M_FONT_SIZE, GetColor(M_TEXT_COLOR));

    if (is_open) {
        int bg_color;
        for (int i = 0; i < itemCount; i++) {
            itemBounds.y += bounds.height;
            bg_color = M_BG_COLOR;

            if (i == itemFocused || i == itemSelected) {
                bg_color = M_BG_SELECTED_COLOR;
            }

            DrawRectangle(itemBounds.x, itemBounds.y, itemBounds.width, itemBounds.height, GetColor(bg_color));
            DrawText(items[i], itemBounds.x + 0.5 * (itemBounds.width - MeasureText(items[i], M_FONT_SIZE)), itemBounds.y + 0.5 * (itemBounds.height - M_FONT_SIZE), M_FONT_SIZE, GetColor(M_TEXT_COLOR));
        }
    }

    DrawRectangleLinesEx(bounds, M_BORDER_WIDTH, GetColor((is_open || mouseHovering) ? M_BORDER_COLOR_FOCUSED : M_BORDER_COLOR_DEFUALT));
    GuiDrawIcon(121, bounds.x + bounds.width - 30, bounds.y + bounds.height / 2 - 6, 1, GetColor(M_TEXT_COLOR));

    *active = itemSelected;

    return result;
}

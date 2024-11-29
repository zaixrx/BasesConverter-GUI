#include "raylib.h"
#include "gui.h"

void DrawInputField(InputField* state, int border_width, int font_size) {
    Color color = DARKGRAY;
    int cursor_state = MOUSE_CURSOR_DEFAULT;

    state->is_hovering = CheckCollisionPointRec(GetMousePosition(), state->bounds);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        state->is_selected = state->is_hovering;

    if (state->is_selected) {
        // --- LOGIC PART --- //
        // returns a queue containing pressed keys after last frame
        // the function will return 0 in case the queue is empty
        int key = GetKeyPressed();
        while (key != 0) {
            if ('0' <= key && key <= '9' && state->characters_count < sizeof(state->content_buffer)) {
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

        // --- VISUALS PART --- //
        color = RED;
        bool weird_condition = (++state->frames_passed_since_selected / 20) % 2 == 0;
        if (state->characters_count < sizeof(state->content_buffer) && weird_condition) 
            DrawText("_", (int)state->bounds.x + 8 + MeasureText(state->content_buffer, font_size), (int)state->bounds.y + 12, font_size, MAROON);
    } else state->frames_passed_since_selected = 0;

    DrawRectangleRec(state->bounds, LIGHTGRAY);
    DrawRectangleLinesEx(state->bounds, border_width, color);
    
    if (state->is_hovering) cursor_state = MOUSE_CURSOR_IBEAM;

    DrawText(state->content_buffer, (int)state->bounds.x + 5, (int)state->bounds.y + 8, font_size, color);
    SetMouseCursor(cursor_state);
}
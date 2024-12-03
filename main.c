#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "convert.h"

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "gui.h"

#define WINDOW_H 600
#define WINDOW_W 800

void Update();

Text text = {
    .base = { .id = 'o', .type = TXT },
    .bounds = (Rectangle){ WINDOW_W * 0.5 - 75, WINDOW_H * 0.5 - 60, 150, 50 },
    .content = ""
};

void handle_click(const GUI_Base** gui_elements, int gui_elements_count) {
    int from, to;
    const char* number = "";

    for (int i = 0; i < gui_elements_count; i++) {
        switch (gui_elements[i]->type) {
            case DROPDOWN_MENU:
                DropdownMenu* dropdown = (DropdownMenu*)gui_elements[i];
                if (gui_elements[i]->id == 'f') from = dropdown->state;
                else to = dropdown->state;
                break;

            case INPUT_FIELD:
                InputField* input_field = (InputField*)gui_elements[i];
                if (gui_elements[i]->id == 'i') number = input_field->content_buffer;
                break;
        }
    }

    float num = to_decimal(number, get_base(from));
    text.content = to_base(num, get_base(to));
}

int main(void) {
    InitWindow(WINDOW_W, WINDOW_H, "Mini Project");
    SetTargetFPS(60);

    char* content_buffer = (char*)calloc(10, sizeof(char));
    content_buffer[0] = '\0';

    Rectangle input_rect = (Rectangle){ WINDOW_W * 0.5 - 75, WINDOW_H * 0.5 - 25, 150, 50 };
    InputField input_field = { 
        .base = { .id = 'i', .type = INPUT_FIELD },
        .bounds = input_rect,
        .characters_capacity = 16,
        .content_buffer = content_buffer,
        .characters_count = 0,
        .frames_passed_since_selected = 0,
        .is_hovering = false,
        .is_selected = false
    };
    DropdownMenu dropdown_from = {
        .base = { .id = 'f', .type = DROPDOWN_MENU },
        .bounds = (Rectangle){ WINDOW_W * 0.5 - 235, WINDOW_H * 0.5 - 25, 150, 50 },
        .content = "Decimal;Binary;Octal;Hexadecimal",
        .state = 0,
        .is_selected = false
    };
    DropdownMenu dropdown_to = {
        .base = { .id = 't', .type = DROPDOWN_MENU },
        .bounds = (Rectangle){ WINDOW_W * 0.5 + 85, WINDOW_H * 0.5 - 25, 150, 50 },
        .content = "Decimal;Binary;Octal;Hexadecimal",
        .state = 0,
        .is_selected = false
    };
    Button button = {
        .base = { .id = 'b', .type = BTN },
        .bounds = (Rectangle){ WINDOW_W * 0.5 - 75, WINDOW_H * 0.5 + 35, 150, 50 },
        .content = "Convert",
        .on_click = &handle_click
    };

    const GUI_Base* gui_elements[] = {
        (GUI_Base*)&input_field,
        (GUI_Base*)&dropdown_from,
        (GUI_Base*)&dropdown_to,
        (GUI_Base*)&button,
        (GUI_Base*)&text,
    };

    while (!WindowShouldClose()) {
        Update(gui_elements, 5);
    }

    CloseWindow();
    free(input_field.content_buffer);
    free(text.content);

    return EXIT_SUCCESS;
}

void Update(const GUI_Base** gui_elements, unsigned int elements_count) {
    BeginDrawing();
    ClearBackground(GetColor(0x161616FF));

    for (int i = 0; i < elements_count; i++) {
        switch (gui_elements[i]->type) {
        case BTN:
            Button* button = (Button*)gui_elements[i];
            if (DrawButton(button->bounds, button->content))
                (*(button->on_click))(gui_elements, elements_count);
            break;
        
        case TXT:
            Text* text = (Text*)gui_elements[i];
            DrawText(text->content, text->bounds.x, text->bounds.y, M_FONT_SIZE, GREEN);
            break;
        
        case INPUT_FIELD:
            InputField* input_field = (InputField*)gui_elements[i];
            DrawInputField(input_field);
            break;

        case DROPDOWN_MENU:
            DropdownMenu* dropdown = (DropdownMenu*)gui_elements[i];
            if (DrawDropdown(dropdown->bounds, dropdown->content, (int*)&(dropdown->state), dropdown->is_selected))
                dropdown->is_selected = !dropdown->is_selected;
            break;
        }
    }

    EndDrawing();
}
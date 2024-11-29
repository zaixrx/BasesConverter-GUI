#ifndef GUI_H
#define GUI_H

#include "raylib.h"

#define FONT_SIZE 12
#define BORDER_SIZE 2

typedef enum {
    BTN,
    TXT,
    INPUT_FIELD,
    DROPDOWN_MENU
} GUIElementType;

typedef struct {
    const char id;
    GUIElementType type;
} GUI_Base;

typedef struct {
    GUI_Base base;
    Rectangle bounds;
    char* content_buffer;
    int characters_count;
    int frames_passed_since_selected;
    bool is_selected;
    bool is_hovering;
} InputField;

typedef struct {
    GUI_Base base;
    Rectangle bounds;
    char* content;
    int state;
    bool is_selected;
} DropdownMenu;

typedef struct {
    GUI_Base base;
    Rectangle bounds;
    char* content;
    void (*on_click)(const GUI_Base**, int);
} Button;

typedef struct {
    GUI_Base base;
    Rectangle bounds;
    char* content;
} Text;

void DrawInputField(InputField* state, int border_width, int font_size);

#endif
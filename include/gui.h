#ifndef GUI_H
#define GUI_H

#include "raylib.h"

#define M_BG_COLOR 0x101010FF
#define M_BG_PRIMARY 0x0000AAFF
#define M_BG_SELECTED_COLOR 0x0A0A0AFA
#define M_BORDER_COLOR_DEFUALT 0xFFFFFFAA
#define M_BORDER_COLOR_FOCUSED 0xFFFFFFFF
#define M_TEXT_COLOR 0xFFFFFFFF
#define M_TEXT_SELECTED_COLOR 0xFFFFFFFF
#define M_TEXT_FOCUSED_COLOR 0xFFFFFFFF
#define M_BORDER_WIDTH 2
#define M_FONT_SIZE 16

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
    int characters_capacity;
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

int DrawButton(Rectangle bounds, const char *text);
void DrawInputField(InputField* state);
int DrawDropdown(Rectangle bounds, const char* text, int* active, bool is_open);

#endif
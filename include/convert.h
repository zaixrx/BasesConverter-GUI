#ifndef CONVERT_H
#define CONVERT_H

typedef enum {
    DECIMAL,
    BINARY,
    OCTAL,
    HEXADECIMAL,
} Base;

char* to_base(float number, int base);
float to_decimal(const char* number, int base);
int get_base(int selection);

#endif
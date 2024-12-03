#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "convert.h"

#define MAX_DIGIT_PRECESION 4

int binary_digits_count(int number) {
  int count = 0;
  for (int i = 0; i < sizeof(number) * 8; i++) {
    if (1 << i & number > 0)
      count = i + 1;
  }
  return count;
}

int reverse_number(int a) {
  int result = 0;
  while (a > 0) {
    result *= 10;
    result += a % 10;
    a /= 10;
  }
  return result;
}

// Do not forget to free the returned memory
// TODO: Check for overflows
char* to_base(float number, int base) {
  int current_digit = 0, digits_count = 64, index = 0;
  char* output = (char*)malloc((digits_count + 1) * sizeof(char)); // +1 for \0

  output[0] = '\0';

  if (number < 0) {
    output[index++] = '-';
    output[index] = '\0';
    number *= -1;
  }

  int intiger_part = (int)reverse_number(number);
  while (intiger_part > 0) {
    current_digit = intiger_part % base;
    current_digit += current_digit >= 10 ? 'A' - 10 : '0';
    intiger_part  /= base;

    output[index++] = (char)current_digit;
    output[index] = '\0';
  }

  float fractional_part = fmod(number, 1);
  if (fractional_part > 0) {
    output[index++] = '.';
    output[index] = '\0';
    
    int i = 0;
    while (fractional_part > 0 && i++ < MAX_DIGIT_PRECESION) {
      current_digit = (int)(fractional_part * base);
      current_digit += current_digit >= 10 ? 'A' - 10 : '0';
      fractional_part = fmod(fractional_part * base, 1);

      output[index++] = (char)current_digit;
      output[index] = '\0';
    }
  }

  return output;
}

// Do not forget to free the returned memory
// returns -1 if there exists an overflow
float to_decimal(const char* number, int base) {
  float result = 0, floating_degree = 1;
  int size = strlen(number), c_digit = 0, is_intiger = 1, is_negative = 0;

  for (int i = 0; i < size; i++) { 
    if (number[i] == '.') {
      is_intiger = 0;
      continue;
    }

    if (number[i] == '-') {
      is_negative = 1;
      continue;
    }

    c_digit = number[i];
    c_digit -= number[i] > '9' ? 'A' : '0';

    if (is_intiger == 1) {
      result *= base;
      result += c_digit;
    } else {
      floating_degree /= 10;
      result += c_digit * floating_degree;
    }
  }

  result = (is_negative == 1) ? -result : result;

  return result;
} 

int get_base(int selection) {
    switch (selection)
    {
    case DECIMAL:
        return 10;
    case BINARY:
        return 2;
    case OCTAL:
        return 8;
    case HEXADECIMAL:
        return 16;
    }
}

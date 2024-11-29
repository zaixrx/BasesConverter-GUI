#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "convert.h"

int digits_count(int number) {
  int count = 0;
  for (int i = 0; i < sizeof(number) * 8; i++) {
    if ((number & (int)pow(2, i)) > 0)
      count = i + 1;
  }
  return count;
}

char* to_base(int number, int base) {
  int current_digit, size = ceil( digits_count(number) * 1 / ceil(log2(base)) ) + 1;
  char* output = (char*)malloc(size * sizeof(char));
  output[--size] = '\0';
  while (size-- > 0) {
    current_digit = number % base;
    current_digit += current_digit < 10 ? '0' : 'A' - 10;
    output[size] = (char)current_digit;
    number /= base;
  }
  return output;
}

int power(int base, unsigned int exponent) {
  if (exponent == 0) return 1;
  return base * power(base, exponent - 1);
}

int to_decimal(const char* number, int base) {
  int result = 0, temp = 0, size = strlen(number);

  // size - the '\0' character
  for (int i = size - 1; i >= 0; --i) {
    temp = number[i] - '0';
    if (temp > 10)
      temp -= 'A' - '0' - 10;
    result += temp * power(base, size - i - 1);
  }

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

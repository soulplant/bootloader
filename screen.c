#include "screen.h"

char* video = (char*)0xb8000;
static int line_number = 0;

void clear_screen() {
  int x, y;
  line_number = 0;
  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++) {
      video[(y * WIDTH + x) * 2] = ' ';
    }
  }
}

void set_char(int x, int y, char c) {
  video[(y * WIDTH + x) * 2] = c;
}

char get_char(int x, int y) {
  return video[(y * WIDTH + x) * 2];
}

void shift_screen() {
  int x, y;
  for (y = 0; y < HEIGHT - 1; y++) {
    for (x = 0; x < WIDTH; x++) {
      set_char(x, y, get_char(x, y + 1));
    }
  }
  for (x = 0; x < WIDTH; x++) {
    set_char(x, HEIGHT - 1, ' ');
  }
}

void print_line(char* line) {
  char c;
  int x;
  int len = strlen(line);
  if (line_number >= HEIGHT) {
    shift_screen();
    line_number--;
  }
  for (x = 0; x < len; x++) {
    set_char(x, line_number, line[x]);
  }
  line_number++;
}

void print_int(int n, int val, char* cs) {
  char buf[n + 1];
  int i;
  int base = strlen(cs);
  for (i = 0; i < n; i++) {
    buf[((n-1)-i)] = cs[val % base];
    val /= base;
  }
  buf[n] = 0;
  print_line(buf);
}

int strlen(char* s) {
  int result = 0;
  while (*s++) result++;
  return result;
}

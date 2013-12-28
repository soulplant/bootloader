#define WIDTH 80
#define HEIGHT 25

extern char* video;
void clear_screen();
void shift_screen();
void set_char(int x, int y, char c);
char get_char(int x, int y);
void print_line(char* line);
void print_int(int n, int val, char* cs);
int strlen(char* s);

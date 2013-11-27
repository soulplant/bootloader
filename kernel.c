#define WIDTH 80
#define HEIGHT 24

char* video = (char*)0xb8000;

void print_char(int x, int y, char c);

int kmain() {
	int x, y;
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			print_char(x, y, 'a');
		}
	}
	while (1);
}

void print_char(int x, int y, char c) {
	video[(y * WIDTH + x) * 2] = c;
}


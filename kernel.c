#include "kutil.h"
#include "idt.h"
#include "screen.h"

char* hex = "0123456789abcdef";
static struct idt_descriptor _idt[I86_MAX_INTERRUPTS] = {};
static struct idtr _idtr;

extern void default_interrupt_handler();
void install_idt();
extern void setup_pic();
void on_interrupt();
void on_keyboard_interrupt(int x);
extern void keyboard_isr();

int kmain() {
  install_idt();
  setup_pic();
  clear_screen();
  asm("sti");
  asm("int $1");
  asm("int $2");
  asm("int $3");
  asm("int $4");
  asm("int $5");
  asm("int $6");
  asm("int $7");
  asm("int $9");
  // asm("cli");

	while (1);
}

void on_keyboard_interrupt(int scancode) {
  print_int(2, scancode, hex);
}

struct interrupt_info {
  uint32_t int_no;
  uint32_t err_code;
};

void on_interrupt(struct interrupt_info info) {
  static int x = 0;
  static int total = 0;

  total++;
  // clear_screen();
  print_int(4, info.int_no, hex);
}

void print_char(int x, int y, char c) {
	video[(y * WIDTH + x) * 2] = c;
}

void set_idt_entry(int i, void (*handler)());

void install_idt() {
  int i;
  memset((void*)&_idt[0], 0, sizeof(struct idt_descriptor) * I86_MAX_INTERRUPTS);
  for (i = 0; i < I86_MAX_INTERRUPTS; i++)
    set_idt_entry(i, default_interrupt_handler);

  set_idt_entry(0, isr0);
  set_idt_entry(1, isr1);
  set_idt_entry(2, isr2);
  set_idt_entry(3, isr3);
  set_idt_entry(4, isr4);
  set_idt_entry(5, isr5);
  set_idt_entry(6, isr6);
  set_idt_entry(7, isr7);
  set_idt_entry(8, isr8);
  set_idt_entry(9, isr9);
  set_idt_entry(10, isr10);
  set_idt_entry(11, isr11);
  set_idt_entry(12, isr12);
  set_idt_entry(13, isr13);
  set_idt_entry(14, isr14);
  set_idt_entry(15, isr15);
  set_idt_entry(16, isr16);
  set_idt_entry(17, isr17);
  set_idt_entry(18, isr18);
  set_idt_entry(19, isr19);
  set_idt_entry(20, isr20);
  set_idt_entry(21, isr21);
  set_idt_entry(22, isr22);
  set_idt_entry(23, isr23);
  set_idt_entry(24, isr24);
  set_idt_entry(25, isr25);
  set_idt_entry(26, isr26);
  set_idt_entry(27, isr27);
  set_idt_entry(28, isr28);
  set_idt_entry(29, isr29);
  set_idt_entry(30, isr30);
  set_idt_entry(31, isr31);

  set_idt_entry(33, keyboard_isr);

  _idtr.limit = sizeof(_idt) - 1;
  _idtr.base = (uint32_t)&_idt[0];
  asm("lidt _idtr");
}

void set_idt_entry(int i, void (*handler)()) {
  uint32_t addr = (uint32_t)handler;
  _idt[i].baseLo = addr & 0xffff;
  _idt[i].baseHi = (addr >> 16) & 0xffff;
  _idt[i].sel = 0x8;
  _idt[i].reserved = 0;
  _idt[i].flags = 0x8e;
}

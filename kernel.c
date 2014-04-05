#include "kutil.h"
#include "idt.h"
#include "screen.h"

char* hex = "0123456789abcdef";
static struct idt_descriptor _idt[I86_MAX_INTERRUPTS] = {};
static struct idtr _idtr;
static char buf[512];

extern void default_interrupt_handler();
void install_idt();
extern void setup_pic();
void on_interrupt();
void on_keyboard_interrupt(int x);
extern void keyboard_isr();

void outb(uint32_t port, uint8_t value);
uint8_t inb(uint32_t port);
uint16_t inw(uint32_t port);
void bl_wait(unsigned short base);
void read_sectors_from_disk(int drive, int count, char* buf);

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

  memset(buf, 0, 512);
  read_sectors_from_disk(0, 1, buf);
  print_line("after read: first two bytes");
  print_int(2, buf[0], hex);
  print_int(2, buf[1], hex);
  print_line("done");
	while (1);
}

#define asmv __asm__ __volatile__
void outb(uint32_t port, uint8_t value) {
  asmv("outb %%al, %%dx" :: "d" (port), "a" (value));
}

uint8_t inb(uint32_t port) {
  uint8_t result;
  asmv("inb %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

uint16_t inw(uint32_t port) {
  uint16_t result;
  asmv("inw %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

void bl_wait(unsigned short base) {
  while(inb(base+0x206) & 0x80);
}

void read_sectors_from_disk(int drive, int count, char* buf) {
  int i;
  int idx;
  uint16_t tmpword;
  uint8_t x;

  bl_wait(0x1f0);
  outb(0x1f1, 0x00);
  outb(0x1f2, count);  // block count
  outb(0x1f3, 0);  // block address
  outb(0x1f4, 0);
  outb(0x1f5, 0);
  // 0xe0 for master, 0xf0 for slave. drive is the selector.
  outb(0x1f6, 0xe0 | (drive << 4));

  // 0x20 is the read sectors command.
  outb(0x1f7, 0x20);
  bl_wait(0x1f0);
  while (!(inb(0x1f7) & 0x08));
  for (idx = 0; idx < 256 * 1; idx++) {
    tmpword = inw(0x1f0);
    buf[idx * 2] = (unsigned char) tmpword;
    buf[idx * 2 + 1] = (unsigned char) (tmpword >> 8);
  }
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

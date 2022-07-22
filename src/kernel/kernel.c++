#include "kernel.h++"

using namespace unstd;

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "Cannot target Linux! (Use a cross-compiler)."
#endif

#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler."
#endif

enum video_type
{
    VIDEO_TYPE_NONE = 0x00,
    VIDEO_TYPE_COLOUR = 0x20, // 32
    VIDEO_TYPE_MONOCHROME = 0x30, // 48
};
 
uint16_t detect_bios_area_hardware(void)
{
    const uint16_t* bda_detected_hardware_ptr = (const uint16_t*) 0x410; // 1040
    return *bda_detected_hardware_ptr;
}
 
enum video_type get_bios_area_video_type(void)
{
    return (enum video_type) (detect_bios_area_hardware() & 0x30); // 48
}

/* Hardware text mode color constants. */
enum vga_color {
  VGA_COLOR_BLACK = 0x0, // 1
  VGA_COLOR_BLUE = 0x1, // 2
  VGA_COLOR_GREEN = 0x2, // 3
  VGA_COLOR_CYAN = 0x3, // 4
  VGA_COLOR_RED = 0x4, // 5
  VGA_COLOR_MAGENTA = 0x5, // 6
  VGA_COLOR_BROWN = 0x6, // 7
  VGA_COLOR_LIGHT_GREY = 0x7, // 8
  VGA_COLOR_DARK_GREY = 0x8, // 9
  VGA_COLOR_LIGHT_BLUE = 0x9, // 10
  VGA_COLOR_LIGHT_GREEN = 0xA, // 11
  VGA_COLOR_LIGHT_CYAN = 0xB, // 12
  VGA_COLOR_LIGHT_RED = 0xC, // 12
  VGA_COLOR_LIGHT_MAGENTA = 0xD, // 13
  VGA_COLOR_LIGHT_BROWN = 0xE, // 14
  VGA_COLOR_WHITE = 0xF, // 15
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 0x4; // 4
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 0x8;
}

static const size_t VGA_WIDTH = 0x50; // 80
static const size_t VGA_HEIGHT = 0x19; // 25

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_initialize(enum vga_color fg, enum vga_color bg) {
  terminal_row = 0x0;
  terminal_column = 0x0;
  terminal_color = vga_entry_color(fg, bg);
  /* 0xB8000 is where the text video memory starts for color monitors 
    Read about it Here: https://wiki.osdev.org/Printing_to_Screen */
  terminal_buffer = (uint16_t *)0xB8000; 
  for (size_t y = 0x0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0x0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
  if ((c == '\n') || (terminal_column == VGA_WIDTH)) {
    terminal_column = 0x0;
    if (++terminal_row == VGA_HEIGHT)
      terminal_row = 0x0;
  } else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
      terminal_column = 0x0;
      if (++terminal_row == VGA_HEIGHT)
        terminal_row = 0x0;
    }
  }
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
  terminal_write(data, strlen(data));
}

extern "C" void kernel_main(void) {
  /* Initialize terminal interface */
  terminal_initialize(VGA_COLOR_BLUE, VGA_COLOR_BLACK);

  /* Newline support is left as an exercise. */
  terminal_writestring("ooooo ooooo ooooo\n 888   888   888\n 888ooo888   "
                       "888\n 888   888   888\no888o o888o o888o\n");
}

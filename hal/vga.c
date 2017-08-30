#include "hal.h"
#include <stdint.h>
#include <string.h>

// Necessary variables
size_t cursorx;
size_t cursory;
uint8_t console_color;
uint16_t *terminal_buffer;
static uint8_t clear_calls = 0;

static size_t getIndex(size_t x, size_t y)
{
    const size_t index = y * 80 + x;
    return index;
}

static uint16_t getVgaByte(char uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

uint8_t getColor(vga_color text, vga_color background)
{
    return text | background << 4;
}

// Function to clear screen
void clear_screen()
{
    for (size_t y = 1; y < 25; y++)
    {
        for (size_t x = 0; x < 80; x++)
        {
            size_t index = getIndex(x, y);
            terminal_buffer[index] = getVgaByte(' ', console_color);
        }
    }
    if (clear_calls != 0) {
        cursorx = 0;
        cursory = 1;
    }
    clear_calls++;
}

// Function to move the cursor
static void move_cursor()
{
    size_t cursorIndex = getIndex(cursorx, cursory);
    outb(0x3D4, 14);
    outb(0x3D5, cursorIndex >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursorIndex);
}

static void scrollConsole()
{
    // Just for now
    if (cursory == 25)
    {
        cursory = 1;
    }
}

void setConsoleColor(uint8_t color)
{
    console_color = color;
}

void consolePutChar(char c, uint8_t color, size_t x, size_t y)
{
    size_t index = getIndex(x, y);
    terminal_buffer[index] = getVgaByte(c, color);
}

void kputc(char c)
{
    if (c == '\n')
    {
        cursorx = 0;
        cursory++;
    }
    else if (c == '\r')
    {
        cursorx = 0;
    }
    else if (c == 0x08)
    {
        consolePutChar(0x20, console_color, --cursorx, cursory);
    }
    else if (c == 0x09)
    {
        cursorx += 4;
    }
    else
    {
        consolePutChar(c, console_color, cursorx, cursory);
    }

    if (c != '\n')
    {
        cursorx++;
    }

    if (cursorx >= 80)
    {
        cursorx = 0;
        cursory++;
    }
    scrollConsole();
}

/* void kputc(char c)
{
    consolePutChar(c, console_color, cursorx, cursory);
    if (++cursorx == 80)
    {
        cursorx = 0;
        if (++cursory == 25)
            cursory = 0;
    }
} */

void kprint(char *c)
{
    size_t i = 0;
    while (c[i])
    {
        kputc(c[i++]);
    }
}

void writeStyledString(char *c, uint8_t color)
{
    size_t i = 0;
    while (c[i])
    {
        if (c[i] == '\n')
        {
            kputc(c[i]);
            break;
        }
        consolePutChar(c[i], color, cursorx, cursory);
        i++;
        cursorx++;
        if (cursorx >= 80)
        {
            cursorx = 0;
            cursory++;
        }
        scrollConsole();
    }
}

/*size_t strlen(char *str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}*/

void startConsole()
{
    cursorx = 0;
    cursory = 0;
    terminal_buffer = (uint16_t *)0xB8000; // Change to 0xCB8000 after kernel is mapped to higher half.
    console_color = getColor(vga_light_grey, vga_black);
    uint8_t statusBarColor = getColor(vga_black, vga_light_grey);
    clear_screen();
    writeStyledString("JSLK Kernel 0.0.2 - 20170830", statusBarColor);
    for (size_t i = 0; i < (80 - strlen("JSLK Kernel 0.0.0 - 00000000")); i++)
    {
        writeStyledString(" ", statusBarColor);
    }
}

void kernelPrintDec(uint32_t n) {

    if (n == 0)
    {
        kputc('0');
        return;
    }

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while (i >= 0)
    {
        c2[i--] = c[j++];
    }
    kprint(c2);
}

void kernelPrintHex(uint32_t n) {
    int32_t tmp;

    kprint("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }

        if (tmp >= 0xA)
        {
            noZeroes = 0;
            kputc(tmp - 0xA + 'a');
        }
        else
        {
            noZeroes = 0;
            kputc(tmp + '0');
        }
    }

    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        kputc(tmp - 0xA + 'a');
    }
    else
    {
        kputc(tmp + '0');
    }
}
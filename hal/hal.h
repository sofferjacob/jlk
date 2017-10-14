#ifndef _x86_hal_h
#define _x86_hal_h

#ifdef _cplusplus
extern "C" {
#endif

// Include auxiliary header files
#include "vga.h"
#include "paging.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Screen I/O
void kputc(char c);
void kprint(char *c);
void writeStyledString(char *c, uint8_t color);
void consolePutChar(char c, uint8_t color, size_t x, size_t y);
void setConsoleColor(uint8_t color);
uint8_t getColor(vga_color text, vga_color background);
void clear_console();
void debugPrint(string msg);
void kernelPrintDec(uint32_t n);
void kernelPrintHex(uint32_t n);
int kprintf(string c, ...);
void setMenubarText(string text);

// Port I/O
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

// HAL Functions and Datatypes
void halInitialize();

enum mod {
    on,
    off,
};

typedef enum mod mode_t;

// System Operation Modes
uint8_t _interrupts(mode_t mode);
void _halt();
void _syscritical(mode_t mode);
void system_panic(string msg);

// System interrupt handler
#define TOTAL_INTERRUPTS 47
int registerInterruptHandler(uint8_t num, void(*hiHand));
void addDescription(uint8_t num, string description);
void genInterrupt(uint8_t num);
string getintDescription();
uint8_t findFreeInterrupt();

// System Timer Related functions
void start_pit(uint32_t freq);
size_t readSystemTime();
#define TICKSTOSEC 100 // 100 ticks = 1 sec at a frequency of 100 Mhz

// System Physical Memmory Manager (PMM)
typedef uint32_t physaddr; // So it doesn't go out of scope...
int pmmMapFirstFree();  // Returns a free frame
void pmmInit(size_t memSize, physaddr bitmap);  // Initializes the PMM
uint32_t getPmmSize();
void pmmFreeBlocks(void *p, size_t size);
void pmmFreeBlock(void *p);
void *pmmAllocBlocks(size_t size);
void *pmmAllocBlock();
void pmmDeinitRegion(physaddr base, size_t size);
void pmmInitRegion(physaddr base, size_t size);

// System Virtual Memory Manager (Paging)
int allocFrame(pageEntry_t* page, bool kernelMode, bool rw);
int freeFrame(pageEntry_t* page);
void initPaging();
void switchPageDirectory(pageDirectory_t* dir);
pageEntry_t* getPage(uint32_t addr, bool make, pageDirectory_t* dir);


// Other functions
#define KERNEL_PANIC(X) system_panic(X);

// Atomical Functions
/*========== WARNING ===========
Use only when necessary and in
quick functions only, else
interrupt latency  and kernel's
performance may be affected
or the kernel may get stuck.
=================================*/
void atomicalStart();
void atomicalRelease();

#ifdef _cplusplus
}
#endif

#endif

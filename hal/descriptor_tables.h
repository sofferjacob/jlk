#include <stdint.h>

// Initialisation function is publicly accessible.
void init_descriptor_tables();

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
struct gdt_entry_struct
{
    uint16_t limit_low;
    // GDT Base Low-Middle (Bits 16-39)
    uint16_t baseLow;
    uint8_t baseMiddle;
    // Flags (Gran and Access) (Bits 40-55)
    uint8_t flags;
    uint8_t gran;
    // Base High (Bits 56-63)
    uint8_t baseHigh;
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;

// This struct describes a GDT pointer. It points to the start of
// our array of GDT entries, and is in the format required by the
// lgdt instruction.
struct gdt_ptr_struct
{
    uint16_t limit; // The upper 16 bits of all selector limits.
    uint32_t base;  // The address of the first gdt_entry_t struct.
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;

// A struct describing an interrupt gate.
struct idt_entry_struct
{
    uint16_t base_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t sel;     // Kernel segment selector.
    uint8_t always0;  // This must always be zero.
    uint8_t flags;    // More flags. See documentation.
    uint16_t base_hi; // The upper 16 bits of the address to jump to.
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
    uint16_t limit;
    uint32_t base; // The address of the first element in our idt_entry_t array.
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

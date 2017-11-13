#ifndef __STDARG_H
#define __STDARG_H
//****************************************************************************
//**
//**    [FILE NAME]
//**    - [FILE DESCRIPTION]
//**
//****************************************************************************
//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <va_list.h>

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

#ifdef __cplusplus
extern "C" {
#endif

/* width of stack == width of int */
#define STACKITEM int

/* round up width of objects pushed on stack. The expression before the
& ensures that we get 0 for objects of size 0. */
#define VA_SIZE(TYPE) \
    ((sizeof(TYPE) + sizeof(STACKITEM) - 1) & ~(sizeof(STACKITEM) - 1))

/* &(LASTARG) points to the LEFTMOST argument of the function call
(before the ...) */
#define va_start(AP, LASTARG) \
    (AP = ((va_list) & (LASTARG) + VA_SIZE(LASTARG)))

/* nothing for va_end */
#define va_end(AP)

#define va_arg(ap, type) *(type *)(ap), \
                         ((ap) += ((sizeof(long) > sizeof(type)) ? sizeof(long) : sizeof(type)))

#ifdef __cplusplus
}
#endif

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END [FILE NAME]
//**
//****************************************************************************

#endif

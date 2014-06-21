/*
 * defs.cpp
 *
 * Basic type definitions.
 *
 */

# pragma once

# include <stdint.h>
# include <stddef.h>
# include <stdlib.h>

# define byte int8_t
# define byte_c INT8_C
# define ubyte uint8_t
# define ubyte_c UINT8_C
# define int16 int16_t
# define int16_c INT16_C
# define uint16 uint16_t
# define uint16_c UINT16_C
# define long32 int32_t
# define ulong32 uint32_t
# define longlong64 int64_t
# define ulonglong64 uint64_t

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#define TRUE 1
#define FALSE 0

enum Trigger_Edge {rising=1, falling=0};
enum Active_Sense {high=1, low=0};

// Used for EEPROM read / write access.
#include <avr/eeprom.h>
#define eeprom_read_to(dst_p, eeprom_field, dst_size) eeprom_read_block(dst_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(dst_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_read(dst, eeprom_field) eeprom_read_to(&dst, eeprom_field, sizeof(dst))
#define eeprom_write_from(src_p, eeprom_field, src_size) eeprom_write_block(src_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(src_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_write(src, eeprom_field) { typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x)); }
#define MIN(x,y) ( x > y ? y : x )
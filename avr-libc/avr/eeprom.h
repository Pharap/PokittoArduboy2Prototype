#pragma once

//
// Includes
//

#include <cstddef>
#include <cstdint>

#include "../__attrs__.h"

//
// General macros
//

#define EEMEM
#define eeprom_is_ready() (true)
#define eeprom_busy_wait() while(!eeprom_is_ready())

//
// EEPROM read functions
//

__ATTR_PURE__ std::uint8_t eeprom_read_byte(const std::uint8_t * pointer);
__ATTR_PURE__ std::uint16_t eeprom_read_word(const std::uint16_t * pointer);
__ATTR_PURE__ std::uint32_t eeprom_read_dword(const std::uint32_t * pointer);
__ATTR_PURE__ float eeprom_read_float(const float * pointer);
void eeprom_read_block(void * destination, const void * source, std::size_t size);

//
// EEPROM write functions
//

void eeprom_write_byte(std::uint8_t * pointer, std::uint8_t value);
void eeprom_write_word(std::uint16_t * pointer, std::uint16_t value);
void eeprom_write_dword(std::uint32_t * pointer, std::uint32_t value);
void eeprom_write_float(float * pointer, float value);
void eeprom_write_block(const void * source, void * destination, std::size_t size);

//
// EEPROM update functions
//

void eeprom_update_byte(std::uint8_t * pointer, std::uint8_t value);
void eeprom_update_word(std::uint16_t * pointer, std::uint16_t value);
void eeprom_update_dword(std::uint32_t * pointer, std::uint32_t value);
void eeprom_update_float(float * pointer, float value);
void eeprom_update_block(const void * source, void * destination, std::size_t size);

//
// IAR C compatibility defines
//

#define _EEPUT(address, value) eeprom_write_byte((std::uint8_t *)(address), (std::uint8_t)(value))
#define __EEPUT(address, value) eeprom_write_byte((std::uint8_t *)(address), (std::uint8_t)(value))
#define _EEGET(variable, address) (variable) = eeprom_read_byte((std::uint8_t *)(address))
#define __EEGET(variable, address) (variable) = eeprom_read_byte((std::uint8_t *)(address))
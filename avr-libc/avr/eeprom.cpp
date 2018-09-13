#include "eeprom.h"

//
// Includes
//

#include <Pokitto.h>

//
// EEPROM read functions
//

// __ATTR_PURE__
std::uint8_t eeprom_read_byte(const std::uint8_t * pointer)
{
	(void)pointer;
	
    std::uint8_t value = ~0;
	// TODO: Implement eeprom_read_byte(const std::uint8_t * pointer)
    //readEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(std::uint8_t));
	return value;
}

// __ATTR_PURE__
std::uint16_t eeprom_read_word(const std::uint16_t * pointer)
{
	(void)pointer;
	
    std::uint16_t value = ~0;
	// TODO: Implement eeprom_read_word(const std::uint16_t * pointer)
    //readEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(std::uint16_t));
	return value;
}

// __ATTR_PURE__
std::uint32_t eeprom_read_dword(const std::uint32_t * pointer)
{
	(void)pointer;
	
    std::uint32_t value = ~0;
	// TODO: Implement eeprom_read_dword(const std::uint32_t * pointer)
    //readEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(std::uint32_t));
	return value;
}

// __ATTR_PURE__
float eeprom_read_float(const float * pointer)
{
	(void)pointer;
	
    float value = ~0;
	// TODO: Implement eeprom_read_float(const float * pointer)
    //readEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(float));
	return value;
}

void eeprom_read_block(void * destination, const void * source, size_t count)
{
	(void)destination;
	(void)source;
	(void)count;
	
	// TODO: Implement eeprom_read_block(void * destination, const void * source, size_t count)
    //readEEPROM((std::uint16_t *)source, (std::uint8_t *)destination, count);
}

//
// EEPROM write functions
//

void eeprom_write_byte(std::uint8_t * pointer, std::uint8_t value)
{
	(void)pointer;
	(void)value;

	// TODO: Implement eeprom_write_byte(std::uint8_t * pointer, std::uint8_t value)
    //writeEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(std::uint8_t));
}

void eeprom_write_word(std::uint16_t * pointer, std::uint16_t value)
{
	(void)pointer;
	(void)value;

	// TODO: Implement eeprom_write_word(std::uint16_t * pointer, std::uint16_t value)
    //writeEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(std::uint16_t));
}

void eeprom_write_dword(std::uint32_t * pointer, std::uint32_t value)
{
	(void)pointer;
	(void)value;

	// TODO: Implement eeprom_write_dword(std::uint32_t * pointer, std::uint32_t value)
    //writeEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(std::uint32_t));
}

void eeprom_write_float(float * pointer, float value)
{
	(void)pointer;
	(void)value;

	// TODO: Implement eeprom_write_float(float * pointer, float value)
    //writeEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(float));
}

void eeprom_write_block(const void * source, void * destination, size_t count)
{
	(void)destination;
	(void)source;
	(void)count;
	
	// TODO: Implement eeprom_write_block(const void * source, void * destination, size_t count)
    //writeEEPROM((std::uint16_t *)source, (std::uint8_t *)destination, count);
}

//
// EEPROM update functions
//

void eeprom_update_byte(std::uint8_t * pointer, std::uint8_t value)
{
	(void)pointer;
	(void)value;

	// TODO: Implement eeprom_update_byte(std::uint8_t * pointer, std::uint8_t value)
	//auto current = eeprom_read_byte(pointer);
	//if(current != value)
		//writeEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(std::uint8_t));
}

void eeprom_update_word(std::uint16_t * pointer, std::uint16_t value)
{
	(void)pointer;
	(void)value;

	// TODO: Implement eeprom_update_word(std::uint16_t * pointer, std::uint16_t value)
	//auto current = eeprom_read_word(pointer);
	//if(current != value)
		//writeEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(std::uint16_t));
}

void eeprom_update_dword(std::uint32_t * pointer, std::uint32_t value)
{
	(void)pointer;
	(void)value;

	// TODO: Implement eeprom_update_dword(std::uint32_t * pointer, std::uint32_t value)
	//auto current = eeprom_read_dword(pointer);
	//if(current != value)
		//writeEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(std::uint32_t));
}

void eeprom_update_float(float * pointer, float value)
{
	(void)pointer;
	(void)value;

	// TODO: Implement eeprom_update_float(float * pointer, float value)
	//auto current = eeprom_read_float(pointer);
	//if(current != value)
		//writeEEPROM(const_cast<std::uint16_t *>(reinterpret_cast<const std::uint16_t *>(pointer)), const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(&value)), sizeof(float));
}

void eeprom_update_block(const void * source, void * destination, size_t count)
{
	(void)destination;
	(void)source;
	(void)count;

	// TODO: Implement eeprom_update_block(const void * source, void * destination, size_t count)
    //writeEEPROM((std::uint16_t *)source, (std::uint8_t *)destination, count);
}
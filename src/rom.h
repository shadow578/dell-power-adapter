#ifndef rom_h
#define rom_h
#include <avr/pgmspace.h>
typedef unsigned char uint8_t;

// ~~ ROM data ~~

/**
 * power adapter vendor
 * this should pretty much always be 'DELL', tho it seems to not be validated
 * (4 bytes)
 */
#define ROM_VENDOR "DELL"

/**
 * power adapter type
 * (4 bytes)
 */
#define ROM_TYPE "00AC"

/**
 * power adapter wattage. This seems to be whitelisted by dell, so make sure to use wattages that dell actually sells.
 * '045' for 45W
 * 
 * DO NOT set this higher than your actual power adapter can handle!
 * (3 bytes)
 */
#define ROM_WATT "045"

/**
 * power adapter voltage, in tenths of volts
 * '195' for 19.5V
 * 
 * DO NOT set this higher than your actual power adapter can handle!
 * (3 bytes)
 */
#define ROM_VOLTAGE "195"

/**
 * power adapter amperage, in tenths of a ampere
 * '023' is 2.3A
 * 
 * DO NOT set this higher than your actual power adapter can handle!
 * (3 bytes)
 */
#define ROM_AMPERAGE "023"

/**
 * power adapter serial number
 * (23 bytes)
 */
#define ROM_SERIAL_NUMBER "CN0CDF577243865Q27F2A05"

/**
 * CRC16 checksum of the rom contents.
 * to calculate this, concatenate all values above into one string and calculate the CRC-16/ARC of that string.
 * you can use a website like https://crccalc.com/ for that.
 * 
 * Example String for checksum calculation: DELL00AC045195023CN0CDF577243865Q27F2A05 (would yield a checksum of 0x943D)
 */
#define ROM_CHECKSUM 0x943D

// ~~ Background Magic ~~
#define EEPROM_DATA_LENGTH 128

#define ROM_LENGTH 40
static const PROGMEM char rom_data[ROM_LENGTH + 1] = (ROM_VENDOR ROM_TYPE ROM_WATT ROM_VOLTAGE ROM_AMPERAGE ROM_SERIAL_NUMBER);

static const uint8_t CHECKSUM_LO_BYTE = ROM_CHECKSUM & 0xff;
static const uint8_t CHECKSUM_HI_BYTE = (ROM_CHECKSUM >> 8) & 0xff;
#define FILL_BYTE 0xff;

inline uint8_t rom_read_byte(const uint8_t *p)
{
    uint8_t index = *p;
    if (index < ROM_LENGTH)
    {
        // rom data
        return pgm_read_byte(rom_data + index);
    }
    else if (index < ROM_LENGTH + 1)
    {
        // checksum LSB
        return CHECKSUM_LO_BYTE;
    }
    else if (index < ROM_LENGTH + 2)
    {
        // checksum MSB
        return CHECKSUM_HI_BYTE;
    }
    else
    {
        // out- of- bounds, return filler
        return FILL_BYTE
    }
}

#endif
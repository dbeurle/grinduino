
#pragma once

#include <stdint.h>

namespace og
{
/// Stores the offset for the EEPROM module on the Arduino
class persistent_storage
{
public:
    static auto allocate() -> uint8_t { return m_byte_offset++; }

private:
    static uint8_t m_byte_offset;
};
}

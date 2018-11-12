
#pragma once

namespace og
{
/// Stores the offset for the EEPROM module on the Arduino
class persistent_storage
{
public:
    auto allocate() -> int { return m_byte_offset++; }

private:
    static int m_byte_offset;
};
}

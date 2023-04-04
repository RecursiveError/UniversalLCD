#include <stdint.h>

namespace universalLCD {
    /// @brief Bus interface for UniversalLCD
    struct BusInterface{
        /// @brief function responsible for sending the information to the LCD, see the API documentation for more information
        /// @param config Represents the state of the config pins (EN2 | EN | R/W |RS)
        /// @param data Represents the state of the data pins (D7-D0)
        virtual void send(uint8_t config, uint8_t data) = 0;
    };
}

#include "businterface.hpp"

namespace universalLCD{

    enum BusType {
    Bus4Bits,
    Bus8Bits = 0x10,
    };


enum Command {
    LCDClear = 0x01,
    LCDReset = 0x02,
    LCDShiftCursotLeft = 0x10,
    LCDShiftCursotRight = 0x14,
    LCDShiftDisplayLeft = 0x18,
    LCDShiftDisplayRight = 0x1C,
};


enum EntryMode{
    LCDShiftMode = 0x01,
    LCDDirection = 0x02,
};

enum isplayControl{
    LCDBlink = 0x01,
    LCDCursor = 0x02,
    LCDDisplay = 0x04,

};

enum FunctionSet{
    LCDChar = 0x04,
    LCDLines = 0x08,
    LCDBits = 0x10,
};

enum CharSize {
    Char5x8,
    Char5x10 = 0x04,
};

/// @brief delay callback type
using delayUs = void(*)(uint16_t);

/// @brief UniversalLCD main class
class UniversalLCD{
    private:
        BusInterface &bridge;
        const BusType bus; // 4 or 8 bit communication bus
        delayUs delay; // delayUs function callback
        static const uint8_t addrs[]; // LCD addresses

        // configuration variables initialized with default configuration
        uint8_t entry_mode{0x06}; // shift Off, written from left to right
        uint8_t display_control{0x0C}; // display on, cursor off, cursor blinking off
        uint8_t function_set{0}; // configured in constructor
        uint8_t enable_set{0b1100}; // SELECT ALL LCDs

        void send4Bits(uint8_t data, uint8_t RS_state);
        void send8Bits(uint8_t data, uint8_t RS_state);

    public:
        /// @brief class constructor
        /// @param _bridge reference of a BusInterface interface
        /// @param _bus communication bits (Bus4Bits Bus8Bits)
        /// @param _delay delayUs function callback
        /// @param line number of display lines (based on the controller, only 1 or 2 lines)
        /// @param size character size (Char5x8, Char5x10)
        UniversalLCD(BusInterface &_bridge, const BusType _bus, delayUs _delay ,const uint8_t line = 2, CharSize size = Char5x8) :
        bridge{_bridge},
        bus{_bus},
        delay{_delay}
        {
            if(line < 2){
                function_set = 0x20 | bus | size;
                return;
            }
            function_set = 0x20 | 0x08 | bus | size;
        }
        /// @brief low level send function
        /// @param data - data to send
        /// @param RS_state 0 = command / 1 - text
        UniversalLCD& send(uint8_t data, uint8_t RS_state);

        /// @brief Starts the display
        UniversalLCD& begin();

        /// @brief function to send commands from LCD command list
        /// @param Command Command enum
        UniversalLCD& command(Command);

        /// @brief Clears the display
        UniversalLCD& clear();

        /// @brief Resets the display's internal variables
        UniversalLCD& reset();

        /// @brief Moves the cursor to the left
        UniversalLCD& moveCursorLeft();

        /// @brief Moves the cursor to the right
        UniversalLCD& moveCursorRight();

        /// @brief Moves the display left
        UniversalLCD& moveDisplayLeft();

        /// @brief Moves the display right
        UniversalLCD& moveDisplayRight();

        /// @brief Enables autoscroll
        UniversalLCD& shiftOn();

        /// @brief Disables autoscroll
        UniversalLCD& shiftOff();

        /// @brief  Autoscroll increments the position
        UniversalLCD& increment();

        /// @brief Autoscroll decrements the position
        UniversalLCD& decrement();

        /// @brief Turns on blinking cursor
        UniversalLCD& cursorBlinkOn();

        /// @brief Turns off cursor blinking
        UniversalLCD& cursorBlinkOff();

        /// @brief Turns on the cursor
        UniversalLCD& cursorOn();

        /// @brief Turns off the cursor
        UniversalLCD& cursorOff();

        /// @brief Turns on the display
        UniversalLCD& displayOn();

        /// @brief Turn off the display
        UniversalLCD& displayOff();

        /// @brief Moves the cursor (count starts from 0)
        /// @param row
        /// @param column
        UniversalLCD& setCursor(uint8_t row, uint8_t column);

        /// @brief Creates a custom character
        /// @param cchar char array [8]
        /// @param slot slot 0-7
        UniversalLCD& createChar(uint8_t cchar[8], uint8_t slot);

        /// @brief Selected all LCDs (enables all ENs)
        UniversalLCD& echo();

        /// @brief Selects an lcd
        /// @param enable 0 = EN1 (Default) | 1 = EN2
        UniversalLCD& selectLcd(uint8_t enable = 0);

        /// @brief send a text to the LCD
        /// @param text c-string
        UniversalLCD& UniversalLCD::write_limit(const char text[], unsigned int size);
        UniversalLCD& UniversalLCD::write_char_delimiter(const char text[], char delimiter);
        UniversalLCD& write(const char* text);
    };

}
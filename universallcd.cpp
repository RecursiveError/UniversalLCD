#include "universallcd.hpp"

namespace universalLCD{

const uint8_t UniversalLCD::addrs[] = {0x80, 0xC0};

void UniversalLCD::send8Bits(uint8_t data, uint8_t RS_state){
    bridge.send(RS_state, data);
    bridge.send(RS_state | enable_set, data);
    delay(1);
    bridge.send(RS_state, data);
}


void UniversalLCD::send4Bits(uint8_t data, uint8_t RS_state){
    uint8_t high_nibble = data & 0xF0;
    uint8_t low_nibble = data << 4;
    send8Bits(high_nibble, RS_state);
    delay(1);
    send8Bits(low_nibble, RS_state);
}

UniversalLCD& UniversalLCD::send(uint8_t data, uint8_t RS_state){
    if(bus == Bus8Bits){
        send8Bits(data, RS_state);
    }else{
        send4Bits(data, RS_state);
    }

    if(RS_state == 1){
        delay(2);
    }
    else{
        delay(40);
    }
    return *this;
}

UniversalLCD& UniversalLCD::begin(){
    delay(55000); //change 2024-01-10 (add power on wait time + init time) (datasheet: power up time = mora than 40ms| begin time = more than 15ms)
    send8Bits(0x30, 0);
    delay(4100);
    send8Bits(0x30, 0);
    delay(100);
    send8Bits(0x30, 0);
    delay(100);

    if(bus == Bus4Bits){
        send8Bits(0x20, 0);
    }

    send(function_set, 0);
    clear();
    reset();
    send(entry_mode, 0);
    send(display_control, 0);
    selectLcd(0);
    return *this;
}

/* ----------------- DISPLAY COMMANDS ---------------------*/


inline UniversalLCD& UniversalLCD::command(Command command){
    send(command, 0);
    return *this;
}

UniversalLCD& UniversalLCD::clear(){
    command(LCDClear);
    delay(1600); //clear e reset demoram 1.52ms para executar
    return *this;
}

UniversalLCD& UniversalLCD::reset(){
    command(LCDReset);
    delay(1600); //clear e reset demoram 1.52ms para executar
    return *this;
}

inline UniversalLCD& UniversalLCD::moveCursorLeft(){
    command(LCDShiftCursotLeft);
    return *this;
}

inline UniversalLCD& UniversalLCD::moveCursorRight(){
    command(LCDShiftCursotRight);
    return *this;
}

inline UniversalLCD& UniversalLCD::moveDisplayLeft(){
    command(LCDShiftDisplayLeft);
    return *this;
}

inline UniversalLCD& UniversalLCD::moveDisplayRight(){
    command(LCDShiftDisplayRight);
    return *this;
}

/* -----------------END OF DISPLAY COMMANDS ---------------------*/

/* ------------------ DISPLAY CONFIGURATION ------------------------*/


UniversalLCD& UniversalLCD::shiftOn(){
    entry_mode |= LCDShiftMode;
    send(entry_mode, 0);
    return *this;
}

UniversalLCD& UniversalLCD::shiftOff(){
    entry_mode &= ~LCDShiftMode;
    send(entry_mode, 0);
    return *this;
}

UniversalLCD& UniversalLCD::increment(){
    entry_mode |= LCDDirection;
    send(entry_mode,0);
    return *this;
}

UniversalLCD& UniversalLCD::decrement(){
    entry_mode &= ~LCDDirection;
    send(entry_mode,0);
    return *this;
}

UniversalLCD& UniversalLCD::cursorBlinkOn(){
    display_control |= LCDBlink;
    send(display_control,0);
    return *this;
}

UniversalLCD& UniversalLCD::cursorBlinkOff(){
    display_control &= ~LCDBlink;
    send(display_control,0);
    return *this;
}

UniversalLCD& UniversalLCD::cursorOn(){
    display_control |= LCDCursor;
    send(display_control,0);
    return *this;
}

UniversalLCD& UniversalLCD::cursorOff(){
    display_control &= ~LCDCursor;
    send(display_control,0);
    return *this;
}

UniversalLCD& UniversalLCD::displayOn(){
    display_control |= LCDDisplay;
    send(display_control,0);
    return *this;
}

UniversalLCD& UniversalLCD::displayOff(){
    display_control &= ~LCDDisplay;
    send(display_control,0);
    return *this;
}
/*------------------- END DISPLAY CONFIGURATION ---------------------*/

UniversalLCD& UniversalLCD::setCursor(uint8_t line, uint8_t col){
    if((line < 2) && (col < 40)){
        send(addrs[line]+col, 0);
    }
    return *this;
}

UniversalLCD& UniversalLCD::createChar(uint8_t c[8], uint8_t pos){
    pos &= 0b00000111;
    send(0x40 | (pos<<3), 0);
    for(int i = 0; i < 8; i++){
        send(c[i], 1);
    }
    return *this;
}


UniversalLCD& UniversalLCD::echo(){
    enable_set = 0b1100;
    return *this;
}

UniversalLCD& UniversalLCD::selectLcd(uint8_t enable){
    if(enable < 2){
        enable_set = 0b100<<enable;
    }
    return *this;
}

UniversalLCD& UniversalLCD::write_char_delimiter(const char text[], char delimiter){
    for(int i = 0; text[i] != delimiter ; i++){
        send(text[i], 1);
    }
    return *this;
}


UniversalLCD& UniversalLCD::write_limit(const char text[], unsigned int size){
    for(int i = 0; i < size ; i++){
        send(text[i], 1);
    }
    return *this;
}

UniversalLCD& UniversalLCD::write(const char text[]){
    return write_char_delimiter(text, '\0');
}

}


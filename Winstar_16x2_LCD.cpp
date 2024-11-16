#include "Winstar_16x2_LCD.h"

Winstar_16x2_LCD::Winstar_16x2_LCD(
    PinName DB_0_pin,
    PinName DB_1_pin,
    PinName DB_2_pin,
    PinName DB_3_pin,
    PinName DB_4_pin,
    PinName DB_5_pin,
    PinName DB_6_pin,
    PinName DB_7_pin
    PinName EN_pin,
    PinName RS_pin
    #ifdef RW_ENABLE
        ,PinName RW_pin = NC,
    #endif
) :lcd_bus(
    DB_0_pin,
    DB_1_pin,
    DB_2_pin,
    DB_3_pin,
    DB_4_pin,
    DB_5_pin,
    DB_6_pin,
    DB_7_pin,
    EN_pin,
    RS_pin
    #ifdef RW_ENABLE
        ,RW_pin
    #endif
    ){
    this->WriteIns(SET_8BIT_INTERFACE_CMD);
    this->WriteIns(SET_8BIT_INTERFACE_CMD);
    this->WriteIns(DISP_ON_CMD);
    this->WriteIns(CLEAR_DISP_CMD);
    ThisThread::sleep_for(2); // Sleep for 2 ms (minimum 1.53 ms)
    this->WriteIns(SET_TOP_LEFT_ORIGIN_CMD);
};

void Winstar_16x2_LCD::WriteIns(uint8_t instruction){
    RS_state = 0
    #ifdef RW_ENABLE
        RW_state = 0;
        lcd_bus.output();
    #endif
    data = instruction;
    EN_state = true;
    this->updateCombinedData();
    lcd_bus.write(this->combined_data);
    #ifdef RW_ENABLE
        this->CheckBusy();
    #else
        ThisThread::sleep_for(1);
    #endif
    EN_state = false;
    this->updateCombinedData();
    lcd_bus.write(this->combined_data);
};

void Winstar_16x2_LCD::updateCombinedData(){
    #ifdef RW_ENABLE
        this->combined_data = data | EN_state << EN_BASE | RS_state << RS_BASE | RW_state << RW_BASE;
    #else 
        this->combined_data = data | EN_state << EN_BASE | RS_state << RS_BASE;
    #endif
};

int Winstart_16x2_LCD::WriteData(uint8_t data){
    RS_state = 1;
    #ifdef RW_ENABLE
        RW_state = 0;
        lcd_bus.output();
    #endif
    this->data = data;
    EN_state = true;
    this->updateCombinedData();
    lcd_bus.write(this->combined_data);
    #ifdef RW_ENABLE
        this->CheckBusy();
    #else
        ThisThread::sleep_for(1);
    #endif
    EN_state = false;
    this->updateCombinedData();
    lcd_bus.write(this->combined_data);
    return kWinstar16x2LcdOk;
};

int Winstart_16x2_LCD::WriteLine0(string msg){
    uint msg_len = msg.size();
    if (msg_len > 16){
        return kWinstar16x2LcdMsgTooLongErr;
    }
    this->WriteIns(SET_DDRAM_LINE_0);
    for (uint8_t i = 0; i < msg_len; i++){
        this->WriteData(msg[i]);
    }
    return kWinstar16x2LcdOk;
};

int Winstart_16x2_LCD::WriteLine1(string msg){
    uint msg_len = msg.size();
    if (msg_len > 16){
        return kWinstar16x2LcdMsgTooLongErr;
    }
    this->WriteIns(SET_DDRAM_LINE_1);
    for (uint8_t i = 0; i < msg_len; i++){
        this->WriteData(msg[i]);
    }
    return kWinstar16x2LcdOk;
};

int Winstart_16x2_LCD::WriteString(string msg){
    uint8_t msg_len = msg.size();
    // TODO Update Function
    for (uint8_t i = 0; i < msg_len; i++){
        this->WriteData(msg[i]);
    }
}

#ifdef RW_ENABLE
void Winstar_16x2_LCD::CheckBusy(){
    uint16_t temp;
    this->RS_state = 0;
    this->RW_state = 1;
    do {
        this->EN_state = 1;
        this->updateCombinedData();
        lcd.output();
        lcd_bus.write(this->combined_data);
        lcd.input();
        temp = lcd.read();  
        busy_f = temp & (1 << BUSY_FLAG_BASE);
        this->EN_state = 0;
    } while (busy_f);
};
#endif
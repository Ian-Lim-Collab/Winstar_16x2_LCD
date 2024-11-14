#pragma once
#import "mbed.h"
#import <string>

#define DATA_BUS_BASE   0
#define DATA_BUS_WIDTH  8
#define EN_BASE         DATA_BUS_BASE + DATA_BUS_WIDTH
#define EN_WIDTH        1
#define RW_BASE         EN_BASE + EN_WIDTH
#define RW_WIDTH        1
#define RS_BASE         RW_BASE + RW_WIDTH
#define RS_WIDTH        1

#define BUSY_FLAG_BASE  7
#define BUSY_FLAG_WIDTH 1

class Winstar_16x2_LCD{
    public:
        Winstar_16x2_LCD(   PinName RW_pin,
                            PinName RS_pin,
                            PinName EN_pin,
                            PinName DB_0_pin,
                            PinName DB_1_pin,
                            PinName DB_2_pin,
                            PinName DB_3_pin,
                            PinName DB_4_pin,
                            PinName DB_5_pin,
                            PinName DB_6_pin,
                            PinName DB_7_pin
                        );
        void WriteIns(uint8_t instruction);
        int WriteData(uint8_t data);
        int WriteString(string msg);
        int WriteLine0(string msg);
        int WriteLine1(String msg);
        int CheckBusy();
    protected:
        const uint8_t SET_8BIT_INTERFACE_CMD    = 0x38;
        const uint8_t DISP_ON_CMD               = 0x0C;
        const uint8_t CLEAR_DISP_CMD            = 0x01;
        const uint8_t SET_TOP_LEFT_ORIGIN_CMD   = 0x06; //Also used to disable autoshift
        const uint8_t SET_DDRAM_LINE_0          = 0x80;
        const uint8_t SET_DDRAM_LINE_1          = 0xC0;

        bool RS_state;
        bool EN_state;
        uint8_t data;
        uint16_t combined_data;

        enum Winstar_16x2_LCD_Error{
            kWinstar16x2LcdOk = 0,
            kWinstar16x2LcdMsgTooLongErr = -100
        };

        void updateCombinedData();
        #ifdef RW_ENABLE
            bool RW_state;
            BusInOut lcd_bus();
        #else 
            BusOut lcd_bus();
        #endif
};
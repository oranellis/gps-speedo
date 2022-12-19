#include "oled.h"

U8G2 *u8g2;

void Display::init() {
    u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
    u8g2->begin();
    u8g2->clearBuffer();
    u8g2->setFont(u8g2_font_inr46_mf);
    u8g2->setCursor(0, 49);
    u8g2->print("GPS");
    u8g2->sendBuffer();
}

void Display::SetUnits(char units_str[4]) {
    for (int i=0; i < 4; i++) {
        units[i] = units_str[i];
    }
}

void Display::SetSpeed(float speed_inp, int sats) {
    u8g2->clearBuffer();
    u8g2->setFont(u8g2_font_logisoso46_tn);
    u8g2->setCursor(0, 49);
    u8g2->printf("%.1f\n", speed_inp);
    u8g2->setFont(u8g2_font_profont17_mr);
    u8g2->setCursor(0, 64);
    u8g2->print(0.00);
    u8g2->setCursor(42, 64);
    u8g2->printf("*%i", sats);
    u8g2->drawStr(84, 64, "km/h");
    u8g2->sendBuffer();
}

void Display::NoMessages() {
    u8g2->clearBuffer();
    u8g2->setFont(u8g2_font_helvR08_tr);
    u8g2->drawButtonUTF8(64, 36, U8G2_BTN_INV|U8G2_BTN_BW2|U8G2_BTN_HCENTER, 0,  2,  2, "No GPS Data" );
    u8g2->sendBuffer();
}
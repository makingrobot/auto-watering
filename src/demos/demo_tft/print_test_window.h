#include "config.h"
#if APP_DEMO_TFT==1

#ifndef _PRINT_TEST_WINDOW_H
#define _PRINT_TEST_WINDOW_H

#include <TFT_eSPI.h>
#include <string>

class PrintTestWindow : public TftWindow {
public:
    PrintTestWindow() { }

    void Setup(TFT_eSPI* driver) override {
        tft = driver;
    }

    void SetStatus(const std::string& status) override { }
    void SetText(uint8_t line, const std::string& text) override { }
   
    void Start() {
        xTaskCreate([this]() {
            while(1) {
                // Fill screen with grey so we can see the effect of printing with and without 
                // a background colour defined
                tft->fillScreen(TFT_GREY);
                
                // Set "cursor" at top left corner of display (0,0) and select font 2
                // (cursor will move to next line automatically during printing with 'tft.println'
                //  or stay on the line is there is room for the text with tft.print)
                tft->setCursor(0, 0, 2);
                // Set the font colour to be white with a black background, set text size multiplier to 1
                tft->setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
                // We can now plot text on screen using the "print" class
                tft->println("Hello World!");
                
                // Set the font colour to be yellow with no background, set to font 7
                tft->setTextColor(TFT_YELLOW); tft.setTextFont(7);
                tft->println(1234.56);
                
                // Set the font colour to be red with black background, set to font 4
                tft->setTextColor(TFT_RED,TFT_BLACK);    tft.setTextFont(4);
                //tft.println(3735928559L, HEX); // Should print DEADBEEF

                // Set the font colour to be green with black background, set to font 4
                tft->setTextColor(TFT_GREEN,TFT_BLACK);
                tft->setTextFont(4);
                tft->println("Groop");
                tft->println("I implore thee,");

                // Change to font 2
                tft->setTextFont(2);
                tft->println("my foonting turlingdromes.");
                tft->println("And hooptiously drangle me");
                tft->println("with crinkly bindlewurdles,");
                // This next line is deliberately made too long for the display width to test
                // automatic text wrapping onto the next line
                tft->println("Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!");
                
                // Test some print formatting functions
                float fnumber = 123.45;
                // Set the font colour to be blue with no background, set to font 4
                tft->setTextColor(TFT_BLUE);    tft.setTextFont(4);
                tft->print("Float = "); tft.println(fnumber);           // Print floating point number
                tft->print("Binary = "); tft.println((int)fnumber, BIN); // Print as integer value in binary
                tft->print("Hexadecimal = "); tft.println((int)fnumber, HEX); // Print as integer number in Hexadecimal
            
                delay(10000);
            }
        }, "Test_Task", 4096, NULL, 1, NULL),
    }

private:

    TFT_eSPI* tft = nullptr;

};

#endif //_PRINT_TEST_WINDOW_H

#endif
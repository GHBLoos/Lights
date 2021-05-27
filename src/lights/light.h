#ifndef _light_H_
#define _light_H_

#include <Arduino.h>

#include "diyHue.h"


/**
 * @brief Represents a light
 *
 * The constructor ...
 *
 * @param pixel_start ...
 * @param pixel_count ...
 * 
 **/
class light
{
private:
    uint8_t colors_[4], bri_ = 100, sat_ = 254, color_mode_ = 2;
    ledRGBW led_;
    currentledRGBW current_led_;
    bool light_state_;
    int ct_ = 200, hue_;
    float step_level_[4], current_colors_[4], x_, y_;
    int pixel_start_, pixel_count_;
    bool in_transition_;

    RgbwColor convFloat(float color[4]);
    void convertHue();
    void convertXy();
    void convertCt();

public:
    light(int pixel_start = 0, int pixel_count = 0);
    ~light();
  
    std::vector<RgbwColor> lpixels();

    bool lightState();
    void lightState(bool lightstate);

    void processLightdata(float transitiontime);
    void lightEngine();

    uint8_t bri();
    void bri(uint8_t bri);

    uint8_t sat();
    void sat(uint8_t sat);

    uint8_t colorMode();
    void colorMode(uint8_t colormode);

    int ct();
    void ct(int ct);

    int hue();
    void hue(int hue);

    float x();
    void x(float x);

    float y();
    void y(float y);

    ledRGBW colors();
    void colors(ledRGBW led);
    currentledRGBW currentColors();
    void currentColors(currentledRGBW currentColors);



    // float stepLevel();
    // void stepLevel (float steplevel[4]);
};

#endif
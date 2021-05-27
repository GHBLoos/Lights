#include "light.h"

light::light(int pixel_start, int pixel_count)
{
    pixel_start_ = pixel_start;
    pixel_count_ = pixel_count;
}

light::~light() // destructor
{
}

RgbwColor light::convFloat(float color[4])
{
    return RgbwColor((uint8_t)color[0], (uint8_t)color[1], (uint8_t)color[2], (uint8_t)color[3]);
}

void light::lightEngine()
{
    {

        if (light_state_) //als licht aan staat
        {
            if (led_.colors[0] != current_led_.colors[0] || led_.colors[1] != current_led_.colors[1] || led_.colors[2] != current_led_.colors[2] || led_.colors[3] != current_led_.colors[3])
            {
                in_transition_ = true;          //in transition betekent dat de huidige kleur afwijkt van de eindkleur van één van de kleuren van de lamp.
                for (uint8_t k = 0; k < 4; k++) //voor elke led kleur (RGBW)
                {
                    if (led_.colors[k] != current_led_.colors[k]) // als de eindkleur nog niet de huidige kleur is,
                        current_led_.colors[k] += step_level_[k]; // dan de kleur een stapje wijzigen
                    if ((step_level_[k] > 0.0 && current_led_.colors[k] > led_.colors[k]) || (step_level_[k] < 0.0 && current_led_.colors[k] < led_.colors[k]))
                        current_led_.colors[k] = led_.colors[k]; // als de huidige kleur voorbij de eindkleur komt, dan is de huidige kleur de eindkleur
                }

                // strip->ClearTo(convFloat(current_led_.colors), 0, pixel_count_ - 1);
                // strip->Show();
            }
        }

        if (in_transition_)
        {
            delay(6);
            in_transition_ = false;
        }
    }
}
void light::convertHue()
{
    colors_[3] = 0;

    double hh_, p_, q_, t_, ff_, s_, v_;
    long i;

    s_ = sat_ / 255.0;
    v_ = bri_ / 255.0;

    if (s_ <= 0.0)
    { // < is bogus, just shuts up warnings
        colors_[0] = v_;
        colors_[1] = v_;
        colors_[2] = v_;
        return;
    }
    hh_ = hue_;
    if (hh_ >= 65535.0)
        hh_ = 0.0;
    hh_ /= 11850, 0;
    i = (long)hh_;
    ff_ = hh_ - i;
    p_ = v_ * (1.0 - s_);
    q_ = v_ * (1.0 - (s_ * ff_));
    t_ = v_ * (1.0 - (s_ * (1.0 - ff_)));

    switch (i)
    {
    case 0:
        colors_[0] = v_ * 255.0;
        colors_[1] = t_ * 255.0;
        colors_[2] = p_ * 255.0;
        break;
    case 1:
        colors_[0] = q_ * 255.0;
        colors_[1] = v_ * 255.0;
        colors_[2] = p_ * 255.0;
        break;
    case 2:
        colors_[0] = p_ * 255.0;
        colors_[1] = v_ * 255.0;
        colors_[2] = t_ * 255.0;
        break;

    case 3:
        colors_[0] = p_ * 255.0;
        colors_[1] = q_ * 255.0;
        colors_[2] = v_ * 255.0;
        break;
    case 4:
        colors_[0] = t_ * 255.0;
        colors_[1] = p_ * 255.0;
        colors_[2] = v_ * 255.0;
        break;
    case 5:
    default:
        colors_[0] = v_ * 255.0;
        colors_[1] = p_ * 255.0;
        colors_[2] = q_ * 255.0;
        break;
    }
}

void light::convertXy()
{
    colors_[3] = 0;

    int optimal_bri_ = bri_;
    if (optimal_bri_ < 5)
    {
        optimal_bri_ = 5;
    }
    float Y_ = y_;
    float X_ = x_;
    float Z_ = 1.0f - x_ - y_;

    // sRGB D65 conversion
    float r_ = X_ * 3.2406f - Y_ * 1.5372f - Z_ * 0.4986f;
    float g_ = -X_ * 0.9689f + Y_ * 1.8758f + Z_ * 0.0415f;
    float b_ = X_ * 0.0557f - Y_ * 0.2040f + Z_ * 1.0570f;

    // Apply gamma correction
    r_ = r_ <= 0.04045f ? r_ / 12.92f : pow((r_ + 0.055f) / (1.0f + 0.055f), 2.4f);
    g_ = g_ <= 0.04045f ? g_ / 12.92f : pow((g_ + 0.055f) / (1.0f + 0.055f), 2.4f);
    b_ = b_ <= 0.04045f ? b_ / 12.92f : pow((b_ + 0.055f) / (1.0f + 0.055f), 2.4f);

    float maxv_ = 0; // calc the maximum value of r g and b
    if (r_ > maxv_)
        maxv_ = r_;
    if (g_ > maxv_)
        maxv_ = g_;
    if (b_ > maxv_)
        maxv_ = b_;

    if (maxv_ > 0)
    {                // only if maximum value is greater than zero, otherwise there would be division by zero
        r_ /= maxv_; // scale to maximum so the brightest light is always 1.0
        g_ /= maxv_;
        b_ /= maxv_;
    }

    r_ = r_ < 0 ? 0 : r_;
    g_ = g_ < 0 ? 0 : g_;
    b_ = b_ < 0 ? 0 : b_;

    colors_[0] = (int)(r_ * optimal_bri_);
    colors_[1] = (int)(g_ * optimal_bri_);
    colors_[2] = (int)(b_ * optimal_bri_);
}

void light::convertCt()
{
    colors_[3] = bri_;
    int hectemp_ = 10000 / ct_;
    int r_, g_, b_;
    if (hectemp_ <= 66)
    {
        r_ = 255;
        g_ = 99.4708025861 * log(hectemp_) - 161.1195681661;
        b_ = hectemp_ <= 19 ? 0 : (138.5177312231 * log(hectemp_ - 10) - 305.0447927307);
    }
    else
    {
        r_ = 329.698727446 * pow(hectemp_ - 60, -0.1332047592);
        g_ = 288.1221695283 * pow(hectemp_ - 60, -0.0755148492);
        b_ = 255;
    }
    r_ = r_ > 255 ? 255 : r_;
    g_ = g_ > 255 ? 255 : g_;
    b_ = b_ > 255 ? 255 : b_;
    colors_[0] = r_ * (bri_ / 255.0f);
    colors_[1] = g_ * (bri_ / 255.0f);
    colors_[2] = b_ * (bri_ / 255.0f);
}

void light::processLightdata(float transition_time_)
{
    //deze functie wordt aangeroepen als er iets verandert via een webserver instructie en nadat de entertainment is uitgezet.
    //Het lijkt alsof stap 1 en 2 niets met elkaar te maken hebben. Toch worden ze altijd beiden doorlopen.
    //1. kleuren die de lampen moeten aannemer worden bepaald met de stappen hieronder
    if (color_mode_ == 1 && light_state_ == true)
    {
        convertXy();
    }
    else if (color_mode_ == 2 && light_state_ == true)
    {
        convertCt();
    }
    else if (color_mode_ == 3 && light_state_ == true)
    {
        convertHue();
    }

    //2. stapjes waarin de kleuren moeten worden aangepast wordt hieronder berekend.
    transition_time_ *= 17 - (pixel_count_ / 40); //every extra led add a small delay that need to be counted
    for (uint8_t i = 0; i < 4; i++)
    {
        if (light_state_)
        {
            step_level_[i] = ((float)colors_[i] - current_colors_[i]) / transition_time_; //stapjes bepalen om van huidige kleur naar nieuwe kleur te gaan (denk ik)
        }
        else
        {
            step_level_[i] = current_colors_[i] / transition_time_; //stapjes bepalen om te dimmen naar 0 (denk ik)
        }
    }
}

ledRGBW light::colors() { return led_; }

void light::colors(ledRGBW led) { led_ = led; }

uint8_t light::bri() { return bri_; }
void light::bri(uint8_t bri) { bri_ = bri; }

uint8_t light::sat() { return sat_; }
void light::sat(uint8_t sat) { sat_ = sat; }

uint8_t light::colorMode() { return color_mode_; }
void light::colorMode(uint8_t colormode) { color_mode_ = colormode; }

bool light::lightState() { return light_state_; }
void light::lightState(bool lightstate) { light_state_ = lightstate; }

int light::ct() { return ct_; }
void light::ct(int ct) { ct_ = ct; }

int light::hue() { return hue_; }
void light::hue(int hue) { hue_ = hue; }
// float light::stepLevel() { return step_level_[]; }

// void light::stepLevel(float steplevel[4]) { step_level_ = steplevel; }

currentledRGBW light::currentColors(){return current_led_;}

void light::currentColors(currentledRGBW currentcolors) { current_led_ = currentcolors; }

float light::x() { return x_; }
void light::x(float x) { x_ = x; }

float light::y() { return y_; }

void light::y(float y) { y_ = y; }
#include "switchButtons.h"
switchButtons::switchButtons(uint8_t onPin, uint8_t offPin)
{
    pinMode(onPin, INPUT);
    pinMode(offPin, INPUT);
};
switchButtons::~switchButtons(){};
bool switchButtons::onPressed() { return false; };
bool switchButtons::onPressedLong() { return false; };
bool switchButtons::offPressed() { return false; };
bool switchButtons::offPressedLong() { return false; };

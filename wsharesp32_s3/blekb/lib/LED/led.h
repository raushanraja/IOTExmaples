#include "Print.h"
#include <Adafruit_SSD1306.h>


class OLED {
    public:
        OLED();
        Adafruit_SSD1306 led;
        void init();
        void clear();
        void display(String text);
};
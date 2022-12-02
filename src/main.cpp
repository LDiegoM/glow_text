#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CS_PIN    5

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup(void) {
    P.begin();
}

void loop(void) {
    if (P.displayAnimate())
        P.displayText("This is a text!!", PA_LEFT, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

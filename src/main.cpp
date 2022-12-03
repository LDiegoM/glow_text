#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include <storage.h>
#include <display.h>
#include <wifi_connection.h>
#include <http_handlers.h>

// Define Parola display settings
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 5

Storage *storage;
WiFiConnection *wifi;
MD_Parola *parola;
Display *display = nullptr;

void setup(void) {
    parola = new MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
    parola->begin();

    storage = new Storage();
    if (!storage->begin())
        return;

    display = new Display(parola, storage);
    if (!display->begin()) {
        display->setMessage("display err");
        return;
    }

    wifi = new WiFiConnection();
    if (!wifi->begin()) {
        display->setMessage("wifi err");
        return;
    }

    httpHandlers = new HttpHandlers(wifi, storage, display);
    if (!httpHandlers->begin()) {
        display->setMessage("http err");
        return;
    }
}

void loop(void) {
    if (display != nullptr)
        display->loop();
    
    if (httpHandlers != nullptr)
        httpHandlers->loop();
}

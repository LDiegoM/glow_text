#include <display.h>

//////////////////// Constructor
Display::Display(MD_Parola *parola) {
    m_parola = parola;

    m_message = "Connect glow_text wifi to change this message!";
    m_speed = 50;
    m_pause = 0;
    m_effectIn = PA_SCROLL_LEFT;
    m_effectOut = PA_SCROLL_LEFT;
}

//////////////////// Public methods implementation
void Display::loop() {
    if (m_parola->displayAnimate())
        m_parola->displayText(m_message.c_str(), PA_LEFT, m_speed, m_pause, m_effectIn, m_effectOut);
}

void Display::setMessage(String message) {
    m_message = message;
}
String Display::getMessage() {
    return m_message;
}

void Display::setSpeed(uint16_t speed) {
    m_speed = speed;
}
uint16_t Display::getSpeed() {
    return m_speed;
}
void Display::setPause(uint16_t pause) {
    m_pause = pause;
}
uint16_t Display::getPause() {
    return m_pause;
}

//////////////////// Private methods implementation

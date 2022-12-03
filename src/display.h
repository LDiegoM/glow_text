#ifndef display_h
#define display_h

#include <Arduino.h>
#include <MD_Parola.h>

class Display {
    private:
        MD_Parola *m_parola;
        String m_message;

        uint16_t m_speed, m_pause;
        textEffect_t m_effectIn, m_effectOut;

    public:
        Display(MD_Parola *parola);

        void loop();
        
        void setMessage(String message);
        String getMessage();
        void setSpeed(uint16_t speed);
        uint16_t getSpeed();
        void setPause(uint16_t pause);
        uint16_t getPause();
};

#endif

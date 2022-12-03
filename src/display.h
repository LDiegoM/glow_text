#ifndef display_h
#define display_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <MD_Parola.h>

#include <storage.h>

extern const char* DATA_FILE;

struct data_t {
    String message;
    uint16_t speed, pause;
};

class Display {
    private:
        MD_Parola *m_parola;
        Storage *m_storage;
        data_t m_data;
        textEffect_t m_effectIn, m_effectOut;

        bool readData();
        String createJson();
        void defaultData();

    public:
        Display(MD_Parola *parola, Storage *storage);

        bool begin();

        void loop();
        
        void setMessage(String message);
        String getMessage();
        void setSpeed(uint16_t speed);
        uint16_t getSpeed();
        void setPause(uint16_t pause);
        uint16_t getPause();

        bool saveData();
};

#endif

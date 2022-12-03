#include <display.h>

const char* DATA_FILE = "/display/glow_text.json";

//////////////////// Constructor
Display::Display(MD_Parola *parola, Storage *storage) {
    m_parola = parola;
    m_storage = storage;

    m_effectIn = PA_SCROLL_LEFT;
    m_effectOut = PA_SCROLL_LEFT;
}

//////////////////// Public methods implementation
bool Display::begin() {
    if (!readData()) {
        // Create new defaulted data file
        defaultData();
        if (!saveData()) {
            return false;
        }
    }
    return true;
}

void Display::loop() {
    if (m_parola->displayAnimate())
        m_parola->displayText(m_data.message.c_str(), PA_LEFT, m_data.speed, m_data.pause, m_effectIn, m_effectOut);
}

void Display::setMessage(String message) {
    m_data.message = message;
}
String Display::getMessage() {
    return m_data.message;
}
void Display::setSpeed(uint16_t speed) {
    m_data.speed = speed;
}
uint16_t Display::getSpeed() {
    return m_data.speed;
}
void Display::setPause(uint16_t pause) {
    m_data.pause = pause;
}
uint16_t Display::getPause() {
    return m_data.pause;
}

bool Display::saveData() {
    m_storage->remove(DATA_FILE);
    String json = createJson();
    if (json.equals(""))
        return false;
    
    return m_storage->writeFile(DATA_FILE, json.c_str());
}

//////////////////// Private methods implementation
bool Display::readData() {
    if (!m_storage->exists(DATA_FILE)) {
        return false;
    }

    StaticJsonDocument<1024> data;
    String dataJson = m_storage->readAll(DATA_FILE);
    DeserializationError error = deserializeJson(data, dataJson);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }
    JsonObject jsonObj = data.as<JsonObject>();

    m_data.message = jsonObj["msg"].as<String>();
    m_data.speed = jsonObj["speed"].as<uint16_t>();
    m_data.pause = jsonObj["pause"].as<uint16_t>();

    return true;
}

String Display::createJson() {
    StaticJsonDocument<1024> doc;

    doc["msg"] = m_data.message;
    doc["speed"] = m_data.speed;
    doc["pause"] = m_data.pause;

    String json;
    serializeJsonPretty(doc, json);

    return json;
}

void Display::defaultData() {
    m_data.message = "Connect glow_text wifi to change this message!";
    m_data.speed = 50;
    m_data.pause = 0;
}

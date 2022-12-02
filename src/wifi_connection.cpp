#include <wifi_connection.h>

//////////////////// Constructor
WiFiConnection::WiFiConnection() {}

//////////////////// Private methods implementation
bool WiFiConnection::beginAP() {
    WiFi.mode(WIFI_AP);
    return WiFi.softAP(SSID_AP);
}

//////////////////// Public methods implementation
bool WiFiConnection::begin() {
    return beginAP();
}

bool WiFiConnection::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiConnection::isModeAP() {
    return WiFi.getMode() == WIFI_AP;
}

String WiFiConnection::getIP() {
    if (WiFi.getMode() == WIFI_AP)
        return WiFi.softAPIP().toString();

    if (!isConnected())
        return "";

    return WiFi.localIP().toString();
}

String WiFiConnection::getSSID() {
    if (WiFi.getMode() == WIFI_AP)
        return SSID_AP;
    
    if (WiFi.status() == WL_CONNECTED)
        return WiFi.SSID();
    
    return "";
}

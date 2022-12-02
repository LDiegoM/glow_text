#ifndef wifi_connection_h
#define wifi_connection_h

#include <WiFiMulti.h>

class WiFiConnection {
    private:
        const char *SSID_AP = "glow_text";

        bool beginAP();

    public:
        WiFiConnection();

        bool begin();
        bool connect(bool verbose);
        bool isConnected();
        bool isModeAP();
        String getIP();
        String getSSID();
};

#endif

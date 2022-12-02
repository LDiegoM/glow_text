#ifndef http_handlers_h
#define http_handlers_h

#include <WebServer.h>
#include <ArduinoJson.h>

#include <storage.h>
#include <wifi_connection.h>
#include <display.h>

struct animation_t {
    uint16_t speed, pause;
};

/////////// HTTP Handlers
void restart();

void getBootstrapCSS();
void getBootstrapJS();
void getNotFound();

void getMain();

void getAdmin();

void setMessage();
void setAnimation();

class HttpHandlers {
    private:
        const uint16_t APP_HTTP_PORT = 80;

        const char* MSG_OK = "ok";

        const char* ERR_MESSAGE_IS_EMPTY = "Message parameters can't be empty";
        const char* ERR_ANIMATION_IS_EMPTY = "Animation parameters can't be empty";


        WiFiConnection *m_wifi;
        Storage *m_storage;
        Display *m_display;
        WebServer *m_server;

        void defineRoutes();

        String getHeaderHTML(String section);
        String getFooterHTML(String page, String section);
        
        String getMainHTML();

        String getAdminHTML();

        String parseMessageBody(String body);

        animation_t parseAnimationBody(String body);

    public:
        HttpHandlers(WiFiConnection *wifi, Storage *storage, Display *display);

        bool begin();
        void loop();

        // HTTP handlers
        void handleRestart();

        void handleGetBootstrapCSS();
        void handleGetBootstrapJS();
        void handleGetNotFound();

        void handleGetMain();

        void handleGetAdmin();

        void handleSetMessage();

        void handleSetAnimation();
};

extern HttpHandlers *httpHandlers;

#endif

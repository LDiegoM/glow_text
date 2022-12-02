#include <http_handlers.h>

HttpHandlers *httpHandlers = nullptr;

//////////////////// HTTP Handlers
void restart() {
    httpHandlers->handleRestart();
}

void getBootstrapCSS() {
    httpHandlers->handleGetBootstrapCSS();
}
void getBootstrapJS() {
    httpHandlers->handleGetBootstrapJS();
}
void getNotFound() {
    httpHandlers->handleGetNotFound();
}

void getMain() {
    httpHandlers->handleGetMain();
}

void getAdmin() {
    httpHandlers->handleGetAdmin();
}

void setMessage() {
    httpHandlers->handleSetMessage();
}

void setAnimation() {
    httpHandlers->handleSetAnimation();
}

//////////////////// Constructor
HttpHandlers::HttpHandlers(WiFiConnection *wifi, Storage *storage, Display *display) {
    m_wifi = wifi;
    m_storage = storage;
    m_display = display;
}

//////////////////// Public methods implementation
bool HttpHandlers::begin() {
    m_server = new WebServer(APP_HTTP_PORT);

    defineRoutes();

    m_server->begin();

    return true;
}

void HttpHandlers::loop() {
    m_server->handleClient();
}

/////////// HTTP Handlers
void HttpHandlers::handleRestart() {
    m_server->send(200, "text/plain", MSG_OK);
    ESP.restart();
}

void HttpHandlers::handleGetBootstrapCSS() {
    File file = m_storage->open("/wwwroot/bootstrap.min.css.gz");
    if (!file) {
        m_server->send(404);
        return;
    }
    m_server->streamFile(file, "text/css");
    file.close();
}
void HttpHandlers::handleGetBootstrapJS() {
    File file = LittleFS.open("/wwwroot/bootstrap.bundle.min.js.gz");
    if (!file) {
        m_server->send(404);
        return;
    }
    m_server->streamFile(file, "text/js");
    file.close();
}
void HttpHandlers::handleGetNotFound() {
    String html = m_storage->readAll("/wwwroot/error.html");
    html.replace("{error_description}", "Resource not found");
    m_server->send(404, "text/html", html);
}

void HttpHandlers::handleGetMain() {
    String html = getHeaderHTML("main");
    html += getMainHTML();
    html += getFooterHTML("main", "main");
    m_server->send(200, "text/html", html);
}

void HttpHandlers::handleGetAdmin() {
    String html = getHeaderHTML("admin");
    html += getAdminHTML();
    html += getFooterHTML("admin", "admin");
    m_server->send(200, "text/html", html);
}

void HttpHandlers::handleSetMessage() {
    String body = m_server->arg("plain");
    if (body.equals("")) {
        m_server->send(400, "text/plain", ERR_MESSAGE_IS_EMPTY);
        return;
    }

    String newMessage = parseMessageBody(body);
    if (newMessage.equals("")) {
        m_server->send(400, "text/plain", ERR_MESSAGE_IS_EMPTY);
        return;
    }

    m_display->setMessage(newMessage);

    m_server->send(200, "text/plain", MSG_OK);
}

void HttpHandlers::handleSetAnimation() {
    String body = m_server->arg("plain");
    if (body.equals("")) {
        m_server->send(400, "text/plain", ERR_ANIMATION_IS_EMPTY);
        return;
    }

    animation_t newValues = parseAnimationBody(body);

    m_display->setSpeed(newValues.speed);
    m_display->setPause(newValues.pause);

    m_server->send(200, "text/plain", MSG_OK);
}

//////////////////// Private methods implementation
void HttpHandlers::defineRoutes() {
    m_server->on("/", HTTP_GET, getMain);

    m_server->on("/restart", HTTP_POST, restart);

    m_server->on("/bootstrap.min.css", HTTP_GET, getBootstrapCSS);
    m_server->on("/bootstrap.bundle.min.js", HTTP_GET, getBootstrapJS);

    m_server->on("/admin", HTTP_GET, getAdmin);
    
    m_server->on("/message", HTTP_PUT, setMessage);
    m_server->on("/animation", HTTP_PUT, setAnimation);

    m_server->onNotFound(getNotFound);
}

String HttpHandlers::getHeaderHTML(String section) {
    String header = m_storage->readAll("/wwwroot/header.html");

    header.replace("{active_main}", (section.equals("main") ? " active" : ""));
    header.replace("{active_admin}", (section.equals("admin") ? " active" : ""));

    return header;
}

String HttpHandlers::getFooterHTML(String page, String section) {
    String footer = m_storage->readAll("/wwwroot/footer.html");
    String js = "";
    js += "<script>";
    js += m_storage->readAll("/wwwroot/utils.js");
    js += "\n";
    if (!page.equals("") && !section.equals(""))
        js += m_storage->readAll((String("/wwwroot/") + page + "/" + section + ".js").c_str());
    js += "</script>";
    footer.replace("<!--{utils.js}-->", js);

    return footer;
}

String HttpHandlers::getMainHTML() {
    String html = m_storage->readAll("/wwwroot/main/main.html");

    html.replace("{current_message}", m_display->getMessage());
    html.replace("{anim_speed}", String(m_display->getSpeed()));
    html.replace("{anim_pause}", String(m_display->getPause()));

    return html;
}

String HttpHandlers::getAdminHTML() {
    String html = m_storage->readAll("/wwwroot/admin/admin.html");

    html.replace("{freeMem}", String((float) ESP.getFreeHeap() / 1024) + " kb");
    html.replace("{free_storage}", m_storage->getFree());

    return html;
}

String HttpHandlers::parseMessageBody(String body) {
    String newMessage;

    StaticJsonDocument<500> values;
    DeserializationError error = deserializeJson(values, body);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return newMessage;
    }
    JsonObject jsonObj = values.as<JsonObject>();

    newMessage = jsonObj["message"].as<String>();

    return newMessage;
}

animation_t HttpHandlers::parseAnimationBody(String body) {
    animation_t animationParams;

    StaticJsonDocument<100> values;
    DeserializationError error = deserializeJson(values, body);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return animationParams;
    }
    JsonObject jsonObj = values.as<JsonObject>();

    animationParams.speed = jsonObj["speed"].as<uint16_t>();
    animationParams.pause = jsonObj["pause"].as<uint16_t>();

    return animationParams;
}

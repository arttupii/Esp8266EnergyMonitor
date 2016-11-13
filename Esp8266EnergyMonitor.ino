#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <wifiSetupMenu.h>
#include "indexHtml.h"
#include "meter.h"
#include "customMenu.h"
#include "config.h"


ESP8266WebServer server ( 80 );
Meter meter(5);
WifiSetupMenu setupMenu(WIFI_MENU_EEPROM_MEMORY_START_ADDRESS);

void handleRoot() {
	server.send_P( 200, "text/html", index_html, index_html_len );
}

MenuTemplate *customMenuCb(MenuTemplate*parent){
    return new CustomMenu(parent);
}

void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );

}

void handleApiGetAll() {
	server.send ( 200, "text/plain", meter.getPulseArray());
}

void handleApiReset() {
	meter.reset();
	server.send ( 200, "text/plain", "OK" );
}
void handleGetUpdate() {
	server.send ( 200, "text/plain", meter.getUpdate());
}
void handleGetCustomSetup() {
    String ret = "[";
    ret+=String(customConfig.impPerKwh) + ",";
    ret+=String(customConfig.cPerKwh) + "]";
	server.send ( 200, "text/plain", ret);
}
void setup ( void ) {
	Serial.begin ( 115200 );

	server.on ( "/", handleRoot );
	server.on ( "/api/getAll", handleApiGetAll );
	server.on ( "/api/doReset", handleApiReset);
    server.on ( "/api/getUpdate", handleGetUpdate);
    server.on ( "/api/getCustomSetup", handleGetCustomSetup);

	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
    setupMenu.setCustomMenuCallback(customMenuCb);
    readCustomConfigFromEeprom();

	meter.begin();
}

void loop ( void ) {
  server.handleClient();
  meter.run();
  setupMenu.run();
}

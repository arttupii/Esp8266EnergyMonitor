#ifndef CUSTEMSETUPMENU_H
#define CUSTEMSETUPMENU_H
#include "menu.h"
#include "config.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>

struct customMenuConfig {
    int cPerKwh;
    int impPerKwh;
};

struct customMenuConfig customConfig;

void saveCustomConfigToEeprom() {
  EEPROM.put(CUSTOM_MENU_EEPROM_MEMORY_START_ADDRESS, customConfig);
  EEPROM.commit(); //note here the commit!
}
void readCustomConfigFromEeprom() {
  EEPROM.begin(512);
  EEPROM.get(CUSTOM_MENU_EEPROM_MEMORY_START_ADDRESS, customConfig);
}

class AskImpPerKwhMenu: public AskStringMenu {
  public:
    AskImpPerKwhMenu(MenuTemplate *parent): AskStringMenu(parent) {
    }
    void ready(String &str) {
      customConfig.impPerKwh = atoi(str.c_str());
      saveCustomConfigToEeprom();
    }
};

class AskCPerKwhMenu: public AskStringMenu {
  public:
    AskCPerKwhMenu(MenuTemplate *parent): AskStringMenu(parent) {
    }
    void ready(String &str) {
      customConfig.cPerKwh = atoi(str.c_str());
      saveCustomConfigToEeprom();
    }
};

class CustomMenu: public MenuTemplate {
  public:
    CustomMenu(MenuTemplate *parent): MenuTemplate(parent) {}
    void printMenu() {
      Serial.println("Custom menu\n\r\n\r");

      Serial.printf(" imp/kWh: %d\n\r", customConfig.impPerKwh);
      Serial.printf(" c/kWh: %d\n\r\n\r", customConfig.cPerKwh);


      Serial.println(" 1. Set imp/kWh");
      Serial.println(" 2. Set c/kWh");
      Serial.println(" 9. Set exit");
    }
    void menuSelect(int c) {

      switch (c) {
        case '1': subMenu((new AskImpPerKwhMenu(this))->menuText(" imp/kWh: ")); return;
        case '2': subMenu((new AskCPerKwhMenu(this))->menuText(" c/kWh: ")); return;

        case '9': returnToParentMenu(); return;
      }
    }
};

#endif

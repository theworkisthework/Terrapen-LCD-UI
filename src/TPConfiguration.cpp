#include <TPConfiguration.hpp>
#include "FS.h"
#include <LittleFS.h>
#include <fstream>
#include <ArduinoJson.h>

int TPConfiguration::readConfig()
{

    if (LittleFS.begin(true))
    {
        JsonDocument doc;
        Serial.println("Started Filesystem");
        std::ifstream fileToRead("/littlefs/config.json");
        if (fileToRead.fail())
        {
            Serial.println("Failed to open config file");
            return 1;
        }
        else
        {
            deserializeJson(doc, fileToRead);
        }
        strlcpy(this->Wifi_Mode,          // <- destination
                doc["wifiMode"] | "None", // <- source
                sizeof(this->Wifi_Mode));
        Serial.printf("JSON : %s", this->Wifi_Mode);
        Serial.println();
        fileToRead.close();
    }
    return 0;
}
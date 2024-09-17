#pragma once
#include <Arduino.h>

class TPConfiguration
{
private:
    char Wifi_Mode[20];

public:
    int readConfig();
};

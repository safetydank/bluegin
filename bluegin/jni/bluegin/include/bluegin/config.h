#pragma once

#include "bluegin/bluegin.h"

#include <string>
#include <vector>

namespace bluegin {

struct KeyValue
{
    std::string key;    
    std::vector<float>  numval;
    std::vector<std::string> strval;

    void reset() 
    {
        key = std::string();
        numval.clear();
        strval.clear();
    }

    float getNumber(int i=0) {
        return numval[i];
    }

    std::string getString(int i=0) {
        return strval[i];
    }
};

enum ResourceType
{
    TEXTURE,
    GRAPHIC,
    SOUND,
    MUSIC,
    FONT
};

struct ResourceConfig
{
    ResourceType          resourceType;
    std::string           name;
    std::vector<KeyValue> keyValues;
};

bool resourceConfigParse(char* str, int length, vector<ResourceConfig*>& resources);

}

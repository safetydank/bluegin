#pragma once

#include "bluegin/bluegin.h"

#include <string>
#include <vector>

namespace bluegin {

/**
 * An internal struct used to store a configuration key-value pair.  Supports
 * arrays of values.
 */
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
    ///  Texture resource (png, jpeg)
    TEXTURE,
    ///  Graphic resource, defined in a resources.pack file as a region of a texture
    GRAPHIC,
    ///  Sound effect resource (wav, mp3, ogg)
    SOUND,
    ///  A music resource (mp3, ogg)
    MUSIC,
    ///  A font resource (Haaf's Game Engine .fnt)
    FONT
};

///  A structure representing a single resource
struct ResourceConfig
{
    ///  The type of resource
    ResourceType          resourceType;
    ///  The configured name of the resource
    std::string           name;
    ///  A list of key-value pairs associated with this resource
    std::vector<KeyValue> keyValues;
};

/**
 * Parse a resource configuration string
 *
 * @param str pointer to configuration text
 * @param length length of configuration text
 * @param resources the resource config list that is populated with the config
 */
bool resourceConfigParse(char* str, int length, vector<ResourceConfig*>& resources);

}

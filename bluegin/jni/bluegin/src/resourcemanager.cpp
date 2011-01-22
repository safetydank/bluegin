#include "bluegin/resourcemanager.h"
#include "bluegin/config.h"
#include "bluegin/common.h"

#include "cinder/Rect.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::gl;
using namespace bluegin;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::loadTexture(ResourceConfig& rc)
{
    bool success = true;
    string name = rc.name;
    string texturePath;

    //  Read key values
    for (vector<KeyValue>::iterator it = rc.keyValues.begin();
                                    it != rc.keyValues.end(); ++it) {
        KeyValue& kv = *it;
        if (kv.key.compare("source") == 0) {
            texturePath = kv.getString();
        }
    }

    if (texturePath.empty()) {
        Log("Error: Empty texture path for texture '%s'", name.c_str());
        success = false;
    }
    Texture tex(bluegin::getTextureAsset(texturePath.c_str()));
    if (!tex) {
        Log("Error loading texture '%s' from path %s", name.c_str(), texturePath.c_str());
        success = false;
    }
    else {
        //  Set default filtering to NEAREST
        tex.setMagFilter(GL_NEAREST);
        tex.setMinFilter(GL_NEAREST);
        tex.setWrapS(GL_CLAMP_TO_EDGE);
        tex.setWrapT(GL_CLAMP_TO_EDGE);

        Log("Texture %s loaded id %d", name.c_str(), tex.getId());
    }
    mTextures[name] = tex;

    return success;
}

bool ResourceManager::loadGraphic(ResourceConfig& rc)
{
    bool success = true;

    string name = rc.name;
    string texName;
    Rectf sourceRect(0, 0, 0, 0);

    //  Read key values
    for (vector<KeyValue>::iterator it = rc.keyValues.begin();
                                    it != rc.keyValues.end(); ++it) {
        KeyValue& kv = *it;
        if (kv.key.compare("texture") == 0) {
            texName = kv.getString();
        }
        else if (kv.key.compare("area") == 0) {
            if (kv.numval.size() != 4) {
                Log("Error: parsed Graphic 'area' attribute in '%s' does not specify 4 coordinates", rc.name.c_str());
            }
            sourceRect = Rectf(kv.numval[0], kv.numval[1], kv.numval[0]+kv.numval[2], kv.numval[1]+kv.numval[3]);
        }
    }

    Graphic graphic(texture(texName), sourceRect);
    mGraphics[name] = graphic;

    return success;
}

bool ResourceManager::loadFont(ResourceConfig& rc)
{
    bool success = true;
    string name = rc.name;
    string source;
    string texture;

    //  Read key values
    for (vector<KeyValue>::iterator it = rc.keyValues.begin();
                                    it != rc.keyValues.end(); ++it) {
        KeyValue& kv = *it;
        if (kv.key.compare("source") == 0) {
            source = kv.getString();
        }
        else if (kv.key.compare("texture") == 0) {
            texture = kv.getString();
        }
    }

    if (source.empty()) {
        Log("Error: Empty source file for font '%s'", name.c_str());
        success = false;
    }
    if (texture.empty()) {
        Log("Error: Empty texture referenced by font '%s'", name.c_str());
        success = false;
    }
    Texture tex = this->texture(texture);
    if (!tex) {
        Log("Error: unable to resolve texture reference in font %s", name.c_str());
        success = false;
    }
    else {
        //tex.setMagFilter(GL_LINEAR);
        //tex.setMinFilter(GL_LINEAR);
    }

    mFonts[name] = shared_ptr<hgeFont>(new hgeFont(source.c_str(), tex));
    if (success) {
        Log("Loaded font %s texture %s", source.c_str(), texture.c_str());
    }
    return success;
}

bool ResourceManager::loadSound(SoundType type, ResourceConfig& rc)
{
    bool success = true;
    string name = rc.name;
    string source;

    //  Read key values
    for (vector<KeyValue>::iterator it = rc.keyValues.begin();
                                    it != rc.keyValues.end(); ++it) {
        KeyValue& kv = *it;
        if (kv.key.compare("source") == 0) {
            source = kv.getString();
        }
    }

    if (source.empty()) {
        Log("Error: Empty source path for sound/music '%s'", name.c_str());
        success = false;
    }

    AudioSourcePtr audio;
    audio = AudioSourcePtr(new AudioSource(type, source));
    mSounds[name] = audio;

    if (success) {
        Log("Loaded sound %s from path %s", name.c_str(), source.c_str());
    }
    return success;
}

void ResourceManager::loadResourceConfig(const char* configPath)
{
    int configLength;
    char* resourceConfig = static_cast<char*>(bluegin_load_resource("resources.config", &configLength));
    if (resourceConfig == NULL) {
        Log("Error loading resource configuration, no resources loaded.");
        return;
    }

    vector<ResourceConfig*> resources;
    resourceConfigParse(resourceConfig, configLength, resources);
    delete[] resourceConfig;
    
    for (vector<ResourceConfig*>::iterator it=resources.begin(); it != resources.end(); ++it) {
        ResourceConfig& rc = **it;
        switch (rc.resourceType) {
            case TEXTURE:
                loadTexture(rc);
                break;
            case SOUND:
                loadSound(SOUND_TYPE, rc);
                break;
            case MUSIC:
                loadSound(MUSIC_TYPE, rc);
                break;
        }
    }

    //  Load graphic resources after textures so width and height information is correctly processed
    for (vector<ResourceConfig*>::iterator it=resources.begin(); it != resources.end(); ++it) {
        ResourceConfig& rc = **it;
        if (rc.resourceType == GRAPHIC) {
            loadGraphic(rc);
        }
        else if (rc.resourceType == FONT) {
            loadFont(rc);
        }
    }

    //  Free resource configs
    for (vector<ResourceConfig*>::iterator it=resources.begin(); it != resources.end();
            ++it) {
        delete *it;
    }
}

Texture ResourceManager::texture(string texName)
{
    map<string, Texture>::iterator it = mTextures.find(texName);
    if (it == mTextures.end()) {
        Log("Error accessing non-existent texture '%s' - returned empty", texName.c_str());
        return Texture();
    }
    return it->second;
}

Graphic ResourceManager::graphic(string graphicName)
{
    map<string, Graphic>::iterator it = mGraphics.find(graphicName);
    if (it == mGraphics.end()) {
        Log("Error accessing non-existent graphic '%s' - returned empty", graphicName.c_str());
        return Graphic();
    }
    return it->second;    
}

FontPtr ResourceManager::font(string fontName)
{
    map<string, FontPtr>::iterator it = mFonts.find(fontName);
    if (it == mFonts.end()) {
        Log("Error accessing non-existent font '%s' - returned empty", fontName.c_str());
        return FontPtr();
    }
    return it->second; 
}

AudioSourcePtr ResourceManager::sound(string soundName)
{
    map<string, AudioSourcePtr>::iterator it = mSounds.find(soundName);
    if (it == mSounds.end()) {
        Log("Error accessing non-existent sound '%s' - returned empty", soundName.c_str());
        return AudioSourcePtr();
    }
    return it->second; 
}

//  Prime a sound for playback
void ResourceManager::primeSound(AudioSourcePtr source)
{
    if (source->type == SOUND_TYPE && source->soundID == -1) {
        int soundID = bluegin_sound_load(source->path.c_str());
        source->soundID = soundID;
    }
    else if (source->soundID != -1) {
        Log("primeSound called for already primed sound %s",
                source->path.c_str());
    }
}

//  Prime all the loaded sounds for playback
void ResourceManager::primeAllSounds()
{
    for (map<string, AudioSourcePtr>::iterator it = mSounds.begin();
            it != mSounds.end(); ++it) {
        AudioSourcePtr source = it->second;
        if (source->type == SOUND_TYPE && source->soundID == -1) {
            primeSound(source);
        }
    }
}

void ResourceManager::resetSounds()
{
    for (map<string, AudioSourcePtr>::iterator it = mSounds.begin();
            it != mSounds.end(); ++it) {
        it->second->soundID = -1;
    }
    bluegin_sound_init();
}


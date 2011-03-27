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
    //  Use nearest neighbour filtering by default
    mMagFilter = GL_NEAREST;
    mMinFilter = GL_NEAREST;
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::loadTexture(ResourceConfig& rc)
{
    CachedTexture tex;

    string name = rc.name;
    string source;

    //  Read key values
    for (vector<KeyValue>::iterator it = rc.keyValues.begin();
                                    it != rc.keyValues.end(); ++it) {
        KeyValue& kv = *it;
        if (kv.key.compare("source") == 0) {
            tex.source = kv.getString();
        }
    }

    if (tex.source.empty()) {
        Log("Error: Empty texture path for texture '%s'", name.c_str());
        return false;
    }

    mTextures[name] = tex;
    return true;
}

bool ResourceManager::acquireTexture(string name)
{
    CachedTexture& ct = mTextures[name];

    if (!ct.active) {
        Texture tex(bluegin::getTextureAsset(ct.source.c_str()));
        if (!tex) {
            Log("Error acquiring texture '%s' from path %s", name.c_str(), ct.source.c_str());
            ct.texture = Texture();
            return false;
        }
        else {
            //  Set default filtering to NEAREST
            tex.setMagFilter(mMagFilter);
            tex.setMinFilter(mMinFilter);

            tex.setWrapS(GL_CLAMP_TO_EDGE);
            tex.setWrapT(GL_CLAMP_TO_EDGE);

            ct.active = true;
            ct.texture = tex;
            Log("Texture %s loaded to GL id %d", name.c_str(), tex.getId());
        }
    }
    return true;
}

void ResourceManager::acquireAllTextures(bool updateGraphics)
{
    for (map<string, CachedTexture>::iterator it=mTextures.begin();
         it != mTextures.end(); ++it) {
        Log("Acquiring texture %s", (it->first).c_str());
        acquireTexture(it->first);
    }

    if (updateGraphics)
        this->updateGraphics();
}

void ResourceManager::releaseTexture(string texName)
{
    CachedTexture& ct = mTextures[texName];
    if (ct.active) {
        // XXX we manually release the texture here, this invalidates
        // any already held references to the texture (e.g. Graphic classes)
        ct.active = false;
        GLuint texId = ct.texture.getId();
        glDeleteTextures(1, &texId);
        Log("Released texture %s", texName.c_str());
    }
}

void ResourceManager::updateGraphics()
{
    for (map<string, Graphic>::iterator it=mGraphics.begin(); it != mGraphics.end(); ++it) {
        Graphic& gr = it->second;
        map<string, CachedTexture>::iterator cit = mTextures.find(gr.texName);
        if (cit == mTextures.end()) {
            Log("updateGraphics error: unable to find texture '%s' referenced by a Graphic", gr.texName.c_str());
        }
        else {
            CachedTexture& tex = cit->second;
            if (tex.active) {
                Log("updating a Graphic bound to %s", gr.texName.c_str());
                gr.setTexture(tex.texture);
            }
        }
    }
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

    //  create graphic, not bound to a texture
    Graphic graphic(texName, sourceRect);
    mGraphics[name] = graphic;

    return success;
}

bool ResourceManager::loadFont(ResourceConfig& rc)
{
    bool success = true;
    string name = rc.name;
    string source;
    string texName;

    //  Read key values
    for (vector<KeyValue>::iterator it = rc.keyValues.begin();
                                    it != rc.keyValues.end(); ++it) {
        KeyValue& kv = *it;
        if (kv.key.compare("source") == 0) {
            source = kv.getString();
        }
        else if (kv.key.compare("texture") == 0) {
            texName = kv.getString();
        }
    }

    if (source.empty()) {
        Log("Error: Empty source file for font '%s'", name.c_str());
        success = false;
    }
    if (texName.empty()) {
        Log("Error: Empty texture referenced by font '%s'", name.c_str());
        success = false;
    }

    //  TODO lazy-load font texture references instead of acquiring them upfront
    acquireTexture(texName);
    Texture tex = this->texture(texName);

    if (!tex) {
        Log("Error: unable to resolve texture reference in font %s", name.c_str());
        success = false;
    }

    mFonts[name] = shared_ptr<hgeFont>(new hgeFont(source.c_str(), tex));
    if (success) {
        Log("Loaded font %s texture %s", source.c_str(), texName.c_str());
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

    //  Load font resources after textures so width and height information is correctly processed
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
    map<string, CachedTexture>::iterator it = mTextures.find(texName);
    if (it == mTextures.end()) {
        Log("Error accessing non-existent texture '%s' - returned empty", texName.c_str());
        return Texture();
    }
    CachedTexture& tex = it->second;
    if (tex.active == false) {
        Log("Error: texture %s is not loaded into texture memory (forgot to acquireTexture?)", texName.c_str());
    }
    return (it->second).texture;
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

void ResourceManager::acquireSound(AudioSourcePtr source)
{
    if (source->type == SOUND_TYPE && source->soundID == -1) {
        int soundID = bluegin_sound_load(source->path.c_str());
        source->soundID = soundID;
    }
    else if (source->soundID != -1) {
        Log("acquireSound called for already acquired sound %s",
                source->path.c_str());
    }
}

void ResourceManager::acquireAllSounds()
{
    for (map<string, AudioSourcePtr>::iterator it = mSounds.begin();
            it != mSounds.end(); ++it) {
        AudioSourcePtr source = it->second;
        if (source->type == SOUND_TYPE && source->soundID == -1) {
            acquireSound(source);
        }
    }
}

void ResourceManager::releaseAllSounds()
{
    for (map<string, AudioSourcePtr>::iterator it = mSounds.begin();
            it != mSounds.end(); ++it) {
        it->second->soundID = -1;
    }
    bluegin_sound_init();
}


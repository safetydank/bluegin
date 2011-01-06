// a resources config file reader implemented with Ragel
//
// config files are of the form:
//
// <ResourceType> <name> {
//    <key> = <value>
//    ...
// }
// value is a string or number (float) or space separated list of strings/numbers
//
// e.g.
//
// Texture gibs { 
//     source = gibs.png
// }
// Graphic gibs { 
//     texture = gibs
//     texrect = 0 0 1.0 1.0
// }
// Texture font_a {
//     source = font_a.png
// }
// Font font_a {
//     texture = font_a
//     source  = font_a.fnt
// }
// Sound sound1 {
//     source = sound1.wav
// }
// Music music1 {
//     source = music1.mp3
// }


#ifdef TEST_RESOURCE_CONFIG_MAIN
#define Log(...) printf(__VA_ARGS__)
#endif

#include "bluegin/config.h"

#include <cstdio>
#include <cstdlib>

using std::string;
using std::vector;

namespace bluegin {

%%{
    machine resparser;
    write data;
}%%

char* getToken(char* buf, char* ts, char* te)
{
    char* itr = buf;
    while (ts != te)
        *itr++ = *ts++;
    *itr = '\0';
    return buf;
}

ResourceConfig* newResourceConfig(string rcType)
{
    ResourceType resourceType;
    if (rcType.compare("Texture") == 0) {
        resourceType = TEXTURE;
    }
    else if (rcType.compare("Graphic") == 0) {
        resourceType = GRAPHIC;
    }
    else if (rcType.compare("Sound") == 0) {
        resourceType = SOUND;
    }
    else if (rcType.compare("Music") == 0) {
        resourceType = MUSIC;
    }
    else if (rcType.compare("Font") == 0) {
        resourceType = FONT;
    }
    ResourceConfig* rc = new ResourceConfig;
    rc->resourceType = resourceType;
    return rc;
}

bool resourceConfigParse(char* str, int length, vector<ResourceConfig*>& resources)
{
	char *p = str, *pe = str + length, *eof = pe;
	char *ts, *te = 0;
	int cs=0, act;
    char token[256];
    int top;
    int stack[32];
    int curLine = 1;

    KeyValue keyValue;
    ResourceConfig* resourceConfig = NULL;

%%{
     newline     = '\n' | '\r\n' @{ curLine += 1; };
     ws          = space+ ;
     tabspace    = [ \t] ;
     rctype      = "Texture" | "Graphic" | "Font" | "Sound" | "Music" ;
     string      = [A-Za-z_](alnum | [_.\-])+ ;
     number      = digit+ '.' digit+ | digit+ ;
     eq          = "=" ;
     key         = string ;
     block_start = "{" ;
     block_end   = "}" ;

     main := |*
        '//' [^\n]* '\n' { ++curLine; };   # ignore comments
        newline { ++curLine; };
        space ;
        rctype { 
            string rcType(getToken(token, ts, te));
            resourceConfig = newResourceConfig(rcType);
        }; 
        string { 
            if (resourceConfig == NULL) {
                Log("Error: resource config not declared before resource name");
                goto Finished;
            }
            resourceConfig->name = string(getToken(token, ts, te));
        };
        block_start { 
            if (resourceConfig == NULL) {
                Log("Error: block started without a resource declaration");
                goto Finished;
            }
            keyValue.reset();
            fcall resource; 
        };
     *|;

     resource := |*
        '//' [^\n]* '\n' { ++curLine; };   # ignore comments
        newline { ++curLine; };
        space ;
        key  {
            keyValue.key = string(getToken(token, ts, te));
        };
        eq { 
            if (keyValue.key.empty()) {
                Log("Error: no key name specified before = (L%d)\n", curLine);
                goto Finished;
            }
            fcall kvalue;
        };
        block_end { 
            resources.push_back(resourceConfig);
            Log("Added resource %s with %d keyvalues\n", 
                resourceConfig->name.c_str(), resourceConfig->keyValues.size());

            resourceConfig = NULL;
            fret; 
        };
     *|;

     kvalue := |*
        tabspace ;
        string {
            keyValue.strval.push_back(string(getToken(token, ts, te)));
        };
        number { 
            float numToken = atof(getToken(token, ts, te));
            keyValue.numval.push_back(numToken);
        };
        newline {
            resourceConfig->keyValues.push_back(keyValue);
            keyValue.reset();
            ++curLine;
            fret;
        };
     *|;
 
     write init;
     write exec;
}%%

Finished:
    if (p != pe) {
        Log("Error parsing configuration file (around line %d)\n", curLine);
        return false;
    }

    Log("%d resource configurations loaded\n", resources.size());
    return true;
}

}

#ifdef TEST_RESOURCE_CONFIG_MAIN
int main(int argc, char* argv[])
{
    FILE* file;
    char* path = argv[1];

    if (argc != 2) {
        Log("Usage: config <configFile>");
        return 0;
    }

    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = new char[length];
    int readBytes = fread(buffer, 1, length, file);
    if (readBytes != length) {
        Log("Error reading from file %s size %d : %d bytes read\n", path, length, readBytes);
    }
    fclose(file);

    vector<bluegin::ResourceConfig*> resources;
    bluegin::resourceConfigParse(buffer, length, resources);
}
#endif


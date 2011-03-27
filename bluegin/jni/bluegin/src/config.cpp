
#line 1 "config.rl"
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


#line 53 "config.c"
static const char _resparser_actions[] = {
	0, 1, 1, 1, 2, 1, 3, 1, 
	6, 1, 7, 1, 8, 1, 9, 1, 
	10, 1, 11, 1, 12, 1, 13, 1, 
	14, 1, 15, 1, 16, 1, 17, 1, 
	18, 1, 19, 1, 20, 1, 21, 1, 
	22, 1, 23, 1, 24, 2, 0, 7, 
	2, 0, 14, 2, 0, 21, 2, 3, 
	4, 2, 3, 5
};

static const short _resparser_key_offsets[] = {
	0, 0, 1, 2, 9, 17, 25, 33, 
	41, 49, 50, 51, 58, 59, 61, 68, 
	85, 86, 93, 101, 109, 117, 125, 133, 
	141, 149, 157, 165, 173, 181, 189, 197, 
	205, 213, 221, 234, 235, 242, 253, 256, 
	258
};

static const char _resparser_trans_keys[] = {
	47, 10, 95, 45, 57, 65, 90, 97, 
	122, 95, 111, 45, 57, 65, 90, 97, 
	122, 95, 114, 45, 57, 65, 90, 97, 
	122, 95, 117, 45, 57, 65, 90, 97, 
	122, 95, 111, 45, 57, 65, 90, 97, 
	122, 95, 101, 45, 57, 65, 90, 97, 
	122, 47, 10, 95, 45, 57, 65, 90, 
	97, 122, 10, 48, 57, 95, 45, 57, 
	65, 90, 97, 122, 10, 13, 32, 47, 
	70, 71, 77, 83, 84, 95, 123, 9, 
	12, 65, 90, 97, 122, 10, 95, 45, 
	57, 65, 90, 97, 122, 95, 110, 45, 
	57, 65, 90, 97, 122, 95, 116, 45, 
	57, 65, 90, 97, 122, 95, 97, 45, 
	57, 65, 90, 98, 122, 95, 112, 45, 
	57, 65, 90, 97, 122, 95, 104, 45, 
	57, 65, 90, 97, 122, 95, 105, 45, 
	57, 65, 90, 97, 122, 95, 99, 45, 
	57, 65, 90, 97, 122, 95, 115, 45, 
	57, 65, 90, 97, 122, 95, 117, 45, 
	57, 65, 90, 97, 122, 95, 110, 45, 
	57, 65, 90, 97, 122, 95, 100, 45, 
	57, 65, 90, 97, 122, 95, 120, 45, 
	57, 65, 90, 97, 122, 95, 116, 45, 
	57, 65, 90, 97, 122, 95, 117, 45, 
	57, 65, 90, 97, 122, 95, 114, 45, 
	57, 65, 90, 97, 122, 95, 101, 45, 
	57, 65, 90, 97, 122, 10, 13, 32, 
	47, 61, 95, 125, 9, 12, 65, 90, 
	97, 122, 10, 95, 45, 57, 65, 90, 
	97, 122, 9, 10, 13, 32, 95, 48, 
	57, 65, 90, 97, 122, 46, 48, 57, 
	48, 57, 95, 45, 57, 65, 90, 97, 
	122, 0
};

static const char _resparser_single_lengths[] = {
	0, 1, 1, 1, 2, 2, 2, 2, 
	2, 1, 1, 1, 1, 0, 1, 11, 
	1, 1, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 7, 1, 1, 5, 1, 0, 
	1
};

static const char _resparser_range_lengths[] = {
	0, 0, 0, 3, 3, 3, 3, 3, 
	3, 0, 0, 3, 0, 1, 3, 3, 
	0, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 0, 3, 3, 1, 1, 
	3
};

static const unsigned char _resparser_index_offsets[] = {
	0, 0, 2, 4, 9, 15, 21, 27, 
	33, 39, 41, 43, 48, 50, 52, 57, 
	72, 74, 79, 85, 91, 97, 103, 109, 
	115, 121, 127, 133, 139, 145, 151, 157, 
	163, 169, 175, 186, 188, 193, 202, 205, 
	207
};

static const char _resparser_indicies[] = {
	0, 1, 2, 0, 3, 3, 3, 3, 
	1, 3, 4, 3, 3, 3, 1, 3, 
	5, 3, 3, 3, 1, 3, 6, 3, 
	3, 3, 1, 3, 7, 3, 3, 3, 
	1, 3, 8, 3, 3, 3, 1, 9, 
	1, 10, 9, 11, 11, 11, 11, 1, 
	12, 1, 14, 13, 15, 15, 15, 15, 
	1, 17, 18, 16, 19, 21, 22, 23, 
	24, 25, 20, 26, 16, 20, 20, 1, 
	28, 27, 3, 3, 3, 3, 29, 3, 
	31, 3, 3, 3, 30, 3, 32, 3, 
	3, 3, 30, 3, 33, 3, 3, 3, 
	30, 3, 34, 3, 3, 3, 30, 3, 
	35, 3, 3, 3, 30, 3, 36, 3, 
	3, 3, 30, 3, 32, 3, 3, 3, 
	30, 3, 35, 3, 3, 3, 30, 3, 
	37, 3, 3, 3, 30, 3, 38, 3, 
	3, 3, 30, 3, 32, 3, 3, 3, 
	30, 3, 39, 3, 3, 3, 30, 3, 
	40, 3, 3, 3, 30, 3, 41, 3, 
	3, 3, 30, 3, 42, 3, 3, 3, 
	30, 3, 32, 3, 3, 3, 30, 44, 
	45, 43, 46, 47, 48, 49, 43, 48, 
	48, 1, 51, 50, 11, 11, 11, 11, 
	52, 53, 54, 55, 53, 57, 56, 57, 
	57, 1, 59, 56, 58, 14, 58, 15, 
	15, 15, 15, 60, 0
};

static const char _resparser_trans_targs[] = {
	2, 0, 15, 17, 18, 20, 25, 26, 
	29, 10, 34, 36, 37, 37, 39, 40, 
	15, 15, 16, 1, 3, 4, 5, 6, 
	7, 8, 15, 15, 15, 15, 15, 19, 
	17, 21, 22, 23, 24, 27, 28, 30, 
	31, 32, 33, 34, 34, 35, 9, 34, 
	11, 34, 34, 34, 34, 37, 37, 12, 
	38, 14, 37, 13, 37
};

static const char _resparser_trans_actions[] = {
	0, 0, 7, 57, 0, 0, 0, 0, 
	0, 0, 21, 0, 51, 43, 0, 0, 
	11, 9, 0, 0, 0, 0, 0, 0, 
	0, 0, 13, 15, 45, 19, 17, 0, 
	54, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 25, 23, 0, 0, 27, 
	0, 29, 31, 48, 33, 35, 37, 0, 
	5, 0, 41, 0, 39
};

static const char _resparser_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 0, 0, 1, 0, 0, 
	0
};

static const char _resparser_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 3, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 3, 0, 0, 3, 0, 0, 
	0
};

static const unsigned char _resparser_eof_trans[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 14, 0, 0, 
	28, 30, 31, 31, 31, 31, 31, 31, 
	31, 31, 31, 31, 31, 31, 31, 31, 
	31, 31, 0, 51, 53, 0, 59, 59, 
	61
};

static const int resparser_start = 15;
static const int resparser_first_final = 15;
static const int resparser_error = 0;

static const int resparser_en_main = 15;
static const int resparser_en_resource = 34;
static const int resparser_en_kvalue = 37;


#line 52 "config.rl"


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


#line 275 "config.c"
	{
	cs = resparser_start;
	top = 0;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 284 "config.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_acts = _resparser_actions + _resparser_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 2:
#line 1 "NONE"
	{ts = p;}
	break;
#line 305 "config.c"
		}
	}

	_keys = _resparser_trans_keys + _resparser_key_offsets[cs];
	_trans = _resparser_index_offsets[cs];

	_klen = _resparser_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _resparser_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _resparser_indicies[_trans];
_eof_trans:
	cs = _resparser_trans_targs[_trans];

	if ( _resparser_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _resparser_actions + _resparser_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 100 "config.rl"
	{ curLine += 1; }
	break;
	case 3:
#line 1 "NONE"
	{te = p+1;}
	break;
	case 4:
#line 115 "config.rl"
	{act = 4;}
	break;
	case 5:
#line 119 "config.rl"
	{act = 5;}
	break;
	case 6:
#line 112 "config.rl"
	{te = p+1;{ ++curLine; }}
	break;
	case 7:
#line 113 "config.rl"
	{te = p+1;{ ++curLine; }}
	break;
	case 8:
#line 114 "config.rl"
	{te = p+1;}
	break;
	case 9:
#line 126 "config.rl"
	{te = p+1;{ 
            if (resourceConfig == NULL) {
                Log("Error: block started without a resource declaration");
                goto Finished;
            }
            keyValue.reset();
            {stack[top++] = cs; cs = 34; goto _again;} 
        }}
	break;
	case 10:
#line 114 "config.rl"
	{te = p;p--;}
	break;
	case 11:
#line 119 "config.rl"
	{te = p;p--;{ 
            if (resourceConfig == NULL) {
                Log("Error: resource config not declared before resource name");
                goto Finished;
            }
            resourceConfig->name = string(getToken(token, ts, te));
        }}
	break;
	case 12:
#line 1 "NONE"
	{	switch( act ) {
	case 4:
	{{p = ((te))-1;} 
            string rcType(getToken(token, ts, te));
            resourceConfig = newResourceConfig(rcType);
        }
	break;
	case 5:
	{{p = ((te))-1;} 
            if (resourceConfig == NULL) {
                Log("Error: resource config not declared before resource name");
                goto Finished;
            }
            resourceConfig->name = string(getToken(token, ts, te));
        }
	break;
	}
	}
	break;
	case 13:
#line 137 "config.rl"
	{te = p+1;{ ++curLine; }}
	break;
	case 14:
#line 138 "config.rl"
	{te = p+1;{ ++curLine; }}
	break;
	case 15:
#line 139 "config.rl"
	{te = p+1;}
	break;
	case 16:
#line 143 "config.rl"
	{te = p+1;{ 
            if (keyValue.key.empty()) {
                Log("Error: no key name specified before = (L%d)\n", curLine);
                goto Finished;
            }
            {stack[top++] = cs; cs = 37; goto _again;}
        }}
	break;
	case 17:
#line 150 "config.rl"
	{te = p+1;{ 
            resources.push_back(resourceConfig);
            Log("Added resource %s with %d keyvalues\n", 
                resourceConfig->name.c_str(), resourceConfig->keyValues.size());

            resourceConfig = NULL;
            {cs = stack[--top]; goto _again;} 
        }}
	break;
	case 18:
#line 139 "config.rl"
	{te = p;p--;}
	break;
	case 19:
#line 140 "config.rl"
	{te = p;p--;{
            keyValue.key = string(getToken(token, ts, te));
        }}
	break;
	case 20:
#line 161 "config.rl"
	{te = p+1;}
	break;
	case 21:
#line 169 "config.rl"
	{te = p+1;{
            resourceConfig->keyValues.push_back(keyValue);
            keyValue.reset();
            ++curLine;
            {cs = stack[--top]; goto _again;}
        }}
	break;
	case 22:
#line 162 "config.rl"
	{te = p;p--;{
            keyValue.strval.push_back(string(getToken(token, ts, te)));
        }}
	break;
	case 23:
#line 165 "config.rl"
	{te = p;p--;{ 
            float numToken = atof(getToken(token, ts, te));
            keyValue.numval.push_back(numToken);
        }}
	break;
	case 24:
#line 165 "config.rl"
	{{p = ((te))-1;}{ 
            float numToken = atof(getToken(token, ts, te));
            keyValue.numval.push_back(numToken);
        }}
	break;
#line 521 "config.c"
		}
	}

_again:
	_acts = _resparser_actions + _resparser_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 1:
#line 1 "NONE"
	{ts = 0;}
	break;
#line 534 "config.c"
		}
	}

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _resparser_eof_trans[cs] > 0 ) {
		_trans = _resparser_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	_out: {}
	}

#line 179 "config.rl"


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


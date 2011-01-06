/*
 * Font implementation derived from Haaf's Game Engine
 *
 * Haaf's Game Engine 1.7
 * Copyright (C) 2003-2007, Relish Games
 * hge.relishgames.com
 *
 * hgeFont helper class implementation
 */

#include "bluegin/hgefont.h"
#include <algorithm>
#include <sstream>
#include <iterator>

using namespace bluegin;
using namespace cinder;
using namespace std;

const char FNTHEADERTAG[] = "[HGEFONT]";
const char FNTBITMAPTAG[] = "Bitmap";
const char FNTCHARTAG[]   = "Char";

#define BUFFER_LENGTH 1024
char hgeFont::buffer[BUFFER_LENGTH];

hgeFont::hgeFont(const char *szFont, gl::Texture texture)
: mTexture(texture)
{
	const void	*data;
	int		size;
	char	*desc, *pdesc;
	char	linebuf[256];
	char	buf[256], *pbuf;
	char	chr;
	int		i, x, y, w, h, a, c;

    // itodo: font data in code if this doesn't compile!

	// Setup variables

	fHeight=0.0f;
	fScale=1.0f;
	fProportion=1.0f;
	fRot=0.0f;
	fTracking=0.0f;
	fSpacing=1.0f;

	fZ=0.5f;
	nBlend=0;
	color = ColorA(1.0f, 1.0f, 1.0f, 1.0f);

	memset( &letters, 0, sizeof(letters) );
	memset( &pre, 0, sizeof(letters) );
	memset( &post, 0, sizeof(letters) );
	
	// Load font description
	desc = static_cast<char*>(bluegin_load_resource(szFont, &size));
	if(!desc) return;

	pdesc=_get_line(desc,linebuf);
	if(strcmp(linebuf, FNTHEADERTAG))
	{
		Log("Font %s has incorrect format.", szFont);
		delete[] desc;	
		return;
	}

	// Parse font description

	while(pdesc = _get_line(pdesc,linebuf))
	{
		if(!strncmp(linebuf, FNTBITMAPTAG, sizeof(FNTBITMAPTAG)-1 ))
		{
            //  Texture is explicitly assigned in constructor
		}
		else if(!strncmp(linebuf, FNTCHARTAG, sizeof(FNTCHARTAG)-1 ))
		{
			pbuf=strchr(linebuf,'=');
			if(!pbuf) continue;
			pbuf++;
			while(*pbuf==' ') pbuf++;
			if(*pbuf=='\"')
			{
				pbuf++;
				i=(unsigned char)*pbuf++;
				pbuf++; // skip "
			}
			else
			{
				i=0;
				while((*pbuf>='0' && *pbuf<='9') || (*pbuf>='A' && *pbuf<='F') || (*pbuf>='a' && *pbuf<='f'))
				{
					chr=*pbuf;
					if(chr >= 'a') chr-='a'-':';
					if(chr >= 'A') chr-='A'-':';
					chr-='0';
					if(chr>0xF) chr=0xF;
					i=(i << 4) | chr;
					pbuf++;
				}
				if(i<0 || i>255) continue;
			}
			sscanf(pbuf, " , %d , %d , %d , %d , %d , %d", &x, &y, &w, &h, &a, &c);
            // Log("fontdata: %d %d %d %d %d %d", x, y, w, h, a, c);

			letters[i] = new FontQuad();

            int tw = mTexture.getWidth(); 
            int th = mTexture.getHeight();
            letters[i]->w = (float)w;
            letters[i]->h = (float)h;
            letters[i]->u0 = (float)x / (float)tw;
            letters[i]->u1 = (float)(x + w) / (float)tw;
            letters[i]->v0 = (float)y / (float)th;
            letters[i]->v1 = (float)(y + h) / (float)th;


			pre[i]=(float)a;
			post[i]=(float)c;
			if(h>fHeight) fHeight=(float)h;
		}
	}

	delete[] desc;	
}


hgeFont::~hgeFont()
{
	for(int i=0; i<256; i++)
		if(letters[i]) delete letters[i];
}

FontLine::FontLine(int lo, int lc, float lw, float ly)
    : offset(lo), count(lc), width(lw), y(ly)
{
}

string hgeFont::Layout(float width, string text)
{
    float widthLeft = width;
    istringstream iss(text);
    ostringstream oss;
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter<vector<string> >(tokens));

    bool first = true;    // first word of new line?
    float spaceWidth = GetStringWidth(" ", false);
    for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        float tokenWidth = GetStringWidth(it->c_str(), false);

        if (!first) {
            if (widthLeft > tokenWidth + spaceWidth) {
                oss << " " << *it;
                widthLeft -= tokenWidth + spaceWidth;
            }
            else {
                oss << "\n";
                first = true;
                widthLeft = width;
            }
        }

        if (first) {
            oss << *it;
            if (tokenWidth > widthLeft) {
                oss << "\n";
                first = true;
                widthLeft = width;
            }
            else {
                first = false;
            }
            widthLeft -= tokenWidth;
        }
    }

    return oss.str();
}

//  Render this font to a vector of positions and vector of texcoords
void hgeFont::Render(std::vector<ci::Rectf>& letterRects, 
                     std::vector<ci::Rectf>& texRects,
                     float x, float y, float width, int align, const char* text)
{
    int i;
    float fx=x;

    //  clear previous data
    letterRects.clear();
    texRects.clear();

    string layoutText(Layout(width, string(text)));
    const char* itext = layoutText.c_str();

    float textWidth = GetStringWidth(itext, false);
    align &= HGETEXT_HORZMASK;
    if (align == HGETEXT_RIGHT)
        fx -= textWidth;
    if (align == HGETEXT_CENTER)
        fx -= int(textWidth/2.0f);
    
    while(*itext) {
    	if(*itext=='\n')
    	{
    		y += int(fHeight*fScale*fSpacing);
    		fx = x;
            textWidth = GetStringWidth(itext+1, false);
    		if(align == HGETEXT_RIGHT)  fx -= textWidth;
    		if(align == HGETEXT_CENTER) fx -= int(textWidth/2.0f);
    	}
    	else
    	{
    		i=(unsigned char)*itext;
    		if(!letters[i]) i='?';
    		if(letters[i])
    		{
    			fx += pre[i]*fScale*fProportion;
    
                float x2 = fx+letters[i]->w * fScale * fProportion;
                float y2 = y + letters[i]->h * fScale;
                letterRects.push_back(Rectf(fx, y, x2, y2));
                texRects.push_back(Rectf(letters[i]->u0, letters[i]->v0, letters[i]->u1, letters[i]->v1));
    
    			fx += (letters[i]->w+post[i]+fTracking)*fScale*fProportion;
    		}
    	}
    	itext++;
    }
}

gl::Texture& hgeFont::getTexture()
{
    return mTexture;
}

float hgeFont::GetStringWidth(const char *string, bool bMultiline) const
{
	int i;
	float linew, w = 0;

	while(*string)
	{
		linew = 0;

		while(*string && *string != '\n')
		{
			i=(unsigned char)*string;
			if(!letters[i]) i='?';
			if(letters[i])
				linew += letters[i]->w + pre[i] + post[i] + fTracking;

			string++;
		}

		if(!bMultiline) return linew*fScale*fProportion;

		if(linew > w) w = linew;

		while (*string == '\n' || *string == '\r') string++;
	}

	return w*fScale*fProportion;
}

void hgeFont::SetColor(Color col)
{
	color = col;
}

void hgeFont::SetZ(float z)
{
	fZ = z;
}

void hgeFont::SetBlendMode(int blend)
{
	nBlend = blend;
}

char *hgeFont::_get_line(char *file, char *line)
{
	int i=0;

	if(!file[i]) return 0;

	while(file[i] && file[i]!='\n' && file[i]!='\r')
	{
		line[i]=file[i];
		i++;
	}
	line[i]=0;

	while(file[i] && (file[i]=='\n' || file[i]=='\r')) i++;

	return file + i;
}

/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeFont helper class header
*/

#pragma once

#include "bluegin/bluegin.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"

namespace bluegin {

#define HGETEXT_LEFT		0
#define HGETEXT_RIGHT		1
#define HGETEXT_CENTER		2
#define HGETEXT_HORZMASK	0x03

#define HGETEXT_TOP			0
#define HGETEXT_BOTTOM		4
#define HGETEXT_MIDDLE		8
#define HGETEXT_VERTMASK	0x0C

struct FontQuad
{
    float w;
    float h;

    float u0;
    float u1;
    float v0;
    float v1;
};

struct TextRenderData
{
    ci::Vec2f position;
    std::vector<ci::Rectf> letterRects;
    std::vector<ci::Rectf> texRects;

    float width;
    float size;
    std::string text;
    int align;
};

struct FontLine
{
    int offset;
    int count;
    float width;
    float y;

    FontLine(int lo, int lc, float lw, float ly); 
};

struct LineBreak
{
    int start;
    int end;
    float width;

    LineBreak() : start(0), end(0), width(0) { }
    LineBreak(int s, int e, float w) : start(s), end(e), width(w) { }
};

/*
** HGE Font class
*/
class hgeFont
{
public:
    hgeFont(const char *filename, ci::gl::Texture texture);
    ~hgeFont();

    std::string Layout(float width, std::string text);
    void        Render(std::vector<ci::Rectf>& letterRects, 
                       std::vector<ci::Rectf>& texRects,
                       float x, float y, float width, int align, const char* text);

    void		printf(float x, float y, int align, const char *format, ...);
    void		printfb(float x, float y, float w, float h, int align, const char *format, ...);

    void		SetColor(ci::Color col);
    void		SetZ(float z);
    void		SetBlendMode(int blend);
    void		SetScale(float scale) {fScale=scale;}
    void		SetProportion(float prop) { fProportion=prop; }
    void		SetRotation(float rot) {fRot=rot;}
    void		SetTracking(float tracking) {fTracking=tracking;}
    void		SetSpacing(float spacing) {fSpacing=spacing;}

    ci::ColorA  GetColor() const {return color;}
    float		GetZ() const {return fZ;}
    int			GetBlendMode() const {return nBlend;}
    float		GetScale() const {return fScale;}
    float		GetProportion() const { return fProportion; }
    float		GetRotation() const {return fRot;}
    float		GetTracking() const {return fTracking;}
    float		GetSpacing() const {return fSpacing;}

    //hgeSprite*	GetSprite(char chr) const { return letters[(unsigned char)chr]; }
    float		GetPreWidth(char chr) const { return pre[(unsigned char)chr]; }
    float		GetPostWidth(char chr) const { return post[(unsigned char)chr]; }
    float		GetHeight() const { return fHeight; }
    float		GetStringWidth(const char *string, bool bMultiline=true) const;

    ci::gl::Texture& getTexture();

protected:
    void addFontLine(vector<FontLine>& lines, LineBreak* lbreak, float ly);

private:
    hgeFont();
    hgeFont(const hgeFont &fnt);
    hgeFont&	operator= (const hgeFont &fnt);

    char*		_get_line(char *file, char *line);

    static char	buffer[1024];

    ci::gl::Texture mTexture;
    FontQuad*	letters[256];
    float		pre[256];
    float		post[256];
    float		fHeight;
    float		fScale;
    float		fProportion;
    float		fRot;
    float		fTracking;
    float		fSpacing;

    ci::ColorA  color;
    float		fZ;
    int			nBlend;
};

typedef shared_ptr<hgeFont> FontPtr;

}

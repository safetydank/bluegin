#include "flx/flxG.h"
#include "flx/text.h"
#include "flx/render.h"


using namespace ci;
using namespace flx;
using namespace bluegin;

extern FlxGlobal FlxG;

TextPtr Text::create(float X, float Y, int Width, std::string text)
{
    TextPtr txt(new Text(X, Y, Width, text));
    return txt;
}

Text::Text(float X, float Y, int Width, std::string Text)
    : Sprite(X, Y)
{
    //_tf = new TextField();
    //_tf.height = 1;
    //_tf.embedFonts = true;
    //_tf.selectable = false;
    //_tf.sharpness = 100;
    //_tf.multiline = true;
    //_tf.wordWrap = true;
    //_tf.defaultTextFormat = new TextFormat("system",8,0xffffff);
    //

    _trd.align = ALIGN_LEFT;

    _font = FlxG.defaultFont;
    _trd.size = 8.0f;

    width = _trd.width = Width;
    _trd.text = Text;

    _regen = true;
    _shadow = ColorA(0, 0, 0, 0);
    solid = false;
    if (_font && !Text.empty())
        calcFrame();
}

/**
 * You can use this if you have a lot of text parameters
 * to set instead of the individual properties.
 * 
 * @param	Font		The name of the font face for the text display.
 * @param	Size		The size of the font (in pixels essentially).
 * @param	Color		The color of the text in traditional flash 0xRRGGBB format.
 * @param	Alignment	A string representing the desired alignment ("left,"right" or "center").
 * @param	ShadowColow	A uint representing the desired text shadow color in flash 0xRRGGBB format.
 * 
 * @return	This FlxText instance (nice for chaining stuff together, if you're into that).
 */
//FlxText& Text::setFormat(Font:String=null,Size:Number=8,Color:uint=0xffffff,Alignment:String=null,ShadowColor:uint=0):FlxText
//{
//    if(Font == null)
//        Font = "";
//    var tf:TextFormat = dtfCopy();
//    tf.font = Font;
//    tf.size = Size;
//    tf.color = Color;
//    tf.align = Alignment;
//    _tf.defaultTextFormat = tf;
//    _tf.setTextFormat(tf);
//    _shadow = ShadowColor;
//    _regen = true;
//    calcFrame();
//
//    return *this;
//}

/**
 * The text being displayed.
 */
std::string Text::getText()
{
    return _trd.text;
}

/**
 * @private
 */
void Text::setText(string Text)
{
    _trd.text = Text;
    _regen = true;
    calcFrame();
}

Vec2f Text::getPos()
{
    return Vec2f(x, y);
}

void Text::setPos(Vec2f pos)
{
    x = pos.x;
    y = pos.y;
    calcFrame();
}

#define BUFFER_LENGTH 1024
#define VARARGS_TO_BUFFER(buf) {								  \
		va_list argList;										  \
		va_start(argList, format);								  \
		int i = vsnprintf(buf, BUFFER_LENGTH-2, format, argList); \
		if (i < 0 || i > BUFFER_LENGTH-2) {                       \
			buf[BUFFER_LENGTH-1] = '\0';                          \
		}                                                         \
		va_end(argList);                                          \
}

void Text::printf(const char* format, ...)
{
    char buffer[BUFFER_LENGTH];
    VARARGS_TO_BUFFER(buffer);
    setText(string(buffer));
}

float Text::getSize()
{
    return _trd.size;
}

void Text::setSize(float Size)
{
    _trd.size = Size;
    // _tf.defaultTextFormat = tf;
    // _tf.setTextFormat(tf);
    _regen = true;
    calcFrame();
}

/**
 * The font used for this text.
 */
hgeFont* Text::getFont()
{
    return _font;
}

/**
 * @private
 */
void Text::setFont(FontPtr font)
{
    // var tf:TextFormat = dtfCopy();
    // tf.font = Font;
    // _tf.defaultTextFormat = tf;
    // _tf.setTextFormat(tf);
    _font = font.get();
    _regen = true;
    calcFrame();
}

//ci::Color Text::getShadow()
//{
//    return _shadow;
//}
//
//void Text::setShadow(ci::Color color)
//{
//    _shadow = color;
//    calcFrame();
//}

/**
 * Internal function to update the current animation frame.
 */
void Text::calcFrame()
{
    // Just leave if there's no text to render
    if (_font == NULL || _trd.text.empty()) {
        _trd.letterRects.clear();
        _trd.texRects.clear();
        return;
    }

    if (_regen)
    {
        _font->SetScale(_trd.size / _font->GetHeight());
        // _font->Render(_trd.letterRects, _trd.texRects, 0, 0, 0, _trd.text.c_str());
        _font->Render(_trd.letterRects, _trd.texRects, 0, 0, _trd.width, _trd.align, _trd.text.c_str());
    }
    //     //Render a single pixel shadow beneath the text
    //     if(_shadow > 0)
    //     {
    //         _tf.setTextFormat(new TextFormat(tfa.font,tfa.size,_shadow,null,null,null,null,null,tfa.align));				
    //         _mtx.translate(1,1);
    //         _framePixels.draw(_tf,_mtx,_ct);
    //         _mtx.translate(-1,-1);
    //         _tf.setTextFormat(new TextFormat(tfa.font,tfa.size,tfa.color,null,null,null,null,null,tfa.align));
    //     }
    //     //Actually draw the text onto the buffer
    //     _framePixels.draw(_tf,_mtx,_ct);
    //     _tf.setTextFormat(new TextFormat(tf.font,tf.size,tf.color,null,null,null,null,null,tf.align));
    //     if(solid)
    //         refreshHulls();
}

void Text::render()
{
    renderText();
}

void Text::renderText()
{
    BatchState batch(_font->getTexture(), false);
    FlxG.renderer->setBatchState(batch);

    vector<Rectf>::iterator texIter = _trd.texRects.begin();
    for (vector<Rectf>::iterator letter = _trd.letterRects.begin();
            letter != _trd.letterRects.end(); ++letter, ++texIter) {

        Vec2f pos(x+letter->x1, y+letter->y1);
        if (_trd.align == ALIGN_CENTER) {
            pos.x += _trd.width * 0.5f;
        }
        else if (_trd.align == ALIGN_RIGHT) {
            pos.x += _trd.width;
        }
        FlxG.renderer->renderQuad(mColor, letter->getWidth(), letter->getHeight(), *texIter, 
                NULL, &pos);
    }
}

TextRenderData& Text::getTextRenderData()
{
    return _trd;
}

TextAlign Text::getAlignment()
{
    return static_cast<TextAlign>(_trd.align);
}

void Text::setAlignment(TextAlign Alignment)
{
    _trd.align = Alignment;
    calcFrame();
}

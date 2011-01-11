#pragma once

#include "flx/sprite.h"
#include "cinder/Color.h"
#include "cinder/Rect.h"
#include "bluegin/hgeFont.h"
#include <string>

namespace flx {

//  Same values as used by Font class
enum TextAlign
{
    ALIGN_LEFT   = 0,
    ALIGN_RIGHT  = 1,
    ALIGN_CENTER = 2
};

class Text;
typedef shared_ptr<Text> TextPtr;

/**
 * Extends <code>Sprite</code> to support rendering text.
 * Can tint, fade, rotate and scale just like a sprite.
 * Doesn't really animate though, as far as I know.
 * Also does nice pixel-perfect centering on pixel fonts
 * as long as they are only one liners.
 */
class Text : public Sprite
{
protected:
    // _tf:TextField;
    bluegin::TextRenderData _trd;
    bool _regen;
    ci::Color _shadow;
    bluegin::hgeFont* _font;
    
public:
    static TextPtr create(float X, float Y, int Width, std::string Text=std::string());

    /**
     * Creates a new <code>Text</code> object at the specified position.
     * 
     * @param	X		The X position of the text.
     * @param	Y		The Y position of the text.
     * @param	Width	The width of the text object (height is determined automatically).
     * @param	Text	The actual text you would like to display initially.
     */
    Text(float X, float Y, int Width, std::string Text=std::string());
    virtual ~Text() { }
    
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
     * @return	This Text instance (nice for chaining stuff together, if you're into that).
     */
    // Text& setFormat(Font:String=null,Size:Number=8,Color:uint=0xffffff,Alignment:String=null,ShadowColor:uint=0);
    
    /**
     * The text being displayed.
     */
    std::string getText();
    
    /**
     * @private
     */
    void setText(std::string Text);

    void printf(const char* format, ...);
    
    /**
     * The size of the text being displayed.
     */
    float getSize();

    /**
    * @private
    */
    void setSize(float Size);

     /**
     * The font used for this text.
     */
     bluegin::hgeFont* getFont();

     /**
     * @private
     */
     void setFont(bluegin::FontPtr font);

     ci::Vec2f getPos();
     void setPos(ci::Vec2f pos);

     /**
     * The alignment of the font (ALIGN_LEFT, ALIGN_RIGHT or ALIGN_CENTER)
     */
     TextAlign getAlignment();
     

     /**
     * @private
     */
     void setAlignment(TextAlign Alignment);
     

     //ci::Color getShadow();
     //void setShadow(ci::Color color);

     /**
     * Internal function to update the current animation frame.
     */
     virtual void calcFrame();

     virtual void render();
     void renderText();

     bluegin::TextRenderData& getTextRenderData();
};

}

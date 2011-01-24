#pragma once

#include "flx/object.h"
#include "bluegin/graphic.h"
#include "cinder/rect.h"

#include <rapidxml.hpp>
#include <map>
#include <string>

// typedef rapidxml::xml_node<>     XmlNode;
// typedef rapidxml::xml_document<> XmlDocument;

namespace flx {

enum Tiling
{
    /// No auto-tiling.
    OFF,
    /// Platformer-friendly auto-tiling.
    AUTO,
    /// Top-down auto-tiling.
    ALT
};

/**
 * This is a traditional tilemap display and collision class.
 */
class Tilemap : public Object
{
public:
    /**
     * What tile index will you start colliding with (default: 1).
     */
    int collideIndex;
    /**
     * The first index of your tile sheet (default: 1) If you want to change it, do so before calling loadMap().
     */
    int startingIndex;
    /**
     * What tile index will you start drawing with (default: 1)  NOTE: should always be >= startingIndex.
     * If you want to change it, do so before calling loadMap().
     */
    int drawIndex;
    /**
     * Set this flag to use one of the 16-tile binary auto-tile algorithms (OFF, AUTO, or ALT).
     */
    Tiling autoTiling;

    /**
     * Read-only variable, do NOT recommend changing after the map is loaded!
     */
    int widthInTiles;
    /**
     * Read-only variable, do NOT recommend changing after the map is loaded!
     */
    int heightInTiles;
    /**
     * Read-only variable, do NOT recommend changing after the map is loaded!
     */
    int totalTiles;

    //  Tiles source graphic
    bluegin::Graphic graphic;

protected:
    // _pixels:BitmapData;
    // _bbPixels:BitmapData;
    vector<int> _data;
    vector<ci::Rectf> _texrects;
    int _tileWidth;
    int _tileHeight;
    Object _block;
    // int _callbacks:Array;
    int _screenRows;
    int _screenCols;

    ci::Vec2f _flashPoint;
    ci::Rectf _flashRect;

public:
    /**
     * The tilemap constructor just initializes some basic variables.
     */
    Tilemap();

    /**
     * Loads a tilemap from a resource data
     *
     * Tiles - flat vector of tile IDs.  This will be deleted after loadMap
     */
    void loadMap(bluegin::Graphic& TileImages,
                 float Width, float Height, 
                 float TileWidth, float TileHeight, 
                 vector<int>& Tiles);

protected:
    /**
     * Internal function that actually renders the tilemap.  Called by render().
     */
    void renderTilemap();

public:
    /**
     * Draws the tilemap.
     */
    virtual void render();

    /**
     * @private
     */
    void setSolid(bool Solid);

    /**
     * @private
     */
    void setFixed(bool Fixed);

    struct Block
    {
        float x;
        float y;
        int data;
        Block(float bx, float by, int bdata) : x(bx), y(by), data(bdata) { }
    };

    /**
     * Checks for overlaps between the provided object and any tiles above the collision index.
     * 
     * @param	Core		The <code>Object</code> you want to check against.
     */
    virtual bool overlaps(Object& Core);

    /**
     * Called by <code>Object.updateMotion()</code> and some constructors to
     * rebuild the basic collision data for this object.
     */
    virtual void refreshHulls();

    /**
     * <code>FlxU.collide()</code> (and thus <code>Object.collide()</code>) call
     * this function each time two objects are compared to see if they collide.
     * It doesn't necessarily mean these objects WILL collide, however.
     * 
     * @param	Object	The <code>Object</code> you're about to run into.
     */
    virtual void preCollide(Object& object);

    /**
     * Check the value of a particular tile.
     * 
     * @param	X		The X coordinate of the tile (in tiles, not pixels).
     * @param	Y		The Y coordinate of the tile (in tiles, not pixels).
     * 
     * @return	A uint containing the value of the tile at this spot in the array.
     */
    int getTile(int X,int Y);

    /**
     * Get the value of a tile in the tilemap by index.
     * 
     * @param	Index	The slot in the data array (Y * widthInTiles + X) where this tile is stored.
     * 
     * @return	A uint containing the value of the tile at this spot in the array.
     */
    int getTileByIndex(int Index);

    /**
     * Change the data and graphic of a tile in the tilemap.
     * 
     * @param	X				The X coordinate of the tile (in tiles, not pixels).
     * @param	Y				The Y coordinate of the tile (in tiles, not pixels).
     * @param	Tile			The new integer data you wish to inject.
     * @param	UpdateGraphics	Whether the graphical representation of this tile should change.
     * 
     * @return	Whether or not the tile was actually changed.
     */ 
    bool setTile(int X,int Y,int Tile,bool UpdateGraphics);

    /**
     * Change the data and graphic of a tile in the tilemap.
     * 
     * @param	Index			The slot in the data array (Y * widthInTiles + X) where this tile is stored.
     * @param	Tile			The new integer data you wish to inject.
     * @param	UpdateGraphics	Whether the graphical representation of this tile should change.
     * 
     * @return	Whether or not the tile was actually changed.
     */
    bool setTileByIndex(int Index,int Tile,bool UpdateGraphics);

    /**
     * Call this function to lock the automatic camera to the map's edges.
     * 
     * @param	Border		Adjusts the camera follow boundary by whatever number of tiles you specify here.  Handy for blocking off deadends that are offscreen, etc.  Use a negative number to add padding instead of hiding the edges.
     */
    void follow(int Border=0);

    /**
     * Shoots a ray from the start point to the end point.
     * If/when it passes through a tile, it stores and returns that point.
     * 
     * @param	StartX		The X component of the ray's start.
     * @param	StartY		The Y component of the ray's start.
     * @param	EndX		The X component of the ray's end.
     * @param	EndY		The Y component of the ray's end.
     * @param	Result		A <code>Point</code> object containing the first wall impact.
     * @param	Resolution	Defaults to 1, meaning check every tile or so.  Higher means more checks!
     * @return	Whether or not there was a collision between the ray and a colliding tile.
     */
    bool ray(float StartX, float StartY, float EndX, float EndY, ci::Vec2f& Result, float Resolution=1.0f);

protected:
    /**
     * An internal function used by the binary auto-tilers.
     * 
     * @param	Index		The index of the tile you want to analyze.
     */
    void autoTile(int Index);

    /**
     * Internal function used in setTileByIndex() and the constructor to update the map.
     * 
     * @param	Index		The index of the tile you want to update.
     */
    void updateTile(int Index);
};

typedef shared_ptr<Tilemap> TilemapPtr;

/**
 * Internal struct used to parse Ogmo XML entities
 */
struct OgmoObject
{
    int x;
    int y;
    std::map<const char*, const char*> values;

    int getIntegerValue(const char* key);
    double getNumberValue(const char* key);
    std::string getStringValue(const char* key);
    bool getBooleanValue(const char* key);
};

/**
 *   A class for reading tilemaps from an Ogmo XML level file
 */
class OgmoLoader
{
public:
    /**  Construct a loader from an Ogmo level description (XML) */
    OgmoLoader(const char* LevelFile);
    ~OgmoLoader();

    ///  Load a named tiles layer to a tilemap
    void loadTiles(Tilemap& tilemap, const char* Layer);
    ///  Load level values
    bool loadValues(OgmoObject& object);
    ///  Load first matching object
    bool loadObject(OgmoObject& object, const char* Parent, const char* Name);
    ///  Load all matching objects
    bool loadObjects(vector<OgmoObject>& objects, const char* Parent, const char* Name);

    //  XML level node
    rapidxml::xml_node<>* level;

protected:
    char* _xml;
    rapidxml::xml_document<> doc;
    float width;
    float height;

};

}

#include "flx/tilemap.h"

// XXX FlxG only used by OgmoLoader
#include "flx/flxG.h"
#include "bluegin/bluegin.h"

using namespace ci;
using namespace bluegin;
using namespace flx;
using namespace rapidxml;

extern FlxGlobal FlxG;

Tilemap::Tilemap() : Object(0, 0, 0, 0), _block(0, 0, 0, 0)
{
    autoTiling = OFF;
    collideIndex = 1;
    startingIndex = 1;
    drawIndex = 1;
    widthInTiles = 0;
    heightInTiles = 0;
    totalTiles = 0;
    _data.clear();
    _tileWidth = 0;
    _tileHeight = 0;
    _texrects.clear();
    _block.width = _block.height = 0;
    _block.fixed = true;
    // _callbacks = new Array();
    fixed = true;
}

void Tilemap::loadMap(Graphic& TileImages,
                      float Width, float Height, 
                      float TileWidth, float TileHeight, 
                      vector<int>& Tiles)
{
    graphic = TileImages;

    //Figure out the map dimensions based on the data string
    int c;
    heightInTiles = Height / TileHeight;
    widthInTiles  = Width / TileWidth;
    _data.clear();
    _data.swap(Tiles);    

    //Pre-process the map data if it's auto-tiled
    totalTiles = widthInTiles*heightInTiles;
    if(autoTiling > OFF)
    {
        collideIndex = startingIndex = drawIndex = 1;
        for(int i = 0; i < totalTiles; i++)
            autoTile(i);
    }

    //Figure out the size of the tiles
    // _pixels = FlxG.addBitmap(TileGraphic);
    this->graphic = graphic;
    _tileWidth = TileWidth;
    if(_tileWidth == 0)
        _tileWidth = TileImages.getHeight();
    _tileHeight = TileHeight;
    if(_tileHeight == 0)
        _tileHeight = _tileWidth;
    _block.width = _tileWidth;
    _block.height = _tileHeight;

    //Then go through and create the actual map
    width = widthInTiles*_tileWidth;
    height = heightInTiles*_tileHeight;
    _texrects.clear();
    _texrects.resize(totalTiles);
    for(int i = 0; i < totalTiles; i++)
        updateTile(i);

    //Pre-set some helper variables for later
    _screenRows = FlxU::ceil(FlxG.height/_tileHeight)+1;
    if(_screenRows > heightInTiles)
        _screenRows = heightInTiles;
    _screenCols = FlxU::ceil(FlxG.width/_tileWidth)+1;
    if(_screenCols > widthInTiles)
        _screenCols = widthInTiles;

    refreshHulls();
}

void Tilemap::renderTilemap()
{
    getScreenXY(_point);
    _flashPoint = _point;
    int tx = FlxU::floor(-_flashPoint.x/_tileWidth);
    int ty = FlxU::floor(-_flashPoint.y/_tileHeight);
    if(tx < 0) tx = 0;
    if(tx > widthInTiles-_screenCols) tx = widthInTiles-_screenCols;

    if(ty < 0) ty = 0;
    if(ty > heightInTiles-_screenRows) ty = heightInTiles-_screenRows;

    int ri = ty*widthInTiles+tx;
    _flashPoint.x += tx*_tileWidth;
    _flashPoint.y += ty*_tileHeight;
    float opx = _flashPoint.x;
    int c;
    int cri;

    BatchState bs(graphic.getTexture(), false);
    FlxG.renderer->setBatchState(bs);

    for(int r = 0; r < _screenRows; r++)
    {
        cri = ri;
        for(c = 0; c < _screenCols; c++)
        {
            Rectf& texRect = _texrects[cri++];
            if (texRect.x1 != texRect.x2 && texRect.y1 != texRect.y2)
                FlxG.renderer->renderQuad(Color::white(), _tileWidth, _tileHeight, texRect, NULL, &_flashPoint);
            _flashPoint.x += _tileWidth;
        }
        ri += widthInTiles;
        _flashPoint.x = opx;
        _flashPoint.y += _tileHeight;
    }
}

void Tilemap::render()
{
    renderTilemap();
}

//
bool Tilemap::overlaps(Object& Core)
{
    int c;
    int d;
    int i;
    int dd;
    vector<Block> blocks;
    //First make a list of all the blocks we'll use for collision
    int ix = FlxU::floor((Core.x - x)/_tileWidth);
    int iy = FlxU::floor((Core.y - y)/_tileHeight);
    int iw = FlxU::ceil(Core.width/_tileWidth)+1;
    int ih = FlxU::ceil(Core.height/_tileHeight)+1;
    for(int r = 0; r < ih; r++)
    {
        if(r >= heightInTiles) break;
        d = (iy+r)*widthInTiles+ix;
        for(c = 0; c < iw; c++)
        {
            if(c >= widthInTiles) break;
            dd = _data[d+c];
            if(dd >= collideIndex)
                blocks.push_back(Block(x+(ix+c)*_tileWidth,y+(iy+r)*_tileHeight,dd));
        }
    }

    //Then check for overlaps
    for (vector<Block>::iterator it = blocks.begin(); it != blocks.end(); ++it)
    {
        _block.x = it->x;
        _block.y = it->y;
        if(_block.overlaps(Core))
            return true;
    }
    return false;
}

//
void Tilemap::refreshHulls()
{
    colHullX.x = 0;
    colHullX.y = 0;
    colHullX.width = _tileWidth;
    colHullX.height = _tileHeight;
    colHullY.x = 0;
    colHullY.y = 0;
    colHullY.width = _tileWidth;
    colHullY.height = _tileHeight;
}

// XXX uses int pixel coords
void Tilemap::preCollide(Object& object)
{
    int r;
    int c;
    int rs;
    colOffsets.clear();
    int ix = FlxU::floor((object.x - x)/_tileWidth);
    int iy = FlxU::floor((object.y - y)/_tileHeight);
    int iw = ix + FlxU::ceil(object.width/_tileWidth)+1;
    int ih = iy + FlxU::ceil(object.height/_tileHeight)+1;
    if(ix < 0)
        ix = 0;
    if(iy < 0)
        iy = 0;
    if(iw > widthInTiles)
        iw = widthInTiles;
    if(ih > heightInTiles)
        ih = heightInTiles;
    rs = iy*widthInTiles;
    for(r = iy; r < ih; r++)
    {
        for(c = ix; c < iw; c++)
        {
            if(_data[rs+c] >= collideIndex)
                colOffsets.push_back(Vec2f(x+c*_tileWidth, y+r*_tileHeight));
        }
        rs += widthInTiles;
    }
}

//
int Tilemap::getTile(int X,int Y)
{
    return getTileByIndex(Y * widthInTiles + X);
}

//
int Tilemap::getTileByIndex(int Index)
{
    return _data[Index];
}

// 
bool Tilemap::setTile(int X,int Y,int Tile,bool UpdateGraphics)
{
    if((X >= widthInTiles) || (Y >= heightInTiles))
        return false;
    return setTileByIndex(Y * widthInTiles + X,Tile,UpdateGraphics);
}

//
bool Tilemap::setTileByIndex(int Index,int Tile,bool UpdateGraphics)
{
    if(Index >= _data.size())
        return false;

    bool ok = true;
    _data[Index] = Tile;

    if(!UpdateGraphics)
        return ok;

    if(autoTiling == OFF)
    {
        updateTile(Index);
        return ok;
    }

    //If this map is autotiled and it changes, locally update the arrangement
    int i;
    int r = int(float(Index)/widthInTiles) - 1;
    int rl = r+3;
    int c = Index%widthInTiles - 1;
    int cl = c+3;
    for(r = r; r < rl; r++)
    {
        for(c = cl - 3; c < cl; c++)
        {
            if((r >= 0) && (r < heightInTiles) && (c >= 0) && (c < widthInTiles))
            {
                i = r*widthInTiles+c;
                autoTile(i);
                updateTile(i);
            }
        }
    }

    return ok;
}

// void setCallback(Tile:uint,Callback:Function,Range:uint=1)
// {
//     Log("FlxTilemap.setCallback() has been temporarily deprecated, sorry!");
//     if(Range <= 0) return;
//     for(int i = Tile; i < Tile+Range; i++)
//         _callbacks[i] = Callback;
// }

void Tilemap::follow(int Border)
{
    FlxG.followBounds(x+Border*_tileWidth,y+Border*_tileHeight,width-Border*_tileWidth,height-Border*_tileHeight);
}

bool Tilemap::ray(float StartX, float StartY, float EndX, float EndY, ci::Vec2f& Result, float Resolution)
{
    float step = _tileWidth;
    if(_tileHeight < _tileWidth)
        step = _tileHeight;
    step /= Resolution;
    float dx = EndX - StartX;
    float dy = EndY - StartY;
    float distance = sqrt(dx*dx + dy*dy);
    int steps = FlxU::ceil(distance/step);
    float stepX = dx/steps;
    float stepY = dy/steps;
    float curX = StartX - stepX;
    float curY = StartY - stepY;
    int tx;
    int ty;
    for(int i = 0; i < steps; i++)
    {
        curX += stepX;
        curY += stepY;

        if((curX < 0) || (curX > width) || (curY < 0) || (curY > height))
            continue;

        tx = curX/_tileWidth;
        ty = curY/_tileHeight;
        if((_data[ty*widthInTiles+tx]) >= collideIndex)
        {
            //Some basic helper stuff
            tx *= _tileWidth;
            ty *= _tileHeight;
            float rx = 0;
            float ry = 0;
            float q;
            float lx = curX-stepX;
            float ly = curY-stepY;

            //Figure out if it crosses the X boundary
            q = tx;
            if(dx < 0)
                q += _tileWidth;
            rx = q;
            ry = ly + stepY*((q-lx)/stepX);
            if((ry > ty) && (ry < ty + _tileHeight))
            {
                Result.x = rx;
                Result.y = ry;
                return true;
            }

            //Else, figure out if it crosses the Y boundary
            q = ty;
            if(dy < 0)
                q += _tileHeight;
            rx = lx + stepX*((q-ly)/stepY);
            ry = q;
            if((rx > tx) && (rx < tx + _tileWidth))
            {
                Result.x = rx;
                Result.y = ry;
                return true;
            }
            return false;
        }
    }
    return false;
}

void Tilemap::autoTile(int Index)
{
    if(_data[Index] == 0) return;
    _data[Index] = 0;
    if((Index-widthInTiles < 0) || (_data[Index-widthInTiles] > 0)) 		//UP
        _data[Index] += 1;
    if((Index%widthInTiles >= widthInTiles-1) || (_data[Index+1] > 0)) 		//RIGHT
        _data[Index] += 2;
    if((Index+widthInTiles >= totalTiles) || (_data[Index+widthInTiles] > 0)) //DOWN
        _data[Index] += 4;
    if((Index%widthInTiles <= 0) || (_data[Index-1] > 0)) 					//LEFT
        _data[Index] += 8;
    if((autoTiling == ALT) && (_data[Index] == 15))	//The alternate algo checks for interior corners
    {
        if((Index%widthInTiles > 0) && (Index+widthInTiles < totalTiles) && (_data[Index+widthInTiles-1] <= 0))
            _data[Index] = 1;		//BOTTOM LEFT OPEN
        if((Index%widthInTiles > 0) && (Index-widthInTiles >= 0) && (_data[Index-widthInTiles-1] <= 0))
            _data[Index] = 2;		//TOP LEFT OPEN
        if((Index%widthInTiles < widthInTiles) && (Index-widthInTiles >= 0) && (_data[Index-widthInTiles+1] <= 0))
            _data[Index] = 4;		//TOP RIGHT OPEN
        if((Index%widthInTiles < widthInTiles) &&(Index+widthInTiles < totalTiles) && (_data[Index+widthInTiles+1] <= 0))
            _data[Index] = 8; 		//BOTTOM RIGHT OPEN
    }
    _data[Index] += 1;
}

void Tilemap::updateTile(int Index)
{
    //  Map a tile index to a tile texture rectangle
    if(_data[Index] < drawIndex)
    {
        _texrects[Index] = Rectf(0,0,0,0);
        return;
    }
    int rx = (_data[Index]-startingIndex)*_tileWidth;
    int ry = 0;
    if(rx >= graphic.getWidth())
    {
        ry = rx/graphic.getWidth()*_tileHeight;
        rx %= graphic.getHeight();
    }
    Rectf frameRect(rx,ry,rx+_tileWidth,ry+_tileHeight);
    _texrects[Index] = graphic.mapFrameRect(frameRect);
}


int OgmoObject::getIntegerValue(const char* key)
{
    return atoi(values[key]);
}

double OgmoObject::getNumberValue(const char* key)
{
    return atof(values[key]);
}

string OgmoObject::getStringValue(const char* key)
{
    return string(values[key]);
}

bool OgmoObject::getBooleanValue(const char* key)
{
    return bool(!strcmp("true", values[key]));
}

OgmoLoader::OgmoLoader(const char* LevelFile)
{
    int filesize;
    _xml = (char *) bluegin_load_resource(LevelFile, &filesize);
    doc.parse<0>(_xml);

    level = doc.first_node("level");

    //  Set width and height from XML
    width  = atoi(level->first_node("width")->value());
    height = atoi(level->first_node("height")->value());
}

OgmoLoader::~OgmoLoader()
{
    //  Free the text resource, invalidates any held XML nodes
    delete _xml;
}

void OgmoLoader::loadTiles(Tilemap& tilemap, const char* Layer)
{
    ResourceManager& res = *(FlxG.resources);

    xml_node<>* tilesLayer = level->first_node(Layer);
    if (!tilesLayer) {
        Log("Error loading Tiles layer \"%s\", not found in level file.");
    }

    Graphic tileImages = res.graphic(tilesLayer->first_attribute("set")->value());

    int tileWidth  = atoi(tilesLayer->first_attribute("tileWidth")->value());
    int tileHeight = atoi(tilesLayer->first_attribute("tileHeight")->value());
    int widthInTiles = width / tileWidth;
    int heightInTiles = height / tileHeight;
    int numTiles = widthInTiles * heightInTiles;

    vector<int> tileData;
    tileData.resize(numTiles);
    for (vector<int>::iterator it=tileData.begin(); it != tileData.end(); ++it) {
        *it = 0;
    }

    for (xml_node<>* tile = tilesLayer->first_node("tile"); tile; tile = tile->next_sibling()) {
        int tx = atoi(tile->first_attribute("x")->value());
        int ty = atoi(tile->first_attribute("y")->value());
        int id = atoi(tile->first_attribute("id")->value()) + 1;
        int index = (tx / tileWidth) + ((ty / tileHeight) * widthInTiles);
        tileData[index] = id;
    }

    tilemap.loadMap(tileImages, width, height, tileWidth, tileHeight, tileData);
}

static void loadSingleObject(OgmoObject& object, xml_node<>* objNode)
{
    //  parse all the attributes to a map
    for (xml_attribute<>* attr = objNode->first_attribute();
        attr; attr = attr->next_attribute()) {
        if (!strcmp("x", attr->name())) {
            object.x = atoi(attr->value());
        }
        else if (!strcmp("y", attr->name())) {
            object.y = atoi(attr->value());
        }
        else {
            object.values[attr->name()] = attr->value();
        }
    }
}

bool OgmoLoader::loadValues(OgmoObject& object)
{
    loadSingleObject(object, level);
    return true;
}

bool OgmoLoader::loadObject(OgmoObject& object, const char* Parent, const char* Name)
{
    //  reset object
    object.x = object.y = 0;
    if (!object.values.empty())
        object.values.clear();

    xml_node<>* parent = level->first_node(Parent);
    if (!parent) {
        Log("Error loading object, can't find parent node %s", Parent);
        return false;
    }
    xml_node<>* objNode = parent->first_node(Name);
    if (!objNode) {
        Log("Error loading object, can't find object tag %s", Name);
        return false;
    }

    loadSingleObject(object, objNode);
    return true;
}

bool OgmoLoader::loadObjects(vector<OgmoObject>& objects, const char* Parent, const char* Name)
{
    xml_node<>* parent = level->first_node(Parent);
    if (!parent) {
        Log("Error loading objects, can't find parent node %s", Parent);
        return false;
    }

    xml_node<>* objNode = parent->first_node(Name);
    if (!objNode) {
        Log("Error loading objects, can't find object tag %s", Name);
        return false;
    }

    while (objNode) {
        OgmoObject object;
        loadSingleObject(object, objNode);
        objects.push_back(object);
        objNode = objNode->next_sibling(Name);
    }
    return true;
}

namespace rapidxml {
    void parse_error_handler(const char *what, void *where)
    {
        Log("Parse error: %s", what);
        std::abort();
    }
}

namespace {
    void force_link_rapidxml() {
        OgmoLoader loader(NULL);
        Tilemap t;
        loader.loadTiles(t, NULL);
        rapidxml::parse_error_handler(NULL, NULL);
    }
}


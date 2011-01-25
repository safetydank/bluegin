using namespace ci;
using namespace flx;

class PlayState : public State
{
public:
    SpritePtr sprite;

    void PlayState::create()
    {
        ResourceManager& res = *(FlxG.resources);
        sprite = Sprite::create(FlxG.width/2, FlxG.height/2, res.graphic("ball"));
        sprite->velocity = Rand::randVec2f() * 20.0f;
        //  Add sprite to the state's default group
        add(sprite);
    }

    void PlayState::update()
    {
        //  Make a shortcut so we can use use dot notation
        Sprite& sp = *sprite;

        //  Bounce when hitting the view edges
        if (sp.x < 0 || sp.x > FlxG.width) {
            sp.velocity.x = -sp.velocity.x;
        }
        else if (sp.y < 0 || sp.y > FlxG.height) {
            sp.velocity.y = -sp.velocity.y;
        }

        //  Limit sprite position
        sp.x = math<float>::clamp(sp.x, 0, FlxG.width);
        sp.y = math<float>::clamp(sp.y, 0, FlxG.height);

        //  Updates the state's default group
        State::update();
    }
};


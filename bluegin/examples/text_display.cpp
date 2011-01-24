using namespace flx;

void PlayState::create()
{
    TextPtr text = Text::create(0, 0, FlxG.width, "Hello world!");
    text->setSize(30.0f);
    text->setAlignment(ALIGN_RIGHT);
    add(text);
}


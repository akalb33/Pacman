#include "stdafx.h"
#include "Background.h"

Background::Background()
   : Character(typeid(Background))
{
   SetTexture(TextureManager::background);
   SetDisplayOrder(1);

}

Background::~Background()
{
}

void Background::Process(float deltaTime)
{
}

void Background::Kill()
{
}

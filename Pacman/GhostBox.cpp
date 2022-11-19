#include "stdafx.h"
#include "GhostBox.h"

GhostBox::GhostBox()
   : Character(typeid(GhostBox))
{
   SetTexture(TextureManager::ghostbox);
   SetDisplayOrder(2);

}

GhostBox::~GhostBox()
{
}

void GhostBox::Process(float deltaTime)
{
}

void GhostBox::Kill()
{
}

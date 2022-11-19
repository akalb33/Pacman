#include "stdafx.h"
#include "TunnelCover.h"

TunnelCover::TunnelCover()
   : Character(typeid(TunnelCover))
{
   SetTexture(TextureManager::black);
   SetDisplayOrder(5);

}

TunnelCover::~TunnelCover()
{
}

void TunnelCover::Process(float deltaTime)
{
}

void TunnelCover::Kill()
{
}

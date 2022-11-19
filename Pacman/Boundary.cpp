#include "stdafx.h"
#include "Boundary.h"

Boundary::Boundary()
   : Character(typeid(Boundary))
{
   SetTexture(TextureManager::boundary0);

   SetDisplayOrder(2);
}

Boundary::~Boundary()
{
}

void Boundary::Reset()
{
   SetStatus(StatusManager::alive);
   SetTexture(TextureManager::boundary0);
   mAngle = 0;

}

void Boundary::Process(float deltaTime)
{
   ApplyInertia(deltaTime);
}

void Boundary::Kill()
{
   SetStatus(StatusManager::dead);
}
                                     
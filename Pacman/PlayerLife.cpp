#include "stdafx.h"
#include "PlayerLife.h"

PlayerLife::PlayerLife()
   : Character(typeid(PlayerLife))
{
   SetTexture(TextureManager::pacman2);
   SetDisplayOrder(2);
   mScale = 0.7f;
}

PlayerLife::~PlayerLife()
{
}

void PlayerLife::Process(float deltaTime)
{
}

void PlayerLife::Kill()
{
   SetStatus(StatusManager::dead);
}
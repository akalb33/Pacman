#include "stdafx.h"
#include "PlayerLivesManager.h"
#include <assert.h>

PlayerLivesManager::PlayerLivesManager()
   : mpPlayerLives()
   , mStartingX(0.0f)
   , mStartingY(0.0f)
{
}

void PlayerLivesManager::AddPlayerLife()
{
   PlayerLife* pPlayerLife = Character::Resurrect<PlayerLife>();
   assert(pPlayerLife != 0);
   pPlayerLife->mX = mStartingX + mpPlayerLives.size() * (0.75f * (pPlayerLife->GetMaxX() - pPlayerLife->GetMinX()));
   pPlayerLife->mY = mStartingY;
   pPlayerLife->mScale = mScale * 0.4f;
   pPlayerLife->mAngle = 180.0f;
   mpPlayerLives.push_back(pPlayerLife);
}

void PlayerLivesManager::RemovePlayerLife()
{
   if (!mpPlayerLives.empty())
   {
      (*(mpPlayerLives.end() - 1))->Kill();
      mpPlayerLives.pop_back();
   }
}
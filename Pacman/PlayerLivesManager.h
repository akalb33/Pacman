#pragma once
#include "PlayerLife.h"          

class PlayerLivesManager
{
public:

   PlayerLivesManager();
   void AddPlayerLife();
   void RemovePlayerLife();
   void SetScale(float scale) { mScale = scale; }
   void SetStartPosition(float x, float y) 
   { 
      mStartingX = x; 
      mStartingY = y; 
   }
   bool Empty() { return mpPlayerLives.empty(); }
private:
   std::vector<PlayerLife*> mpPlayerLives;
   float mStartingX;
   float mStartingY;
   float mScale;
};

#include "stdafx.h"
#include "ScoreSign.h"

ScoreSign::ScoreSign()
   : Character(typeid(ScoreSign))
   , mValue(200)
   , mFruit(false)
   , mTimeAccumulator(0.0f)
{
   SetTexture(TextureManager::score200);
   SetDisplayOrder(10);
   mScale = 0.7f;
}

ScoreSign::~ScoreSign()
{
}

void ScoreSign::Process(float deltaTime)
{
   if (mFruit)
   {
      mTimeAccumulator += deltaTime;
      if (mTimeAccumulator > 1.0f)
      {                          
         Kill();
         mTimeAccumulator = 0.0f;
      }
   }
}

void ScoreSign::Kill()
{
   mFruit = false;
   SetStatus(StatusManager::dead);                       
}

void ScoreSign::SetValue(unsigned int value)
{
   mValue = value; 
   switch (value)
   {
   case 100:
      SetTexture(TextureManager::fruitScore100);
      mFruit = true;
      break;
   case 1600:
      SetTexture(TextureManager::score1600);
      break;
   case 800:
      SetTexture(TextureManager::score800);
      break;
   case 400:
      SetTexture(TextureManager::score400);
      break;
   default:
      SetTexture(TextureManager::score200);
      break;
   }
}

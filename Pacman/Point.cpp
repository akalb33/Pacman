#include "stdafx.h"
#include "Point.h"
#include "Character.h"
#include "Player.h"

unsigned int Point::mPointCount = 0;

Point::Point()
   : Character(typeid(Point))
   , mPowerPoint(false)
   , mClock()
   , mPowerPointVisible(true)
{
   SetTexture(TextureManager::point);
   mScale = 0.5f;
   SetDisplayOrder(2);
}

Point::~Point()
{
}

void Point::Initialize()
{
   mPointCount++;
   SetStatus(StatusManager::alive);
}

void Point::Process(float deltaTime)
{
   if (mPowerPoint)
   {
      if (mClock.getElapsedTime().asSeconds() > 0.25f)
      {
         if (mPowerPointVisible)
         {
            SetDisplayOrder(2);
            mPowerPointVisible = false;
         }
         else
         {
            SetDisplayOrder(0);
            mPowerPointVisible = true;
         }
         mClock.restart();
      }
   }
} 

void Point::Kill()
{
   mPointCount--;
   SetStatus(StatusManager::dead);
}

void Point::SetPowerPoint(bool powerPoint)
{
   mPowerPoint = powerPoint;
   if (!mPowerPoint)
   {
      SetTexture(TextureManager::point);
      AdjustPoints();
      mScale = 0.8f;
   }
   else
   {
      SetTexture(TextureManager::powerPoint);
      AdjustPoints();
      mScale = 1.2f;
   }
}

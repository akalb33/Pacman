#pragma once

#include "Character.h"

class Point : public Character
{
public:
   Point();
   virtual ~Point();
   void Initialize();
   virtual void Process(float deltaTime);
   virtual void Kill();
   void SetPowerPoint(bool powerPoint);
   bool IsPowerPoint() { return mPowerPoint; }


   static unsigned int mPointCount;
private:
   bool mPowerPoint;
   sf::Clock mClock;
   bool mPowerPointVisible;
};

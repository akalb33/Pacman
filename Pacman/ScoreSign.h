#pragma once

#include "Character.h"

class ScoreSign : public Character
{
public:
   ScoreSign();
   virtual ~ScoreSign();
   virtual void Process(float deltaTime);
   virtual void Kill();

   void SetValue(unsigned int value);
   bool IsFruit() { return mFruit; }

private:
   unsigned int mValue;
   bool mFruit;
   float mTimeAccumulator;
};

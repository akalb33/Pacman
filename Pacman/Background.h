#pragma once

#include "Character.h"

class Background : public Character
{
public:
   Background();
   virtual ~Background();
   virtual void Process(float deltaTime);
   virtual void Kill();

private:
};

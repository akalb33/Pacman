#pragma once

#include "Character.h"

class PlayerLife : public Character
{
public:
   PlayerLife();
   virtual ~PlayerLife();
   virtual void Process(float deltaTime);
   virtual void Kill();

private:
};

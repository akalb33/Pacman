#pragma once

#include "Character.h"

class GhostBox : public Character
{
public:
   GhostBox();
   virtual ~GhostBox();
   virtual void Process(float deltaTime);
   virtual void Kill();

private:
};
            
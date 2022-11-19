#pragma once

#include "Character.h"

class TunnelCover : public Character
{
public:
   TunnelCover();
   virtual ~TunnelCover();
   virtual void Process(float deltaTime);
   virtual void Kill();

private:
};

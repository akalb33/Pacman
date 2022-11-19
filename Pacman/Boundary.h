#pragma once
#include "Character.h"

class Boundary : public Character
{
public:
   Boundary();
   virtual ~Boundary();
   void Reset();
   virtual void Process(float deltaTime);
   virtual void Kill();
private:


};         
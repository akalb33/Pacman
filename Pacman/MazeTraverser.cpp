#include "stdafx.h"
#include "MazeTraverser.h"

MazeTraverser::MazeTraverser()
   : mPauseLeft(0.0f)
   , mDistanceToNextIntersection(0.0f)
   , mMazeIntersectionI(0)
   , mMazeIntersectionJ(0)
{
}

bool MazeTraverser::Paused(float deltaTime)
{
   bool paused = false;
   if (mPauseLeft > 0.0f)
   {
      mPauseLeft -= deltaTime;
      paused = true;
   }
   return paused;
}

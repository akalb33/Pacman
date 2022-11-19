#pragma once

class MazeTraverser
{
public:
   MazeTraverser();
   bool Paused(float deltaTime);
   void Pause(float duration) { mPauseLeft = duration; }

protected:
   float mPauseLeft;
   float mDistanceToNextIntersection;
   int mMazeIntersectionI;
   int mMazeIntersectionJ;
};
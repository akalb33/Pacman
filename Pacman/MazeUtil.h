#pragma once
#include <vector>
#include "MovementState.h"

class MazeUtil
{
public:
   MazeUtil();

   void SetDistanceBetweenIntersections(float distance) { mDistanceBetweenIntersections = distance; }
   float GetDistanceBetweenIntersections() const { return mDistanceBetweenIntersections; }
   void SetUpperLeftStartingPositionX(float x) { mStartingX = x; }
   void SetUpperLeftStartingPositionY(float y) { mStartingY = y; }
   float GetUpperLeftStartingPositionX() const { return mStartingX; }
   float GetUpperLeftStartingPositionY() const { return mStartingY; }
   bool DirectionAvailable(MovementState direction, int mazeIntersectionI, int mazeIntersectionJ) const;
   bool InTunnel(MovementState direction, int mazeIntersectionI, int mazeIntersectionJ) const;
   bool SnapToIntersection(float& x, float& y, int& mazeIntersectionI, int& mazeIntersectionJ) const;
   bool SnapYToIntersection(float& y, int& mazeIntersectionJ) const;
   bool SnapXToIntersection(float& x, int& mazeIntersectionI) const;
   void SetMaze(std::vector<std::vector<char>> textMaze);
   MovementState GetDirectionDecision(float targetMinusSelfX,
                                      float targetMinusSelfY,
                                      int intersectionI, 
                                      int intersectionJ, 
                                      MovementState currentState, 
                                      unsigned int bestChoiceChancePercent) const;
   void GetXAndYFromIAndJ(int mazeIntersectionI, int mazeIntersectionJ, float& x, float& y) const;
   void InitializeGridVariables(MovementState direction, 
                                float& x, 
                                float& y, 
                                float& distanceToNextIntersection, 
                                int& mazeIntersectionI, 
                                int& mazeIntersectionJ) const;

private: 
   float ConvertIToX(int i) const
   {
      return  mStartingX + mDistanceBetweenIntersections * i;
   }

   float ConvertJToY(int j) const
   {
      return  mStartingY + mDistanceBetweenIntersections * j;
   }

   int ConvertXToI(float x) const
   {
      float i = (x - mStartingX) / mDistanceBetweenIntersections;
      if (i < 0.0f)
      {
         i -= 0.5f;
      }
      else
      {
         i += 0.5f;
      }
      return static_cast<int>(i);
   }

   int ConvertYToJ(float y) const
   {
      float j = (y - mStartingY) / mDistanceBetweenIntersections;
      if (j < 0.0f)
      {
         j -= 0.5f;
      }
      else
      {
         j += 0.5f;
      }
      return static_cast<int>(j);
   }

   float mDistanceBetweenIntersections;
   float mStartingX;
   float mStartingY;
   unsigned int mWidth;
   unsigned int mHeight;
   std::vector<std::vector<char>> mMaze;
};
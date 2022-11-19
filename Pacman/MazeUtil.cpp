#include "stdafx.h"
#include "MazeUtil.h"
#include "assert.h"
#include "Util.h"
#include <utility>

MazeUtil::MazeUtil()
   : mDistanceBetweenIntersections(0.0f)
   , mStartingX(0.0f)
   , mStartingY(0.0f)
   , mWidth(0)
   , mHeight(0)
   , mMaze()
{
   Util::InitRandomizer();
}


bool MazeUtil::DirectionAvailable(MovementState direction, int mazeIntersectionI, int mazeIntersectionJ) const
{
   bool result = false;

   if (mazeIntersectionI <= 1 || mazeIntersectionI >= mWidth - 1)
   {
      if (direction == MovementState::LEFT || direction == MovementState::RIGHT)
         result = true;
   }
   else if (mazeIntersectionJ <= 1 || mazeIntersectionJ >= mHeight - 1)
   {
      if (direction == MovementState::UP || direction == MovementState::DOWN)
         result = true;
   }
   else
   {
      result = ((direction == MovementState::UP && mMaze[mazeIntersectionI - 1][mazeIntersectionJ - 2] != '*' && mMaze[mazeIntersectionI][mazeIntersectionJ - 2] != '*') ||
         (direction == MovementState::DOWN && mMaze[mazeIntersectionI - 1][mazeIntersectionJ + 1] != '*' && mMaze[mazeIntersectionI][mazeIntersectionJ + 1] != '*') ||
         (direction == MovementState::LEFT && mMaze[mazeIntersectionI - 2][mazeIntersectionJ - 1] != '*' && mMaze[mazeIntersectionI - 2][mazeIntersectionJ] != '*') ||
         (direction == MovementState::RIGHT && mMaze[mazeIntersectionI + 1][mazeIntersectionJ - 1] != '*' && mMaze[mazeIntersectionI + 1][mazeIntersectionJ] != '*'));
   }
   return result;
}


bool MazeUtil::InTunnel(MovementState direction, int mazeIntersectionI, int mazeIntersectionJ) const
{
   bool result = false;
   if (mazeIntersectionI < 1 || mazeIntersectionI > mWidth - 1 || mazeIntersectionJ < 1 || mazeIntersectionJ > mHeight - 1)
   {
      result = true;
   }
   else if (mMaze[mazeIntersectionI - 1][mazeIntersectionJ - 1] == 't' || mMaze[mazeIntersectionI][mazeIntersectionJ - 1] == 't')
   {
      result = true;
   }
    return result;
}   


bool MazeUtil::SnapXToIntersection(float& x, int& mazeIntersectionI) const
{
   bool wrapped = false;
   mazeIntersectionI = ConvertXToI(x);

   if (mazeIntersectionI == -2)
   {
      mazeIntersectionI = mWidth + 2;
      wrapped = true;
   }
   else if (mazeIntersectionI == mWidth + 2)
   {
      mazeIntersectionI = -2;
      wrapped = true;
   }

   x = ConvertIToX(mazeIntersectionI);
   return wrapped;
}


bool MazeUtil::SnapYToIntersection(float& y, int& mazeIntersectionJ) const
{
   bool wrapped = false;
   mazeIntersectionJ = ConvertYToJ(y);

   if (mazeIntersectionJ == -2)
   {
      mazeIntersectionJ = mHeight + 2;
      wrapped = true;
   }
   else if (mazeIntersectionJ == mHeight + 2)
   {
      mazeIntersectionJ = -2;
      wrapped = true;
   }

   y = ConvertJToY(mazeIntersectionJ);
   return wrapped;
}


bool MazeUtil::SnapToIntersection(float& x, float& y, int& mazeIntersectionI, int& mazeIntersectionJ) const
{
   return SnapXToIntersection(x, mazeIntersectionI) || SnapYToIntersection(y, mazeIntersectionJ);
}

void MazeUtil::SetMaze(std::vector<std::vector<char>> textMaze)
{
   mMaze = textMaze;
   mHeight = mMaze[0].size();
   mWidth = mMaze.size();
}

MovementState MazeUtil::GetDirectionDecision(float targetMinusSelfX,
                                             float targetMinusSelfY,
                                             int intersectionI, 
                                             int intersectionJ,
                                             MovementState currentState,
                                             unsigned int bestChoiceChancePercent) const
{
   assert(currentState != MovementState::STUCK);

   MovementState mostDesiredDirection;
   MovementState secondMostDesiredDirection;
   if (abs(targetMinusSelfX) > abs(targetMinusSelfY))
   {
      if (targetMinusSelfX < 0.0f)
         mostDesiredDirection = MovementState::LEFT;
      else
         mostDesiredDirection = MovementState::RIGHT;

      if (targetMinusSelfY < 0.0f)
         secondMostDesiredDirection = MovementState::UP;
      else
         secondMostDesiredDirection = MovementState::DOWN;
   }
   else
   {
      if (targetMinusSelfY < 0.0f)
         mostDesiredDirection = MovementState::UP;
      else
         mostDesiredDirection = MovementState::DOWN;

      if (targetMinusSelfX < 0.0f)
         secondMostDesiredDirection = MovementState::LEFT;
      else
         secondMostDesiredDirection = MovementState::RIGHT;
   }

   std::vector<std::pair<MovementState, float>> prioritizedOptions;
   float random = Util::RandomFloat(0.0f, 1.0f);
   float bestChoiceThreshold = bestChoiceChancePercent / 100.0f;

   if (currentState != !mostDesiredDirection && DirectionAvailable(mostDesiredDirection, intersectionI, intersectionJ))
   {
      std::pair<MovementState, float> pair;  
      pair.first = mostDesiredDirection;
      pair.second = 1.0f;
      prioritizedOptions.push_back(pair);
   }
   if (currentState != !secondMostDesiredDirection && DirectionAvailable(secondMostDesiredDirection, intersectionI, intersectionJ))
   {
      std::pair<MovementState, float> pair;
      pair.first = secondMostDesiredDirection;
      pair.second = 1.0f;
      prioritizedOptions.push_back(pair);
   }
   if (currentState != secondMostDesiredDirection && DirectionAvailable(!secondMostDesiredDirection, intersectionI, intersectionJ))
   {
      std::pair<MovementState, float> pair;
      pair.first = !secondMostDesiredDirection;
      pair.second = 1.0f;
      prioritizedOptions.push_back(pair);
   }
   if (currentState != mostDesiredDirection && DirectionAvailable(!mostDesiredDirection, intersectionI, intersectionJ))
   {
      std::pair<MovementState, float> pair;
      pair.first = !mostDesiredDirection;
      pair.second = 1.0f;
      prioritizedOptions.push_back(pair);
   }

   if (prioritizedOptions.size() > 1)
   {
      prioritizedOptions.at(0).second = bestChoiceThreshold;
   }

   if (prioritizedOptions.size() > 2)
   {
      prioritizedOptions.at(1).second = bestChoiceThreshold + (bestChoiceThreshold * (prioritizedOptions.size() - 1));
   }

   MovementState selectedDirection;
   bool done = false;
   for (int i = 0; !done && i < prioritizedOptions.size(); i++)
   {
      if (random < prioritizedOptions.at(i).second)
      {
         selectedDirection = prioritizedOptions.at(i).first;
         done = true;
      }
   }

   return selectedDirection;
}

void MazeUtil::GetXAndYFromIAndJ(int mazeIntersectionI, int mazeIntersectionJ, float& x, float& y) const
{
   x = mStartingX + mDistanceBetweenIntersections * mazeIntersectionI;
   y = mStartingY + mDistanceBetweenIntersections * mazeIntersectionJ;
}

void MazeUtil::InitializeGridVariables(MovementState direction, 
                                       float& x, 
                                       float& y, 
                                       float& distanceToNextIntersection, 
                                       int& mazeIntersectionI, 
                                       int& mazeIntersectionJ) const
{
   if (direction == MovementState::LEFT || direction == MovementState::RIGHT)
   {
      SnapYToIntersection(y, mazeIntersectionJ);
      if (direction == MovementState::LEFT)
      {
         mazeIntersectionI = ConvertXToI(x + mDistanceBetweenIntersections / 2.0f);
         distanceToNextIntersection = x - ConvertIToX(mazeIntersectionI - 1);
      }
      else
      {
         mazeIntersectionI = ConvertXToI(x - mDistanceBetweenIntersections / 2.0f);
         distanceToNextIntersection = ConvertIToX(mazeIntersectionI + 1) - x;
      }
   }
   else if (direction == MovementState::UP || direction == MovementState::DOWN)
   {
      SnapXToIntersection(x, mazeIntersectionI);
      if (direction == MovementState::UP)
      {
         mazeIntersectionJ = ConvertYToJ(y + mDistanceBetweenIntersections / 2.0f);
         distanceToNextIntersection = y - ConvertJToY(mazeIntersectionJ - 1);
      }
      else
      {
         mazeIntersectionJ = ConvertYToJ(y - mDistanceBetweenIntersections / 2.0f);
         distanceToNextIntersection = ConvertJToY(mazeIntersectionJ + 1) - y;
      }
   }
}

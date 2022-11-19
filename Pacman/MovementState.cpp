#include "stdafx.h"
#include "MovementState.h"

MovementState::MovementState()
   : mState(LEFT)
{
}

MovementState::MovementState(const State& state)
{
   mState = state;
}

bool MovementState::operator==(const MovementState& state)
{
   return mState == state.mState;
}

bool MovementState::operator!=(const MovementState& state)
{
   return mState != state.mState;
}

bool MovementState::operator==(const State& state)
{
   return mState == state;
}

bool MovementState::operator!=(const State& state)
{
   return mState != state;
}

void MovementState::operator=(const State& state)
{
   mState = state;
}

const MovementState& MovementState::operator!()
{
   MovementState notState;
   if (mState == LEFT)
   {
      notState = RIGHT;
   }
   else if (mState == RIGHT)
   {
      notState = LEFT;
   }
   else if (mState == UP)
   {
      notState = DOWN;
   }
   else if (mState == DOWN)
   {
      notState = UP;
   }
   return notState;
}
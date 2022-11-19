#pragma once

class MovementState
{
public:
   enum State
   {
      LEFT,
      RIGHT,
      UP,
      DOWN,
      STUCK
   };

   MovementState();
   MovementState(const State& state);

   bool operator==(const MovementState& state);
   bool operator!=(const MovementState& state);
   bool operator==(const State& state);
   bool operator!=(const State& state);
   void operator=(const State& state);
   const MovementState& operator!();
private:
   State mState;
};
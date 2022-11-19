#pragma once

#include "Character.h"
#include "Player.h"
#include "MazeUtil.h"
#include "MazeTraverser.h"

class Ghost : public Character, public MazeTraverser
{
public:
   static unsigned int mGhostValue;

   enum Color
   {
      BLUE,
      ORANGE,
      PINK,
      RED
   };

   enum State
   {
      RETURNING,
      IDLING,
      RUNNING,
      CHASING,
      LEAVING
   };

   Ghost();
   virtual ~Ghost();
   virtual void Process(float deltaTime);
   virtual void Kill();
   void SetIdlePositionAndLimits(float x, float minY, float maxY);
   void SetExitPosition(float x, float y);
   void SetSpeedScale(float scale) { mSpeedScale = scale; }
   void SetColor(Color c);
   Color GetColor() { return mColor; }
   void SetMazeUtil(const MazeUtil& mazeUtil) { mpMazeUtil = &mazeUtil; }
   void SetState(State state) 
   { 
      mpMazeUtil->InitializeGridVariables(mMovementState, mX, mY, mDistanceToNextIntersection, mMazeIntersectionI, mMazeIntersectionJ);
      mState = state; 
   }
   State GetState() const { return mState; }
   void SetTarget(Player* target) { mpTarget = target; }
   void SetIdleTimeSeconds(float seconds) { mIdleTimeSeconds = seconds; }
   void SetPause(bool pause) { mPause = pause; }
   void SetDeathOccurred(bool deathOccurred) { mDeathOccurred = deathOccurred; }
   bool GetDeathOccurred() { return mDeathOccurred; }
   void SetPlayerKilled(bool playerKilled) { mPlayerKilled = playerKilled; }
   bool GetPlayerKilled() { return mPlayerKilled; }
   void Reset();

   void SetSpeed();
private:
   MovementState mMovementState;
   State mState;
   Color mColor;
   float mIdleX;
   float mIdleMinY;
   float mIdleMaxY;
   float mExitX;
   float mExitY;
   float mSpeed;
   float mChaseSpeed;
   const float mSlowSpeed = 105.0f;   
   const float mTunnelSpeed = 105.0f;
   const float mExitSpeed = 105.0f;
   const float mReturnSpeed = 367.5f;
   float mSpeedScale;
   bool mIdlingUpward;
   bool mMovedToCenter;
   bool mTunnelDebuff;
   float mTimeAccumulator;
   float mIdleTimeAccumulator;
   float mIdleTimeSeconds;
   TextureManager::Textures mGhostFrames[2];
   const TextureManager::Textures mVulnerableGhostFrames[4] = { TextureManager::vulnerableGhost1, TextureManager::vulnerableGhost2, TextureManager::vulnerableGhostBlink1, TextureManager::vulnerableGhostBlink2 };
   unsigned int mFrameIndex;
   Player* mpTarget;
   MazeUtil const* mpMazeUtil;
   bool mPause;
   bool mDeathOccurred;
   bool mPlayerKilled;
};

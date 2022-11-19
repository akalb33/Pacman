#pragma once

#include "Character.h"
#include "MazeUtil.h"
#include "MazeTraverser.h"

class Player : public Character, public MazeTraverser
{
public:
   Player();
   virtual ~Player();
   virtual void Kill();
   virtual void Process(float deltaTime);
   float GetTimeSincePowerUp() { return mPowerUpTimeAccumulator; }
   float GetPowerUpDuration() { return mPowerUpDuration; }
   bool GetJustPoweredUpFlag() { return mJustPoweredUpFlag; }
   void SetJustPoweredUpFlag(bool flag) { mJustPoweredUpFlag = flag; }
   void SetPoweredUp(bool poweredUp);
   void SetSpeed(float speed) { mSpeed = speed; }
   void SetSpeedScale(float scale) { mSpeedScale = scale; }
   void SetMazeUtil(const MazeUtil& mazeUtil) { mpMazeUtil = &mazeUtil; }
   void Initialize(MovementState state, float x, float y);
   void SetPause(bool pause) { mPause = pause; }

private:
   float mSpeed;
   float mSpeedScale;
   int mPacmanFrameIndex;
   float mTimeAccumulator;
   float mPowerUpTimeAccumulator;
   float mPowerUpDuration = 10.0f;
   bool mPoweredUp;
   bool mJustPoweredUpFlag;
   const TextureManager::Textures mPacmanFrames[3] = { TextureManager::pacman1, TextureManager::pacman2, TextureManager::pacman3 };
   MovementState mState;
   MovementState mNextState;
   bool mAtIntersection;
   MazeUtil const* mpMazeUtil;
   bool mPause;
   float mDeathTimeAccumulator;
   unsigned int mRotateCounter;
};
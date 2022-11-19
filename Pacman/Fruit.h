#pragma once

#include "Character.h"
#include "MazeUtil.h"
#include "MazeTraverser.h"

class Fruit : public Character, public MazeTraverser
{
public:
   enum Type
   {
      CHERRY,
      STRAWBERRY,
      ORANGE,
      PRETZEL,
      APPLE,
      PEAR,
      BANANA
   };

   enum State
   {
      ENTERING,
      LOITERING,
      LEAVING
   };

   Fruit();
   virtual ~Fruit();
   virtual void Process(float deltaTime);
   virtual void Kill();
   void SetMazeUtil(const MazeUtil& mazeUtil) { mpMazeUtil = &mazeUtil; }
   void SetType(Type type);
   Type GetType() { return mType; }
   void SetSpeedScale(float scale) { mSpeedScale = scale; }
   bool GetFruitEaten() { return mFruitEaten; }        
   void SetFruitEaten(bool eaten) { mFruitEaten = eaten; }
   void Reset();
   void SetTunnelPosition(bool entrance_nExit, int i, int j, MovementState direction);
   void SetMovementState(MovementState direction) { mMovementState = direction; }
   void SetPause(bool pause) 
   { 
      mPause = pause; 
      if (pause)
      {
         Character::mpSoundManager->Pause(SoundManager::fruitBounce);
      }
      else
      {
         Character::mpSoundManager->UnPause(SoundManager::fruitBounce);
      }
   }

private:
   MazeUtil const* mpMazeUtil;
   State mState;
   MovementState mMovementState;
   Type mType;
   float mSpeed;
   float mSpeedScale;
   float mTimeAccumulator;
   float mAnimationTimeAccumulator;
   TextureManager::Textures mFruitFrames[2];
   unsigned int mFrameIndex;
   bool mSpawned;
   bool mPause;
   bool mFruitEaten;
   unsigned int mPointValue;     
   float mEntranceTunnelX;
   float mEntranceTunnelY;
   float mExitTunnelX;
   float mExitTunnelY;
   MovementState mTunnelDirection;
};

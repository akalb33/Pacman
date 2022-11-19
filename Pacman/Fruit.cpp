#include "stdafx.h"
#include "Fruit.h"
#include "Util.h"
#include "Player.h"
#include "ScoreManager.h"
#include "ScoreSign.h"
#include <assert.h>

Fruit::Fruit()
   : Character(typeid(Fruit))
   , mpMazeUtil(nullptr)
   , mState(ENTERING)
   , mMovementState(MovementState::LEFT)
   , mType(CHERRY)
   , mSpeed(100.0f)
   , mSpeedScale()
   , mTimeAccumulator(0.0f)
   , mAnimationTimeAccumulator(0.0f)
   , mFruitFrames()
   , mFrameIndex(0)
   , mSpawned(true)
   , mPause(false)
   , mPointValue(0)
   , mEntranceTunnelX(0.0f)
   , mEntranceTunnelY(0.0f)
   , mExitTunnelX(0.0f)
   , mExitTunnelY(0.0f)
   , mTunnelDirection(MovementState::LEFT)
{
   SetDisplayOrder(3);
   Reset();
   SetType(CHERRY);
   Util::InitRandomizer();
}

Fruit::~Fruit()
{
}

void Fruit::Process(float deltaTime)
{
   if (mSpawned)
   {
      Character::mpSoundManager->Play(SoundManager::fruitBounce);
      mSpawned = false;
   }
   
   if (!mPause)
   {
      mTimeAccumulator += deltaTime;
      mAnimationTimeAccumulator += deltaTime;
      if (mTimeAccumulator > 15.0f)
      {
         mState = LEAVING;
      }
      else if (mTimeAccumulator > 5.0f)
      {
         mState = LOITERING;
      }

      std::vector<Player*> pPlayer;
      float radius = (GetMaxX() - GetMinX()) / 2.0f;
      if (DetectWithinRadius(radius, pPlayer, true))
      {
         Character::mpSoundManager->Stop(SoundManager::fruitBounce);
         SetStatus(StatusManager::dead);
         mTimeAccumulator = 0.0f;
         ScoreSign* pScoreSign = Character::Resurrect<ScoreSign>();
         assert(pScoreSign != 0);
         pScoreSign->SetValue(100);
         pScoreSign->mX = mX;
         pScoreSign->mY = mY;
         pScoreSign->SetDisplayOrder(2);
         pScoreSign->mScale = 0.5f;
         ScoreManager::AddPoints(mPointValue);
         mFruitEaten = true;
         Character::mpSoundManager->Play(SoundManager::fruit);
      }

      if (mAnimationTimeAccumulator > 0.25f)
      {
         mAnimationTimeAccumulator = 0.0f;
         if (mFrameIndex == 2)
         {
            mFrameIndex -= 2;
         }
         SetTexture(mFruitFrames[mFrameIndex]);
         mFrameIndex++;
      }

      float pixelsToMove = deltaTime * mSpeed * mSpeedScale;
      std::vector<MovementState> options;


      mDistanceToNextIntersection -= pixelsToMove;
      if (mDistanceToNextIntersection < 0.0f)
      {
         pixelsToMove = -mDistanceToNextIntersection;
         mDistanceToNextIntersection = mpMazeUtil->GetDistanceBetweenIntersections();
         if (mpMazeUtil->SnapToIntersection(mX, mY, mMazeIntersectionI, mMazeIntersectionJ))
         {
            Kill();
         }
         else
         {
            float destX = 960.0f;
            float destY = mEntranceTunnelY;
            if (mState == LOITERING)
            {
               destY = 540.0f;
            }
            else if (mState == LEAVING)
            {
               destX = mExitTunnelX;
               destY = mExitTunnelY;
            }
            mMovementState = mpMazeUtil->GetDirectionDecision(destX - mX,
                                                               destY - mY,
                                                               mMazeIntersectionI,
                                                               mMazeIntersectionJ,
                                                               mMovementState,
                                                               100);
         }
      }
      else
      {
         if (mMovementState == MovementState::UP)
            mY -= pixelsToMove;
         else if (mMovementState == MovementState::DOWN)
            mY += pixelsToMove;
         else if (mMovementState == MovementState::LEFT)
            mX -= pixelsToMove;
         else if (mMovementState == MovementState::RIGHT)
            mX += pixelsToMove;
      }
   }
}


void Fruit::Kill()
{
   Character::mpSoundManager->Stop(SoundManager::fruitBounce);
   SetStatus(StatusManager::dead);
   mSpawned = true;
}


void Fruit::SetType(Type type)
{
   mType = type;
   switch (type)
   {
   case CHERRY:
      mFruitFrames[0] = TextureManager::cherry;
      mFruitFrames[1] = TextureManager::cherry2;
      mPointValue = 100;
      break;
   case STRAWBERRY:
      mFruitFrames[0] = TextureManager::strawberry;
      mFruitFrames[1] = TextureManager::strawberry2;
      mPointValue = 200;
      break;
   case ORANGE:
      SetTexture(TextureManager::orange);
      mPointValue = 500;
      break;
   case PRETZEL:
      SetTexture(TextureManager::pretzel);
      mPointValue = 700;
      break;
   case APPLE:
      SetTexture(TextureManager::apple);
      mPointValue = 1000;
      break;
   case PEAR:
      SetTexture(TextureManager::pear);
      mPointValue = 2000;
      break;
   case BANANA:
      SetTexture(TextureManager::banana);
      mPointValue = 5000;
      break;
   }
}

void Fruit::Reset()
{
   mState = ENTERING;
   mTimeAccumulator = 0.0f;
   mAnimationTimeAccumulator = 0.0f;
   mPause = false;
   SetDisplayOrder(3);
   mDistanceToNextIntersection = 0.0f;
   mX = mEntranceTunnelX;
   mY = mEntranceTunnelY;
}

void Fruit::SetTunnelPosition(bool entrance_nExit, int i, int j, MovementState direction)
{
   if (entrance_nExit)
   {
      mpMazeUtil->GetXAndYFromIAndJ(i, j, mEntranceTunnelX, mEntranceTunnelY);
      mTunnelDirection = direction;
   }
   else
   {
      mpMazeUtil->GetXAndYFromIAndJ(i, j, mExitTunnelX, mExitTunnelY);
   }
}
#include "stdafx.h"
#include "Ghost.h"
#include "Util.h"
#include "Boundary.h"
#include "GhostBox.h"

unsigned int Ghost::mGhostValue = 200;


Ghost::Ghost()
   : Character(typeid(Ghost))
   , mMovementState(MovementState::LEFT)
   , mState(Ghost::IDLING)
   , mColor(Ghost::BLUE)
   , mIdleX(0.0f)
   , mIdleMinY(0.0f)
   , mIdleMaxY(0.0f)
   , mExitX(0.0f)
   , mExitY(0.0f)
   , mSpeed(0.0f)
   , mChaseSpeed(210.0f)
   , mSpeedScale(1.0f)
   , mIdlingUpward(true)
   , mMovedToCenter(false)
   , mTunnelDebuff(false)
   , mTimeAccumulator(0.0f)
   , mIdleTimeAccumulator(0.0f)
   , mIdleTimeSeconds(0.0f)
   , mGhostFrames()
   , mFrameIndex(0)
   , mpTarget(nullptr)
   , mpMazeUtil(nullptr)
   , mPause(false)
   , mDeathOccurred(false)
{
   SetTexture(TextureManager::blueGhost1);
   SetDisplayOrder(5);
   SetColor(BLUE);
   Util::InitRandomizer();
   mSpeed = mChaseSpeed;
}

Ghost::~Ghost()
{
}

void Ghost::Process(float deltaTime)
{
   if (!Paused(deltaTime))
   {
      SetSpeed();
      mTimeAccumulator += deltaTime;          // DONT DO IF PAUSE? TODO
      mIdleTimeAccumulator += deltaTime;
      if (mState == RUNNING)
      {
         if (!mPause)
         {
            if (mpTarget->GetTimeSincePowerUp() > mpTarget->GetPowerUpDuration())
            {
               mState = CHASING;
               SetColor(mColor);
               SetDisplayOrder(5);
               mFrameIndex = mFrameIndex % 2;
            }
            else
            {
               std::vector<Player*> pPlayer;
               float radius = (GetMaxX() - GetMinX()) / 2.0f;
               if (DetectWithinRadius(radius, pPlayer, true))
               {
                  mTunnelDebuff = false;
                  mDeathOccurred = true;
                  mState = RETURNING;
               }
               else
               {
                  if (mpTarget->GetTimeSincePowerUp() > (0.75f * mpTarget->GetPowerUpDuration()))
                  {
                     if (mTimeAccumulator > 0.07f)
                     {
                        mTimeAccumulator = 0.0f;
                        if (mFrameIndex >= 4)
                        {
                           mFrameIndex = 0;
                        }
                        SetTexture(mVulnerableGhostFrames[mFrameIndex]);
                        mFrameIndex++;
                     }
                  }
                  else
                  {
                     if (mTimeAccumulator > 0.15f)
                     {
                        mTimeAccumulator = 0.0f;
                        if (mFrameIndex >= 2)
                        {
                           mFrameIndex = 0;
                        }
                        SetTexture(mVulnerableGhostFrames[mFrameIndex]);
                        mFrameIndex++;
                     }
                  }
               }

               float pixelsToMove = deltaTime * mSpeed * mSpeedScale;
               std::vector<MovementState> options;

               mDistanceToNextIntersection -= pixelsToMove;
               if (mDistanceToNextIntersection < 0.0f)
               {
                  pixelsToMove = -mDistanceToNextIntersection;
                  mDistanceToNextIntersection = mpMazeUtil->GetDistanceBetweenIntersections();
                  (void)mpMazeUtil->SnapToIntersection(mX, mY, mMazeIntersectionI, mMazeIntersectionJ);

                  mMovementState = mpMazeUtil->GetDirectionDecision(mX - mpTarget->mX,
                                                                     mY - mpTarget->mY,
                                                                     mMazeIntersectionI,
                                                                     mMazeIntersectionJ,
                                                                     mMovementState,
                                                                     60);
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
      }
      else if (mState == IDLING)
      {
         if (!mPause)
         {
            if (mTimeAccumulator > 0.15f)
            {
               mTimeAccumulator = 0.0f;
               if (mFrameIndex >= 2)
               {
                  mFrameIndex = 0;
               }
               SetTexture(mGhostFrames[mFrameIndex]);
               mFrameIndex++;
            }

            if (mIdleTimeAccumulator > mIdleTimeSeconds)
            {
               if (MoveTowardLocation(mIdleX, ((mIdleMaxY + mIdleMinY) / 2.0f), mSpeed * mSpeedScale, deltaTime))
               {
                  mState = LEAVING;
               }
            }
            else
            {
               if (mIdlingUpward)
               {
                  if (MoveTowardLocation(mIdleX, mIdleMinY, mSpeed * mSpeedScale, deltaTime))
                  {
                     mIdlingUpward = false;
                  }
               }
               else
               {
                  if (MoveTowardLocation(mIdleX, mIdleMaxY, mSpeed * mSpeedScale, deltaTime))
                  {
                     mIdlingUpward = true;
                  }
               }
            }
         }
      }
      else if (mState == CHASING)
      {
         if (!mPause)
         {
            if (mpTarget->GetJustPoweredUpFlag())
            {
               mGhostValue = 200;
               mMovementState = !mMovementState;
               mDistanceToNextIntersection = mpMazeUtil->GetDistanceBetweenIntersections() - mDistanceToNextIntersection;
               SetTexture(TextureManager::vulnerableGhost1);
               mState = RUNNING;
               SetDisplayOrder(3);
            }
            else
            {
               std::vector<Player*> pPlayer;
               float radius = (GetMaxX() - GetMinX()) / 2.0f;
               if (DetectWithinRadius(radius, pPlayer, true))
               {
                  mPlayerKilled = true;
               }


               if (mTimeAccumulator > 0.15f)
               {
                  mTimeAccumulator = 0.0f;
                  if (mFrameIndex >= 2)
                  {
                     mFrameIndex = 0;
                  }
                  SetTexture(mGhostFrames[mFrameIndex]);
                  mFrameIndex++;
               }

               float pixelsToMove = deltaTime * mSpeed * mSpeedScale;
               std::vector<MovementState> options;


               mDistanceToNextIntersection -= pixelsToMove;
               if (mDistanceToNextIntersection < 0.0f)
               {
                  pixelsToMove = -mDistanceToNextIntersection;
                  mDistanceToNextIntersection = mpMazeUtil->GetDistanceBetweenIntersections();
                  (void)mpMazeUtil->SnapToIntersection(mX, mY, mMazeIntersectionI, mMazeIntersectionJ);

                  MovementState previousMovementState = mMovementState;
                  mMovementState = mpMazeUtil->GetDirectionDecision(mpTarget->mX - mX,
                                                                    mpTarget->mY - mY,
                                                                    mMazeIntersectionI,
                                                                    mMazeIntersectionJ,
                                                                    mMovementState,
                                                                    85);
                  if (mMovementState != previousMovementState)
                  {
                     Pause(0.02f);
                  }

                  mTunnelDebuff = mpMazeUtil->InTunnel(mMovementState, mMazeIntersectionI, mMazeIntersectionJ);
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
      }
      else if (mState == RETURNING)
      {
         // 1 second global pause
         // score increment

         if (!mPause)
         {
            if (mDeathOccurred)
            {
               SetTexture(TextureManager::eyes);
               mDeathOccurred = false;
            }
         }

         if (!mDeathOccurred)
         {
            float pixelsToMove = deltaTime * mSpeed * mSpeedScale;

            if (mMovedToCenter || (mX >= mExitX - pixelsToMove && mX <= mExitX + pixelsToMove && mY >= mExitY - pixelsToMove && mY <= mExitY + pixelsToMove))
            {
               mMovedToCenter = true;
               if (MoveTowardLocation(mX, ((mIdleMaxY + mIdleMinY) / 2.0f), mSpeed * mSpeedScale, deltaTime))
               {
                  mState = LEAVING;
                  mFrameIndex = 0;
               }
            }
            else
            {
               mDistanceToNextIntersection -= pixelsToMove;
               if (mDistanceToNextIntersection < 0.0f)
               {
                  pixelsToMove = -mDistanceToNextIntersection;
                  mDistanceToNextIntersection = mpMazeUtil->GetDistanceBetweenIntersections();
                  (void)mpMazeUtil->SnapToIntersection(mX, mY, mMazeIntersectionI, mMazeIntersectionJ);


                  mMovementState = mpMazeUtil->GetDirectionDecision(mExitX - mX,
                                                                     mExitY - mY,
                                                                     mMazeIntersectionI,
                                                                     mMazeIntersectionJ,
                                                                     mMovementState,
                                                                     100);
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
      }
      else if (mState == LEAVING)
      {
         if (!mPause)
         {
            if (mTimeAccumulator > 0.15f)
            {
               mTimeAccumulator = 0.0f;
               if (mFrameIndex >= 2)
               {
                  mFrameIndex = 0;
               }
               SetTexture(mGhostFrames[mFrameIndex]);
               mFrameIndex++;
            }

            if (!mMovedToCenter)
            {
               if (MoveTowardLocation(mExitX, mY, mSpeed * mSpeedScale, deltaTime))
               {
                  mMovedToCenter = true;
               }
            }
            else
            {
               if (MoveTowardLocation(mX, mExitY, mSpeed * mSpeedScale, deltaTime))
               {
                  mState = CHASING;
                  mpMazeUtil->InitializeGridVariables(mMovementState, mX, mY, mDistanceToNextIntersection, mMazeIntersectionI, mMazeIntersectionJ);
                  mMovedToCenter = false;
               }
            }
         }
      }
   }
}


void Ghost::Kill()
{
}

void Ghost::SetIdlePositionAndLimits(float x, float minY, float maxY)
{
   mIdleX = x;
   mIdleMinY = minY;
   mIdleMaxY = maxY;
}

void Ghost::SetExitPosition(float x, float y)
{
   mExitX = x;
   mExitY = y;
}

void Ghost::SetColor(Color c)
{
   mColor = c;
   switch (c)
   {
      case BLUE:
         mGhostFrames[0] = TextureManager::blueGhost1;
         mGhostFrames[1] = TextureManager::blueGhost2;
         break;
      case ORANGE:
         mGhostFrames[0] = TextureManager::orangeGhost1;
         mGhostFrames[1] = TextureManager::orangeGhost2;
         break;
      case PINK:
         mGhostFrames[0] = TextureManager::pinkGhost1;
         mGhostFrames[1] = TextureManager::pinkGhost2;
         break;
      case RED:
         mGhostFrames[0] = TextureManager::redGhost1;
         mGhostFrames[1] = TextureManager::redGhost2;
   }
   SetTexture(mGhostFrames[0]);
}

void Ghost::Reset()
{
   mMovementState = MovementState::LEFT;
   mState = Ghost::IDLING;
   mIdlingUpward = true;
   mMovedToCenter = false;
   mTunnelDebuff = false;
   mTimeAccumulator = 0.0f;
   mIdleTimeAccumulator = 0.0f;
   mDistanceToNextIntersection = 0.0f;
   mMazeIntersectionI = 0;
   mMazeIntersectionJ = 0;
   mPause = false;
   mDeathOccurred = false;
   SetDisplayOrder(5);
}

void Ghost::SetSpeed()
{
   if (mTunnelDebuff)
   {
      mSpeed = mTunnelSpeed;
   }
   else
   {
      switch (mState)
      {
      case CHASING:
         mSpeed = mChaseSpeed;
         break;
      case RUNNING:
         mSpeed = mSlowSpeed;
         break;
      case LEAVING:
         mSpeed = mExitSpeed;
         break;
      case RETURNING:
         mSpeed = mReturnSpeed;
         break;
      case IDLING:
         mSpeed = mChaseSpeed;
         break;
      }
   }
}
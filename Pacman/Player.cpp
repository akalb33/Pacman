#include "stdafx.h"
#include "Player.h"
#include "Character.h"
#include "Boundary.h"
#include "Util.h"

#include "Point.h"
#include "GhostBox.h"
#include "ScoreManager.h"

Player::Player()
   : Character(typeid(Player))
   , mSpeed(200.0f)
   , mSpeedScale(1.0f)
   , mPacmanFrameIndex(0)
   , mTimeAccumulator(0.0f)
   , mPowerUpTimeAccumulator(20.0f)
   , mPoweredUp(false)
   , mJustPoweredUpFlag(false)
   , mState(MovementState::LEFT)
   , mNextState(MovementState::LEFT)
   , mAtIntersection(false)
   , mpMazeUtil(nullptr)
   , mPause(false)
   , mDeathTimeAccumulator(0.0f)
   , mRotateCounter(0)
{

   SetTexture(TextureManager::pacman1);
   SetDisplayOrder(3);
}

Player::~Player()
{

}

void Player::Kill()
{
   SetStatus(StatusManager::dying);
   SetTexture(mPacmanFrames[2]);
   Character::mpSoundManager->Play(SoundManager::death);
}

void Player::Process(float deltaTime)
{
   if (!Paused(deltaTime))
   {
      mTimeAccumulator += deltaTime;
      mPowerUpTimeAccumulator += deltaTime;
      if (GetStatus() != StatusManager::dying)
      {
         if (!mPause)
         {
            if (mPoweredUp)
            {
               mJustPoweredUpFlag = false;
               if (mPowerUpTimeAccumulator > mPowerUpDuration)
               {
                  SetPoweredUp(false);
               }
            }

            std::vector<Point*> pPoints;
            float radius = (mPolygon.GetAdjustedMaxX() - mPolygon.GetAdjustedMinX()) / 2.0f;
            if (DetectWithinRadius(radius, pPoints, true))
            {
               for (std::vector<Point*>::iterator it = pPoints.begin(); it != pPoints.end(); it++)
               {
                  if ((*it)->IsPowerPoint())
                  {
                     // power up sound?
                     SetPoweredUp(true);
                     mJustPoweredUpFlag = true;
                     ScoreManager::AddPoints(50);
                  }
                  else
                  {
                     Character::mpSoundManager->Play(SoundManager::dot);
                     ScoreManager::AddPoints(10);
                  }
                  (*it)->Kill();
               }
            }

            if (mState != MovementState::STUCK)
            {
               if (mTimeAccumulator > 0.05f)
               {
                  mTimeAccumulator = 0.0f;
                  if (mPacmanFrameIndex == 3)
                  {
                     mPacmanFrameIndex -= 3;
                  }
                  SetTexture(mPacmanFrames[mPacmanFrameIndex]);
                  mPacmanFrameIndex++;
               }
            }

            float pixelsToMove = deltaTime * mSpeed * mSpeedScale;

            // mMazeIntersectionI=0, mMazeIntersectionJ=0 is upper left corner of maze[0][0] which corresponds to mazeStartX, mazeStartY

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -50.0f)
            {
               mNextState = MovementState::UP;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 50.0f)
            {
               mNextState = MovementState::DOWN;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -50.0f)
            {
               mNextState = MovementState::LEFT;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 50.0f)
            {
               mNextState = MovementState::RIGHT;
            }


            if ((mNextState == MovementState::UP && mState == MovementState::DOWN) ||
               (mNextState == MovementState::DOWN && mState == MovementState::UP) ||
               (mNextState == MovementState::LEFT && mState == MovementState::RIGHT) ||
               (mNextState == MovementState::RIGHT && mState == MovementState::LEFT))
            {
               mState = mNextState;
               mDistanceToNextIntersection = mpMazeUtil->GetDistanceBetweenIntersections() - mDistanceToNextIntersection;
            }

            bool done = false;
            do
            {
               if (!mAtIntersection)
               {
                  mDistanceToNextIntersection -= pixelsToMove;
                  if (mDistanceToNextIntersection < 0.0f)
                  {
                     pixelsToMove = -mDistanceToNextIntersection;
                     mDistanceToNextIntersection = mpMazeUtil->GetDistanceBetweenIntersections();
                     (void)mpMazeUtil->SnapToIntersection(mX, mY, mMazeIntersectionI, mMazeIntersectionJ);
                     mAtIntersection = true;
                  }
                  else
                  {
                     if (mState == MovementState::UP)
                     {
                        mAngle = 90.0f;
                        mY -= pixelsToMove;
                     }
                     else if (mState == MovementState::DOWN)
                     {
                        mAngle = 270.0f;
                        mY += pixelsToMove;
                     }
                     else if (mState == MovementState::LEFT)
                     {
                        mAngle = 0.0f;
                        mX -= pixelsToMove;
                     }
                     else
                     {
                        mAngle = 180.0f;
                        mX += pixelsToMove;
                     }
                     done = true;
                  }
               }
               else
               {
                  if (mpMazeUtil->DirectionAvailable(mNextState, mMazeIntersectionI, mMazeIntersectionJ))
                  {
                     mState = mNextState;
                     mAtIntersection = false;
                  }
                  else if (mState != MovementState::STUCK)
                  {
                     if ((mState != mNextState) && mpMazeUtil->DirectionAvailable(mState, mMazeIntersectionI, mMazeIntersectionJ))
                     {
                        mAtIntersection = false;
                     }
                     else
                     {
                        mState = MovementState::STUCK;
                        done = true;
                     }
                  }
                  else
                  {
                     done = true;
                  }
               }
            } while (!done);
         }
      }
      else
      {
         if (mRotateCounter > 7)
         {
            SetStatus(StatusManager::dead);
         }
         else
         {
            mDeathTimeAccumulator += deltaTime;
            if (mDeathTimeAccumulator > 0.15f)
            {
               mRotateCounter++;
               mAngle -= 90.0f;
               mDeathTimeAccumulator = 0.0f;
            }
         }
      }
   }
}

void Player::SetPoweredUp(bool poweredUp) 
{ 
   mPoweredUp = poweredUp;
   if (mPoweredUp)
   {
      mPowerUpTimeAccumulator = 0.0f;
   }
}

void Player::Initialize(MovementState state, float x, float y)
{
   mRotateCounter = 0;
   mTimeAccumulator = 0.0f;
   mPowerUpTimeAccumulator = 20.0f;
   mAngle = 0.0f;
   mSpeed = 200.0f;
   mPacmanFrameIndex = 1;
   SetTexture(mPacmanFrames[1]);
   mState = state;
   mNextState = state;
   mX = x;
   mY = y;
   mpMazeUtil->InitializeGridVariables(state, mX, mY, mDistanceToNextIntersection, mMazeIntersectionI, mMazeIntersectionJ);
   mAtIntersection = false;
}
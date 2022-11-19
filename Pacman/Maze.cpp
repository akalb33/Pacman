#include "stdafx.h"
#include "Boundary.h"
#include "Point.h"
#include "TunnelCover.h"
#include "Maze.h"
#include <cassert>
#include "GhostBox.h"
#include "Fruit.h"
#include "ScoreManager.h"

#include "Screen.h"
#include <iostream>

Maze::Maze()
   : mMaze()
   , mWidth(0)
   , mHeight(0)
   , mMazeUtil()
   , mpGhosts()
   , mGhostSpawnPoints()
   , mpPlayer(nullptr)
   , mpScoreSign(nullptr)
   , mPauseRemaining(0.0f)
   , mPlayerLivesManager()
   , mPlayerSpawnX(0.0f)
   , mPlayerSpawnY(0.0f)
   , mFruitSpawned(false)
   , mScale(0.0f)
   , mState(INITIALIZATION)

{
   Util::InitRandomizer();
}

void Maze::SetMaze(std::vector<std::string> textMaze)
{
   mWidth = textMaze.at(0).length();
   mHeight = textMaze.size();

   mMaze.resize(mWidth);
   for (unsigned int i = 0; i < mWidth; i++)
   {
      mMaze[i].resize(mHeight);
   }

   for (unsigned int i = 0; i < mWidth; i++)
   {
      for (unsigned int j = 0; j < mHeight; j++)
      {
         mMaze[i][j] = textMaze[j].at(i);
      }
   }
}

void Maze::Display()
{
   bool ghostBoxPlaced = false;

   Character* pCharacter = Character::Resurrect<Boundary>();
   assert(pCharacter != 0);
   pCharacter->mScale = 1.0f;


   bool offsetY = false;
   float desiredSize = pCharacter->mpScreen->GetViewWidth() / mWidth;

   if ((pCharacter->mpScreen->GetViewWidth() / pCharacter->mpScreen->GetViewHeight()) > (mWidth / mHeight))
   {
      desiredSize = pCharacter->mpScreen->GetViewHeight() / mHeight;
      offsetY = true;
   }

   mScale = desiredSize / (pCharacter->GetMaxX() - pCharacter->GetMinX());

   float startingX = offsetY * (pCharacter->GetMaxX() - pCharacter->GetMinX()) / 2.0f + ((pCharacter->mpScreen->GetViewWidth() - desiredSize * mWidth) / 2.0f);
   float startingY = offsetY * (pCharacter->GetMaxX() - pCharacter->GetMinX()) / 2.0f + ((pCharacter->mpScreen->GetViewHeight() - desiredSize * mHeight) / 2.0f);

   mMazeUtil.SetUpperLeftStartingPositionX(startingX - (desiredSize / 2.0f));
   mMazeUtil.SetUpperLeftStartingPositionY(startingY - (desiredSize / 2.0f));
   mMazeUtil.SetDistanceBetweenIntersections(desiredSize);

   pCharacter->Kill();

   mpPlayer = Character::Resurrect<Player>();
   assert(mpPlayer != 0);

   for (unsigned int j = 0; j < mHeight; j++)
   {
      for (unsigned int i = 0; i < mWidth; i++)
      {
         char c = mMaze[i][j];
         if (c == '*')
         {
            Boundary* pBoundary = Character::Resurrect<Boundary>();
            assert(pBoundary != 0);
            pBoundary->Reset();
            pBoundary->mX = (startingX + desiredSize * i);
            pBoundary->mY = (startingY + desiredSize * j);
            pBoundary->mScale = mScale;

            unsigned int patternIndex = 0;
            if (j > 0)
            {
               if (mMaze[i][j - 1] == '*')
               {
                  patternIndex += 1;
               }
            }

            if (i < mWidth - 1)
            {
               if (mMaze[i + 1][j] == '*')
               {
                  patternIndex += 2;
               }
            }

            if (j < mHeight - 1)
            {
               if (mMaze[i][j + 1] == '*')
               {
                  patternIndex += 4;
               }
            }

            if (i > 0)
            {
               if (mMaze[i - 1][j] == '*')
               {
                  patternIndex += 8;
               }
            }

            pBoundary->SetTexture(mPattern[patternIndex]);
            
         }
         else
         {
            unsigned int patternIndex = 0;
            if (j > 0)
            {
               if (mMaze[i][j - 1] == '*')
               {
                  patternIndex += 1;
               }
            }

            if (i < mWidth - 1)
            {
               if (mMaze[i + 1][j] == '*')
               {
                  patternIndex += 2;
               }
            }

            if (j < mHeight - 1)
            {
               if (mMaze[i][j + 1] == '*')
               {
                  patternIndex += 4;
               }
            }

            if (i > 0)
            {
               if (mMaze[i - 1][j] == '*')
               {
                  patternIndex += 8;
               }
            }

            if (patternIndex == 3)
            {
               Boundary* pBoundary = Character::Resurrect<Boundary>();
               assert(pBoundary != 0);
               pBoundary->Reset();
               pBoundary->mX = (startingX + desiredSize * i);
               pBoundary->mY = (startingY + desiredSize * j);
               pBoundary->mScale = mScale;
               pBoundary->SetTexture(TextureManager::boundaryCorner);
               pBoundary->mAngle = 90.0f;
            }
            else if (patternIndex == 6)
            {
               Boundary* pBoundary = Character::Resurrect<Boundary>();
               assert(pBoundary != 0);
               pBoundary->Reset();
               pBoundary->mX = (startingX + desiredSize * i);
               pBoundary->mY = (startingY + desiredSize * j);
               pBoundary->mScale = mScale;
               pBoundary->SetTexture(TextureManager::boundaryCorner);
               pBoundary->mAngle = 180.0f;
            }
            else if (patternIndex == 12)
            {
               Boundary* pBoundary = Character::Resurrect<Boundary>();
               assert(pBoundary != 0);
               pBoundary->Reset();
               pBoundary->mX = (startingX + desiredSize * i);
               pBoundary->mY = (startingY + desiredSize * j);
               pBoundary->mScale = mScale;
               pBoundary->SetTexture(TextureManager::boundaryCorner);
               pBoundary->mAngle = 270.0f;
            }
            else if (patternIndex == 9)
            {
               Boundary* pBoundary = Character::Resurrect<Boundary>();
               assert(pBoundary != 0);
               pBoundary->Reset();
               pBoundary->mX = (startingX + desiredSize * i);
               pBoundary->mY = (startingY + desiredSize * j);
               pBoundary->mScale = mScale;
               pBoundary->SetTexture(TextureManager::boundaryCorner);
               pBoundary->mAngle = 0.0f;
            }



            if (c == 't')
            {
               TunnelCover* pTunnel = Character::Resurrect<TunnelCover>();
               assert(pTunnel != 0);
               if (i == 0)
               {
                  pTunnel->mX = startingX + desiredSize * (i - 3.5f);
                  pTunnel->mY = startingY + desiredSize * (j + 0.5f);
                  mTunnelPoints.push_back(TunnelInfo(i - 1, j + 1, MovementState::RIGHT));
               }
               else if (i == mWidth - 1)
               {
                  pTunnel->mX = startingX + desiredSize * (i + 3.5f);
                  pTunnel->mY = startingY + desiredSize * (j + 0.5f);
                  mTunnelPoints.push_back(TunnelInfo(i + 1, j + 1, MovementState::LEFT));
               }
               else if (j == 0)
               {
                  pTunnel->mX = startingX + desiredSize * (i + 0.5f);
                  pTunnel->mY = startingY + desiredSize * (j - 3.5f);   // TODO: potentially adjust for vertical tunnels
                  mTunnelPoints.push_back(TunnelInfo(i - 1, j + 1, MovementState::DOWN)); // adjust i and j as well
               }
               else if (j == mHeight - 1)
               {
                  pTunnel->mX = startingX + desiredSize * (i + 0.5f);
                  pTunnel->mY = startingY + desiredSize * (j + 4.5f);   // TODO: potentially adjust for vertical tunnels
                  mTunnelPoints.push_back(TunnelInfo(i - 1, j + 1, MovementState::UP));    // adjust i and j as well

               }

            }
            else if (c == 'c')
            {
               if (RightAndLowerAdjacentCharactersPresent(i, j, 'c'))
               {
                  mpPlayer->SetMazeUtil(mMazeUtil);
                  mPlayerSpawnX = (startingX + desiredSize * i);
                  mPlayerSpawnY = (startingY + desiredSize * j + (desiredSize / 2.0f));
                  mpPlayer->Initialize(MovementState::LEFT, mPlayerSpawnX, mPlayerSpawnY);
                  mpPlayer->mScale = mScale / 2.05f;
                  mpPlayer->SetSpeedScale(mScale);
               }
            }
            else if (c == ' ')
            {
               if (RightAndLowerAdjacentCharactersPresent(i, j, ' ', 'o'))
               {
                  Point* pPoint = Character::Resurrect<Point>();
                  assert(pPoint != 0);
                  pPoint->Initialize();
                  pPoint->mX = (startingX + desiredSize * i + (desiredSize / 2.0f));
                  pPoint->mY = (startingY + desiredSize * j + (desiredSize / 2.0f));
                  pPoint->SetPowerPoint(false);
                  pPoint->mScale *= mScale;
               }
            }
            else if (c == 'o')
            {
               if (RightAndLowerAdjacentCharactersPresent(i, j, 'o'))
               {
                  Point* pPoint = Character::Resurrect<Point>();
                  assert(pPoint != 0);
                  pPoint->Initialize();
                  pPoint->mX = (startingX + desiredSize * i + (desiredSize / 2.0f));
                  pPoint->mY = (startingY + desiredSize * j + (desiredSize / 2.0f));
                  pPoint->SetPowerPoint(true);
                  pPoint->mScale *= mScale;
               }
               else if (RightAndLowerAdjacentCharactersPresent(i, j, 'o', ' '))
               {
                  Point* pPoint = Character::Resurrect<Point>();
                  assert(pPoint != 0);
                  pPoint->Initialize();
                  pPoint->mX = (startingX + desiredSize * i + (desiredSize / 2.0f));
                  pPoint->mY = (startingY + desiredSize * j + (desiredSize / 2.0f));
                  pPoint->SetPowerPoint(false);
                  pPoint->mScale *= mScale;
               }
            }
            else if (c == 'n')
            {
               if (!ghostBoxPlaced)
               {
                  pCharacter = Character::Resurrect<GhostBox>();
                  assert(pCharacter != 0);
                  pCharacter->mX = (startingX + desiredSize * (i + 3));
                  pCharacter->mY = (startingY + desiredSize * (j + 1) + (desiredSize / 2.0f));
                  pCharacter->mScale = mScale * 0.98f;
                  ghostBoxPlaced = true;

                  Ghost* pGhost = Character::Resurrect<Ghost>();
                  pGhost->Reset();
                  pGhost->SetExitPosition(pCharacter->mX, pCharacter->mY - (desiredSize * 3.0f));
                  pGhost->SetIdlePositionAndLimits(pCharacter->mX, pCharacter->mY - desiredSize, pCharacter->mY + desiredSize);
                  pGhost->mScale = mScale;
                  pGhost->SetSpeedScale(mScale);
                  pGhost->SetTarget(mpPlayer);
                  pGhost->SetMazeUtil(mMazeUtil);
                  pGhost->SetIdleTimeSeconds(3.0f);
                  pGhost->mX = pCharacter->mX;
                  pGhost->mY = pCharacter->mY - desiredSize;
                  mpGhosts.push_back(pGhost);
                  mGhostSpawnPoints.push_back(CoordinatePoint(pGhost->mX, pGhost->mY));

                  pGhost = Character::Resurrect<Ghost>();
                  pGhost->Reset();
                  pGhost->SetColor(Ghost::ORANGE);
                  pGhost->SetExitPosition(pCharacter->mX, pCharacter->mY - (desiredSize * 3.0f));
                  pGhost->SetIdlePositionAndLimits(pCharacter->mX + 2.0f * desiredSize, pCharacter->mY - desiredSize, pCharacter->mY + desiredSize);
                  pGhost->mScale = mScale;
                  pGhost->SetSpeedScale(mScale);
                  pGhost->SetTarget(mpPlayer);
                  pGhost->SetMazeUtil(mMazeUtil);
                  pGhost->SetIdleTimeSeconds(6.0f);
                  pGhost->mX = pCharacter->mX + 2.0f * desiredSize;
                  pGhost->mY = pCharacter->mY + desiredSize;
                  mpGhosts.push_back(pGhost);
                  mGhostSpawnPoints.push_back(CoordinatePoint(pGhost->mX, pGhost->mY));

                  pGhost = Character::Resurrect<Ghost>();
                  pGhost->Reset();
                  pGhost->SetColor(Ghost::PINK);
                  pGhost->SetExitPosition(pCharacter->mX, pCharacter->mY - (desiredSize * 3.0f));
                  pGhost->SetIdlePositionAndLimits(pCharacter->mX - 2.0f * desiredSize, pCharacter->mY - desiredSize, pCharacter->mY + desiredSize);
                  pGhost->mScale = mScale;
                  pGhost->SetSpeedScale(mScale);
                  pGhost->SetTarget(mpPlayer);
                  pGhost->SetMazeUtil(mMazeUtil);
                  pGhost->SetIdleTimeSeconds(9.0f);
                  pGhost->mX = pCharacter->mX - 2.0f * desiredSize;
                  pGhost->mY = pCharacter->mY + desiredSize;
                  mpGhosts.push_back(pGhost);
                  mGhostSpawnPoints.push_back(CoordinatePoint(pGhost->mX, pGhost->mY));

                  pGhost = Character::Resurrect<Ghost>();
                  pGhost->Reset();
                  pGhost->SetColor(Ghost::RED);
                  pGhost->SetExitPosition(pCharacter->mX, pCharacter->mY - (desiredSize * 3.0f));
                  pGhost->SetIdlePositionAndLimits(pCharacter->mX, pCharacter->mY, pCharacter->mY);
                  pGhost->mScale = mScale;
                  pGhost->SetSpeedScale(mScale);
                  pGhost->SetTarget(mpPlayer);
                  pGhost->SetMazeUtil(mMazeUtil);
                  pGhost->SetIdleTimeSeconds(0.0f);
                  pGhost->SetState(Ghost::CHASING);
                  pGhost->mX = pCharacter->mX;
                  pGhost->mY = pCharacter->mY - (desiredSize * 3.0f);
                  mpGhosts.push_back(pGhost);
                  mGhostSpawnPoints.push_back(CoordinatePoint(pGhost->mX, pGhost->mY));

               }
            }
         }
      }
   }
   for (unsigned int j = 0; j < mHeight; j++)
   {
      for (unsigned int i = 0; i < mWidth; i++)
      {
         if (mMaze[i][j] == 'n')
         {
            mMaze[i][j] = '*';
         }
      }
   }

   mMazeUtil.SetMaze(mMaze);

   mPlayerLivesManager.SetScale(mScale);
   mPlayerLivesManager.SetStartPosition(0.0f + desiredSize, 1080.0f - desiredSize);
   mPlayerLivesManager.AddPlayerLife();
   mPlayerLivesManager.AddPlayerLife();
}

void Maze::Clear()
{
   Character::KillAll();
}

bool Maze::RightAndLowerAdjacentCharactersPresent(unsigned int col, unsigned int row, char search)
{
   bool found = true;
   if (!(col + 1 < mWidth))
   {
      found = false;
   }
   if (!(row + 1 < mHeight))
   {
      found = false;
   }

   if (found)
   {
      if ((mMaze[col + 1][row] == search) &&
         (mMaze[col][row + 1] == search) &&
         (mMaze[col + 1][row + 1] == search))
      {
         found = true;
      }
      else
      {
         found = false;
      }
   }
   return found;
}

bool Maze::RightAndLowerAdjacentCharactersPresent(unsigned int col, unsigned int row, char search, char search2)
{
   bool found = true;
   if (!(col + 1 < mWidth))
   {
      found = false;
   }
   if (!(row + 1 < mHeight))
   {
      found = false;
   }

   if (found)
   {
      if ((mMaze[col + 1][row] == search || mMaze[col + 1][row] == search2) &&
         (mMaze[col][row + 1] == search || mMaze[col][row + 1] == search2) &&
         (mMaze[col + 1][row + 1] == search || mMaze[col + 1][row + 1] == search2))
      {
         found = true;
      }
      else
      {
         found = false;
      }
   }
   return found;
}


void Maze::Process(float deltaTime)
{
   mPauseRemaining -= deltaTime;
   switch (mState)
   {
      case NO_PAUSES:
      {
         if (!mFruitSpawned && Point::mPointCount < 120)
         {
            mFruitSpawned = true;
            Fruit* pFruit = Character::Resurrect<Fruit>();
            pFruit->SetType(Fruit::CHERRY);
            assert(!mTunnelPoints.empty());
            TunnelInfo tunnel = mTunnelPoints.at(Util::RandomInteger(0, mTunnelPoints.size() - 1));
            TunnelInfo tunnel2 = mTunnelPoints.at(Util::RandomInteger(0, mTunnelPoints.size() - 1));
            pFruit->SetMazeUtil(mMazeUtil);
            pFruit->SetSpeedScale(mScale);
            pFruit->SetTunnelPosition(true, tunnel.i, tunnel.j, tunnel.directionToEnter);
            pFruit->SetTunnelPosition(false, tunnel2.i, tunnel2.j, tunnel2.directionToEnter);
            pFruit->Reset();
            pFruit->SetMovementState(tunnel.directionToEnter);
         }

         bool done = false;
         for (std::vector<Ghost*>::iterator it = mpGhosts.begin(); !done && it != mpGhosts.end(); it++)
         {
            if ((*it)->GetDeathOccurred())
            {
               Character::mpSoundManager->Play(SoundManager::ghostDeath);
               (*it)->SetTexture(TextureManager::nothing);
               mpPlayer->SetTexture(TextureManager::nothing);
               mpScoreSign = Character::Resurrect<ScoreSign>();
               assert(mpScoreSign != nullptr);
               mpScoreSign->mX = (*it)->mX;
               mpScoreSign->mY = (*it)->mY;
               mpScoreSign->mScale = 0.7f;      // * mScale
               ScoreManager::AddPoints(Ghost::mGhostValue);
               mpScoreSign->SetValue(Ghost::mGhostValue);
               Ghost::mGhostValue *= 2;

               done = true;
               for (std::vector<Ghost*>::iterator it2 = mpGhosts.begin(); it2 != mpGhosts.end(); it2++)
               {
                  (*it2)->SetPause(true);
               }
               mpPlayer->SetPause(true);
               Fruit* pFruit = Character::FindCharacter<Fruit>(Character::alive);
               if (pFruit != nullptr)
               {
                  pFruit->SetPause(true);
               }

               mPauseRemaining = 1.0f;
               Character::mpSoundManager->Pause(SoundManager::background);
               mState = GHOST_SCORE_SHOWING;
            }

            if ((*it)->GetPlayerKilled())
            {
               // show ghost for a brief moment before hiding all ghosts
               for (std::vector<Ghost*>::iterator it2 = mpGhosts.begin(); it2 != mpGhosts.end(); it2++)
               {
                  (*it2)->SetPause(true);
               }
               mpPlayer->SetPause(true);
               (*it)->SetPlayerKilled(false);
               Fruit* pFruit = Character::FindCharacter<Fruit>(Character::alive);
               if (pFruit != nullptr)
               {
                  pFruit->Kill();
               }

               mPauseRemaining = 1.0f;
               Character::mpSoundManager->Pause(SoundManager::background);
               mState = PAUSE_BEFORE_PLAYER_DEATH;
            }
         }

         break;
      }
      case START_PAUSE:
      {
         if (mPauseRemaining < 0.0f)
         {
            Character::mpSoundManager->UnPauseAll();
            for (std::vector<Ghost*>::iterator it = mpGhosts.begin(); it != mpGhosts.end(); it++)
            {
               (*it)->SetPause(false);
            }
            mpPlayer->SetPause(false);
            mState = NO_PAUSES;
         }
         break;
      }
      case GHOST_SCORE_SHOWING:
      {
         if (mPauseRemaining < 0.0f)
         {
            if (mpScoreSign != nullptr)
            {
               mpScoreSign->Kill();
            }
            for (std::vector<Ghost*>::iterator it = mpGhosts.begin(); it != mpGhosts.end(); it++)
            {
               (*it)->SetPause(false);
            }
            mpPlayer->SetPause(false);
            mpPlayer->SetTexture(TextureManager::pacman2);
            Fruit* pFruit = Character::FindCharacter<Fruit>(Character::alive);
            if (pFruit != nullptr)
            {
               pFruit->SetPause(false);
            }
            mState = NO_PAUSES;
            Character::mpSoundManager->UnPauseAll();
         }
         break;
      }
      case PAUSE_BEFORE_PLAYER_DEATH:
      {
         if (mPauseRemaining < 0.0f)
         {
            for (std::vector<Ghost*>::iterator it = mpGhosts.begin(); it != mpGhosts.end(); it++)
            {
               (*it)->mX = -1000.0f;
               (*it)->mY = -1000.0f;
            }
            mpPlayer->Kill();

            mState = PLAYER_DEATH_ANIMATION;
         }
         break;
      }
      case PLAYER_DEATH_ANIMATION:
      {
         if (mpPlayer->GetStatus() == StatusManager::dead)
         {
            if (mPlayerLivesManager.Empty())
            {
               // game over
               mState = NO_PAUSES;
            }
            else
            {
               mPlayerLivesManager.RemovePlayerLife();


               mpPlayer->SetStatus(StatusManager::alive);
               mpPlayer->Initialize(MovementState::LEFT, mPlayerSpawnX, mPlayerSpawnY);
               mpPlayer->SetPause(true);
               for (int i = 0; i < mpGhosts.size(); i++)
               {
                  Ghost* pGhost = mpGhosts.at(i);
                  pGhost->Reset();
                  pGhost->mX = mGhostSpawnPoints.at(i).x;
                  pGhost->mY = mGhostSpawnPoints.at(i).y;
                  if (pGhost->GetColor() == Ghost::RED)
                  {
                     pGhost->SetState(Ghost::CHASING);
                  }
                  pGhost->SetPause(true);
               }

               mState = INITIALIZATION;
            }
         }
         break;
      }
      case INITIALIZATION:
      {
         mpPlayer->SetPause(true);
         for (std::vector<Ghost*>::iterator it = mpGhosts.begin(); it != mpGhosts.end(); it++)
         {
            (*it)->SetPause(true);
         }
         Character::mpSoundManager->PauseAll();

         mPauseRemaining = 1.0f;
         mState = START_PAUSE;
         break;
      }
   }
}
#pragma once
#include <string>
#include <vector>
#include "MazeUtil.h"   
#include "Player.h"
#include "Ghost.h"
#include "Util.h"
#include "ScoreSign.h"
#include "PlayerLivesManager.h"

class Maze
{
public:
   struct TunnelInfo
   {
      TunnelInfo(int i, int j, MovementState direction)
      {
         this->i = i;
         this->j = j;
         directionToEnter = direction;
      }

      int i;
      int j;
      MovementState directionToEnter;
   };
   
   Maze();
   void SetMaze(std::vector<std::string> textMaze);
   void Display();
   void Clear();
   void Process(float deltaTime);
private:
   enum GameState
   {
      NO_PAUSES,
      START_PAUSE,
      GHOST_SCORE_SHOWING,
      PAUSE_BEFORE_PLAYER_DEATH,
      PLAYER_DEATH_ANIMATION,
      PLAYER_DEATH_PAUSE,
      INITIALIZATION
   };

   struct CoordinatePoint
   {
      CoordinatePoint(float x, float y)
      {
         this->x = x;
         this->y = y;
      }

      float x;
      float y;
   };

   bool RightAndLowerAdjacentCharactersPresent(unsigned int col, unsigned int row, char search);
   bool RightAndLowerAdjacentCharactersPresent(unsigned int col, unsigned int row, char search, char search2);

   std::vector<std::vector<char>> mMaze;
   unsigned int mWidth;
   unsigned int mHeight;
   MazeUtil mMazeUtil;
   std::vector<Ghost*> mpGhosts;
   std::vector<CoordinatePoint> mGhostSpawnPoints;
   std::vector<TunnelInfo> mTunnelPoints;
   Player* mpPlayer;
   ScoreSign* mpScoreSign;
   float mPauseRemaining;
   PlayerLivesManager mPlayerLivesManager;
   float mPlayerSpawnX;
   float mPlayerSpawnY;
   bool mFruitSpawned;
   float mScale;
   GameState mState;
   TextureManager::Textures mPattern[16] =
   {
      TextureManager::boundary0,
      TextureManager::boundary1,
      TextureManager::boundary2,
      TextureManager::boundary3,
      TextureManager::boundary4,
      TextureManager::boundary5,
      TextureManager::boundary6,
      TextureManager::boundary7,
      TextureManager::boundary8,
      TextureManager::boundary9,
      TextureManager::boundary10,
      TextureManager::boundary11,
      TextureManager::boundary12,
      TextureManager::boundary13,
      TextureManager::boundary14,
      TextureManager::boundary15
   };
};
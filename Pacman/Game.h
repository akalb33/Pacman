#pragma once
#include "Screen.h"
#include "Character.h"
#include "Player.h"
#include "Menu.h"
#include "TextureManager.h"
#include "Maze.h"

class Game : public IMenuCallBack
{
public:
   Game();
   ~Game();

private:

   void SetMaxCharacters();
   virtual void UpdateMenuOptions();
   void StartNewGame();

   Screen mScreen;
   TextureManager mTextureManager;
   SoundManager mSoundManager;
   Menu mMenu;
   Maze mMaze;
};

#include "stdafx.h"
#include <iostream>
#include "Game.h"
#include "ScoreManager.h"
#include "Util.h"
#include <assert.h>
#include <SFML/System/Time.hpp>
#include <random>

#include "Point.h"
#include "Player.h"
#include "Background.h"
#include "TunnelCover.h"
#include "Boundary.h"
#include "GhostBox.h"
#include "Ghost.h"
#include "Fruit.h"
#include "ScoreSign.h"
#include "PlayerLife.h"

Game::Game()
   : mScreen(0.0f, 0.0f, 1920.0f, 1080.0f, false, "Pacman")
   , mTextureManager()
   , mSoundManager()
   , mMenu()
   , mMaze()
{
   Util::InitRandomizer();
   mScreen.setVerticalSyncEnabled(true);

   Character::SetTextureManager(&mTextureManager);
   Character::SetSoundManager(&mSoundManager);
   Character::SetScreen(&mScreen);
   SetMaxCharacters();

   sf::Font font;
   assert(font.loadFromFile("arial.ttf"));
   sf::Text text;
   text.setFont(font);
   text.setString("Score");

   ScoreManager::Load();
   ScoreManager::AddPlayer(1750.0f, 50.0f, text, 50.0f);
   ScoreManager::SelectPlayer(0);

   mScreen.SetToFullScreen(false);

   mMenu.SetMenuTitle("Pacman");
   mMenu.SetText(text);
   mMenu.AddSetting("Start Game");
   mMenu.AddSetting("Fullscreen");
   mMenu.AddSetting("Exit");
   mMenu.Load();

   bool goToMenu = true;

   sf::Clock clock;
   while (mScreen.isOpen())
   {
      sf::Event event;
      while (mScreen.pollEvent(event) || goToMenu)
      {
         if (event.type == sf::Event::Closed)
            mScreen.close();
         else if (event.type == sf::Event::Resized)
            mScreen.HandleResizeEvent();
         else if (event.type == sf::Event::KeyReleased || goToMenu)
         {
            if (event.key.code == sf::Keyboard::Escape || goToMenu)
            {
               Character::mpSoundManager->PauseAll();

               std::string selection = mMenu.ShowMenu(mScreen);
               goToMenu = false;
               if ((selection == "Exit") || (selection == "WINDOW_CLOSED_EVENT"))
                  mScreen.close();
               else if (selection == "Start Game")
               {
                  StartNewGame();
               }
               else if (selection == "Fullscreen")
               {
                  mScreen.ToggleFullScreen();
                  goToMenu = true;
               }
               else 
               {       
                  Character::mpSoundManager->UnPauseAll();
               }
               clock.restart();
            }
         }
      }

      mScreen.clear();
      float deltaTime = clock.restart().asSeconds();
      if (deltaTime > 0.025f)
      {
         deltaTime = 0.025f;
      }
      Character::ProcessAll(deltaTime);
      mMaze.Process(deltaTime);
      /*if (Point::mPointCount < 150)
      {
         // iterate thru array of mazes?
         std::vector<std::string> s;
         s.push_back("*****************************");
         s.push_back("tttttttt*           *tttttttt");
         s.push_back("      xx*           *xx      ");
         s.push_back("******xx*  *******  *xx******");
         s.push_back("*             *             *");
         s.push_back("*             *             *");
         s.push_back("*  ******  *  *  *  ******  *");
         s.push_back("*  *       *  *  *       *  *");
         s.push_back("*  *       *     *       *  *");
         s.push_back("*  *  ***  *     *  ***  *  *");
         s.push_back("*       *  *******  *       *");
         s.push_back("*       *           *       *");
         s.push_back("******  * xxxxxxxxx *  ******");
         s.push_back("*       * xnnnnnnnx *       *");
         s.push_back("*       * xnnnnnnnx *       *");
         s.push_back("*  ***    xnnnnnnnx    ***  *");
         s.push_back("*    *    xnnnnnnnx    *    *");
         s.push_back("*    *  * xxxxxxxxx *  *    *");
         s.push_back("***  *  *           *  *  ***");
         s.push_back("***  *  ***  ***  ***  *  ***");
         s.push_back("***          ***          ***");
         s.push_back("***          ***          ***");
         s.push_back("***  ******  ***  ******  ***");
         s.push_back("txx     *     cc    *     xxt");
         s.push_back("        *     cc    *        ");
         s.push_back("***  *  *  *******  *  *  ***");
         s.push_back("*    *        *        *    *");
         s.push_back("*    *        *        *    *");
         s.push_back("*  ***  ****  *  ****  ***  *");
         s.push_back("*                           *");
         s.push_back("*                           *");
         s.push_back("*****************************");   


         mMaze.Clear();
         mMaze.SetMaze(s);
         mMaze.Display();
      }        */
      Character::DisplayAll();
      ScoreManager::DrawScores(mScreen);
      mScreen.setMouseCursorVisible(false);


      mScreen.DisplayNewScreen();
   }

   mMenu.Save();
   ScoreManager::Save();
}

Game::~Game()
{

}

void Game::SetMaxCharacters()
{
   Character::SetMaxLimit<Point>(300);
   Character::SetMaxLimit<Player>(1);
   Character::SetMaxLimit<Boundary>(1000);
   Character::SetMaxLimit<Background>(1);
   Character::SetMaxLimit<TunnelCover>(20);
   Character::SetMaxLimit<GhostBox>(1);
   Character::SetMaxLimit<Ghost>(4);
   Character::SetMaxLimit<Fruit>(1);
   Character::SetMaxLimit<ScoreSign>(2);
   Character::SetMaxLimit<PlayerLife>(3);
}

void Game::UpdateMenuOptions()
{
}

void Game::StartNewGame()
{
   Character::KillAll();
   Character::mpSoundManager->StopAll();

   Background* pBackground = Character::Resurrect<Background>();
   pBackground->mX = 960.0f;
   pBackground->mY = 540.0f;

   std::vector<std::string> s;
   s.push_back("*****************************");
   s.push_back("*       *           *       *");
   s.push_back("*oo     *           *     oo*");
   s.push_back("*oo***  *  *******  *  ***oo*");
   s.push_back("*                           *");
   s.push_back("*                           *");
   s.push_back("***  *  ****  *  ****  *  ***");
   s.push_back("***  *  ****  *  ****  *  ***");
   s.push_back("ttt  *        *        *  ttt");
   s.push_back("     *        *        *     ");
   s.push_back("***  ****x ******* x****  ***");
   s.push_back("***  xxxxxx       xxxxxx  ***");
   s.push_back("***       xxxxxxxxx       ***");
   s.push_back("***  **** xnnnnnnnx ****  ***");
   s.push_back("***  * xxxxnnnnnnnxxxx *  ***");
   s.push_back("***  * x  xnnnnnnnx  x *  ***");
   s.push_back("***  * x* xnnnnnnnx *x *  ***");
   s.push_back("ttt    x* xxxxxxxxx *x    ttt");
   s.push_back("     xxx*           *xxx     ");
   s.push_back("***  ******* x*x *******  ***");
   s.push_back("***          x*x          ***");
   s.push_back("***        xxx*xxx        ***");                                   
   s.push_back("***  ****  *******  ****  ***");
   s.push_back("*             cc            *");
   s.push_back("*             cc            *");
   s.push_back("*  ***  ****  *  ****  ***  *");
   s.push_back("*  ***  *     *     *  ***  *");
   s.push_back("*  ***  *     *     *  ***  *");
   s.push_back("*oo***  *  *******  *  ***oo*");
   s.push_back("*oo                       oo*");
   s.push_back("*                           *");
   s.push_back("*****************************");


   mMaze.SetMaze(s);
   mMaze.Display();

   Character::mpSoundManager->Play(SoundManager::background);
}
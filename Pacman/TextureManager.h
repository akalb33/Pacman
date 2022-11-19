#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>
#include "Polygon.h"

class TextureManager
{
public:
   enum Textures
   {
      background,
      point,
      powerPoint,
      pacman1,
      pacman2,
      pacman3,
      boundary0,
      boundary1,
      boundary2,
      boundary3,
      boundary4,
      boundary5,
      boundary6,
      boundary7,
      boundary8,
      boundary9,
      boundary10,
      boundary11,
      boundary12,
      boundary13,
      boundary14,
      boundary15,
      boundaryCorner,
      black,
      ghostbox,
      blueGhost1,
      blueGhost2,
      pinkGhost1,
      pinkGhost2,
      orangeGhost1,
      orangeGhost2,
      redGhost1,
      redGhost2,
      vulnerableGhost1,
      vulnerableGhost2,
      vulnerableGhostBlink1,
      vulnerableGhostBlink2,
      eyes,
      cherry,
      cherry2,
      strawberry,
      strawberry2,
      orange,
      pretzel,
      apple,
      pear,
      banana,
      fruitScore100,
      score200,
      score400,
      score800,
      score1600,
      nothing
   };

   TextureManager()
   {
      Init();

      AddTexture(background, "background.jpg");
      AddTexture(point, "point.png");
      AddTexture(powerPoint, "powerPoint.png");
      AddTexture(pacman1, "pacmanClosed.png");
      AddTexture(pacman2, "pacman.png");
      AddTexture(pacman3, "pacmanOpen.png");
      AddTexture(boundary0, "boundary0.png");
      AddTexture(boundary1, "boundary1.png");
      AddTexture(boundary2, "boundary2.png");
      AddTexture(boundary3, "boundary3.png");
      AddTexture(boundary4, "boundary4.png");
      AddTexture(boundary5, "boundary5.png");
      AddTexture(boundary6, "boundary6.png");
      AddTexture(boundary7, "boundary7.png");
      AddTexture(boundary8, "boundary8.png");
      AddTexture(boundary9, "boundary9.png");
      AddTexture(boundary10, "boundary10.png");
      AddTexture(boundary11, "boundary11.png");
      AddTexture(boundary12, "boundary12.png");
      AddTexture(boundary13, "boundary13.png");
      AddTexture(boundary14, "boundary14.png");
      AddTexture(boundary15, "boundary15.png");
      AddTexture(boundaryCorner, "boundarycorner.png");
      AddTexture(black, "black.jpg");
      AddTexture(ghostbox, "ghostbox.png");
      AddTexture(blueGhost1, "blue1.png");
      AddTexture(blueGhost2, "blue2.png");
      AddTexture(pinkGhost1, "pink1.png");
      AddTexture(pinkGhost2, "pink2.png");
      AddTexture(orangeGhost1, "orange1.png");
      AddTexture(orangeGhost2, "orange2.png");
      AddTexture(redGhost1, "red1.png");
      AddTexture(redGhost2, "red2.png");
      AddTexture(vulnerableGhost1, "vulnerable1.png");
      AddTexture(vulnerableGhost2, "vulnerable2.png");
      AddTexture(vulnerableGhostBlink1, "vulnerableBlink1.png");
      AddTexture(vulnerableGhostBlink2, "vulnerableBlink2.png");
      AddTexture(eyes, "eyes.png");
      AddTexture(cherry, "cherry.png");
      AddTexture(cherry2, "cherry2.png");
      AddTexture(strawberry, "strawberry.png");
      AddTexture(strawberry2, "strawberry2.png");
      AddTexture(orange, "eyes.png");
      AddTexture(pretzel, "eyes.png");
      AddTexture(apple, "eyes.png");
      AddTexture(pear, "eyes.png");
      AddTexture(banana, "eyes.png");
      AddTexture(fruitScore100, "fruitScore100.png");
      AddTexture(score200, "score200.png");
      AddTexture(score400, "score400.png");
      AddTexture(score800, "score800.png");
      AddTexture(score1600, "score1600.png");
      AddTexture(nothing, "nothing.png");


   }
   // This automatically sets default origin and dimensions based on texture
   //    size. Call SetOrigin() and AddPoint() to overwrite these values.
   void AddTexture(Textures textureId, std::string fileName);

   const sf::Texture& GetTexture(Textures textureId) const;

   // This will set the origin of the texture. Rotation, movement and scaling
   //    are relative to the origin. Default dimensions are automatically set
   //    based on the size of the texture. Call AddPoint() to overwrite these.
   void SetOrigin(Textures textureId, float x, float y);

   // The first time this is called, the dimensions will be a point.
   // The second time this is called, the dimensions will be a line.
   // Three or more calls will define a polygon with an automatic line drawn
   //    from the last point added to the first point that was added.
   void AddPoint(Textures textureId, float x, float y);

   const Polygon& GetPolygon(Textures textureId);

private:
   enum State
   {
      noTexture,
      textureWithDefaultDimensions,
      originSetDefaultDimensions,
      pointsAdded
   };

   void Init();
   void SetDefaultDimensions(Textures textureId);

   static const unsigned int MAX_TEXTURES = 100;
   sf::Texture mTexture[MAX_TEXTURES];
   Polygon mPolygon[MAX_TEXTURES];
   State mState[MAX_TEXTURES];
};

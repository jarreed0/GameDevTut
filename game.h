#ifndef GAME_H
#define GAME_H

#include <iostream>
using namespace std;
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>
#include <vector>

#include "audio.h"
#include "object.h"
#include "entity.h"

#define WIDTH 1280
#define HEIGHT 720
#define TILE_SIZE 16

class Game {
public:
  Game();
  ~Game();
  void loop();
  void update();
  void input();
  void render();
  void draw(Object o);
  void draw(const char* msg, int x, int y, int r, int g, int b);
  void loadMap(const char* filename);
  void drawMap();
  void scroll();
private:
  SDL_Renderer* ren;
  SDL_Window* win;
  TTF_Font *font;
  bool running;
  int count;
  int frameCount, timerFPS, lastFrame;
  int mousex, mousey;
  Entity player;
  vector<Object> map;
  int mapX, mapY;
  int speed;
};

#endif //GAME_H

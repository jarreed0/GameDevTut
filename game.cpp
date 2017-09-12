#include "game.h"

Game::Game() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &ren);
  SDL_SetWindowTitle(win, "Our First Game!!!");
  loadMap("res/1.level");  
  TTF_Init();
  running=true;
  count=0;
  font = TTF_OpenFont("res/font.ttf", 24);
  loop();
}

Game::~Game() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

void Game::loop() {
  while(running) {

    lastFrame=SDL_GetTicks();
    static int lastTime;
    if(lastFrame >= (lastTime+1000)) {
      lastTime=lastFrame;
      frameCount=0;
    }
     
    render();
    input();
    update();
  }
}

void Game::render() {
  SDL_SetRenderDrawColor(ren, 126, 192, 238, 255);
  SDL_Rect rect;
  rect.x=rect.y=0;
  rect.w=WIDTH;
  rect.h=HEIGHT;
  SDL_RenderFillRect(ren, &rect);

  //draw(player);
  drawMap();

  frameCount++;
  int timerFPS = SDL_GetTicks()-lastFrame;
  if(timerFPS<(1000/60)) {
    SDL_Delay((1000/60)-timerFPS);
  }

  SDL_RenderPresent(ren);
}


void Game::draw(Object o) {
 SDL_Rect dest = o.getDest();
 SDL_Rect src = o.getSource();
 SDL_RenderCopyEx(ren, o.getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

void Game::draw(const char* msg, int x, int y, int r, int g, int b) {
 SDL_Surface* surf;
 SDL_Texture* tex;
 SDL_Color color;
 color.r=r;
 color.g=g;
 color.b=b;
 color.a=255;
 SDL_Rect rect;
 surf = TTF_RenderText_Solid(font, msg, color);
 tex = SDL_CreateTextureFromSurface(ren, surf);
 rect.x=x;
 rect.y=y;
 rect.w=surf->w;
 rect.h=surf->h;
 SDL_FreeSurface(surf);
 SDL_RenderCopy(ren, tex, NULL, &rect);
 SDL_DestroyTexture(tex);
}

void Game::input() {
  SDL_Event e;
  while(SDL_PollEvent(&e)) {
    if(e.type == SDL_QUIT) {running=false; cout << "Quitting" << endl;}
    if(e.type == SDL_KEYDOWN) {
      if(e.key.keysym.sym == SDLK_ESCAPE) running=false;
    }
    if(e.type == SDL_KEYUP) {
    }
     SDL_GetMouseState(&mousex, &mousey);
  }
}

void Game::update() {
  //player.updateAnimation();
}

void Game::loadMap(const char* filename) {
  int current, mx, my, mw, mh;
  ifstream in(filename);
  if(!in.is_open()) {
    cout << "Failed to open map file." << endl;
    return;
  }
  in >> mw;
  in >> mh;
  in >> mx;
  in >> my;
  for(int i=0; i<mh; i++) {
    for(int j=0; j<mw; j++) {
      if(in.eof()) {
        cout << "Reached end of map file too soon." << endl;
        return;
      }
      in >> current;
      if(current != 0) {
        Object tmp;
        tmp.setImage("res/tileset.png", ren);
        tmp.setSource((current-1)*32, 0, 32, 32);
        tmp.setDest((j*TILE_SIZE)+mx, (i*TILE_SIZE)+my, TILE_SIZE, TILE_SIZE);
        if(current==2 || current==4) tmp.setSolid(0);
        map.push_back(tmp);
      }
    }
  }
  in.close();
}

void Game::drawMap() {
  for(int i=0; i<map.size(); i++) {
    draw(map[i]);
  }
}
#include "game.h"

Game::Game() {
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) cout << "Failed at SDL_Init()" << SDL_GetError() << endl;
  if(SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &ren) < 0) cout << "Failed at SDL_CreateWindowAndRenderer()" << SDL_GetError() << endl;
  SDL_SetWindowTitle(win, "Our First Game!!!");
  loadMap("res/1.level");  
  TTF_Init();
  running=true;
  count=0;
  font = TTF_OpenFont("res/font.ttf", 24);
  mapX=mapY=0;
  speed=4;
  player.setDest(WIDTH/2, HEIGHT/2, 24, 32);
  player.setImage("res/player.png", ren);
  player.setSource(0, 0, 24, 32);
  idoll = player.createCycle(2, 24, 32, 2, 20);
  idolr = player.createCycle(1, 24, 32, 2, 20);
  runl = player.createCycle(4, 24, 32, 4, 4);
  runr = player.createCycle(3, 24, 32, 4, 4);
  player.setCurAnimation(idolr);
  loop();
}

Game::~Game() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

void Game::loop() {
  static int lastTime = 0;
  while(running) {

    lastFrame=SDL_GetTicks();
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

  drawMap();
  draw(player);

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
      if(e.key.keysym.sym == SDLK_a) {l=1;r=0;}
      if(e.key.keysym.sym == SDLK_d) {r=1;l=0;}
      if(e.key.keysym.sym == SDLK_w) u=1;
      if(e.key.keysym.sym == SDLK_s) d=1;
    }
    if(e.type == SDL_KEYUP) {
      if(e.key.keysym.sym == SDLK_a) {l=0;player.setCurAnimation(idoll);}
      if(e.key.keysym.sym == SDLK_d) {r=0;player.setCurAnimation(idolr);}
      if(e.key.keysym.sym == SDLK_w) u=0;
      if(e.key.keysym.sym == SDLK_s) d=0;
    }
     SDL_GetMouseState(&mousex, &mousey);
  }
}

void Game::update() {
  if(l) {if(player.getCurAnimation()!=runl) {player.setCurAnimation(runl);} player.setDest(player.getDX()-speed, player.getDY());}
  if(r) {if(player.getCurAnimation()!=runr) {player.setCurAnimation(runr);} player.setDest(player.getDX()+speed, player.getDY());}
  player.updateAnimation();
  fall=1;
  for(int i=0; i<map.size(); i++) {
    if(collision(player, map[i])) {
      if(map[i].getSolid()) fall=0;
       if(map[i].getId() == 35) {
        if(u) {player.setDest(player.getDX(), player.getDY()-(10*TILE_SIZE));u=d=0;}
        if(d) {player.setDest(player.getDX(), player.getDY()+(10*TILE_SIZE));u=d=0;}
      }
    }
  }

  if(fall) player.setDest(player.getDX(), player.getDY()+GRAV);

  if(player.getDX() < 100) {player.setDest(100, player.getDY()); scroll(speed, 0);}
  if(player.getDX() > WIDTH-100) {player.setDest(WIDTH-100, player.getDY()); scroll(-speed, 0);}
  if(player.getDY() < 100) {player.setDest(player.getDX(), 100); scroll(0, speed);}
  if(player.getDY() > HEIGHT-100) {player.setDest(player.getDX(), HEIGHT-100); scroll(0, -speed);}
}

void Game::loadMap(const char* filename) {
  Object tmp;
  tmp.setImage("res/tileset.png", ren);
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
        tmp.setSolid(1);
        tmp.setSource((current-1)*32, 0, 32, 32);
        tmp.setDest((j*TILE_SIZE)+mx, (i*TILE_SIZE)+my, TILE_SIZE, TILE_SIZE);
        tmp.setId(current);
        if(current==12 || current==30 || current==35 || current==22) tmp.setSolid(0);
        map.push_back(tmp);
      }
    }
  }
  in.close();
}

void Game::scroll(int x, int y) {
  for(int i=0; i<map.size(); i++) {
    map[i].setDest(map[i].getDX()+x, map[i].getDY()+y);
  }
}

void Game::drawMap() {
  for(int i=0; i<map.size(); i++) {
    if(map[i].getDX() >= mapX-TILE_SIZE
    & map[i].getDY() >= mapY-TILE_SIZE
    & map[i].getDX() <= mapX+WIDTH+TILE_SIZE
    & map[i].getDY() <= mapY+HEIGHT+TILE_SIZE) {
      draw(map[i]);
    }
  }
}

bool Game::collision(Object a, Object b) {
  if((a.getDX() < (b.getDX()+b.getDW())) && ((a.getDX() + a.getDW()) > b.getDX()) 
  && (a.getDY() < (b.getDY() + b.getDH())) && ((a.getDY() + a.getDH()) > b.getDY())) {
    return true;
  } else {
    return false;
  }
}
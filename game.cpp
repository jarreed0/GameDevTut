#include "game.h"

Game::Game() {
  SDL_Init(0);
  SDL_CreateWindowAndRenderer(360, 240, 0, &win, &ren);
  SDL_SetWindowTitle(win, "Our First Game!!!");
  running=true;
  count=0;
  star.setDest(100, 100, 100, 120);
  star.setSource(0, 0, 75, 50);
  star.setImage("image.png", ren);
  loop();
}

Game::~Game() {
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
      count++;
    }

    render();
    input();
    update();

    if(count>3) running=false;
  }
}

void Game::render() {
  SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
  SDL_Rect rect;
  rect.x=rect.y=0;
  rect.w=360;
  rect.h=240;
  SDL_RenderFillRect(ren, &rect);

  draw(star);

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

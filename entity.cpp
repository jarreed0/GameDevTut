#include "entity.h"


int Entity::createCycle(int r, int w, int h, int amount, int speed) {
    cycle tmp;
    tmp.row=r-1;
    tmp.w=w;
    tmp.h=h;
    tmp.amount=amount;
    tmp.speed=speed;
    tmp.tick = 0;
    animations.push_back(tmp);
    return animations.size()-1;
}

void Entity::updateAnimation() {
    setSource(animations[curAnim].w*animations[curAnim].tick, animations[curAnim].row*animations[curAnim].h, animations[curAnim].w, animations[curAnim].h);
    if(begin>animations[curAnim].speed){
        if(!rev)animations[curAnim].tick++;
        if(rev)animations[curAnim].tick--;
        begin=0;
    }
    begin++;
    if(animations[curAnim].tick >= animations[curAnim].amount) {animations[curAnim].tick=0;}
    if(animations[curAnim].tick <= 0 ) {
        if(nAb) {
            curAnim=newAnim;
            nAb=0;
            rev=0;
        } else {
            animations[curAnim].tick=0;
        }
    }
}
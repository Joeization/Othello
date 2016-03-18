#ifndef TRAIN_H
#define TRAIN_H
#include "ai.h"
#include "game.h"

extern ai *umikaze;

const int GrpSize = 6;
const int LBor = GrpSize/3;
const int RBor = RBor*2;

class train{
private:
    ai *grp[GrpSize];
    int score[GrpSize];
    ai *tmp[GrpSize];
    game *board;
    void mate(ai *a, ai *b,int id);
    void mutate(ai *a);
public:
    void next_gen();
    train();
    ~train();
    ai *best();
};

#endif

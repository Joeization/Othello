#include "train.h"

train::train(){
    for(int i=0;i<GrpSize;i++)
        grp[i] = new ai(1);
}

train::~train(){
}

void train::mate(ai *a, ai *b, int id){
    tmp[id] = new ai(0);
    int m = rand()%acc;
    for(int l=0;l<Layer-1;l++){
        for(int fi=0;fi<8;fi++){
            for(int fj=0;fj<8;fj++){
                for(int ti=0;ti<8;ti++){
                    for(int tj=0;tj<8;tj++){
                        if(fi!=ti&&fj!=tj){
                            for(int i=0;i<m;i++)
                                tmp[id]->weight[l][fi][fj][ti][tj][i]
                                =a->weight[l][fi][fj][ti][tj][i];
                            for(int i=m;i<acc;i++)
                                tmp[id]->weight[l][fi][fj][ti][tj][i]
                                =b->weight[l][fi][fj][ti][tj][i];
                        }
                    }
                }
            }
        }
    }
}

void train::mutate(ai *a){
    if(rand()%100>=80){
        int l=rand()%(Layer-1);
        int fi=rand()%8;
        int fj=rand()%8;
        int ti=rand()%8;
        int tj=rand()%8;
        for(int p=0;p<acc;p++)
            a->weight[l][fi][fj][ti][tj][p]=rand()%10;
    }
}

void train::next_gen(){
    memset(score,0,sizeof(score));
    for(int i=0;i<GrpSize;i++)
        mutate(grp[i]);
    for(int i=0;i<GrpSize;i++){
        for(int j=0;j<GrpSize;j++){
            if(i!=j){
                #ifdef OUTPUT
                    cout << i << " vs " << j << endl;
                #endif
                board = new game();
                for(int k=0;k<64;k++){
                    if(board->can_drop(1).size()>0){
                        grp[i]->set_value(board);
                        grp[i]->run(board, false);
                    }
                    else{
                        //board->change_player();
                    }
                    if(board->can_drop(-1).size()>0){
                        grp[j]->set_value(board);
                        grp[j]->run(board, false);
                    }
                    else{
                        //board->change_player();
                    }
                }
                int win = board->winner();
                if(win == 1)score[i]++;
                else if(win == -1)score[j]++;
                #ifdef OUTPUT
                    cout << "rd: " << win << endl;
                #endif
                
                delete board;
            }
        }
    }
    //bubble sort is enough for 10 elements
    for(int i=0;i<GrpSize;i++){
        for(int j=i+1;j<GrpSize;j++){
            if(score[j]>score[i]){
                iter_swap(grp+i, grp+j);
                iter_swap(score+i, score+j);
            }
        }
    }
    for(int i=0;i<GrpSize;i++)
        cout << score[i] << " ";
    cout<<endl;
    for(int i=0;i<LBor;i++){
        tmp[i] = grp[i];
    }
    for(int i=LBor;i<RBor;i++){
        int l=rand()%GrpSize;
        int r=rand()%GrpSize;
        mate(grp[l],grp[r],i);
    }
    for(int i=RBor;i<GrpSize;i++){
        tmp[i] = new ai(1);
    }
    for(int i=0;i<GrpSize;i++){
        grp[i] = tmp[i];
    }
}

ai *train::best(){
    return grp[0];
}

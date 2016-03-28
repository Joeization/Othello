#include "ai.h"

ai *umikaze = new ai(0);

float sigmoid(float v){
    return v/(abs(v)+1);
}

void ai::set_weight(){
    for(int l=0;l<Layer;l++){
        for(int fi=0;fi<8;fi++){
            for(int fj=0;fj<8;fj++){
                for(int ti=0;ti<8;ti++){
                    for(int tj=0;tj<8;tj++){
                        if(fi!=ti&&fj!=tj)
                            for(int p=0;p<acc;p++)
                                weight[l][fi][fj][ti][tj][p]=rand()%10;
                    }
                }
            }
        }
    }
}

ai::ai(bool init){
    //init weight, there may be a better way.
    if(init){
        set_weight();
    }
}
ai::~ai(){
    //do nothing
}
void ai::set_value(game *v){
    //init value for current map
    int co = v->get_now_color();
    for(int fi=0;fi<8;fi++){
        for(int fj=0;fj<8;fj++){
            if(v->get_pos(make_pair(fi,fj)) == co)
                value[fi][fj] = 0.1;
            else
                value[fi][fj] = -0.1;
        }
    }
    //compute
    for(int l=0;l<Layer;l++){
        memset(tmp,0,sizeof(tmp));
        for(int fi=0;fi<8;fi++){
            for(int fj=0;fj<8;fj++){
                for(int ti=0;ti<8;ti++){
                    for(int tj=0;tj<8;tj++){
                        if(fi!=ti&&fj!=tj&&(abs(fi-ti)+abs(fj-tj))<dis){
                            float base=1;
                            float w=0;
                            for(int p=0;p<acc;p++){
                                w += base*weight[l][fi][fj][ti][tj][p];
                                base /= 10;
                            }
                            w -= 5;
                            tmp[ti][tj] += value[fi][fj]*w;
                        }
                    }
                }
            }
        }
        float stimu;
        //stimulate
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++){
                stimu = sigmoid(tmp[i][j]);
                if(l < Layer-1){
                    if(abs(stimu) > threshold)
                        value[i][j] = stimu;
                    else
                        value[i][j] = 0;
                }
                else
                    value[i][j] = stimu;
            }
    }
}

bool ai::run(game *v, bool sh){
    //testing
    int color = v->get_now_color();
    vector<pair<int, int> > s = v->can_drop(color);
    if(s.size() > 0){
        //bubble sort is enough for 64 elements
        for(int i=0;i<s.size();i++)
            for(int j=i+1;j<s.size();j++){
                if(value[s[j].first][s[j].second]
                    >value[s[i].first][s[i].second])
                    iter_swap(s.begin()+i,s.begin()+j);
            }
        v->drop(s[0], color);
        if(sh){
            cout << value[s[0].first][s[0].second] << endl;
            cout << "gap: " << value[s[0].first][s[0].second]
                - value[s[s.size()-1].first][s[s.size()-1].second] << endl;
        }
        return true;
    }
    else{
        cout << "not a valid move." << endl;
        return false;
    }
}
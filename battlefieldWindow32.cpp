#include "battlefieldWindow32.h"
#include <cmath>
Battlefield * btl;
void BattlefieldWindow32::Init(Battlefield * _btl)
{
    btl=_btl;
    if(btl->NumberOfUnitsTotal()<=0)
        return;

    R2vector botom,top;
    R2vector tmp;

    botom=btl->dump[0][0].x;
    top=botom+R2vector(10,10);
    int i,j;

    for(i=0; i<btl->dump.size(); ++i)
        for(j=0; j<btl->dump[i].size(); ++j)
        {
            tmp=btl->dump[i][j].x;

            if(top.x<tmp.x)
                top.x=tmp.x;
            if(top.y<tmp.y)
                top.y=tmp.y;


            if(botom.x>tmp.x)
                botom.x=tmp.x;
            if(botom.y>tmp.y)
                botom.y=tmp.y;
        }
    top=top+0.8*(top-botom);
    botom=botom-0.8*(top-botom);
    SetDisplayOptions(top.x,botom.x,top.y,botom.y);

}

bool BattlefieldWindow32::ProcessEvent(UINT messg,WPARAM wParam, LPARAM lParam)
{
    return false;
}
void BattlefieldWindow32::Paint()
{
    PaintBattlefield();
    //PaintStat();
}

void BattlefieldWindow32::PaintBattlefield()
{
    int step,i,d_step=1;
    int pen[btl->NumberOfUnitsTotal()][3];
    for(i=0; i<btl->NumberOfUnitsTotal(); ++i)
    {
        pen[i][0]=225;
        pen[i][1]=50;
        pen[i][2]=0;
    }
    if(norm(btl->dump[0][0].v)*dt<fabs(k_x))
        d_step=fmax(fmin(k_x/(norm(btl->dump[0][0].v)*dt)/4,btl->step/1000),1);
    else
        d_step=1;

    DrawGrid("X, km","Y, km",1e-3,1e-3);

    /*
    d_step=1;
    for(step=d_step; step<btl->step; step+=d_step)
        for(i=0; i<btl->dump[step].size(); ++i)
        {
            PickPen(pen[i][0],pen[i][1],pen[i][2]);
            if(btl->dump[step][i].status==OPERATES)
                RLine(btl->dump[step-d_step][i].x,btl->dump[step][i].x);
        }
        return;
    */
    for(i=0; i<btl->dump[0].size(); ++i)
    {
        PickPen(pen[i][0],pen[i][1],pen[i][2],2);
        for(step=d_step; step<btl->step; step+=d_step)
            if(btl->dump[step][i].status==OPERATES && (RIsInsideScreen(btl->dump[step][i].x) || RIsInsideScreen(btl->dump[step-d_step][i].x)))
                RLine(btl->dump[step-d_step][i].x,btl->dump[step][i].x);
    }
    d_step=60/dt;
    for(i=0; i<btl->dump[0].size(); ++i)
    {
        PickPen(pen[i][0],pen[i][1],pen[i][2],1);
        for(step=d_step; step<btl->step; step+=d_step)
            if(btl->dump[step][i].status==OPERATES && (RIsInsideScreen(btl->dump[step][i].x) || RIsInsideScreen(btl->dump[step-d_step][i].x)))
            {
                RLine(btl->dump[step][i].x+5*cross(btl->dump[step][i].v),btl->dump[step][i].x-5*cross(btl->dump[step][i].v));
            }


    }

}
void BattlefieldWindow32::PaintStat()
{
    long double *res=new long double[btl->step];
    long double max_stat=-1e-100,min_stat=1e-100;
    int i;
    long double t_len,st_len;
    long double t_k=1,st_k;


    for(i=1; i<btl->step; ++i)
    {
        res[i]=StatFunction(i);
        if(res[i]>max_stat)
            max_stat=res[i];
        if(res[i]<min_stat)
            min_stat=res[i];
    }
    res[0]=res[1];



    t_len=i*dt;
    st_len=max_stat-min_stat;

    t_k=1;
    st_k=t_k*RScreenDiaganal().y/RScreenDiaganal().x*t_len/st_len;

    st_k=powf(10,(int)log10f(st_k)-1);

    DrawGrid("T","X=F(T)",1/t_k,1/st_k);
    R2vector current,last=R2vector(0,res[0]*st_k);
    PickPen(225,0,0);
    for(i=1; i<btl->step; ++i)
    {
        current=R2vector(i*dt*t_k,res[i]*st_k);
        RLine(last,current);
        last=current;
    }
}
long double BattlefieldWindow32::StatFunction(int step)
{
    return range(btl->dump[step][1],btl->dump[step][0]);
}
long double * BattlefieldWindow32::GenStat()
{
    long double * res=new long double[btl->step];
    int i;
    for(i=0; i<btl->step; ++i)
        res[i]=StatFunction(i);
    return res;
}

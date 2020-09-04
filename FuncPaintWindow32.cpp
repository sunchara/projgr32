#include "FuncPaintWindow32.h"
#include <cmath>


void funcPaintWindow32::Init(long double * _val,int _val_size,double _arg_begin,double _arg_end,
                             std::string _func_name,std::string _par_name)
{
    val=_val;
    val_size=_val_size;
    arg_begin=_arg_begin;
    arg_end=_arg_end;
    func_name=_func_name;
    par_name=_par_name;


    double val_min=1e100;
    double val_max=-1e100;

    for(int i=0; i<val_size; ++i)
    {
        val_min=fmin(val[i],val_min);
        val_max=fmax(val[i],val_max);
    }
    double range_x=fabs(arg_end-arg_begin)/2;
    double range_y=fmax(fabs(val_min),fabs(val_max));
    SetDisplayOptions(arg_begin-0.1*range_x,arg_end+0.1*range_x,range_y*0.8,-range_y*0.8);
}

void funcPaintWindow32::Zoom (double k)
{
    k_x*=k;
}



void funcPaintWindow32::Moove(double horizontal,double vertical)
{
    Rfocus.x+=RScreenDiaganal().x*horizontal;
}
void funcPaintWindow32::Moove(const R2vector & dx)
{
    Rfocus.x+=dx.x;
}
void funcPaintWindow32::Moove(const I2vector & dx)
{
    Rfocus.x=ItoR(dx).x;
}

void funcPaintWindow32::RecalculateMappings()
{
    int a0=fmax(((RDownLeft().x -arg_begin)*val_size)/(arg_end-arg_begin),0);
    int a1=fmin(((RDownRight().x-arg_begin)*val_size)/(arg_end-arg_begin),val_size);

    double val_min=1e100;
    double val_max=-1e100;

    for(int i=a0; i<a1; ++i)
    {
        val_min=fmin(val[i],val_min);
        val_max=fmax(val[i],val_max);
    }
    double range_y=fmax(fabs(val_min),fabs(val_max));
    k_y=(range_y*5)/(double)(IUpLeft().y-IDownLeft().y)/2;

}


void funcPaintWindow32::Resize(int _width,int _height)
{
    width=_width;
    height=_height;
}
void funcPaintWindow32::Paint()
{
    int step=1;
    double arg_cur,arg_last;
    arg_last=arg_begin;
    RecalculateMappings();
    DrawGrid(par_name,func_name);
    PickPen(225,10,10,2);
    int a0=fmax(((RDownLeft().x -arg_begin)*val_size)/(arg_end-arg_begin),0);
    int a1=fmin(((RDownRight().x-arg_begin)*val_size)/(arg_end-arg_begin),val_size);


    for(int i=a0+1; i<a1; i+=step)
    {
        arg_cur=(arg_begin*(val_size-i)+arg_end*i)/(val_size);
        RLine(R2vector(arg_last,val[i-1]),R2vector(arg_cur,val[i]));
        arg_last=arg_cur;
    }

}


bool funcPaintWindow32::ProcessEvent(UINT messg,WPARAM wParam, LPARAM lParam)
{
    return false;
}

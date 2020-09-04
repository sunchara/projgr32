#ifndef FUNCPAINTWINDOW32_H_INCLUDED
#define FUNCPAINTWINDOW32_H_INCLUDED
#include "gr32.h"


class funcPaintWindow32: public Window32
{
public:
    long double * val;
    int val_size;
    double arg_begin;
    double arg_end;
    double d_arg;

    double arg_left;
    double arg_right;
    std::string par_name;
    std::string func_name;
    void Init(long double * _val,int _val_size,double _arg_begin,double _arg_end,
              std::string _func_name="F(t)",std::string _par_name="t");

    virtual void RecalculateMappings();



    virtual void Zoom(double k);
    virtual void Moove(double horizontal,double vertical);
    virtual void Moove(const R2vector & dx);
    virtual void Moove(const I2vector & dx);

    virtual void Resize(int _width,int _height);
    virtual void Paint();
    virtual bool ProcessEvent(UINT messg,WPARAM wParam, LPARAM lParam);

};


#endif // FUNCPAINTWINDOW32_H_INCLUDED

#ifndef GR32_H_INCLUDED
#define GR32_H_INCLUDED

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <Winuser.h>
#include <Windowsx.h>
//#include "stdafx.h"

#include<math.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

#include "I2vector.h"
#include "R2vector.h"

#define wlog(x) Window32::Window32LogStream<<x<<std::endl
#define wlog2(x) Window32::Window32LogStream<<x<<std::endl,std::cerr<<x<<std::endl;


LRESULT CALLBACK Window32Proc(HWND hWnd, UINT messg,
                              WPARAM wParam, LPARAM lParam);


class Window32
{


public:
///=============================================================================
///
///  STATIC MEMBERS AND METHODS
///
///=============================================================================

protected:
public:
    static HINSTANCE hThisInstance;
    static HINSTANCE hPrevInstance;
    static LPSTR lpszArgument;
    static int nCmdShow;


    static WNDCLASSEX wincl;// window class


    //static Window32** WndList;
    //static int        Wnd_num;


    static std::stringstream Window32LogStream;
    static std::vector <Window32*> wlist;

    static std::string GetLog();
    static std::string GetAndCleanLog();





    static ATOM         RegisterClassWindow32
    (HINSTANCE _hThisInstance,
     HINSTANCE _hPrevInstance,
     LPSTR _lpszArgument,
     int _nCmdShow);

    static Window32*  FindWindow32(HWND _hWnd);

    void              RegisterWindow32();


///=============================================================================
///
///=============================================================================

    HWND hWnd;// window handle
    HDC hdc;
    PAINTSTRUCT ps;
    HPEN hPen;

    bool destroyed;


    int width;
    int height;
    double focus_width;
    double focus_height;
    I2vector Ifocus;
    R2vector Rfocus;
    double k_x;
    double k_y;
    R2vector show_top;
    R2vector show_bottom;
    bool is_mappig_generated;
    bool show_grid;


///=============================================================================
///
///  CONSTRUCTOR AND DISPLAY OUTPUT COFIGURATION
///
///=============================================================================

    Window32();

    virtual void SetDisplayOptions(const R2vector & _show_top, const R2vector & _show_bottom,double _focus_width=0.5,double _focus_height=0.5);
    virtual void SetDisplayOptions(double x1,double x2,double y1,double y2,double _focus_width=0.5,double _focus_height=0.5);

    virtual void GenerateMappings(double x1,double x2,double y1,double y2);
    virtual void GenerateMappings(const R2vector & _show_top, const R2vector & _show_botom);

    HWND Create(TCHAR name[]=_T("Window32"));

///=============================================================================
///
///    WINDOW CONTROLS
///
///=============================================================================

    virtual void Zoom(double k);
    virtual void Moove(double horizontal,double vertical);
    virtual void Moove(const I2vector & dx);
    virtual void Moove(const R2vector & dx);




///=============================================================================
///
///    DISPLAY SPACE INFORMATION
///
///=============================================================================
    inline I2vector RtoI(const R2vector & v) const
    {
        return I2vector(1./k_x*(v.x-Rfocus.x),1./k_y*(v.y-Rfocus.y));
    }
    inline R2vector ItoR(const I2vector & v) const
    {
        return R2vector(Rfocus.x+k_x*v.x,Rfocus.y+k_y*v.y);
    }
    inline I2vector RtoIpure(const R2vector & v) const
    {
        return I2vector(1./k_x*(v.x),1./k_y*(v.y));
    }
    inline R2vector ItoRpure(const I2vector & v) const
    {
        return R2vector(k_x*v.x,k_y*v.y);
    }

    I2vector IDownLeft();
    I2vector IDownRight();
    I2vector IUpLeft();
    I2vector IUpRight();

    R2vector RDownLeft();
    R2vector RDownRight();
    R2vector RUpLeft();
    R2vector RUpRight();

    I2vector IScreenTop();
    R2vector RScreenTop();
    I2vector IScreenDiaganal();
    R2vector RScreenDiaganal();

    bool RIsInsideScreen(const R2vector &v);
    bool IIsInsideScreen(const I2vector &v);

///=============================================================================
///
///    PAINTING METHODTS
///
///=============================================================================

    void PickPen(int r,int g,int b,int width=1,int style=PS_SOLID);


    void IEllipse(const I2vector &x1,const I2vector &x2);
    void ICircle(const I2vector &x1,int r);
    void REllipse(const R2vector &x1,const R2vector &x2);
    void RCircle(const R2vector &x1,double r);

    void ILine(const I2vector &x1,const I2vector &x2);
    void RLine(const R2vector &x1,const R2vector &x2);
    void IPrintText(const I2vector& x1,  std::string s);
    void RPrintText(const R2vector& x1,  std::string s);
    void DrawGrid2();
    void DrawGrid(std::string x_Axis_name="",std::string y_Axis_name="",double x_Axis_koef=1,double y_Axis_koef=1);


///=============================================================================
///
///    INIT, START AND END PAINTING
///
///=============================================================================
    bool StartPaint();
    void FinishPaint();


///=============================================================================
///
///    METHODS TO DO IN WINPROC
///
///=============================================================================
private:
    void Print2(char* filename);

public:
    void Update();
    void Print(char* filename);

    virtual void Resize(int _width,int _height);
    virtual void Paint();

    void InitPaintFunction(double * arr, int n, double t0, double t1);
    void PaintFunction(double * arr, int n, double t0, double t1);



    virtual bool ProcessEvent(UINT messg,WPARAM wParam, LPARAM lParam);


    static int ProcessMessages();
    static LRESULT CALLBACK Window32Proc(HWND hWnd, UINT messg,
                                         WPARAM wParam, LPARAM lParam);


};



#endif // GR32_H_INCLUDED

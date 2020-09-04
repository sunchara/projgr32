#include "battlefieldWindow32.h"
#include "FuncPaintWindow32.h"
#include <iostream>
#include <fstream>

using namespace std;
std::ofstream fout("log.txt");

class MyWind:public Window32
{
    virtual void Paint()
    {
        PickPen(0,0,180);
        ICircle(IUpLeft(),25);
        PickPen(200,0,0);
        RLine(R2vector(100,100),R2vector(-100,-100));
        RLine(R2vector(100,-100),R2vector(-100,100));
    }
};


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    Window32::RegisterClassWindow32(hThisInstance,hPrevInstance, lpszArgument,nCmdShow);
    BattlefieldWindow32 wnd;
    funcPaintWindow32 wst;
    Battlefield btl;
    BFUnit US=btl.AddCountry("1");
    BFUnit RU=btl.AddCountry("2");
    BFUnit jet=btl.AddJET(R2vector(100000,100000),R2vector(150,0),US,"JET1");
    jet->me->HP=110;
    jet->Start();
    BFUnit rk1=btl.AddRocket(R2vector(300000,0),R2vector(200,300),RU,"rocket1");
    BFUnit rk2=btl.AddRocket(R2vector(100000,0),R2vector(-200,200),RU,"rocket2");

    ((Rocket_ICS*)rk1)->Launch(jet);
    ((Rocket_ICS*)rk2)->Launch(jet);


    btl.Play(100000);

    perr("Total time="<<btl.step*dt<<" sec");
    fout<<btl;
    fout.close();


    //wst.Init(btl.GenStat(), btl.step-2, 0, btl.time,"relative speed jet roket 2","TIME");

    wnd.Init(&btl);
    wnd.Create("�������� ��������");
    //wst.Create("stst");



    return Window32::ProcessMessages();
}

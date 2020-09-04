#ifndef BATTLEFIELDWINDOW32_H_INCLUDED
#define BATTLEFIELDWINDOW32_H_INCLUDED


#include "gr32.h"
#include "battlefield.h"

class BattlefieldWindow32:public Window32
{
public:
    Battlefield * btl;
    void Init(Battlefield * _btl);

    virtual bool ProcessEvent(UINT messg,WPARAM wParam, LPARAM lParam);

    virtual void Paint();
    virtual void PaintBattlefield();


    virtual void PaintStat();
    virtual long double StatFunction(int step);
    virtual long double * GenStat();
};

#endif // BATTLEFIELDWINDOW32_H_INCLUDED

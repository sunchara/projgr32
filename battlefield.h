#ifndef BATTLEFIELD_H_INCLUDED
#define BATTLEFIELD_H_INCLUDED
#include "plane_phys.h"
#include "plane_ICS.h"
#include <vector>
typedef Plane_ICS* BFUnit;

class Battlefield
{
public:
    std::vector <std::vector <Plane_phys> > dump;
    std::vector <Plane_phys*> plane;
    std::vector <Plane_ICS *> ICS;
    int step;
    double time;

    Battlefield();
    void Init();
    void Store();
    bool Evolute();

    void Play(int steps);
    void Play();



    Plane_ICS * AddCountry(std::string name="ALIENS");
    Plane_ICS * AddRocket(const R2vector & x,const R2vector & v, Plane_ICS * owner,std::string name="ROCKET");
    Plane_ICS * AddJET (const R2vector & x,const R2vector & v, Plane_ICS * owner,std::string name="JET");


    int NumberOfUnitsTotal();
    int NumberOfUnitsOperates();
    std::string Inf();
    std::string DumpInf();
    virtual long double * GenStat();
    virtual long double StatFunction(int step);

};

std::ostream& operator<<(std::ostream &out, Battlefield & b);

#endif // BATTLEFIELD_H_INCLUDED

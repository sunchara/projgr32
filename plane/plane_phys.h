#ifndef PLANE_PHYS_H_INCLUDED
#define PLANE_PHYS_H_INCLUDED

#include "R2vector.h"
#include <math.h>
#include <vector>

#include<iostream>
#include<fstream>
#include<sstream>

static long double G=10;
static long double dt=1*1e-2, T=3e4;
static long double j_max=G*6,a_max=2*G;
//static std::ofstream fout("onboard.txt");

enum Color {FUTURE_COLORS};
enum Plane_type {UFO,PLANE,ROCKET,BASE,DRONE,JET};
enum Plane_Status {UNKNOWN,UNACTIVE,OPERATES,DESTROYED};

std::string ToStr(Plane_Status s);
std::string ToStr(Plane_type t);


class Plane_phys
{
public:
    R2vector x;
    R2vector v;
    long double a;
    long double j;
    long double t;
    double HP;

    Color color;
    Plane_type type;
    Plane_Status status;
    Plane_phys();
    Plane_phys(const Plane_phys& r);
    Plane_phys(const R2vector& _x,const R2vector& _v);

    void Init(const R2vector& _x,const R2vector& _v);
    bool SetControl(long double _j=0, long double _a=0);
    bool Destroy();
    bool Start();
    bool Evolute();
    bool CopyCoordinates(const Plane_phys& r);
    bool Hit(double dmg);
    std::string Inf();



};

std::ostream& operator<<(std::ostream&out, const Plane_phys & pl);
long double peleng(const Plane_phys & from, const Plane_phys & to);
long double range(const Plane_phys & from, const Plane_phys & to);
long double relative_speed(const Plane_phys & from, const Plane_phys & to);
long double omega(const Plane_phys & from, const Plane_phys & to);


#endif // PLANE_PHYS_H_INCLUDED

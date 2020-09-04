#include "../vect2/R2vector.h"
#include "plane_phys.h"
#include <strstream>
#include <math.h>

long double peleng(const Plane_phys & from, const Plane_phys & to)
{
    return angle(from.v,to.x-from.x);
}
long double range(const Plane_phys & from, const Plane_phys & to)
{
    return norm(to.x-from.x);
}

long double relative_speed(const Plane_phys & from, const Plane_phys & to)
{
    R2vector R(to.x - from.x),V(to.v - from.v);
    return -scal(R,V)/norm(R);
}
long double omega(const Plane_phys & from, const Plane_phys & to)
{
    R2vector R(to.x - from.x),V(to.v - from.v);
    return (crossR(V,R))/scal(R,R);
}


//========================================================================================================
//
//========================================================================================================


Plane_phys::Plane_phys():
    x(0,0),
    v(0,0),
    a(0),
    j(0),
    t(0),
    HP(100),
    status(UNACTIVE),
    type(UFO)
{
}

Plane_phys::Plane_phys(const R2vector& _x,const R2vector& _v):
    x(_x),
    v(_v),
    a(0),
    j(0),
    t(0),
    HP(100),
    status(UNACTIVE),
    type(UFO)
{
}


void Plane_phys::Init(const R2vector& _x,const R2vector& _v)
{
    x=_x;
    v=_v;
    a=0;
    j=0;
    t=0;
    HP=100;
    status=UNACTIVE;
    type=UFO;
}

Plane_phys::Plane_phys(const Plane_phys& r):
    x(r.x),
    v(r.v),
    a(r.a),
    j(r.j),
    t(r.t),
    HP(r.HP),
    status(r.status),
    type(r.type)
{
}

bool Plane_phys::SetControl(long double _j, long double _a)
{
    a=_a;
    j=_j;
    return true;
}
bool Plane_phys::Destroy()
{
    status=DESTROYED;
    v=R2vector(0,0);
    a=j=0;
    return true;
}
bool Plane_phys::Start()
{
    if(status==UNACTIVE)
        status=OPERATES;
    return true;
}

bool Plane_phys::Evolute()
{

    t+=dt;
    if(status!=OPERATES)
        return true;

    long double alpha;
    long double V=norm(v);

    R2vector dx;
    R2vector v_normal=normalize(v);

    if(V<1e-4 || fabs(j)<1e-5)
    {
        x=x+v*dt;
        v=v+(v_normal*a+cross(v_normal)*j)*dt;
        return true;
    }

    alpha=dt*(j/(2*V));
    dx=(v*(2*cos(alpha))+cross(v)*sin(alpha))*((V*sin(alpha))/j);
    x=x+dx;
    v=v*cos(2*alpha) - cross(v) * sin(2*alpha)+ (v_normal*a)*dt;


    return true;

}
bool Plane_phys::CopyCoordinates(const Plane_phys& r)
{
    x=r.x;
    v=r.v;
    a=r.a;
    j=r.j;
    t=r.t;
    return true;
}

bool Plane_phys::Hit(double dmg)
{
    HP-=dmg;
    return true;

}
std::string Plane_phys::Inf()
{
    std::stringstream ss;
    ss<<(*this);
    return ss.str();

}
std::string ToStr(Plane_Status s)
{
    switch (s)
    {
    case UNACTIVE:
        return "UNACTIVE";
    case OPERATES:
        return "OPERATES";
    case DESTROYED:
        return "DESTROYED";
    case UNKNOWN:
        return "UNKNOWN";
    }
    return "?";
}
std::string ToStr(Plane_type t)
{
    switch (t)
    {
    case UFO:
        return "UFO";
    case PLANE:
        return "PLANE";
    case ROCKET:
        return "ROCKET";
    case BASE:
        return "BASE";
    case DRONE:
        return "DRONE";
    case JET:
        return "JET";
    }
    return "?";
}

std::ostream& operator<<(std::ostream &out, const Plane_phys & pl)
{
    out<<"type="<<ToStr(pl.type)<<std::endl;
    out<<"status="<<ToStr(pl.status)<<std::endl;
    //out<<"HP="<<pl.HP<<std::endl;

    out<<"coordinates: "<<pl.x<<std::endl;
    out<<"speed vector: "<<pl.v<<", velicity: "<<norm(pl.v)<<std::endl;
    out<<"control a="<<pl.a<<", j="<<pl.j<<std::endl;
    return out;
}

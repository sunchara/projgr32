#include "R2vector.h"

const long double R2vector::eps=1e-5;
const R2vector operator+(const R2vector & l, const R2vector & r)
{
    return R2vector(l.x+r.x , l.y+r.y);
}

const R2vector operator-(const R2vector & l, const R2vector & r)
{
    return R2vector(l.x-r.x , l.y-r.y);
}
const R2vector operator*(const R2vector & l, long double r)
{
    return R2vector(r*l.x,r*l.y);
}
const R2vector operator*(long double l, const R2vector & r)
{
    return R2vector(l*r.x,l*r.y);
}

const R2vector comb(long double a,const R2vector & l, long double b, const R2vector & r)
{
    return R2vector(a*l.x+b*r.x , a*l.y+b*r.y);
}
const R2vector cross(const R2vector & l)
{
    return R2vector(-l.y,l.x);
}
double crossR(const R2vector & l,const R2vector & r)
{
    return l.x*r.y - r.x*l.y;
}

long double norm(const R2vector & l)
{
    return sqrt(l.x*l.x+l.y*l.y);
}

bool is_zero(const R2vector & l)
{
    return (norm(l)<R2vector::eps);
}
long double scal(const R2vector & l,const R2vector & r)
{
    return l.x*r.x+l.y*r.y;
}

R2vector normalize(const R2vector &l)
{
    if(is_zero(l))
        return R2vector(0,0);
    return R2vector(l.x/norm(l),l.y/norm(l));
}

double angle(const R2vector & l,const R2vector & r)
{
    /*double xx = scal(l,r);
    double yy = scal(l,normalize(r));
    return atan2(yy, xx);
    */
    if(!is_zero(l) && !is_zero(r))
    {
        double _sin=crossR(l,r)/(norm(l)*norm(r));
        double _cos= scal(l,r)/(norm(l)*norm(r));
        return atan2(_sin,_cos);
    }
//        return asin(cross(l,r)/(norm(l)*norm(r)));

    //if(norm(r)>R2vector::eps)
    //return atan2(scal(l,r),scal(l,r)/norm(r));
    return 0;
}
std::ostream & operator<<(std::ostream &out, const R2vector &r)
{
    return out<<r.x<<' '<<r.y;
}


std::istream & operator>>(std::istream &in, R2vector &r)
{
    return in>>r.x>>r.y;
}

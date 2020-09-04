#include "I2vector.h"

const I2vector operator+(const I2vector & l, const I2vector & r)
{
    return I2vector(l.x+r.x , l.y+r.y);
}

const I2vector operator-(const I2vector & l, const I2vector & r)
{
    return I2vector(l.x-r.x , l.y-r.y);
}
const I2vector operator*(const I2vector & l, long double r)
{
    return I2vector(r*l.x,r*l.y);
}
const I2vector operator*(long double l, const I2vector & r)
{
    return I2vector(l*r.x,l*r.y);
}

const I2vector comb(long double a,const I2vector & l, long double b, const I2vector & r)
{
    return I2vector(a*l.x+b*r.x , a*l.y+b*r.y);
}

const I2vector cross(const I2vector & l)
{
    return I2vector(-l.y,l.x);
}
double crossR(const I2vector & l,const I2vector & r)
{
    return l.x*r.y - r.x*l.y;
}

long double norm(const I2vector & l)
{
    return sqrt(l.x*l.x+l.y*l.y);
}

bool is_zero(const I2vector & l)
{
    return (l.x==0 && l.y==0);
}
long double scal(const I2vector & l,const I2vector & r)
{
    return l.x*r.x+l.y*r.y;
}

double angle(const I2vector & l,const I2vector & r)
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

    //if(norm(r)>I2vector::eps)
    //return atan2(scal(l,r),scal(l,r)/norm(r));
    return 0;
}
std::ostream & operator<<(std::ostream &out, const I2vector &r)
{
    return out<<r.x<<' '<<r.y;
}


std::istream & operator>>(std::istream &in, I2vector &r)
{
    return in>>r.x>>r.y;
}

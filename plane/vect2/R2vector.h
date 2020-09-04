#ifndef R2VECTOR_H_INCLUDED
#define R2VECTOR_H_INCLUDED
#include <iostream>
#include <math.h>
#define perr2(x) std::cerr<<__FILE__<<':'<<__LINE__<<" "<<x<<std::endl;
#define perr(x) std::cerr<<"!: "<<x<<std::endl;

class R2vector
{
public:
    static const long double eps;//=1e-5;
    long double x;
    long double y;

    R2vector(const R2vector & r)
    {
        x=r.x;
        y=r.y;
    }
    R2vector(long double _x=0,long double _y=0)
    {
        x=_x;
        y=_y;
    }
    R2vector & operator=(const R2vector & r)
    {
        if(this==&r)
            return *this;
        x=r.x;
        y=r.y;
        return (*this);
    }
    R2vector & normalize_it()
    {
        long double l=sqrt(x*x+y*y);
        if(l>eps)
        {
            x/=l;
            y/=l;
        }
        return *this;
    }

};

//const long double R2vector::eps=1e-6;

const R2vector operator+(const R2vector & l, const R2vector & r);
const R2vector operator-(const R2vector & l, const R2vector & r);
const R2vector operator*(const R2vector & l, long double r);
const R2vector operator*(long double l, const R2vector & r);

const R2vector comb(long double a,const R2vector & l, long double b, const R2vector & r);
const R2vector cross(const R2vector & l);
double crossR(const R2vector & l,const R2vector & r);
long double norm(const R2vector & l);
R2vector normalize(const R2vector &l);

long double scal(const R2vector & l,const R2vector & r);
double angle(const R2vector & l,const R2vector & r);
bool is_zero(const R2vector & l);



std::istream & operator>>(std::istream &in, R2vector &r);
std::ostream & operator<<(std::ostream &out, const R2vector &r);






#endif // R2VECTOR_H_INCLUDED

#ifndef I2VECTOR_H_INCLUDED
#define I2VECTOR_H_INCLUDED
#include <iostream>
#include <math.h>
class I2vector
{
public:
    int x;
    int y;

    I2vector(const I2vector & r)
    {
        x=r.x;
        y=r.y;
    }
    I2vector(int _x=0,int _y=0)
    {
        x=_x;
        y=_y;
    }
    I2vector & operator=(const I2vector & r)
    {
        if(this==&r)
            return *this;
        x=r.x;
        y=r.y;
        return (*this);
    }

};

//const int I2vector::eps=1e-6;

const I2vector operator+(const I2vector & l, const I2vector & r);
const I2vector operator-(const I2vector & l, const I2vector & r);
const I2vector operator*(const I2vector & l, long double r);
const I2vector operator*(long double l, const I2vector & r);

const I2vector comb(long double a,const I2vector & l, long double b, const I2vector & r);
const I2vector cross(const I2vector & l);
double crossR(const I2vector & l,const I2vector & r);
long double norm(const I2vector & l);
I2vector normalize(const I2vector &l);

long double scal(const I2vector & l,const I2vector & r);
double angle(const I2vector & l,const I2vector & r);
bool is_zero(const I2vector & l);



std::istream & operator>>(std::istream &in, I2vector &r);
std::ostream & operator<<(std::ostream &out, const I2vector &r);


#endif // I2VECTOR_H_INCLUDED

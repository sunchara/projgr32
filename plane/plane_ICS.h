#ifndef PLANE_ICS_H_INCLUDED
#define PLANE_ICS_H_INCLUDED
#include "plane_phys.h"

enum TaskType{DO,DO_IN_TIME,HIT,HIT_IN_TIME};

class Plane_ICS
{
public:
    int id;
    static int number_of_planes;
    Color color;
    Plane_type type;

    Plane_phys * me;
    Plane_ICS * parrent;
    std::vector <Plane_ICS *> children;
    std::string name;

    double j_max;
    double a_max;


    Plane_ICS();
    Plane_ICS(Plane_phys * _me=NULL,Plane_ICS * owner=NULL, std::string _name="UFO");

    Plane_Status GetStatus();
    Plane_Status SetStatus(Plane_Status _s);


    virtual void Init(Plane_phys * _me=NULL, Plane_ICS * owner=NULL,std::string _name="UFO");

    virtual void Destroy();
    virtual void Start();

    virtual void AppendBody(Plane_phys * _me);
    virtual void AppendChild(Plane_ICS * child);

    virtual bool Evolute();
    bool IsOperates();

    virtual std::string Inf();

};

//Plane_phys * operator()
//=========================================================================


class Rocket_ICS : public Plane_ICS
{
public:
    Plane_ICS * target;

//����������� ��� ��������� ���������

// ���������� � ���������
    TaskType task;
    long double V_tg;
    long double R_tg;
    long double omega_tg;
    long double phi_tg;
    long double t_tg;
//������ � ������
    long double dmg;
    long double range_of_hit;





    Rocket_ICS       (Plane_phys * _me, Plane_ICS * owner, std::string _name="ROCKET");
    virtual void Init(Plane_phys * _me, Plane_ICS * owner, std::string _name="ROCKET");

    virtual void Launch(Plane_ICS * tg);
    virtual bool IsTargetInRange();

    virtual void Metring();
    virtual bool Evolute();

    virtual std::string Inf();
};


class JET_ICS : public Plane_ICS
{
public:
    std::vector <Plane_ICS> * enviroment;
    JET_ICS        (Plane_phys * _me, Plane_ICS * owner, std::string _name="JET");
    virtual void Init(Plane_phys * _me, Plane_ICS * owner, std::string _name="JET");
    virtual bool Evolute();

    virtual std::string Inf();
};

std::ostream& operator<<(std::ostream&out, Plane_ICS  pl);

#endif // PLANE_ICS_H_INCLUDED

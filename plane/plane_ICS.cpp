#include "plane_ICS.h"

int Plane_ICS::number_of_planes=0;

Plane_ICS::Plane_ICS()
{
    Init(NULL,NULL,"UFO");
}
Plane_ICS::Plane_ICS(Plane_phys * _me, Plane_ICS * owner, std::string _name)
{
    Init(_me,owner,_name);
}
void Plane_ICS::AppendBody(Plane_phys * _me)
{
    me=_me;
}
void Plane_ICS::Init(Plane_phys * _me, Plane_ICS * owner, std::string _name)
{
    name=_name;
    me=_me;
    type=UFO;
    a_max=0;
    j_max=0;
    parrent=owner;

    Plane_ICS::number_of_planes++;
    id=Plane_ICS::number_of_planes;
}

void Plane_ICS::AppendChild(Plane_ICS * child)
{
    children.push_back(child);
    child->parrent=this;
}


Plane_Status Plane_ICS::GetStatus()

{
    if(me==NULL)
        return UNKNOWN;
    return me->status;

}
Plane_Status Plane_ICS::SetStatus(Plane_Status _s)
{
    if(me==NULL)
        return UNKNOWN;
    Plane_Status s=GetStatus();
    me->status=_s;
    return s;

}
bool Plane_ICS::IsOperates()
{
    return GetStatus()==OPERATES;

}
void Plane_ICS::Destroy()
{
    if(me==NULL)
        return;
    me->Destroy();

    for(int i=0; i<children.size(); ++i)
        if(children[i]->IsOperates()==false)
            children[i]->Destroy();
}
void Plane_ICS::Start()
{
    if(me==NULL)
        return;
    if(GetStatus()==UNACTIVE)
        SetStatus(OPERATES);
    return;

}

bool Plane_ICS::Evolute()
{
    if(me==NULL || me->status==DESTROYED)
        return true;


    if(me->status==UNACTIVE)
    {
        if(parrent==NULL)
            return true;
        else
        {
            me->CopyCoordinates(*(parrent->me));
            return true;
        }
    }


    if(me->HP<=0)
    {
        Destroy();
        return true;
    }
    return false;
}


std::string Plane_ICS::Inf()
{
    std::stringstream ss;
    ss<<"||  "<<name<<"  ||"<<std::endl<<std::endl;
    if(me!=NULL)
        ss<<(*me)<<std::endl;
    return ss.str();
}

//======================================================





void Rocket_ICS::Init(Plane_phys * _me, Plane_ICS *owner, std::string _name)
{
    Plane_ICS::Init(_me, owner,_name);
    type=ROCKET;

    t_tg;

    a_max=2*G;
    j_max=6*G;

    dmg=100;
    range_of_hit=30;

}

Rocket_ICS::Rocket_ICS(Plane_phys * _me, Plane_ICS *owner, std::string _name):Plane_ICS(_me, owner,_name)
{
    Init(_me, owner,_name);
}

void Rocket_ICS::Metring()
{
    phi_tg   =  peleng        (*me, *(target->me));
    R_tg     =  range         (*me, *(target->me));
    V_tg     =  relative_speed(*me, *(target->me));
    omega_tg =  omega         (*me, *(target->me));
}
void Rocket_ICS::Launch(Plane_ICS * tg)
{
    target=tg;
    Start();
    task = TaskType::HIT;
}
bool Rocket_ICS::IsTargetInRange()
{
    return (range(*me, *(target->me))<range_of_hit);
}

bool Rocket_ICS::Evolute()
{
    long double phi_req,omega_req;
    long double K_omega,K_phi;
    long double j;

    if(Plane_ICS::Evolute())
        return true;


    Metring();

    if(IsTargetInRange())
    {
        this->Destroy();
        target->me->Hit(dmg);
        return true;
    }
    if(/*fabs(phi_tg)>3.14/3  || */target->GetStatus()!=OPERATES)
    {
        this->Destroy();
        return true;
    }
    switch(task)
    {
    case HIT:
        //m���� ������� ���������
        j=G*omega_tg*V_tg*500;
        break;
    case HIT_IN_TIME:

        break;
    }

    if(fabs(j)>j_max)
        j=j_max*(fabs(j)/j);

    //std::cerr<<"j is "<<j<<std::endl;
    //perr(j);
    me->SetControl(j,0);
    return true;

}

std::string Rocket_ICS::Inf()
{
    std::stringstream ss;
    ss<<(this->Plane_ICS::Inf());
    ss<<"R="<<R_tg<<", Vrel="<<V_tg<<" ,phi="<<phi_tg<<std::endl;
    return ss.str();
}

//======================================================


bool JET_ICS::Evolute()
{
    Plane_ICS::Evolute();
    me->SetControl(1.,0.2);
    return 1;

}
void JET_ICS::Init(Plane_phys * _me, Plane_ICS *owner, std::string _name)
{
    this->Plane_ICS::Init(_me,owner,_name);
}

JET_ICS::JET_ICS(Plane_phys * _me, Plane_ICS *owner, std::string _name):Plane_ICS(_me, owner,_name)
{
    Init(_me,owner,_name);
}


std::string JET_ICS::Inf()
{
    std::stringstream ss;
    ss<<(this->Plane_ICS::Inf());
    return ss.str();
}
//=============================================================


std::ostream& operator<<(std::ostream&out, Plane_ICS pl)
{
    out<<pl.Inf();
}

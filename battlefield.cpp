#include "battlefield.h"

void Battlefield::Init()
{

}

Battlefield::Battlefield()
{
    step=0;
    time=0;

}
int Battlefield::NumberOfUnitsTotal()
{
    return ICS.size();
}

int Battlefield::NumberOfUnitsOperates()
{
    int res=0;
    for(int i=0; i<NumberOfUnitsTotal(); ++i)
        res+=ICS[i]->IsOperates();
    return res;
}

Plane_ICS * Battlefield::AddCountry(std::string name)
{
    ICS.push_back((Plane_ICS*)(new Plane_ICS(NULL,NULL,name)));
    return ICS.back();
}
Plane_ICS * Battlefield::AddRocket(const R2vector & x,const R2vector & v, Plane_ICS * owner,std::string name)
{
    plane.push_back(new Plane_phys(x,v));
    plane.back()->type=ROCKET;

    ICS.push_back((Plane_ICS*)(new Rocket_ICS(plane.back(),owner,name)));
    return ICS.back();
}
Plane_ICS* Battlefield::AddJET(const R2vector & x,const R2vector & v,Plane_ICS * owner, std::string name)
{
    plane.push_back(new Plane_phys(x,v));
    plane.back()->type=PLANE;
    ICS.push_back((Plane_ICS*)(new JET_ICS(plane.back(),owner,name)));
    return ICS.back();
}

void Battlefield::Store()
{
    dump.resize(dump.size()+1);
    for(int i=0; i<ICS.size(); ++i)
        if(ICS[i]->me!=NULL)
            (dump.back()).push_back(*(ICS[i]->me));
}

bool Battlefield::Evolute()
{
    int i,j;
    for(i=0; i<ICS.size(); ++i)
        ICS[i]->Evolute();
    for(i=0; i<plane.size(); ++i)
        plane[i]->Evolute();
    return true;
}

void Battlefield::Play()
{

    while(NumberOfUnitsOperates()>0)
    {
        Store();
        Evolute();
        ++step;
        time+=dt;
    }
}

void Battlefield::Play(int steps)
{

    while(NumberOfUnitsOperates()>0 && step<steps )
    {
        Store();
        Evolute();
        ++step;
        time+=dt;
    }
}
long double Battlefield::StatFunction(int step)
{
    return relative_speed(this->dump[step][1],this->dump[step][0]);
}
long double * Battlefield::GenStat()
{
    long double * res=new long double[this->step];
    int i;

    for(i=0; i<this->step; ++i)
        res[i]=StatFunction(i);
        res[0]=res[1];
    return res;
}


std::string Battlefield::Inf()
{
    std::stringstream ss;
    ss<<"==========================================================="<<std::endl;
    ss<<"TIME="<<time<<" | ITERATION STEP="<<step<<std::endl<<std::endl;
    for(int i=0; i<NumberOfUnitsTotal(); ++i)
        ss<<(ICS[i])->Inf()<<std::endl;
    //ss<<(ICS[2])->Inf()<<std::endl;
    return ss.str();

}
std::string Battlefield::DumpInf()
{
    std::stringstream ss;
    for(int i=0; i<dump.size(); ++i)
    {
        ss<<"==========================================================="<<std::endl;
        ss<<"ITERATION STEP="<<i<<std::endl<<std::endl;
        for(int j=0; j<dump[i].size(); ++j)
            ss<<dump[i][j].Inf()<<std::endl;
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream &out, Battlefield & b)
{
    out<<b.Inf();
    return out;
}

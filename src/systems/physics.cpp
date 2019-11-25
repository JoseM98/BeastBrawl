#include "physics.h"

using namespace std;

#include <cmath>


void Physics::move(CSpeed *s, CPosition *p)
{
    p->SetX(p->GetX() + s->GetVX());
    p->SetY(p->GetY() + s->GetVY());
    p->SetZ(p->GetZ() + s->GetVZ());
}


float dotProduct(CPosition a, CPosition b){  //calculates dot product of a and b
    return a.GetX() * b.GetX() + a.GetY() * b.GetY() + a.GetZ() * b.GetZ();
}


float magnitude(CPosition a){  //calculates magnitude of a
    return sqrt( a.GetX() * a.GetX() + a.GetY() * a.GetY() + a.GetZ() * a.GetZ() );
}

// a=origen b=destino -- CUIDADO CON EL NEEEW
CPosition vector3d(CPosition a, CPosition b){  //calculates magnitude of a
    CPosition *c = new CPosition( b.GetX()-a.GetX(), b.GetY()-a.GetY(), b.GetZ()-a.GetZ() );  
    return *c;
}

// lo dos vectores seran: uno- la direccion a la que mira el coche y otro el vecotr que uno la posActual y el punto destino
float angle(CPosition a, CPosition b){
    return acos(dotProduct(a,b)/(magnitude(a)*magnitude(b)));
}
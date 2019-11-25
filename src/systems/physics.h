#ifndef __PHYSICS_H__
#define __PHYSICS_H__


#include "../components/speed.h"
#include "../components/position.h"

class Physics
{
public:
    Physics(){};
    ~Physics(){};
    static void move(CSpeed *s, CPosition *p);


    float dotProduct(CPosition a, CPosition b);  //calculates dot product of a y b
    float magnitude(CPosition a);  //calculates magnitude of a
    CPosition vector3d(CPosition a, CPosition b);  //calculates vector3d of a y b
    float angle(CPosition a, CPosition b);  //calculates angle of a y b


protected:
private:
};


#endif
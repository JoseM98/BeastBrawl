#include "PhysicsAI.h"


void CalculatePositionAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);
void CalculatePositionReverseAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);


PhysicsAI::PhysicsAI(){

}



void PhysicsAI::fuzzyRules(CarAI* car){
    auto components = car->GetComponents();
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);
    float maxSpeed = cCar->maxSpeed;
    float minSpeed = cCar->reverseMaxSpeed;
    float accelerationCar = cCar->acceleration;
    //float algo = cCar->
    fuzzyRulesVelocity(maxSpeed, minSpeed, accelerationCar);
    fuzzyRulesAngle();

}
void PhysicsAI::fuzzyRulesVelocity(float maxSpeed, float minSpeed, float accelerationCar){

    FuzzyVariable& ActualVelocity = flVelocity.CreateFLV("ActualVelocity");
    FzSet Velocity_Slow = ActualVelocity.AddLeftShoulderSet("Velocity_Slow", minSpeed, 0, 5);
    FzSet Velocity_Normal = ActualVelocity.AddTriangularSet("Velocity_Normal", 0, 5, maxSpeed/2);
    FzSet Velocity_High = ActualVelocity.AddRightShoulderSet("Velocity_High", 5, maxSpeed/2, maxSpeed);

    FuzzyVariable& Angle = flVelocity.CreateFLV("Angle");
    FzSet Angle_Slow = Angle.AddLeftShoulderSet("Angle_Slow", 0, 30, 60);
    FzSet Angle_Normal = Angle.AddTriangularSet("Angle_Normal", 30, 60, 120);
    FzSet Angle_High = Angle.AddRightShoulderSet("Angle_High", 60, 120, 180);

    //FuzzyVariable& Distance = flVelocity.CreateFLV("Distance");
    //FzSet Distance_Near = Distance.AddLeftShoulderSet("Distance_Near", 0, 0, 10);
    //FzSet Distance_nNormal = Distance.AddTriangularSet("Distance_nNormal", 0, 10, 30);
    //FzSet Distance_Far = Distance.AddRightShoulderSet("Distance_Far", 10, 30, 40);

    FuzzyVariable& Acceleration = flVelocity.CreateFLV("Acceleration");
    FzSet Accelerate_Brake = Acceleration.AddLeftShoulderSet("Accelerate_Brake", -accelerationCar, -accelerationCar/2, 0);
    FzSet Accelerate_None = Acceleration.AddTriangularSet("Accelerate_None", -accelerationCar/2, 0,accelerationCar/2);
    FzSet Accelerate_Max = Acceleration.AddRightShoulderSet("Accelerate_Max", 0, accelerationCar/2, accelerationCar);
    // To-Do: revisar el new por que no se tiene que hacer
    flVelocity.AddRule( *(new FzAND(Velocity_Slow, Angle_Slow)), Accelerate_Max);
    flVelocity.AddRule( *(new FzAND(Velocity_Slow, Angle_Normal)), Accelerate_Max);
    flVelocity.AddRule( *(new FzAND(Velocity_Slow, Angle_High)), Accelerate_None);
    flVelocity.AddRule( *(new FzAND(Velocity_Normal, Angle_Slow)), Accelerate_Max);
    flVelocity.AddRule( *(new FzAND(Velocity_Normal, Angle_Normal)), Accelerate_None);
    flVelocity.AddRule( *(new FzAND(Velocity_Normal, Angle_High)), Accelerate_None);
    flVelocity.AddRule( *(new FzAND(Velocity_High, Angle_Slow)), Accelerate_Max);
    flVelocity.AddRule( *(new FzAND(Velocity_High, Angle_Normal)), Accelerate_Brake);
    flVelocity.AddRule( *(new FzAND(Velocity_High, Angle_High)), Accelerate_Brake);
}

void PhysicsAI::fuzzyRulesAngle(){

    FuzzyVariable& Distance = flAngle.CreateFLV("Distance");
    FzSet Distance_Near = Distance.AddLeftShoulderSet("Distance_Near", 0, 0, 40);
    FzSet Distance_Normal = Distance.AddTriangularSet("Distance_Normal", 20, 40, 60);
    FzSet Distance_Far = Distance.AddRightShoulderSet("Distance_Far", 40, 100, 100);

    FuzzyVariable& Direction = flAngle.CreateFLV("Direction");
    FzSet Direction_Left = Direction.AddLeftShoulderSet("Direction_Left", -180, -180, 0);
    FzSet Direction_Center = Direction.AddTriangularSet("Direction_Center", -20, 0, 20);
    FzSet Direction_Right = Direction.AddRightShoulderSet("Direction_Right", 0, 180, 180);

    FuzzyVariable& Rotation = flAngle.CreateFLV("Rotation");
    FzSet Rotation_Left = Rotation.AddLeftShoulderSet("Rotation_Left", -10, -5, 0);
    FzSet Rotation_None = Rotation.AddTriangularSet("Rotation_None", -5, 0, 5);
    FzSet Rotation_Right = Rotation.AddRightShoulderSet("Rotation_Right", 0, 5, 10);
    // To-Do: revisar el new por que no se tiene que hacer
    flAngle.AddRule( *(new FzAND(Distance_Near, Direction_Left)), Rotation_Left);
    flAngle.AddRule( *(new FzAND(Distance_Near, Direction_Center)), Rotation_None);
    flAngle.AddRule( *(new FzAND(Distance_Near, Direction_Right)), Rotation_Right);
    flAngle.AddRule( *(new FzAND(Distance_Normal, Direction_Left)), Rotation_Left);
    flAngle.AddRule( *(new FzAND(Distance_Normal, Direction_Center)), Rotation_None);
    flAngle.AddRule( *(new FzAND(Distance_Normal, Direction_Right)), Rotation_Right);
    flAngle.AddRule( *(new FzAND(Distance_Far, Direction_Left)), Rotation_Left);
    flAngle.AddRule( *(new FzAND(Distance_Far, Direction_Center)), Rotation_None);
    flAngle.AddRule( *(new FzAND(Distance_Far, Direction_Right)), Rotation_Right);
}




//Nos devuelve el angulo en radianos entre el coche y el waypoint
float calculateAngle(CWayPoint* wayPointNext, CarAI* car,CCar* cCar){
    auto components = car->GetComponents();
    auto cTransformable = static_cast<CTransformable*>(components[CompType::TransformableComp]);

    // calcular vector al wayPoint
    float vetorWaypointX = (wayPointNext->position.x - cTransformable->position.x );
    float vetorWaypointZ = (wayPointNext->position.z - cTransformable->position.z);
    
    // se calcula el siguiente punto al que avanzara el coche
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float posXSiguiente = cTransformable->position.x - cos(angleRotation) * cCar->speed;
    float posZSiguiente = cTransformable->position.z + sin(angleRotation) * cCar->speed;

    // se calcula el vector entre el siguiente punto y y el punto actual del coche
    float xCoche = (posXSiguiente - cTransformable->position.x );
    float zCoche = (posZSiguiente - cTransformable->position.z);

    // se calcula el angulo entre los dos vectores
    float numerador = xCoche*vetorWaypointX + zCoche*vetorWaypointZ;
    float denominador = sqrt(xCoche*xCoche + zCoche*zCoche) * sqrt(vetorWaypointX*vetorWaypointX + vetorWaypointZ*vetorWaypointZ);
    float angle2 = 0.0;
    if(denominador!=0)
        angle2 = acos(numerador/denominador);
    angle2 = (angle2 * 180) / PI;

    // calcular si tiene que girar a la izquierda o derecha
    float valueAtan2 = atan2(vetorWaypointZ,vetorWaypointX)*180/PI;
    if(valueAtan2<0)
        valueAtan2 += 360;

    //calcular opuesto
    float oppositeAngleAtan2 = 0.0;
    if(valueAtan2<180)
        oppositeAngleAtan2 = valueAtan2 + 180;
    else
        oppositeAngleAtan2 = valueAtan2 - 180;
    
    // sentido de giro
    //if(cTransformable->rotation.y<valueAtan2 || cTransformable->rotation.y>oppositeAngleAtan2)
    //    angle2 = angle2*(-1);
    std::cout << "Angulooo: " << angle2 << endl; // primero la Z, luego la X
    return angle2;
}


float PhysicsAI::calculateFuzzyVelocity(float speedCar, float angle){
    flVelocity.Fuzzify("ActualVelocity", speedCar); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
    flVelocity.Fuzzify("Angle", angle);

    float defuzzificacion = flVelocity.DeFuzzify("Acceleration"); 
    //std::cout << "LA DEFUZZIFICACION DA:  " << defuzzificacion << std::endl;
    return defuzzificacion;
}

float PhysicsAI::calculateFuzzyDirection(float distance, float direction){
    flAngle.Fuzzify("Distance", distance); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
    flAngle.Fuzzify("Direction", direction);

    float defuzzificacion = flAngle.DeFuzzify("Rotation"); 
    //std::cout << "LA DEFUZZIFICACION DA:  " << defuzzificacion << std::endl;
    return defuzzificacion;
}

void PhysicsAI::InitPhysicsIA(CarAI* car){
    fuzzyRules(car);
    //fuzzyRulesVelocity(car);
}

void PhysicsAI::Update(vector<WayPoint *> wayPoints, CarAI* car, float deltaTime){
    //Guardamos en varAIbles los componentes
    auto components = car->GetComponents();
	auto cTransformable = static_cast<CTransformable*>(components[CompType::TransformableComp]);
    auto cWayPoint     = static_cast<CWayPoint*>(components[CompType::WayPointComp]);
    auto cCar        = static_cast<CCar*>(components[CompType::CarComp]);
    float angleRange = 0;
    float angle = 0;
    float radious = cWayPoint->radious;
    float distance2P = sqrt( pow((cWayPoint->position.x - cTransformable->position.x),2) + pow((cWayPoint->position.z - cTransformable->position.z),2) );

    //TODO: Cambiar por valores del coche y todas esas vainas

    //Vamos a comprobar si esta en el rango del waypoint
    if((cWayPoint->position.z - radious) < cTransformable->position.z && (cWayPoint->position.z + radious) >= cTransformable->position.z 
        && (cWayPoint->position.x - radious) < cTransformable->position.x && (cWayPoint->position.x + radious) >= cTransformable->position.x){
            //Ha llegado al rango vamos a buscarle otro waypoint
            //TODO: Ya veremos como gestionamos el cambiar de waypoint de momento lo pongo al azar
            int indx = rand() % wayPoints.size();
            cout << "Cambiamos de WayPoint\n";
            car->SetWayPoint(wayPoints[indx]->position);
    }else{

        angle = calculateAngle(cWayPoint, car, cCar);
        if (angle < 0)
            angleRange = angle*(-1);
        else
            angleRange = angle;

        //cout << "Angulo Positivo: " << angleRange*180/PI << endl;
        if(cCar->speed == 0)
            cCar->speed = 20;
            //std::cout << "VOY A ENTRAR A VELOCITY DIFUSA" <<std::endl;
        float fuzzyAceleration = calculateFuzzyVelocity(cCar->speed, angleRange);
        float fuzzyRotation = 0.0;
        if(cCar->speed>3 || cCar->speed < -3){
            fuzzyRotation = calculateFuzzyDirection(distance2P, angle);
        }

        //Aumentamos la velocidad
        //cCar->wheelRotation = angle
        
        cCar->wheelRotation = fuzzyRotation;
        cCar->speed += fuzzyAceleration;
        if(cCar->speed > cCar->maxSpeed){
            cCar->speed = cCar->maxSpeed;
        }


        // calculamos las posiciones
        float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
        cTransformable->position.x -= cos(angleRotation) * cCar->speed * deltaTime;
        cTransformable->position.z += sin(angleRotation) * cCar->speed * deltaTime;
        if(cCar->wheelRotation != 0){
            cTransformable->rotation.y += cCar->wheelRotation * 0.20;
            if(cTransformable->rotation.y>=360.0)
                cTransformable->rotation.y -= 360.0;
            else if(cTransformable->rotation.y < 0.0)
                cTransformable->rotation.y += 360.0;
        }
    }



    
    //std::cout << "Car speed" << cCar->speed << std::endl;
    //Aumentamos la velocidad
    /**
    cCar->speed += cCar->acceleration;
    if(cCar->speed > cCar->maxSpeed){
        cCar->speed = cCar->maxSpeed;
    }

        if(cCar->speed>=0){
            CalculatePositionAI(cCar,cTransformable,deltaTime,angle);            
        }
        
    }*/
        //Si tiene rotacion, rotamos el coche
    
}


//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
//void CalculatePositionAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle){
//    
//    //Modificamos la posicion en X y Z en funcion del angulo
//    cTransformable->position.x += cos(angle) * cCar->speed * deltaTime;
//    cTransformable->position.z += sin(angle) * cCar->speed * deltaTime;
//
//    //Si tiene rotacion, rotamos el coche
//    if(cCar->wheelRotation != 0){
//        cTransformable->rotation.y += cCar->wheelRotation * 0.20;
//    }
//}
//
//
////Calcula la posicion del coche (duda con las formulas preguntar a Jose)
//void CalculatePositionReverseAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle){
//    
//    //Modificamos la posicion en X y Z en funcion del angulo
//    cTransformable->position.x += cos(angle) * cCar->speed * deltaTime;
//    cTransformable->position.z += sin(angle) * cCar->speed * deltaTime;
//
//    //Si tiene rotacion, rotamos el coche
//    if(cCar->wheelRotation != 0){
//        cTransformable->rotation.y -= cCar->wheelRotation * 0.20;
//    }
//}

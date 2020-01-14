#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Manager.h"
#include "../../include/glm/vec3.hpp"
#include "../Aliases.h"


using namespace std;
#define PI 3.141592

class Totem;
class Data;

class ManTotem{
   public:
    ManTotem();
    ~ManTotem();

    void CreateTotem();
    void CreateTotem(glm::vec3 _position);
    //shared_ptr<Totem>& GetTotem() { return totem; };
    vector<shared_ptr<Totem>> GetEntities() const { return totems; };

   private:
   // shared_ptr<Totem> totem;
    vector<shared_ptr<Totem>> totems;
    void AppertainCar(DataMap d);
    void ResetTotem(DataMap d);
    void SubscribeToEvents();
};
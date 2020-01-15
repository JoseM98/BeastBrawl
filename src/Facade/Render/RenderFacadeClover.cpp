#define _USE_MATH_DEFINES

#include "RenderFacadeClover.h"

#include <math.h>
#include "../../Aliases.h"
#include "../../Components/CBoundingPlane.h"
#include "../../Components/CBoundingSphere.h"
#include "../../Components/CCamera.h"
#include "../../Components/CDimensions.h"
#include "../../Components/CId.h"
#include "../../Components/CMesh.h"
#include "../../Components/CNamePlate.h"
#include "../../Components/CTexture.h"
#include "../../Components/CTotem.h"
#include "../../Components/CTransformable.h"
#include "../../Components/CType.h"
#include "../../Components/CWayPoint.h"
#include "../../Components/CWayPointEdges.h"
#include "../../Components/Component.h"
#include "../../Constants.h"
#include "../../Entities/WayPoint.h"
#include "../../Entities/CarAI.h"
#include "../../Game.h"
 
bool RenderFacadeClover::showDebug = false;
 

RenderFacadeClover::~RenderFacadeClover() {
}

RenderFacadeClover::RenderFacadeClover() {

}







// CAMBIOS

 
void RenderFacadeClover::FacadeSuscribeEvents() {

}

void RenderFacadeClover::FacadeInitMenu() {

}

void RenderFacadeClover::FacadeInitPause() {

}

void RenderFacadeClover::FacadeInitEndRace() {
   
}

void RenderFacadeClover::FacadeInitHUD() {

}

void RenderFacadeClover::FacadeUpdatePowerUpHUD(DataMap d) {

}

void RenderFacadeClover::FacadeDrawHUD(Entity* car, ManCar* carsAI) {

}

//Crea las plates de los nombres de los coches
void RenderFacadeClover::FacadeAddPlates(Manager* manNamePlates) {

}

//Actualiza las posiciones de las plates
void RenderFacadeClover::FacadeUpdatePlates(Manager* manNamePlates) {

}
const void RenderFacadeClover::FacadeAddObjects(vector<Entity*> entities) {

}

//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
//TODO: Llevar cuidado con las rutas de las texturas si luego se mueven las carpetas
const uint16_t RenderFacadeClover::FacadeAddObject(Entity* entity) {

}

//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
//TODO: Llevar cuidado con las rutas de las texturas si luego se mueven las carpetas
const uint16_t RenderFacadeClover::FacadeAddObjectCar(Entity* entity) {

}

const uint16_t RenderFacadeClover::FacadeAddObjectTotem(Entity* entity) {

}

//TODO: Esto proximamente le pasaremos todos los entities y los modificará 1 a 1
void RenderFacadeClover::UpdateTransformable(Entity* entity) {

}

//Reajusta la camara
void RenderFacadeClover::UpdateCamera(Entity* cam) {

}

//Añade la camara, esto se llama una sola vez al crear el juego
void RenderFacadeClover::FacadeAddCamera(Entity* camera) {

}

bool RenderFacadeClover::FacadeRun() {

}

uint32_t RenderFacadeClover::FacadeGetTime() {

}

// To-Do: introducir multi input
// Comprobar inputs del teclado
void RenderFacadeClover::FacadeCheckInput() {

}

void RenderFacadeClover::FacadeCheckInputMenu() {

}

void RenderFacadeClover::FacadeCheckInputPause() {

}

void RenderFacadeClover::FacadeCheckInputEndRace() {

}

int RenderFacadeClover::FacadeGetFPS() {

}

void RenderFacadeClover::FacadeSetWindowCaption(std::string title) {

}

//Toda la rutina de limpiar y dibujar de irrlicht
void RenderFacadeClover::FacadeDraw() {

}

void RenderFacadeClover::FacadeDrawMenu() {

}

void RenderFacadeClover::FacadeDrawPause() {

}

void RenderFacadeClover::FacadeDrawEndRace() {

}

//Limpia la pantalla
void RenderFacadeClover::FacadeBeginScene() {

}

void RenderFacadeClover::FacadeDrawAll() {

}

void RenderFacadeClover::FacadeEndScene() {

}

void RenderFacadeClover::FacadeDeviceDrop() {

}

//DEBUG dibuja las aristas entre los nodos del grafo
void RenderFacadeClover::FacadeDrawGraphEdges(ManWayPoint* manWayPoints) {

}

void RenderFacadeClover::Draw3DLine(vec3& pos1, vec3& pos2) const {

}

void RenderFacadeClover::Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const {

}

void RenderFacadeClover::DeleteEntity(Entity* entity) {

}

void RenderFacadeClover::FacadeDrawBoundingPlane(Entity* entity) const {

}

void RenderFacadeClover::FacadeDrawBoundingBox(Entity* entity, bool colliding) {

}

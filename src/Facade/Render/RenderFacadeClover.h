#pragma once

#include "RenderFacade.h"
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>

#include "../../../CLEngine/src/CLEngine.h"

#include <codecvt>
#include <iostream>
#include <locale>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <unordered_map>

using namespace std::chrono;



using namespace CLE;

class RenderFacadeClover : public RenderFacade {
   public:
      RenderFacadeClover();
      ~RenderFacadeClover() override;
      const uint16_t FacadeAddObject(Entity*) override;
      void FacadeAddSphereOnObject(Entity* entity) override;
      const uint16_t FacadeAddObjectCar(Entity*) override;
      const uint16_t FacadeAddObjectTotem(Entity* entity) override;
      const void FacadeAddObjects(std::vector<Entity*>) override;
      void FacadeAddCamera(Entity*) override;
      void UpdateCamera(Entity*, ManCar* manCars) override;
      bool FacadeRun() override;
      uint32_t FacadeGetTime() const override;
      std::vector<Constants::InputTypes> FacadeCheckInputMulti() override;
      void FacadeCheckInputSingle() override;
      int FacadeGetFPS() const override;
      void FacadeSetWindowCaption(std::string, int) const override;
      void FacadeBeginScene() const override;
      void FacadeDrawAll() const override;
      void FacadeEndScene() const override;
      void FacadeDeviceDrop() override;
      void DeleteEntity(Entity*) override;
      void FacadeSetVisibleEntity(Entity*,bool) override;
      void FacadeSetWindowSize(DataMap* d) override;


      void FacadeDraw() const override;
      void FacadeDrawIntro() override;
      void FacadeDrawMenu() override;
      void FacadeDrawSelectCharacter() override;
      void FacadeDrawGameOptions() override;
      void FacadeDrawTournamentOptions() override;
      void FacadeInitResources() override;
      void FacadeDrawPause() override;
      void FacadeDrawEndRace() override;
      void FacadeDrawEndTournament() override;
      void FacadeDrawLobbyMultiConnecting() override;
      void FacadeDrawLobbyMultiExit() override;
      void FacadeDrawLobbyMultiSelChar() override;
      void FacadeDrawLobbyMultiWait() override;
      void FacadeDrawControler() override;
      void FacadeDrawCredits() override;
      void FacadeDrawSettings() override;

      void FacadeInitIntro() override;
      void FacadeInitMenu() override;
      void FacadeInitSelectCharacter() override;
      void FacadeInitGameOptions() override;
      void FacadeInitTournamentOptions() override;
      void FacadeInitPause() override;
      void FacadeInitEndRace() override;
      void FacadeInitEndTournament() override;
      void FacadeInitLobbyMulti() override;
      void FacadeInitControler() override;
      void FacadeInitHUD() override;
      void FacadeInitCredits() override;
      void FacadeInitSettings() override;

      void FacadeCheckInputIntro() override;
      void FacadeCheckInputMenu() override;
      void FacadeCheckInputSelectCharacter() override;
      void FacadeCheckInputGameOptions() override;
      void FacadeCheckInputTournamentOptions() override;
      void FacadeCheckInputPause() override;
      void FacadeCheckInputEndRace() override;
      void FacadeCheckInputEndTournament() override;
      void FacadeCheckInputLobbyMultiConnecting() override;
      void FacadeCheckInputLobbyMultiExit() override;
      void FacadeCheckInputLobbyMultiSelChar() override;
      void FacadeCheckInputLobbyMultiWait() override;
      void FacadeCheckInputControler() override;
      void FacadeCheckInputCredits() override;
      void FacadeCheckInputSettings() override;

      void FacadeUpdatePowerUpHUD(DataMap* d) override;
      void FacadeDrawHUD(Entity* car, ManCar* manCars, Entity* globalClock, ManHUDEvent* manHud, ManGameRules* manGR) override;
      void FacadeSuscribeEvents() override;
      void FacadeSuscribeEventsSettings() override;
      void FacadeAddPlates(Manager* manNamePlates) override;
      void FacadeUpdatePlates(Manager* manNamePlates) override;
      void FacadeUpdateMeshesLoD(vector<shared_ptr<Entity>> entities) override;
      void FacadeUpdateAnimationsLoD(vector<shared_ptr<Entity>> entities) override;
      void FacadeAnimate(vector<shared_ptr<Entity>> entities) override;
      void ThrowEventChangeToMulti(uint16_t IdOnline, const std::vector<uint16_t> IdPlayersOnline) override;
      void FacadeAddSkybox(string right,string left,string top,string bottom,string front,string back) override;
      void FacadeAddShadowMapping(unsigned int lightId) override;
      void CleanScene() override;
      void FacadeUpdateViewport() override;

      void FacadeInitParticleSystem(DataMap* d) const override;
      void FacadeSetParticlesVisibility(DataMap* d) const override;
      void FacadeSetGrassActivate(DataMap* d) const override;
      void FacadeSetShadowsActivate(DataMap* d) const override;

      void FacadeUpdateVisibility(DataMap* d) override;
      bool FacadeOctreeInCamera(float size, const glm::vec3& pos) override {return device->OctreeIncamera(size, pos);};
      void FacadeSetOctreeVisibleById(unsigned int id, bool v) override {device->SetOctreeVisibleById(id, v);};

      float FacadeGetFovActualCamera() override { return device->GetFovActualCamera(); };
      glm::vec3 FacadeGetTargetActualCamera() override { return device->GetTargetActualCamera(); };
      glm::vec3 FacadeGetPositionActualCamera() override { return device->GetPositionActualCamera(); };

      //DEBUG
      void Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const override;
      void Draw3DLine(vec3& pos1, vec3& pos2) const override;
      void Draw2DImage(float x_, float y_, int width_, int height_, float depth_, string file_, bool) const override;
      void FacadeDrawGraphEdges(ManWayPoint* manWayPoints) const override;
      void FacadeDrawBoundingBox(Entity* entity, bool colliding) const override;
      void FacadeDrawBoundingPlane(Entity* entity) const override;
      void FacadeDrawBoundingGround(Entity* entity) const override;
      void FacadeDrawBoundingOBB(Entity* entity) const override;
      void FacadeDrawAIDebug(ManCar* manCars, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) const override;
      void FacadeDrawAIDebugPath(Entity* carAI, ManWayPoint* manWayPoint) const override;

      void SetShowDebug(bool b) override { showDebug = b;};
      void SetShowDebugAI(bool b) override { showAIDebug = b;};
      void SetIDCarAIToDebug(int id) override {idCarAIToDebug = id;};
      void SetCamTarget(glm::vec3 pos) override;

      bool GetShowDebug() override { return showDebug;};
      bool GetShowDebugAI() override { return showAIDebug;};
      int  GetIDCarAIToDebug() override { return idCarAIToDebug;};
      std::tuple<int, int> GetScreenSize() override;

      void SetMenuEndRace(bool b) override { menuER = b; };
      bool GetMenuEndRace() override { return menuER; };
      void SetMenuEndTournament(uint8_t num) override { menuET = num; timeAnimationEnd=system_clock::now(); numShowPanel=0;};
      uint8_t GetMenuEndTournament() override { return menuET; };

      void ResetInputGameOptions() override;
      void ResetInputTournamentOptions() override;
      void ResetInputCharacter() override;

      //Metodos exclusivos de RenderClover
      float GetBoundingByMesh(uint16_t id) {return device->GetBoundingSizeById(id);};
      CLEngine* GetDevice() { return device;};

      inline static bool showDebug = false;
      inline static bool showAIDebug = false;



   private:
      class Animation2D;

      std::string powerUps[7];

      vector<std::string> tipsTexts = { "Si te encuentras perdido, utiliza la camara del totem para localizarlo!" , 
                                   "Utiliza el Robo Jorobo para conseguir el totem de inmediato!",
                                   "El Escudo Merluzo te ayudara a que no te roben el totem!"};

      // En juego
      bool inputShowTable { true };

      //Menu
      int inputMenu { 0 };
      int maxInputMenu { 6 };
      //Pause
      int inputPause { 0 };
      int maxInputPause { 1 };
      //Seleccion de personaje
      int inputSC { 0 };
      int maxInputSC { 5 };
      //End Race
      bool menuER { false };
      int inputER { 0 };
      int maxInputER { 2 };
      //End Tournament
      time_point<system_clock> timeAnimationEnd;
      uint8_t numShowPanel {0};
      int msChange {400};
      uint8_t menuET { 0 };
      int inputET { 0 };
      int maxInputET { 2 };
      //Opciones de partida
      int option { 0 };
      std::vector<int> inputGO {1,1,1,1};
      int maxInputGO[4] {3, 2, 3, 2};
      //Opciones de partida torneo
      int optionTO { 0 };
      std::vector<int> inputTO {1, 1, 1, 1, 1};
      int maxInputTO[5] {3, 2, 3, 2, 2};
      //Ajustes
      int optionSettings { 0 };
      std::vector<int> inputSettings {1,3,1,1,0};     //Sonido, musica, particulas, vegetacion, sombras
      int maxInputSettings[5] {3,3,1,1,1};

      CLEngine* device {nullptr};
      CLNode* smgr {nullptr};
      CLResourceManager* resourceManager {nullptr};
      CLNode* camera1 {nullptr};

      //Animaciones
      unique_ptr<Animation2D> introAnimation {nullptr};
      unique_ptr<Animation2D> powerUpAnimation {nullptr};

      class Animation2D{
         public:
            Animation2D(std::string _path, uint16_t _numFrames, uint16_t _fps);
            ~Animation2D(){};

            void Update();
            void Start();
            void Restart();

            string GetCurrentPath() const { return currentPath + extension; }
            float  GetTime() const { return time; }
            float  GetTimeBetweenFrames() const { return timeBetweenFrames; }
            bool   GetFinished() const {return finished; }

         private:
            string path;
            string extension;
            string currentPath;
            uint16_t numFrames {0};
            uint16_t fps {60};
            float time {0};
            float timeBetweenFrames {0.16};
            int actualFrame {0};
            bool started { false };
            bool finished { false };
            time_point<system_clock> timeStart;


      };
};

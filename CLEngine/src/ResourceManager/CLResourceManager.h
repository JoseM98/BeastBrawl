#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "../../include/glew/glew.h"

#include "CLResourceMesh.h"
#include "CLResourceShader.h"
#include "CLResourceTexture.h"
#include "CLResourceMaterial.h"


#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <thread>

using namespace std;
namespace CLE {

    //! Clase para gestionar la lectura de recursos
    //! Esta clase engloba todos los metodos para leer diferentes tipos de archivos
    //! @note Es singleton
    class CLResourceManager {
        public:
            ~CLResourceManager(){delete instance;};
            static CLResourceManager* GetResourceManager(){
                if(!instance){
                    instance = new CLResourceManager();
                }
                return instance;
            }

            //! Metodo para leer animaciones
            //! @param file Ruta al fichero
            //! @param numKeyFrames Número de keyframes de la animación
            //! @param flipUV Bool para invertir los UV's al leer la textura
            //! @returns Puntero a un CLE::CLResource::CLResourceMesh
            vector<CLResourceMesh*> GetResourceAnimation(const std::string file, uint8_t numKeyFrames, bool flipUV);
            vector<CLResourceMesh*> GetResourceExistingAnimation(const std::string file, uint8_t numKeyFrames, bool flipUV);
            vector<CLResourceMesh*> LoadResourceAnimation(const std::string file, uint8_t numKeyFrames, bool flipUV);
            //! Metodo para leer mallas
            //! @param file Ruta al fichero
            //! @param flipUV Bool para invertir los UV's al leer la textura
            //! @returns Puntero a un CLE::CLResource::CLResourceMesh
            CLResourceMesh* GetResourceMesh(const std::string file, bool flipUV);
            size_t GetResourceMeshIndex(const std::string file);
            //! Metodo para leer mallas
            //! @param file Ruta al fichero
            //! @returns Puntero a un CLE::CLResource::CLResourceMesh
            CLResourceMesh* GetResourceMesh(const std::string file);
            //! Metodo para leer texturas
            //! @param file Ruta al fichero
            //! @param vertically Bool para poner en vertical la textura
            //! @returns Puntero a un CLE::CLResource::CLResourceTexture
            CLResourceTexture* GetResourceTexture(const std::string file, bool vertically);
            //! Metodo para leer texturas
            //! @param file Ruta al fichero
            //! @returns Puntero a un CLE::CLResource::CLResourceTexture
            CLResourceTexture* GetResourceTexture(const std::string file);
            //! Metodo para leer materiales
            //! @param file Ruta al fichero
            //! @param vertically Bool para invertir
            //! @returns Puntero a un CLE::CLResource::CLResourceMaterial
            CLResourceMaterial* GetResourceMaterial(const std::string file, bool vertically);
            //! Metodo para leer materiales
            //! @param file Ruta al fichero
            //! @returns Puntero a un CLE::CLResource::CLResourceMaterial
            CLResourceMaterial* GetResourceMaterial(const std::string file);
            //! Metodo para leer shaders
            //! @param vertex Ruta al vertex shader
            //! @param fragment Ruta al fragment shader
            //! @returns Puntero a un CLE::CLResource::CLResourceShader
            CLResourceShader* GetResourceShader(const std::string vertex, const std::string fragment);
            //! Metodo para leer shaders
            //! @param vertex Ruta al vertex shader
            //! @param fragment Ruta al fragment shader
            //! @param geometry Ruta al geometry shader
            //! @returns Puntero a un CLE::CLResource::CLResourceShader
            CLResourceShader* GetResourceShader(const std::string vertex, const std::string fragment, const std::string geometry);

            //! Libera la memoria de una textura
            //! @param file
            //! @returns bool
            bool DeleteResourceTexture(const std::string file);
            //! Libera la memoria de una malla
            //! @param file
            //! @returns bool
            bool DeleteResourceMesh(const std::string file);
            //! Libera la memoria de un material
            //! @param file
            //! @returns bool
            bool DeleteResourceMaterial(const std::string file);
            
            vector<shared_ptr<CLResourceMesh>> GetMeshes() {return meshes;}

        private:
            CLResourceManager();
            inline static CLResourceManager* instance { nullptr };
            vector<shared_ptr<CLResourceMesh>> meshes;
            vector<shared_ptr<CLResourceTexture>> textures;
            vector<shared_ptr<CLResourceMaterial>> materials;
            vector<shared_ptr<CLResourceShader>> shaders;
            boost::asio::io_context context;
            std::vector<std::shared_ptr<std::thread>> threads;
            std::mutex my_mutex;
    };

}
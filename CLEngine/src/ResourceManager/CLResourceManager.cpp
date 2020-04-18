#include "CLResourceManager.h"

using namespace CLE;

CLResourceManager::CLResourceManager() {
    //TO-DO: Reservar un minimo de espacio
    //meshes = new std::vector<CLResourceMesh*>();
    //shaders = new std::vector<CLResourceShader*>();
}

CLResourceMesh* CLResourceManager::GetResourceMesh(const std::string file){
    return GetResourceMesh(file,false);
}


CLResourceMesh* CLResourceManager::GetResourceMesh(const std::string file, bool flipUV) {
    shared_ptr<CLResourceMesh> resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<meshes.size() && search; ++ i) {
        if (!file.compare(meshes[i]->GetName())) {
            resource = meshes[i];
            search = false;
        }
    }
    if (!resource) {
        resource = make_shared<CLResourceMesh>();
        resource->SetName(file);
        if (resource->LoadFile(file, flipUV)) {
            meshes.push_back(resource);
        }
    }

    return resource.get();
}


vector<CLResourceMesh*> CLResourceManager::GetResourceAnimation(const std::string path, uint8_t numKeyFrames, bool flipUV) {
    string folder = path.substr(0, path.find_last_of("/") + 1);
    string fileName = path.substr(path.find_last_of("/") + 1);
    vector<CLResourceMesh*> keyFrames;
    for(uint8_t i = 1; i <= numKeyFrames; i++) {
        string stringIndex = std::to_string(i);
        // añade ceros al principio para que el fichero no sea 1ojete.obj, sino que sea 001ojete.obj
        string auxIndex = std::string(3 - stringIndex.length(), '0') + stringIndex; 
        string totalPath = folder + auxIndex + fileName;
        CLResourceMesh *resource = GetResourceMesh(totalPath, flipUV);
        keyFrames.push_back(resource);
    }
    return keyFrames;
}

CLResourceTexture* CLResourceManager::GetResourceTexture(const std::string file){
    return GetResourceTexture(file);
}


CLResourceTexture* CLResourceManager::GetResourceTexture(const std::string file, bool vertically) {
    shared_ptr<CLResourceTexture> resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<textures.size() && search; ++ i) {
        if (!file.compare(textures[i]->GetName())) {
            resource = textures[i];
            search = false;
        }
    }
    if (!resource) {
        resource = make_shared<CLResourceTexture>();
        resource->SetName(file);
        if (resource->LoadFile(file, vertically)) {
            textures.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceMaterial* CLResourceManager::GetResourceMaterial(const std::string file) {
    return GetResourceMaterial(file,false);
}

CLResourceMaterial* CLResourceManager::GetResourceMaterial(const std::string file, bool vertically) {
    shared_ptr<CLResourceMaterial> resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<materials.size() && search; ++ i) {
        if (!file.compare(materials[i]->GetName())) {
            resource = materials[i];
            search = false;
        }
    }
    if (!resource) {
        resource = make_shared<CLResourceMaterial>();
        resource->SetName(file);
        if (resource->LoadFile(file, vertically)) {
            materials.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string vertex, const std::string fragment) {
    shared_ptr<CLResourceShader> resource = NULL;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (!vertex.compare(shaders[i]->GetName())) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        cout << "Creo el shader: " << vertex <<endl;
        resource = make_shared<CLResourceShader>();
        resource->SetName(vertex);
        //resource->SetShaderType(type);
        if (resource->LoadFile(vertex,fragment)) {
            shaders.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string vertex, const std::string fragment, const std::string geometry) {
    shared_ptr<CLResourceShader> resource = NULL;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (!vertex.compare(shaders[i]->GetName())) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        cout << "Creo el shader: " << vertex <<endl;
        resource = make_shared<CLResourceShader>();
        resource->SetName(vertex);
        //resource->SetShaderType(type);
        if (resource->LoadFile(vertex,fragment,geometry)) {
            shaders.push_back(resource);
        }
    }

    return resource.get();
}

#include "CLResourceManager.h"

using namespace CLE;

CLResourceManager::CLResourceManager() {
    //TO-DO: Reservar un minimo de espacio
    //meshes = new std::vector<CLResourceMesh*>();
    //shaders = new std::vector<CLResourceShader*>();
}



CLResourceMesh* CLResourceManager::GetResourceMesh(const std::string file) {
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
        if (resource->LoadFile(file)) {
            meshes.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceTexture* CLResourceManager::GetResourceTexture(const std::string file) {
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
        if (resource->LoadFile(file)) {
            textures.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceMaterial* CLResourceManager::GetResourceMaterial(const std::string file) {
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
        if (resource->LoadFile(file)) {
            materials.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string file1, const std::string file2) {
    shared_ptr<CLResourceShader> resource = NULL;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (!file1.compare(shaders[i]->GetName())) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        cout << "Creo el shader: " << file1 <<endl;
        resource = make_shared<CLResourceShader>();
        resource->SetName(file1+file2);
        //resource->SetShaderType(type);
        if (resource->LoadFile(file1,file2)) {
            shaders.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string file1, const std::string file2, const std::string file3) {
    shared_ptr<CLResourceShader> resource = NULL;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (!file1.compare(shaders[i]->GetName())) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        cout << "Creo el shader: " << file1 <<endl;
        resource = make_shared<CLResourceShader>();
        resource->SetName(file1+file2+file3);
        //resource->SetShaderType(type);
        if (resource->LoadFile(file1,file2,file3)) {
            shaders.push_back(resource);
        }
    }

    return resource.get();
}

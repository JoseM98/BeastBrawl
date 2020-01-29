#include "CLNode.h" 

using namespace CLE;

CLNode::CLNode(){

}

CLNode::CLNode(CLEntity* entity){
    this->entity = entity;
}


bool CLNode::AddChild(CLNode* child){
    //Tipica comprobacion de Programacion 2 pero bueno por si acaso
    if(child!=nullptr){
        childs.push_back(child);
        child->SetFather(this);
        return true;
    }
    return false;
}

bool CLNode::RemoveChild(CLNode* child){

    for(unsigned int i = 0; i<childs.size(); ++i){
        if(child == childs[i]){
            childs.erase(childs.begin()+i);
            return true;
        }
    }
    return false;
}

bool CLNode::HasChild(CLNode* child){

    for(auto node : childs){
        if(node == child){
            return true;
        }
    }

    return false;
}


void CLNode::Translate(const glm::vec3 translationVec){

}

void CLNode::Rotate(const glm::vec3 rotationVec){

}

void CLNode::Scale(const glm::vec3 scaleVec){

}

void CLNode::DrawTree(CLNode* root){

    if(root->GetChilds().size()>0){
        //Tiene hijos
        cout << "Nodo " << root->GetEntity()->GetID() << " con hijos: ";
        for(auto nodo : root->GetChilds()){
            cout << nodo->GetEntity()->GetID() << " ";
        }
        cout << "\n";
        for(auto nodo : root->GetChilds()){
            DrawTree(nodo);
        }
    }

    return;
}

//Devuelve el nodo por la id que le mandes
//Lo hace a partir del padre que lo llame, lo suyo es llamarlo siempre con el nodo principal
CLNode* CLNode::GetNodeByID(unsigned int id){
    CLNode* node = nullptr;
    node = GetNodeByIDAux(id, node, this);
    return node;
}

CLNode* CLNode::GetNodeByIDAux(unsigned int id, CLNode* node, CLNode* root){

    if(node!=nullptr) return node; //Caso base, ha encontrado ya al nodo que busca
    if(root->GetChilds().size()>0){
        //Tiene hijos
        for(auto nodo : root->GetChilds()){
            if(nodo->GetEntity()->GetID() == id){
                node = nodo;
                return node;
            }else{
                node = GetNodeByIDAux(id, node, nodo);

            }
        }

    }

    return node;
}

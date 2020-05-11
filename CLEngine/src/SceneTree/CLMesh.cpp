#include "CLMesh.h"

using namespace CLE;

CLMesh::CLMesh() {
}

void CLMesh::Draw(GLuint shaderID) {
    if (material) {
        material->Draw(shaderID);
    }
    if (mesh) {
        
        mesh->Draw(shaderID);
    }
}

void CLMesh::GoToNextKeyFrames() {
    currentKeyFrameIndex++;
    if (currentKeyFrameIndex > keyframes.size() - 1)
        currentKeyFrameIndex = 0;

    nextKeyFrameIndex = currentKeyFrameIndex + 1;
    if (nextKeyFrameIndex > keyframes.size() - 1)
        nextKeyFrameIndex = 0;

    mesh = keyframes[currentKeyFrameIndex];
    nextMesh = keyframes[nextKeyFrameIndex];
}

void CLMesh::Animate() {
    // cambiamos de frame
    GoToNextKeyFrames();
}

void CLMesh::AnimateInterpolated() {
    // si tenemos que cambiar ya de keyFrame...
    if (currentDistance == distanceBetweenKeyFrames[currentKeyFrameIndex]) {
        GoToNextKeyFrames();
        currentDistance = 0;
    }

    float percentTick = std::min(1.0f, (static_cast<float>(currentDistance) / distanceBetweenKeyFrames[currentKeyFrameIndex]));

    // de cada keyframe, recorremos sus mallas
    for (size_t idxMesh = 0; idxMesh < mesh->GetvectorMesh().size(); idxMesh++) {
        auto& prevSubMesh = mesh->GetvectorMeshPtr()->at(idxMesh);
        auto& nextSubMesh = nextMesh->GetvectorMeshPtr()->at(idxMesh);
        for (size_t idxVertex = 0; idxVertex < prevSubMesh.vertices.size(); idxVertex++) {
            auto& prevVertex = prevSubMesh.vertices.at(idxVertex);
            auto& nextVertex = nextSubMesh.vertices.at(idxVertex);

            glm::vec3 position = mix(prevVertex.position, nextVertex.position, percentTick);
            glm::vec3 normal = mix(prevVertex.normal, nextVertex.normal, percentTick);
            prevVertex.animationOffsetPos = position - prevVertex.position;
            prevVertex.animationOffsetNormal = normal - prevVertex.normal;

            // los comentarios de abajo son puramente debug, podrían borrarse
            // cout << "Tenemos un vértice en " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl;
            // cout << "El next un vértice es " << nextVertex.position.x << "," << nextVertex.position.y << "," << nextVertex.position.z << endl;
            // if (idxMesh == 0 && idxVertex == 0) {
            // cout << "El timeElapsed es " << timeElapsed << " y el percentTick es " << percentTick << ". La nueva pos es " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl;
            // cout << "La pos variable local es " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl
            //     << "La pos variable miembro es " << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.x
            //     << "," << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.y
            //     << "," << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.z << endl << endl;
            // }
        }
    }
    currentDistance++;
    // cout << "currentKeyFrameIndex -> " << unsigned(currentKeyFrameIndex) << ", nextKeyFrameIndex -> " << unsigned(nextKeyFrameIndex) << endl;
    // cout << "los archivos cargados son " << mesh->GetName() << " y " << nextMesh->GetName()  << " y la distancia es " << unsigned(distanceBetweenKeyFrames[currentKeyFrameIndex]) << endl;
}

void CLMesh::DrawDepthMap(GLuint shaderID) {
    /*if(material){
        material->Draw(shaderID); 
    }*/
    if (mesh) {
        mesh->DrawDepthMap(shaderID);
    }
}
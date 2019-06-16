#include <cg_model.hpp>

namespace cgicmc {
void Model::load(std::string path){
    loadModel(path);
    createNormalizingMatrix();
    directory = path.substr(0, path.find_last_of('/'));
}

void Model::loadModel(std::string path){
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	
	
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processModelNode(scene->mRootNode,scene);
}

void Model::processModelNode(aiNode *node, const aiScene *scene){
    for(int i=0; i<node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        insertAiMesh(mesh, scene);
    }
    for(int i=0; i<node->mNumChildren; i++){
        processModelNode(node->mChildren[i], scene);
    }
}

void Model::insertAiMesh(aiMesh *m, const aiScene *scene){
    Mesh newMesh;

    // Convertendo dados dos vertices
    for(int i=0; i<m->mNumVertices; i++){
        glm::vec3 vPos, vNormal;
        glm::vec2 vTextC;
        // Coordenadas de posicao dos vertices
        vPos.x = m->mVertices[i].x;
        vPos.y = m->mVertices[i].y;
        vPos.z = m->mVertices[i].z;
        // Normais
        if(m->mNormals){
            vNormal.x = m->mNormals[i].x;
            vNormal.y = m->mNormals[i].y;
            vNormal.z = m->mNormals[i].z;
        }
        else
            vNormal = glm::vec3(0.0f, 0.0f, 0.0f);
        // Coordenadas de textura
        if(m->mTextureCoords[0]){
            vTextC.x = m->mTextureCoords[0][i].x;
            vTextC.y = m->mTextureCoords[0][i].y;
        }
        else
            vTextC = glm::vec2(0.0f, 0.0f);

        newMesh.insertVertice(vPos, vNormal, vTextC);
    }

    // Convertendo dados das faces
    for(int i=0; i<m->mNumFaces; i++){
        aiFace face = m->mFaces[i];
        for(int j=0; j<face.mNumIndices; j++)
            newMesh.insertIndice(face.mIndices[j]);
    }

    std::string texturePathResult;
    if(m->mMaterialIndex >= 0) {
        aiString s;
        aiMaterial* material = scene->mMaterials[m->mMaterialIndex];
        if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0){
            aiString texturePath;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
            texturePathResult = directory + '/' + texturePath.C_Str();
            newMesh.setTextureD(texturePathResult);
        }
        if(material->GetTextureCount(aiTextureType_SPECULAR) > 0){
            aiString texturePath;
            material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath);
            texturePathResult = directory + '/' + texturePath.C_Str();
            newMesh.setTextureD(texturePathResult);
        }
    }

    newMesh.setup();
    meshes.push_back(newMesh);
}

void Model::draw(cgicmc::Shader _shader){
    for(int i=0; i<meshes.size(); i++)
        meshes[i].draw(_shader);
}

void Model::getBoundingValues(glm::vec3 *min, glm::vec3 *max){
    glm::vec3 auxMin, auxMax;
    meshes[0].getBoundingValues(min, max);
    for(int i=1; i<meshes.size(); i++){
        meshes[i].getBoundingValues(&auxMin, &auxMax);
        min->x = (min->x < auxMin.x) ? min->x : auxMin.x;
        min->y = (min->y < auxMin.y) ? min->y : auxMin.y;
        min->z = (min->z < auxMin.z) ? min->z : auxMin.z;
        max->x = (max->x > auxMax.x) ? max->x : auxMax.x;
        max->y = (max->y > auxMax.y) ? max->y : auxMax.y;
        max->z = (max->z > auxMax.z) ? max->z : auxMax.z;
    }
}

Mat4 Model::getNormalizingTransform(){
    return normalizeMatrix;
}

void Model::createNormalizingMatrix(){
	glm::vec3 min, max;
    Mat4 aux;
	this->getBoundingValues(&min, &max);
	float scalingRatio = (max.x-min.x > max.y-min.y) ? max.x-min.x : max.y-min.y;
	scalingRatio = (scalingRatio > max.z-min.z) ? scalingRatio : max.z-min.z;
	scalingRatio = 2/scalingRatio;
	normalizeMatrix.scale(scalingRatio, scalingRatio, scalingRatio);
	aux.translate(-(max.x+min.x)/2, -(max.y+min.y)/2, -(max.z+min.z)/2);
	normalizeMatrix = normalizeMatrix*aux;
}

void Model::show(){
    for(int i=0; i<meshes.size(); i++){
        std::cout << "Mesh: " << i << std::endl;
        meshes[i].show();
    }
}

}
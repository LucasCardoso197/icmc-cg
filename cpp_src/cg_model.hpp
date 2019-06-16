#ifndef __CG_MODEL_HPP__
#define __CG_MODEL_HPP__

// Std includes
#include <iostream>
#include <vector>
// Graphical includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// .obj reading includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// cgicmc includes
#include <shader.hpp>
#include <cg_mesh.hpp>
#include <cg_mat4.hpp>

namespace cgicmc {

class Model {
public:
    Model(std::string path) {
        loadModel(path);
        createNormalizingMatrix();
        directory = path.substr(0, path.find_last_of('/'));
    }
    Model(){};
    void load(std::string path);
    void draw(cgicmc::Shader _shader);
    void getBoundingValues(glm::vec3 *min, glm::vec3 *max);
    Mat4 getNormalizingTransform();
    void show();
private:
    std::vector<Mesh> meshes;
    std::string directory;
    Mat4 normalizeMatrix;

    void loadModel(std::string path);
    void processModelNode(aiNode *node, const aiScene *scene);
    void insertAiMesh(aiMesh *m, const aiScene *scene);
    void createNormalizingMatrix();
};

}

#endif
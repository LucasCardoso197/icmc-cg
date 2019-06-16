#ifndef __CG_MESH_HPP__
#define __CG_MESH_HPP__

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.hpp>
#include <shader.hpp>

namespace cgicmc {

struct Texture {
    unsigned int id;
    std::string path;
    std::string type;
};

class Mesh{
public:
    Mesh(){hasTextureD = 0.0; hasTextureS = 0.0;};
    void insertVertice(const glm::vec3 pos, const glm::vec3 normal, const glm::vec2 textC);
    void insertIndice(const unsigned int i);
    void setTextureD(std::string path);
    void setTextureS(std::string path);
    void setup();
    void draw(cgicmc::Shader _shader);
    void show();
    void getBoundingValues(glm::vec3 *min, glm::vec3 *max);
private:
    std::vector<float> verticeData;
    std::vector<unsigned int> indices;
    struct Texture textureD, textureS;
    int hasTextureD, hasTextureS;
    unsigned int _EBO, _VBO, _VAO;
};

} // namespace cgicmc

#endif
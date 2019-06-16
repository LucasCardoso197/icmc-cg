#include <cg_mesh.hpp>

namespace cgicmc {

void Mesh::insertVertice(const glm::vec3 pos, const glm::vec3 normal, const glm::vec2 textC){
    this->verticeData.push_back(pos.x);
    this->verticeData.push_back(pos.y);
    this->verticeData.push_back(pos.z);
    this->verticeData.push_back(normal.x);
    this->verticeData.push_back(normal.y);
    this->verticeData.push_back(normal.z);
    this->verticeData.push_back(textC.x);
    this->verticeData.push_back(textC.y);
}

void Mesh::insertIndice(const unsigned int i){
    this->indices.push_back(i);
}

void Mesh::setTextureD(std::string path){
    textureD.path = path;
}

void Mesh::setTextureS(std::string path){
    textureS.path = path;
}
void Mesh::setup(){
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);
    // Passando dados para os buffers da OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, verticeData.size() * sizeof(float), verticeData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    // Informando a OpenGL como interpretar o buffer de dados dos vertices
    // v
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // vn
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // vt
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // Carregando a textura difusa e passando para a OpenGL
    if(textureD.path.size() > 0){
        glGenTextures(1, &textureD.id);
        glBindTexture(GL_TEXTURE_2D, textureD.id);
        // Configuração
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Carregando imagem de textura
        int width, height, channels;
        unsigned char *data = stbi_load(textureD.path.c_str(), &width, &height, &channels, 0);
        if(data){
            hasTextureD = 1.0f;
            GLenum aux;
	        if (channels == 1)
	            aux = GL_RED;
	        else if (channels == 3)
	            aux = GL_RGB;
	        else if (channels == 4)
	            aux = GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, aux, width, height, 0, aux, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load diffuse texture image." << std::endl;
        }
        stbi_image_free(data);
    }
    // Carregando a textura especular e passando para OpenGL
    if(textureS.path.size() > 0){
        glGenTextures(1, &textureS.id);
        glBindTexture(GL_TEXTURE_2D, textureS.id);
        // Configuração
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Carregando imagem de textura
        int width, height, channels;
        unsigned char *data = stbi_load(textureS.path.c_str(), &width, &height, &channels, 0);
        if(data){
            hasTextureS = 1.0f;
            GLenum aux;
	        if (channels == 1)
	            aux = GL_RED;
	        else if (channels == 3)
	            aux = GL_RGB;
	        else if (channels == 4)
	            aux = GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, aux, width, height, 0, aux, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load specular texture image." << std::endl;
        }
        stbi_image_free(data);
    }
}

void Mesh::draw(cgicmc::Shader _shader){
    _shader.useShader();
    // Ativando texturas
    glActiveTexture(GL_TEXTURE0);
    if(hasTextureD == 1) //Difusa
        glBindTexture(GL_TEXTURE_2D, textureD.id);
    glUniform1i(glGetUniformLocation(_shader.getId(), "hasTextureD"), hasTextureD);
    glActiveTexture(GL_TEXTURE1);
    if(hasTextureS == 1) //Especular
        glBindTexture(GL_TEXTURE_2D, textureS.id);
    glUniform1i(glGetUniformLocation(_shader.getId(), "hasTextureS"), hasTextureS);
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::show(){
    std::cout << "Vertice data:" << std::endl;
    for(int i=0; i<(verticeData.size()/8); i++){
        std::cout << verticeData[8*i] << " ";
        std::cout << verticeData[8*i+1] << " ";
        std::cout << verticeData[8*i+2] << " ";
        std::cout << verticeData[8*i+3] << " ";
        std::cout << verticeData[8*i+4] << " ";
        std::cout << verticeData[8*i+5] << " ";
        std::cout << verticeData[8*i+6] << " ";
        std::cout << verticeData[8*i+7] << std::endl;
    }

    std::cout << std::endl << "Indices:" << std::endl;
    for(int i=0; i<indices.size(); i++)
        std::cout << indices[i] << std::endl;
}

void Mesh::getBoundingValues(glm::vec3 *min, glm::vec3 *max){
    max->x = verticeData[0];
    min->x = max->x;
    max->y = verticeData[1];
    min->y = max->y;
    max->z = verticeData[2];
    min->z = max->z;

    // Encontrando valores minimos e maximos para cada coordenada
    for(int i=8; i<verticeData.size(); i+=8){
        if(verticeData[i] > max->x) max->x = verticeData[i];
        if(verticeData[i] < min->x) min->x = verticeData[i];
        if(verticeData[i+1] > max->y) max->y = verticeData[i+1];
        if(verticeData[i+1] < min->y) min->y = verticeData[i+1];
        if(verticeData[i+2] > max->z) max->z = verticeData[i+2];
        if(verticeData[i+2] < min->z) min->z = verticeData[i+2];
    }
}

} // namespace cgicmc

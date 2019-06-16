#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform int hasTextureD;
uniform int hasTextureS;

void main() {
    vec4 aux;
    if(hasTextureD == 1) {aux = texture(diffuseTexture, texCoord);}
    else {aux = vec4(0.3294, 0.7216, 0.9686, 1.0);}
    if(hasTextureS == 1) {FragColor = mix(aux, texture(specularTexture, texCoord), 0.5);}
    else {FragColor = aux;}
}
#include <cg_windvane.hpp>

namespace cgicmc {
unsigned int WindVane::loadFragmentShader(){
  const char *fragmentShaderSource;
  fragmentShaderSource = "#version 330 core\n"
                          "out vec4 FragColor;\n"
                          "void main()\n"
                          "{\n"
                          "   FragColor = vec4(0.6f, 0.8f, 0.8f, 1.0f);\n"
                          "}\n\0";
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  int success = 0;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "Fragment shader error: " << infoLog << std::endl;
  }
  return fragmentShader;
}

unsigned int WindVane::loadVertexShader(){
  const char *vertexShaderSource;
  vertexShaderSource =  "#version 330 core\n"
                        "layout(location = 0) in vec3 aPos;\n"
                        "uniform mat4 transform;\n"
                        "void main() {\n"
                        "gl_Position = transform * vec4(aPos, 1.0f);\n"
                        "}\0";
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "Vertex shader error: " << infoLog << std::endl;
  }
  return vertexShader;
}

void WindVane::loadShaders(){
  unsigned int vectorShader = loadVertexShader();
  unsigned int fragmentShader = loadFragmentShader();

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vectorShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "Program shader error: " << infoLog << std::endl;
  }
}

void WindVane::prepare() {
  float vertices[] = {0.0f, 0.0f, 0.0f,
                      -0.7f, 0.0f, 0.0f,
                      -0.7, -0.4, 0.0f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, -0.7f, 0.0f,
                      0.4f, -0.7f, 0.0f,
                      0.0f, 0.0f, 0.0f,
                      0.7f, 0.0f, 0.0f,
                      0.7f, 0.4f, 0.0f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 0.7f, 0.0f,
                      -0.4f, 0.7f, 0.0f,};
  
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void WindVane::run() {

  while (!glfwWindowShouldClose(_window)) {

    // Comandos de entrada
    processInput(_window);

    // Comandos de renderizacao vao aqui
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    float transform[] = {1.0, 0.0, 0.0, 0.0,
                         0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0,
                         0.0, 0.0, 0.0, 1.0};
    
    glUseProgram(shaderProgram);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    // Controla eventos e troca os buffers para renderizacao
    glfwSwapBuffers(_window);
    glfwPollEvents();
  }
}
}
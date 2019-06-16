// Bibliotecas Gráficas
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Auxiliares
#include <glm/glm.hpp>
#include <vector>

// Entrada e saída
#include <iostream>
#include <fstream>

// Bibliotecas cgicmc
#include <shader.hpp>
#include <cg_model.hpp>
#include <cg_mesh.hpp>
#include <cg_mat4.hpp>

// Constantes de operacoes com a janela e entrada/saida
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define TRANSLATE_SPEED 0.3
#define MODEL_DISTANCE 1.5
bool projectionMode = true;

// Estados da câmera
float cameraPos[] = {0.0, 0.0, 1.0};
float cameraFront[] = {0.0, 0.0, -1.0};
float viewUp[] = {0.0, 1.0, 0.0};
float yaw = 0.0f;
float pitch = 0.0f;

// Variaveis para calculo do tempo
float lastTime = 0.0f;
float timeFromLastFrame = 0.0f;

// Variaveis do mouse
#define MOUSE_SENSITIVITY 0.05
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
float lastMousePositionX = WINDOW_WIDTH/2;
float lastMousePositionY = WINDOW_HEIGHT/2;

// Funções de entrada do usuário
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char const *argv[]) {
	// Leitura do diretório dos modelos
	std::ifstream inputFile;
	inputFile.open("scene.txt");
	if (!inputFile){
		std::cerr << "Erro ao abrir arquivo de entrada.\n";
		return 3;
	}

	std::vector <std::string> modelPaths;
	std::string modelPath = "./";
	std::string modelName;
	while(inputFile >> modelName)
		modelPaths.push_back(std::string(modelPath+modelName));
	inputFile.close();

	// Inicialização da janela //
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CG_ICMC_Camera", NULL, NULL);
	if(window == NULL){
		std::cout << "Falha na criação da janela." << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// Configuração de modos e callbacks do glfw
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Habilitando captura do mouse
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
		std::cout << "Falha na inicialização da Glad." << std::endl;
		glfwTerminate();
		return 2;
	}

	/////////////////////////////////////////////////
	// Inicialização do modelo
	cgicmc::Model *models = new cgicmc::Model[modelPaths.size()];
	for(int i=0; i<modelPaths.size(); i++)
		models[i].load(modelPaths[i]);

	// Inicialização de shaders
	cgicmc::Shader _shader;
	_shader.loadVertexShader("./assets/shaders/camera.vert");
	_shader.loadFragmentShader("./assets/shaders/camera.frag");

	// OpenGL settings
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	// Cálculo das matrizes de transformações de projeção
	float *projection;
	cgicmc::Mat4 perspectiveProjectionM;
	perspectiveProjectionM.perspetiveProjection(WINDOW_HEIGHT, WINDOW_WIDTH, 45.0, 0.1f, 100.0f);
	cgicmc::Mat4 ortogonalProjectionM;
	float ortogonalBoxMax[] = {1.0f, 1.0f, 10.0f};
	float ortogonalBoxMin[] = {-1.0f, -1.0f, 0.f};
	ortogonalProjectionM.ortogonalProjection(ortogonalBoxMax, ortogonalBoxMin);

	// Passando uniformes de textura
	_shader.useShader(); 
    glUniform1i(glGetUniformLocation(_shader.getId(), "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(_shader.getId(), "specularTexture"), 1);

	// Loop de execução //
	cgicmc::Mat4 viewTransform;
	float currentTime, *model, lookAtPoint[3], *view;
	while(!glfwWindowShouldClose(window)){
		processInput(window);

		// Controle de tempo para processamento de comandos
		currentTime = glfwGetTime();
		timeFromLastFrame = currentTime - lastTime;
		lastTime = currentTime;

		// Calculando transformacoes de visão
		for(int i=0; i<3; i++) lookAtPoint[i] = cameraPos[i] + cameraFront[i];
		viewTransform.view(cameraPos, lookAtPoint, viewUp);
		view = viewTransform.getData();

		// Renderização
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Passando as uniforms para o vertex shader
		_shader.useShader();
		// Visão
		unsigned int viewLoc = glGetUniformLocation(_shader.getId(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
		// Projeção
		if(projectionMode) projection = perspectiveProjectionM.getData();
		else projection = ortogonalProjectionM.getData();
		unsigned int projectionLoc = glGetUniformLocation(_shader.getId(), "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection);

		// Desenhando os modelos
		for(int i=0; i<modelPaths.size(); i++){
			// Geração da matriz de modelo
			cgicmc::Mat4 modelMatrix = models[i].getNormalizingTransform();
			cgicmc::Mat4 aux;
			aux.translate((float)i*MODEL_DISTANCE, 0.0, 0.0);
			modelMatrix = aux * modelMatrix;
			model = modelMatrix.getData();
			// Passando valor para o shader
			unsigned int transformLoc = glGetUniformLocation(_shader.getId(), "model");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, model);
			// Desenhando de fato
			models[i].draw(_shader);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	///////////////////////////

	// Finalizações
	delete models;
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Translação da câmera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		for(int i=0; i<3; i++)
			cameraPos[i] += TRANSLATE_SPEED*timeFromLastFrame*cameraFront[i];
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		for(int i=0; i<3; i++)
			cameraPos[i] -= TRANSLATE_SPEED*timeFromLastFrame*cameraFront[i];
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		float rightVersor[3];
		crossProduct(viewUp, cameraFront, rightVersor);
		normalizeVector(rightVersor);
		for(int i=0; i<3; i++)
			cameraPos[i] -= TRANSLATE_SPEED*timeFromLastFrame*rightVersor[i];
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		float rightVersor[3];
		crossProduct(viewUp, cameraFront, rightVersor);
		normalizeVector(rightVersor);
		for(int i=0; i<3; i++)
			cameraPos[i] += TRANSLATE_SPEED*timeFromLastFrame*rightVersor[i];
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		float rightVersor[3];
		crossProduct(viewUp, cameraFront, rightVersor);
		normalizeVector(rightVersor);
		float upVersor[3];
		crossProduct(cameraFront, rightVersor, upVersor);
		for(int i=0; i<3; i++)
			cameraPos[i] += TRANSLATE_SPEED*timeFromLastFrame*upVersor[i];
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		float rightVersor[3];
		crossProduct(viewUp, cameraFront, rightVersor);
		normalizeVector(rightVersor);
		float upVersor[3];
		crossProduct(cameraFront, rightVersor, upVersor);
		for(int i=0; i<3; i++)
			cameraPos[i] -= TRANSLATE_SPEED*timeFromLastFrame*upVersor[i];
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	float mouseDifX = MOUSE_SENSITIVITY * (xpos-lastMousePositionX);
	float mouseDifY = MOUSE_SENSITIVITY * (ypos-lastMousePositionY);
	lastMousePositionX = xpos;
	lastMousePositionY = ypos;

	yaw -= mouseDifX;
	pitch -= mouseDifY;

	if(pitch >= 89.9) pitch = 89.9;
	if(pitch <= -89.9) pitch = -89.9;

	float newCameraFront[3];
	newCameraFront[0] = cos(yaw*M_PI/180) * cos(pitch*M_PI/180);
    newCameraFront[1] = sin(pitch*M_PI/180);
    newCameraFront[2] = sin(yaw*M_PI/180) * cos(pitch*M_PI/180);
	normalizeVector(newCameraFront);
    for(int i=0; i<3; i++) cameraFront[i] = newCameraFront[i];
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        projectionMode = !projectionMode;
}
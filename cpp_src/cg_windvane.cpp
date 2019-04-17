#include <cg_windvane.hpp>

namespace cgicmc {
WindVane::WindVane() : cgicmc::Window() {
	// Inicializa no centro com tamanho padrao e rotacao 100 graus por segundo
	xCenter = 0.0f;
	yCenter = 0.0f;
	size = 1.0f;
	rotationSpeed = 100;
};

bool WindVane::rotate = true;

void WindVane::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) rotate = !rotate;
}

unsigned int WindVane::loadFragmentShader(){
	const char *fragmentShaderSource;
	fragmentShaderSource = "#version 330 core\n"
													"out vec4 FragColor;\n"
													"void main()\n"
													"{\n"
													"   FragColor = vec4(0.3f, 0.74f, 0.74f, 1.0f);\n"
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
	float size1 = VANE_SIZE * 0.3;
	float size2 = VANE_SIZE * 0.6; 
	float vertices[] = {					/*Center square*/
											-size1, size1, 0.0f,
											-size1, -size1, 0.0f,
											size1, size1, 0.0f,
											-size1, -size1, 0.0f,
											size1, -size1, 0.0f,
											size1, size1, 0.0f,
											/*Vertical edge triangles*/
											0.0f, size1, 0.0f,
											0.0f, size2, 0.0f,
											-size1, size1, 0.0f,
											0.0f, -size1, 0.0f,
											0.0f, -size2, 0.0f,
											size1, -size1, 0.0f,
											/*Horizontal edge triangles*/
											size1, 0.0f, 0.0f,
											size2, 0.0f, 0.0f,
											size1, size1, 0.0f,
											-size1, 0.0f, 0.0f,
											-size2, 0.0f, 0.0f,
											-size1, -size1, 0.0f};
	
	// Preparando VBO e VAO
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

	// Setando callback para utilizacao mais limpa da tecla espaco para parar rotacao
    glfwSetKeyCallback(_window, key_callback);
}

void WindVane::run() {
	Mat4 M, N, O, result;
	float angle = 0, time = 0;
	int angleI;
	while (!glfwWindowShouldClose(_window)) {

		// Comandos de entrada
		processInput(_window);

		// Comandos de renderizacao
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Geracao da matriz de transformacoes
		M.translate(xCenter, yCenter, 0.0f);
		if(rotate)
			// Velocidade esta em graus/segundo
			angle += rotationSpeed*(glfwGetTime() - time);
		time = glfwGetTime();
		N.rotateZAxis(angle);
		O.scale(size, size, 0);
		result = M*O;
		result = result*N;
		float *transform = result.getData();
		
		// Chamando shaders
		glUseProgram(shaderProgram);
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform);

		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);

		// Controla eventos e troca os buffers para renderizacao
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void WindVane::processInput(GLFWwindow *window){
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, true);
	// Comando de translacao
	if(glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
		yCenter += TRANSLATION_SPEED;
	if(glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
		yCenter -= TRANSLATION_SPEED;
	if(glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
		xCenter -= TRANSLATION_SPEED;
	if(glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
		xCenter += TRANSLATION_SPEED;

	// Comandos de escala
	if(glfwGetKey(_window, GLFW_KEY_Z) == GLFW_PRESS)
		size += TRANSLATION_SPEED;
	if(glfwGetKey(_window, GLFW_KEY_X) == GLFW_PRESS)
		size -= TRANSLATION_SPEED;

	// Comandos para controle da velocidade de rotacao
	if(glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS){
		if(rotationSpeed > -MAX_ROTAION_SPEED)
			rotationSpeed -= ROTATION_ACELERATION;
		rotate = true; // Volta a girar se mudar velocidade
	}
	if(glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS){
		if(rotationSpeed < MAX_ROTAION_SPEED)
			rotationSpeed += ROTATION_ACELERATION;
		rotate = true;
	}
}
}
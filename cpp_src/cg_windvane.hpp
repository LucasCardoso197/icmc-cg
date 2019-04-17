#ifndef __CG_WINDVANE_HPP__
#define __CG_WINDVANE_HPP__

#include <cg_window.hpp>
#include <cg_mat4.hpp>

// Tamanho padrao do catavento
#define VANE_SIZE 0.5

// Velocidade de translacao com as teclas WASD e escala com ZX
#define TRANSLATION_SPEED 0.05

// Aceleracao com as teclas QE
#define ROTATION_ACELERATION 20
// Velocidade maxima de rotacao
#define MAX_ROTAION_SPEED 1800

namespace cgicmc {

class WindVane : public cgicmc::Window  {
public:
	///
	/// Instantiates windvane environment
	WindVane();

	///
	/// Carrega os shaders e callback de tecla
	void prepare();

	///
	/// Run the application in a loop.
	void run() override;

	///
	/// Compile and load OpenGL shaders
	void loadShaders();


	///
	/// Metodo estatico para utilizacao como callback
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	unsigned int shaderProgram, _VAO;
	
	// Velocidade de rotacao do catavento, dado em graus/s
	int rotationSpeed;

	// Posicao do centro do catavento e seu tamanho
	float xCenter, yCenter, size;

	// Booleano estatico que ativa/desativa a rotacao do catavento
	// Alterado no callback de teclas
	static bool rotate;

	// Funcoes auxiliares que carregam os shaders, chamadas na funcao prepare
	unsigned int loadVertexShader();
	unsigned int loadFragmentShader();

	///
	/// Processamento de teclas pressionadas durante a execucao(funcao run)
	void processInput(GLFWwindow *window) override;
};
} // namespace cgicmc

#endif
#ifndef __CG_WINDVANE_HPP__
#define __CG_WINDVANE_HPP__

#include <cg_window.hpp>

namespace cgicmc {

class WindVane : public cgicmc::Window  {
public:
  ///
  /// Instantiates windvane environment
  WindVane() : cgicmc::Window() {};

  void prepare();

  ///
  /// Run the application in a loop.
  void run() override;

  ///
  /// Compile and load OpenGL shaders
  void loadShaders();

protected:
  unsigned int shaderProgram, _VAO;

private:
  unsigned int loadVertexShader();
  unsigned int loadFragmentShader();
};
} // namespace cgicmc

#endif
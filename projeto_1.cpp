#include <cg_windvane.hpp>

int main(int argc, char const *argv[]) {
  cgicmc::WindVane window;
  window.createWindow();
  window.loadShaders();
  window.prepare();
  window.run();
}
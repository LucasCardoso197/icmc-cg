# Computação Gráfica - ICMC 2019

Para a compilação do projeto com uso da biblioteca Assimp foi necessário o download do repositório correspondente no github (está contido na pasta external que foi enviada junto do projeto), e também foi necessária a instalação dos seguintes pacotes em ambiente Linux(além dos já necessários para utilização da glad/glfw):

```
libassimp-dev
libdevil-dev
libqt5opengl5-dev
qt5-default
```

Então com as adaptações ao CMake já contido no diretório importado cg-icmc a compilação permanece a mesma, isto é:

```
mkdir build
cd build
cmake ..
make
```

Por fim, copie a pasta ‘assets’ para o diretório build que acabou de criar, pois ela contém os arquivos que serão usados pelo programa, como os shaders e modelos 3d.

Os arquivo executável gerados (`projeto2CPP`) podem ser rodados da seguinte forma:
./projeto2CPP nomeDoModel.obj
Se o modelo de formato .obj estiver contido no diretório /build/assets/3d_models/ o programa deve funcionar normalmente.
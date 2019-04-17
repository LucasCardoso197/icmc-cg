# Computação Gráfica - ICMC 2019

Para compilar os arquivos, crie uma pasta local (na pasta do repositório) chamada _build_:

```
mkdir build
cd build
```

Em seguida execute os comandos do _CMake_ para compilar e executar o exemplo

```
cmake ..
make
```

São necessários as bibliotecas da GLAD, GLFW, OpenGL, para compilação com sucesso.

Os arquivos executáveis gerados (`projeto1CPP`) podem ser rodados para testar a compilação do projeto.
Deve aparecer um catavendo girando em um fundo azul na janela.

Comandos da aplicação:
- W: Catavento se move para cima
- S: Catavento se move para baixo
- A: Catavento se move para a esquerda
- D: Catavento se move para a direita
- Z: Aumenta o tamanho do catavento
- X: Diminui o tamanho do catavento
- Q: Aumenta rotação horária/Diminui rotação anti-horária
- E: Aumenta rotação anti-horária/Diminui rotação horária
- Espaço: Para/Resume a rotação do catavento
- ESC: Fecha a janela
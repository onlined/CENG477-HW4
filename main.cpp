#include "helper.h"
#include <iostream>

static GLFWwindow* win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;

int widthTexture, heightTexture;


static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void renderFunction()
{
	int width = 4;
	int height = 2;
	for (int i=0; i<=height; i++)
	{
		for (int j=0; j<=width; j++)
		{
			int xpos = j;
			int zpos = height - i;
			std::cout << "For vertex (" << j << "," << i << ") : x = " << xpos << ", z = " << zpos << std::endl;
		}
	}
	std::cout << std::endl;
		std::cout << 0 << std::endl;

	for (int j=0; j<height; j++)
	{
		std::cout << (j+1) * (width+1) << std::endl << j * (width+1) + 1 << std::endl;
		for (int i=1; i<=width; i++)
		{
		    if (i != width)
				std::cout << (j+1) * (width+1) + i << std::endl << j * (width+1) + i + 1 << std::endl;
			else if (j != height - 1)
			{
				std::cout << (j+1) * (width+1) + i << std::endl;
				std::cout << (j+1) * (width+1) + i << std::endl;
				std::cout << (j) * (width+1) + i + 1 << std::endl;
				std::cout << (j) * (width+1) + i + 1 << std::endl;
			}
			else
			{
				std::cout << (j+1) * (width+1) + i << std::endl;


			}
		}
	}

}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Please provide only a texture image\n");
    exit(-1);
  }

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) {
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

  win = glfwCreateWindow(600, 600, "CENG477 - HW4", NULL, NULL);

  if (!win) {
      glfwTerminate();
      exit(-1);
  }
  glfwMakeContextCurrent(win);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

      glfwTerminate();
      exit(-1);
  }

  initShaders();
  glUseProgram(idProgramShader);
  initTexture(argv[1], &widthTexture, &heightTexture);


  renderFunction();
  while(!glfwWindowShouldClose(win)) {
    glfwSwapBuffers(win);
    glfwPollEvents();

  }


  glfwDestroyWindow(win);
  glfwTerminate();

  return 0;
}

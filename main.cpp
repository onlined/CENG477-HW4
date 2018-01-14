#include "helper.h"
#include <iostream>
#include <vector>
#include "glm/glm.hpp"

static GLFWwindow* win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;
GLuint idCameraPosition;
GLuint idHeightFactor;

int widthTexture, heightTexture;
int indexes_size;

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void initVertices()
{
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glEnableVertexAttribArray(0);

    GLuint vertexAttribBuffer, indexBuffer;
	glGenBuffers(1, &vertexAttribBuffer);
	glGenBuffers(1, &indexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	int width = widthTexture;
	int height = heightTexture;
    std::vector<float> vertices;
	for (int i=0; i<=height; i++)
	{
		for (int j=0; j<=width; j++)
		{
			int xpos = j;
			int zpos = height - i;
            vertices.push_back(xpos);
            vertices.push_back(0);
            vertices.push_back(zpos);
			//std::cout << "For vertex (" << j << "," << i << ") : x = " << xpos << ", z = " << zpos << std::endl;
		}
	}

    std::vector<GLuint> indexes;
    indexes.push_back(0);
	//std::cout << std::endl;
	//	std::cout << 0 << std::endl;

	for (int j=0; j<height; j++)
	{
        indexes.push_back((j+1) * (width+1));
        indexes.push_back(j * (width+1) + 1);
		//std::cout << (j+1) * (width+1) << std::endl << j * (width+1) + 1 << std::endl;
		for (int i=1; i<=width; i++)
		{
		    if (i != width)
            {
                indexes.push_back((j+1) * (width+1) + i);
                indexes.push_back(j * (width+1) + i + 1);
				//std::cout << (j+1) * (width+1) + i << std::endl << j * (width+1) + i + 1 << std::endl;
            }
			else if (j != height - 1)
			{
				//std::cout << (j+1) * (width+1) + i << std::endl;
				//std::cout << (j+1) * (width+1) + i << std::endl;
				//std::cout << (j) * (width+1) + i + 1 << std::endl;
				//std::cout << (j) * (width+1) + i + 1 << std::endl;
                indexes.push_back((j+1) * (width+1) + i);
                indexes.push_back((j+1) * (width+1) + i);
                indexes.push_back((j) * (width+1) + i + 1);
                indexes.push_back((j) * (width+1) + i + 1);
			}
			else
			{
				//std::cout << (j+1) * (width+1) + i << std::endl;
                indexes.push_back((j+1) * (width+1) + i);
			}
		}
	}

    indexes_size = indexes.size();

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_size * sizeof(indexes[0]), indexes.data(), GL_STATIC_DRAW);
}

void renderFunction()
{
    idCameraPosition = glGetUniformLocation(idProgramShader, "idCameraPosition");
    GLfloat cameraPosition[4] = {widthTexture/2, widthTexture+heightTexture, heightTexture/2, 0};
    glUniform4fv(idCameraPosition, 1, cameraPosition);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glDrawElements(GL_TRIANGLE_STRIP, indexes_size, GL_UNSIGNED_INT, 0);
}

void MessageCallback( GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar* message,
                      const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
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

  glEnable( GL_DEBUG_OUTPUT );
  glDebugMessageCallback( (GLDEBUGPROC) MessageCallback, 0 );

  initShaders();

  idMVPMatrix = glGetUniformLocation(idProgramShader, "MVP");
  idCameraPosition = glGetUniformLocation(idProgramShader, "idCameraPosition");
  idHeightFactor = glGetUniformLocation(idProgramShader, "heightFactor");

  glUseProgram(idProgramShader);
  initTexture(argv[1], &widthTexture, &heightTexture);


  initVertices();
  while(!glfwWindowShouldClose(win)) {
    glfwSwapBuffers(win);
    renderFunction();
    glfwPollEvents();
  }


  glfwDestroyWindow(win);
  glfwTerminate();

  return 0;
}

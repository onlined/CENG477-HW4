#include "helper.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

static GLFWwindow* win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;
GLuint idCameraPosition;
GLuint idHeightFactor;
GLuint idWidthTexture;
GLuint idHeightTexture;
GLuint idPosition;

int widthTexture, heightTexture;
int indexes_size;

glm::mat4 projectionMatrix;
glm::mat4 viewingMatrix;
glm::mat4 modelingMatrix;
glm::vec3 camera_gaze(0, 0, 1);
glm::vec3 camera_pos(0, 0, 0);
glm::vec3 camera_up(0, 1, 0);

GLfloat heightFactor = 10.0f;

std::unordered_map<int, bool> states;
bool fullscreen = false;
int windowHeight = 600;
int windowWidth = 600;
int windowX = 0;
int windowY = 0;

float speed = 0.0f;

#ifdef INEK
int profile = GLFW_OPENGL_COMPAT_PROFILE;
#else
int profile = GLFW_OPENGL_ANY_PROFILE;
#endif

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void initVertices()
{
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    idPosition = glGetAttribLocation(idProgramShader, "position");
    glEnableVertexAttribArray(idPosition);

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
			int zpos = i;
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
    glVertexAttribPointer(idPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    float fovyRad = M_PI / 4;
	  projectionMatrix = glm::perspective(fovyRad, 1.0f, 0.1f, 1000.0f);
    camera_pos = glm::vec3(widthTexture/2, widthTexture/10, -widthTexture/4);
    camera_up = glm::vec3(0, 1, 0);
    camera_gaze = glm::vec3(0, 0, 1);
}

void makeChanges()
{
    if (states[GLFW_KEY_O])
        heightFactor += 0.5;
    if (states[GLFW_KEY_L])
        heightFactor -= 0.5;
    if (states[GLFW_KEY_A] || states[GLFW_KEY_D])
    {
        const float const_angle = 0.5 * M_PI / 180.0;
        float angle = 0.0f;
        if (states[GLFW_KEY_A])
            angle += const_angle;
        if (states[GLFW_KEY_D])
            angle -= const_angle;
        auto left = glm::cross(camera_up, camera_gaze);
        auto rot_matrix = glm::rotate(glm::mat4(1.0), angle, camera_up);
        camera_gaze = glm::normalize(glm::vec3(rot_matrix * glm::vec4(camera_gaze, 0.0)));
    }
    if (states[GLFW_KEY_S] || states[GLFW_KEY_W])
    {
        const float const_angle = 0.5 * M_PI / 180.0;
        float angle = 0.0f;
        if (states[GLFW_KEY_W])
            angle -= const_angle;
        if (states[GLFW_KEY_S])
            angle += const_angle;
        auto left = glm::cross(camera_up, camera_gaze);
        auto rot_matrix = glm::rotate(glm::mat4(1.0), angle, left);
        camera_gaze = glm::normalize(glm::vec3(rot_matrix * glm::vec4(camera_gaze, 0.0)));
        camera_up = glm::cross(camera_gaze, left);
    }
    if (states[GLFW_KEY_U])
        speed += 0.01;
    if (states[GLFW_KEY_J])
        speed -= 0.01;
}

void renderFunction()
{
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    makeChanges();

    camera_pos += glm::normalize(camera_gaze) * speed;

  	viewingMatrix = glm::lookAt(camera_pos, camera_gaze + camera_pos, camera_up);

    glUniform4f(idCameraPosition, camera_pos[0], camera_pos[1], camera_pos[2], 0);
    glUniformMatrix4fv(idMVPMatrix, 1, GL_FALSE, &(projectionMatrix * viewingMatrix)[0][0]);
    glUniform1f(idHeightFactor, heightFactor);
    glUniform1i(idWidthTexture, widthTexture);
    glUniform1i(idHeightTexture, heightTexture);

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

void toggleFullscreen() {
    fullscreen = !fullscreen;
    GLFWmonitor *monitor;
    const GLFWvidmode *mode;
    if (fullscreen)
    {
        glfwGetWindowSize(win, &windowWidth, &windowHeight);
        glfwGetWindowPos(win, &windowX, &windowY);
        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
    }
    glfwSetWindowMonitor(
        win,
        fullscreen ? monitor : nullptr,
        fullscreen ? 0 : windowX,
        fullscreen ? 0 : windowY,
        fullscreen ? mode->width : windowWidth,
        fullscreen ? mode->height : windowHeight,
        fullscreen ? mode->refreshRate : 0
    );
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_RELEASE)
        return;
    if (action == GLFW_PRESS) // Buttons without hold
    {
        switch (key)
        {
        case GLFW_KEY_F:
            toggleFullscreen();
            return;
        }
    }
    states[key] = action == GLFW_PRESS;
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
  glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

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

  glfwSetKeyCallback(win, keyCallback);
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback((GLDEBUGPROC) MessageCallback, 0);

  initShaders();

  idMVPMatrix = glGetUniformLocation(idProgramShader, "MVP");
  idCameraPosition = glGetUniformLocation(idProgramShader, "idCameraPosition");
  idHeightFactor = glGetUniformLocation(idProgramShader, "heightFactor");
  idWidthTexture = glGetUniformLocation(idProgramShader, "widthTexture");
  idHeightTexture = glGetUniformLocation(idProgramShader, "heightTexture");
  idCameraPosition = glGetUniformLocation(idProgramShader, "idCameraPosition");

  glUseProgram(idProgramShader);
  initTexture(argv[1], &widthTexture, &heightTexture);

  glEnable(GL_CULL_FACE);
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

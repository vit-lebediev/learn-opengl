#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include <iostream>
#include <cmath>

const char *vertexShaderSource = R"glsl(
  #version 150 core

  in vec2 position;
  in vec3 color;
  in vec2 texcoord;

  out vec3 Color;
  out vec2 Texcoord;

  void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    Color = color;
    Texcoord = texcoord;
  }
)glsl";

const char *fragmentShaderSource = R"glsl(
  #version 150 core

  in vec3 Color;
  in vec2 Texcoord;

  out vec4 outColor;

  uniform sampler2D texKitten;
  uniform sampler2D texPuppy;
  uniform float timeDiff;

  void main() {
    if (Texcoord.y > 0.5) {
      outColor = texture(texKitten, vec2(Texcoord.x + sin(Texcoord.y * 50 + mod(timeDiff, 25)) / 15.0, 1.0 - Texcoord.y));
//      outColor = texture(texKitten, vec2(Texcoord.x + sin(Texcoord.y * 60.0 + timeDiff * 2.0) / 30.0, 1.0 - Texcoord.y)) * vec4(0.7, 0.7, 1.0, 1.0);
    } else {
      outColor = texture(texKitten, Texcoord);
    }
  }
)glsl";

int main () {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();

  GLuint vao; // Vertex Array Object
  glGenVertexArrays(1, &vao);

  // As soon as you've bound a certain VAO, every time you call
  // glVertexAttribPointer, that information will be stored in that VAO.
  //
  // Since only calls after binding a VAO stick to it, make sure
  // that you've created and bound the VAO at the start of your
  // program. Any vertex buffers and element buffers bound
  // before it will be ignored.
  glBindVertexArray(vao);

  // Defining vertex data
  float vertices[] = {
  //  Position      Color             Texcoords
      -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
       0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
      -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
  };

  GLuint vbo; // Vertex Buffer Object
  glGenBuffers(1, &vbo); // Generate 1 buffer

  glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind Vertex Buffer Object
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Load vertex data to binded buffer

  GLuint ebo; // Element Buffer Object
  glGenBuffers(1, &ebo);

  GLuint elements[] = {
      0, 1, 2,
      2, 3, 0
  };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);





//  glGenerateMipmap(GL_TEXTURE_2D);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  GLint status;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    // Shader failed to compile, retrieving log...
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, nullptr, buffer);
    std::cout << "Vertex Shader Issues!" << std::endl;
    std::cout << buffer << std::endl;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    // Shader failed to compile, retrieving log...
    char buffer[512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, buffer);
    std::cout << "Fragment Shader Issues!" << std::endl;
    std::cout << buffer << std::endl;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  // Since a fragment shader is allowed to write to multiple buffers,
  // you need to explicitly specify which output is written to which buffer.
  // This needs to happen before linking the program. However, since this is
  // 0 by default and there's only one output right now, the following line
  // of code is not necessary:
  glBindFragDataLocation(shaderProgram, 0,
                         "outColor"); // Use glDrawBuffers when rendering to multiple buffers, because only the first output will be enabled by default.

  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram); // Just like a vertex buffer, only one program can be active at a time.

  // A shader object can be deleted with glDeleteShader, but it will not
  // actually be removed before it has been detached from all programs
  // with glDetachShader

  // The location is a number depending on the order of the input definitions.
  // The first and only input position in this example will always have location 0.
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);

  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);

  GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
  glEnableVertexAttribArray(texAttrib);

  // This function also stores VBO currently bound to GL_ARRAY_BUFFER.
  // This means that with next invocation we can bind different buffer
  glVertexAttribPointer(
      posAttrib, // reference to input
      2,         // number of values for input (number of components of vec)
      GL_FLOAT,  // type of each component
      GL_FALSE,  // whether imput values should be normalized between -1.0 and 1.0
      7 * sizeof(GLfloat), // stride (length of each attributes sub-array, 0 - no data between data attributes)
      nullptr              // offset (how many bytes from the start of each attributes "sub-array" the attributes occur)
  );
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *) (5 * sizeof(GLfloat)));

  // Set up & Load textures
  GLuint textures[0];
  glGenTextures(2, textures);

  int width, height;
  unsigned char *image;

  // Load image of a kitten as a texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);

  image = SOIL_load_image("../resources/images/kitten.png", &width, &height, nullptr, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load image of a puppy as a texture
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);

  image = SOIL_load_image("../resources/images/puppy.png", &width, &height, nullptr, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


  GLenum err;
  std::string errorStr;

  GLuint timeDiff = glGetUniformLocation(shaderProgram, "timeDiff");

  auto t_start = std::chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();

    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

    glUniform1f(timeDiff, time);

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

    // check for errors
    while ((err = glGetError()) != GL_NO_ERROR) {
      switch (err) {
        case GL_INVALID_ENUM:      errorStr = "Invalid enum"; break;
        case GL_INVALID_VALUE:     errorStr = "Invalid value"; break;
        case GL_INVALID_OPERATION: errorStr = "Invalid operation"; break;
        case GL_STACK_OVERFLOW:    errorStr = "Stack overflow"; break;
        case GL_STACK_UNDERFLOW:   errorStr = "Stack underflow"; break;
        case GL_OUT_OF_MEMORY:     errorStr = "Out of memory"; break;
        default:                   errorStr = "Unknown error"; break;
      }

      std::cout << "GL Error: " << errorStr.c_str() << std::endl;
    }
  }

  glDeleteTextures(1, textures);

  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &vbo);

  glDeleteVertexArrays(1, &vao);

  glfwTerminate();

  return 0;
}

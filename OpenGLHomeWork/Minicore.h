#pragma once
#include <GL/glew.h>
//宏定义输出错误信息
#define ERROR_LOG(ErrorMessage) std::cout << ErrorMessage << std::endl;glfwTerminate();exit(-1);
#define WINDOWWIDTH 1500
#define WINDOWHEIGHT 1000

typedef void(*CheckStatus)(GLuint, GLenum, GLint*);
typedef void(*ReadLog)(GLuint, GLsizei, GLsizei*, GLchar*);
#define CheckErrorAndLog(CheckStatus, ReadLog) CheckStatus(obj, checkStatus, &success);if (!success){GLchar InfoLog[1024];ReadLog(obj, 1024, NULL, InfoLog);std::cout << "ERROR::SHADER::PROGRAM_FAILED\n" << InfoLog<< std::endl;}

#include "ShaderProgram.h"



ShaderProgram::ShaderProgram()
{
}

void ShaderProgram::CreateProgram()
{
	ProgramID = glCreateProgram();
}


ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(ProgramID);
}
void ShaderProgram::AddShader(Shader shader)
{
	glAttachShader(ProgramID, shader.ShaderID);
}
void ShaderProgram::LinkProgram()
{
	glLinkProgram(ProgramID);
}
void ShaderProgram::UseProgram()
{
	glUseProgram(ProgramID);
}

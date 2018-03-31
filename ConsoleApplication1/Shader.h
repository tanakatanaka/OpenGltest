#pragma once

#include <stdio.h>
#include <string>

//êîäwä÷åW
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

namespace GLSLShader 
{
    enum GLSLShaderType 
	{
        VERTEX,
		FRAGMENT, 
		GEOMETRY,
        TESS_CONTROL,
		TESS_EVALUATION
    };
};

class Shader
{
private:
	int handle;
	bool linked;
	std::string logString;

	int GetUniformLocation(const char * Name);
	bool FileExists(const std::string & FileName);

public:
	Shader();
	std::string Load(const char *path);
	
	bool   CompileShaderFromFile( const char * fileName, GLSLShader::GLSLShaderType type );
	bool   CompileShaderFromString( const std::string & source, GLSLShader::GLSLShaderType type );
	bool   Link();
	bool   Validate();
	void   Use();

	std::string Log();

	int GetHandle();
	bool IsLinked();

	void   bindAttribLocation( GLuint location, const char * name);
    void   bindFragDataLocation( GLuint location, const char * name );

    void   setUniform( const char *name, float x, float y, float z);
    void   setUniform( const char *name, const vec3 & v);
    void   setUniform( const char *name, const vec4 & v);
    void   setUniform( const char *name, const mat4 & m);
    void   setUniform( const char *name, const mat3 & m);
    void   setUniform( const char *name, float val );
    void   setUniform( const char *name, int val );
    void   setUniform( const char *name, bool val );

    void   printActiveUniforms();
    void   printActiveAttribs();




};

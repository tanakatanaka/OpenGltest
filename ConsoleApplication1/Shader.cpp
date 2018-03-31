#include "stdafx.h"
#include"Shader.h"
#include"fstream"
#include"Sstream"
#include <sys/stat.h>

Shader::Shader()
	:handle(0),
	linked(false)
{

}

std::string Shader::Load(const char *path)
{
	std::ifstream input(path, std::ios::binary | std::ios::in);
	
	if (!input) 
	{ 
		return std::string(); 
	}

    std::ostringstream buf;
	buf << input.rdbuf();
   return buf.str();
}

bool Shader::CompileShaderFromFile( const char * fileName, GLSLShader::GLSLShaderType type)
{
	if(! FileExists(fileName))
	{
		logString = "File not found.";
	}

	if(handle <= 0)
	{
		handle = glCreateProgram();

		if( handle == 0) 
		{
            logString = "Unable to create shader program.";
            return false;
        }
	}

	std::ifstream inFile(fileName, std::ios::in );
	
	if(!inFile)
	{
		return false;
	}

	 std::string fileString = Load(fileName);
	 return CompileShaderFromString(fileString, type);
}

bool Shader::CompileShaderFromString(const std::string & source, GLSLShader::GLSLShaderType type)
{
	//handle�����ꉞ�`�F�b�N
    if( handle <= 0 ) 
	{
        handle = glCreateProgram();
        if( handle == 0) 
		{
            logString = "Unable to create shader program.";
            return false;
        }
    }

	GLuint shaderHandle = 0;
	 switch( type ) 
	 {
		case GLSLShader::VERTEX:
			shaderHandle = glCreateShader(GL_VERTEX_SHADER);
			break;
		case GLSLShader::FRAGMENT:
			shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case GLSLShader::GEOMETRY:
			shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case GLSLShader::TESS_CONTROL:
			shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case GLSLShader::TESS_EVALUATION:
			shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;
		default:
			return false;
    }

	const char *shaderCode = source.c_str();
	glShaderSource(shaderHandle, 1, &shaderCode, NULL);
	// Compile the shader
    glCompileShader(shaderHandle);
	//Check for Errors
	int result;
	glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );

	 if( GL_FALSE == result ) 
	 {
        // Compile failed, store log and return false
        int length = 0;
        logString = "";
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length );
        if( length > 0 ) 
		{
            char * c_log = new char[length];
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }

        return false;
    } 
	else 
	{
        // Compile succeeded, attach shader and return true
        glAttachShader(handle, shaderHandle);
        return true;
    }
}

bool Shader::Link()
{
	if(linked)
	{
		return false;
	}

	if( handle <= 0)
	{
		return false;
	}

	//�����N���s��
	glLinkProgram(handle);

	//�����N�̐��ۂ��`�F�b�N
	GLint status;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);

	if( GL_FALSE == status)
	{
		//Store log and return false
		int length = 0;
		logString = "";

		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 ) 
		{
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }

		return false;
	}

	linked = true;
	return linked;
}


bool Shader::FileExists( const std::string & fileName )
{
	 struct stat info;
	 int ret = -1;

	 ret = stat(fileName.c_str(), &info);

	 return 0 == ret;
}

void Shader::Use()
{
	if( handle <= 0 || (! linked) )
	{
		return;
	}

    glUseProgram( handle );
}

std::string Shader::Log()
{
	return logString;
}

int Shader::GetHandle()
{
	return handle;
}

int Shader::GetUniformLocation(const char *name)
{
	return glGetUniformLocation(handle, name);
}


void Shader::setUniform(const char *name , float x, float y, float z)
{
	//uniform�����擾
	int loc = GetUniformLocation(name);

	if( loc >= 0 ) 
	{
		glUniform3f(loc,x,y,z);
    }
}

//Vector3��
void Shader::setUniform( const char *name, const vec3 &v)
{
    setUniform(name,v.x,v.y,v.z);
}

//Vector4��
void Shader::setUniform(const char *name, const vec4 &v)
{
	int loc = GetUniformLocation(name);

    if( loc >= 0 ) 
	{
		glUniform4f(loc,v.x,v.y,v.z,v.w);
    }
}

//�s���
void Shader::setUniform(const char *name, const mat4 & m)
{
	 int loc = GetUniformLocation(name);
    if( loc >= 0 )
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
    }
}

void Shader::setUniform( const char *name, const mat3 & m)
{
    int loc = GetUniformLocation(name);
    if( loc >= 0 )
    {
        glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
    }
}

void Shader::setUniform( const char *name, float val )
{
    int loc = GetUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1f(loc, val);
    }
}

void Shader::setUniform( const char *name, int val )
{
    int loc = GetUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1i(loc, val);
    }
}

void Shader::setUniform( const char *name, bool val )
{
    int loc = GetUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1i(loc, val);
    }
}





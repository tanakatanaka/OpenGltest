#include "stdafx.h"
#include"Shader.h"
#include"fstream"
#include"Sstream"

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
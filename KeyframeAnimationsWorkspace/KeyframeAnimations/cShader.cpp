#include "cShaderManager.h"
#include "globalOpenGL.h"

cShaderManager::cShader::cShader()
{
	this->ID = 0;
	this->shaderType = cShader::UNKNOWN;
	return;
}

cShaderManager::cShader::~cShader() 
{
	return;
}

std::string cShaderManager::cShader::getShaderTypeString(void)
{
	switch ( this->shaderType )
	{
	case cShader::VERTEX_SHADER:
		return "VERTEX_SHADER";
		break;
	case cShader::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
		break;
	case cShader::UNKNOWN:
	default:
		return "UNKNOWN_SHADER_TYPE";
		break;
	}
	return "UNKNOWN_SHADER_TYPE";
}


bool cShaderManager::cShaderProgram::LoadUniformLocation(std::string variableName)
{

	GLint uniLocation = glGetUniformLocation(this->ID, 
											 variableName.c_str() );

	if ( uniLocation == -1 )
	{	
		return false;
	}

	this->mapUniformName_to_UniformLocation[variableName.c_str()] = uniLocation;

	return true;	
}

int cShaderManager::cShaderProgram::getUniformID_From_Name(std::string name)
{
	std::map< std::string,
			  int>::iterator 
		itUniform = this->mapUniformName_to_UniformLocation.find(name);

	if ( itUniform == this->mapUniformName_to_UniformLocation.end() )
	{
		return -1;
	}

	return itUniform->second;
}

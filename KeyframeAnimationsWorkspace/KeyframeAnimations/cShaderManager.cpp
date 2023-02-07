#include "cShaderManager.h"

#include "globalOpenGL.h"

#include <fstream>
#include <sstream>
#include <vector>

#include <algorithm>	
#include <iterator>	

cShaderManager::cShaderManager()
{
	return;
}

cShaderManager::~cShaderManager()
{
	return;
}


bool cShaderManager::useShaderProgram( unsigned int ID )
{

	glUseProgram(ID);
	return true;
}

bool cShaderManager::useShaderProgram( std::string friendlyName )
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator 
			itShad = this->m_name_to_ID.find(friendlyName);

	if ( itShad == this->m_name_to_ID.end() )
	{	
		return false;
	}
	glUseProgram(itShad->second);

	return true;
}

unsigned int cShaderManager::getIDFromFriendlyName( std::string friendlyName )
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator 
			itShad = this->m_name_to_ID.find(friendlyName);

	if ( itShad == this->m_name_to_ID.end() )
	{	
		return 0;
	}
	return itShad->second;
}

cShaderManager::cShaderProgram* 
	cShaderManager::pGetShaderProgramFromFriendlyName( std::string friendlyName )
{
	unsigned int shaderID = this->getIDFromFriendlyName(friendlyName);
	
		std::map< unsigned int /*ID*/, cShaderProgram >::iterator
			itShad = this->m_ID_to_Shader.find(shaderID);

	if ( itShad == this->m_ID_to_Shader.end() )
	{	
		return NULL;
	}

	cShaderProgram* pShaderIFound = &(itShad->second);

	return pShaderIFound;
}


const unsigned int MAXLINELENGTH = 65536;

void cShaderManager::setBasePath( std::string basepath )
{
	this->m_basepath = basepath;
	return;
}


bool cShaderManager::m_loadSourceFromFile( cShader &shader )
{
	std::string fullFileName = this->m_basepath + shader.fileName;

	std::ifstream theFile( fullFileName.c_str() );
	if ( ! theFile.is_open() )
	{
		return false;
	}


	shader.vecSource.clear();

	char pLineTemp[MAXLINELENGTH] = {0};
	while ( theFile.getline( pLineTemp, MAXLINELENGTH ) )
	{
		std::string tempString(pLineTemp);

			shader.vecSource.push_back( tempString );
	}
	
	theFile.close();
	return true;
}


bool cShaderManager::m_wasThereACompileError( unsigned int shaderID, 
											  std::string &errorText )
{
	errorText = "";	

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);

		errorText.append(pLogText);
 
		this->m_lastError.append("\n");
		this->m_lastError.append( errorText );


		delete [] pLogText;

		return true;
	}
	return false;
}

bool cShaderManager::m_wasThereALinkError( unsigned int programID, std::string &errorText )
{
	errorText = "";

	GLint wasError = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &wasError);
	if(wasError == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];

		glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);

		errorText.append(pLogText);
 
		this->m_lastError.append("\n");
		this->m_lastError.append( errorText );


		delete [] pLogText;	

		return true;	
	}
	
	return false; 
}


std::string cShaderManager::getLastError(void)
{
	std::string lastErrorTemp = this->m_lastError;
	this->m_lastError = "";
	return lastErrorTemp;
}

#include <iostream>

bool cShaderManager::m_compileShaderFromSource( cShaderManager::cShader &shader, std::string &error )
{
	error = "";

	const unsigned int MAXLINESIZE = 8 * 1024;

	unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());

	char** arraySource = new char*[numberOfLines];
	memset( arraySource, 0, numberOfLines );	

	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{
		unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();

		arraySource[indexLine] = new char[numCharacters + 2];	
		memset( arraySource[indexLine], 0, numCharacters + 2 );

		for ( unsigned int indexChar = 0; indexChar != shader.vecSource[indexLine].length(); indexChar++ )
		{
			arraySource[indexLine][indexChar] = shader.vecSource[indexLine][indexChar];
		}
		
		arraySource[indexLine][numCharacters + 0] = '\n';
		arraySource[indexLine][numCharacters + 1] = '\0';

	}

	glShaderSource(shader.ID, numberOfLines, arraySource, NULL);
    glCompileShader(shader.ID); 

	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{	
		delete [] arraySource[indexLine];
	}
	
	delete [] arraySource;

	std::string errorText = "";
	if ( this->m_wasThereACompileError( shader.ID, errorText ) )
	{
		std::stringstream ssError;
		ssError << shader.getShaderTypeString();
		ssError << " compile error: ";
		ssError << errorText;
		error = ssError.str();
		return false;
	}

	return true;
}



bool cShaderManager::createProgramFromFile( 
	        std::string friendlyName,
			cShader &vertexShad, 
			cShader &fragShader )
{
	std::string errorText = "";

	vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
	vertexShad.shaderType = cShader::VERTEX_SHADER;

	if ( ! this->m_loadSourceFromFile( vertexShad ) )
	{
		return false;
	}

	errorText = "";
	if ( ! this->m_compileShaderFromSource( vertexShad, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}

    fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader.shaderType = cShader::FRAGMENT_SHADER;
	if ( ! this->m_loadSourceFromFile( fragShader ) )
	{
		return false;
	}

	if ( ! this->m_compileShaderFromSource( fragShader, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}


	cShaderProgram curProgram;
    curProgram.ID = glCreateProgram();

    glAttachShader(curProgram.ID, vertexShad.ID);
    glAttachShader(curProgram.ID, fragShader.ID);
    glLinkProgram(curProgram.ID);

	errorText = "";
	if ( this->m_wasThereALinkError( curProgram.ID, errorText ) )
	{
		std::stringstream ssError;
		ssError << "Shader program link error: ";
		ssError << errorText;
		this->m_lastError = ssError.str();
		return false;
	}

	curProgram.friendlyName = friendlyName;

	this->m_ID_to_Shader[curProgram.ID] = curProgram;
	this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

	return true;
}

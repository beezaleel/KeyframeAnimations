#ifndef _cShaderManager_HG_
#define _cShaderManager_HG_

#include <string>
#include <vector>
#include <map>

class cShaderManager
{
public:
	class cShader  {
	public:
		cShader();
		~cShader();
		enum eShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			UNKNOWN
		};
		eShaderType shaderType;
		std::string getShaderTypeString(void);

		unsigned int ID;
		std::vector<std::string> vecSource;	
		bool bSourceIsMultiLine;
		std::string fileName;
			
	};

	class cShaderProgram {
	public:
		cShaderProgram() : ID(0) {};
		~cShaderProgram() {};
		unsigned int ID;
		std::string friendlyName;

		std::map<std::string, int> mapUniformName_to_UniformLocation;
		int getUniformID_From_Name(std::string name);
		bool LoadUniformLocation(std::string variableName);

	};

	cShaderManager();
	~cShaderManager();

	bool useShaderProgram( unsigned int ID );
	bool useShaderProgram( std::string friendlyName );
	bool createProgramFromFile( std::string friendlyName, 
		                        cShader &vertexShad, 
					            cShader &fragShader );
	void setBasePath( std::string basepath );
	unsigned int getIDFromFriendlyName( std::string friendlyName );

	cShaderProgram* pGetShaderProgramFromFriendlyName( std::string friendlyName );

	std::string getLastError(void);
private:
	bool m_loadSourceFromFile( cShader &shader );
	std::string m_basepath;

	bool m_compileShaderFromSource( cShader &shader, std::string &error );
	bool m_wasThereACompileError( unsigned int shaderID, std::string &errorText );
	bool m_wasThereALinkError( unsigned int progID, std::string &errorText );

	std::string m_lastError;

	std::map< unsigned int, cShaderProgram > m_ID_to_Shader;
	std::map< std::string, unsigned int> m_name_to_ID;
};

#endif
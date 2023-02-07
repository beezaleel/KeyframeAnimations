#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

#include <string>
#include <map>
#include "sModelDrawInfo.h"

class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);

private:

	std::map< std::string,
		      sModelDrawInfo>
		m_map_ModelName_to_VAOID;

};

#endif

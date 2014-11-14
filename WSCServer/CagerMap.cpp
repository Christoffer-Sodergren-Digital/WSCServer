#include "include\game\CagerMap.h"
#include "include\lodepng.h"
#include <vector>

using namespace Cager;

MapGenerator::MapGenerator(){
	m_iSparsity=10;
	m_uiWidth=100;
	m_uiHeight=100;
	m_puiMapData=NULL;
}

MapGenerator::~MapGenerator(){
	if(m_puiMapData){
		delete m_puiMapData;
	}
}

unsigned int *MapGenerator::Generate(){
	return NULL;
}


CagerMap::CagerMap(unsigned int p_uiSeed){
	m_pxData=NULL;
	m_uiSeed=p_uiSeed;
}

CagerMap::~CagerMap(void){
	if(m_pxData){
		delete [] m_pxData;
	}
}

void CagerMap::GenerateFromPNG(std::string p_sPath, unsigned int & p_ouiWidth, unsigned int & p_ouiHeight){
	std::vector<unsigned char> xImage;
	unsigned uError=lodepng::decode(xImage,p_ouiWidth,p_ouiHeight,p_sPath.c_str());

	if(m_pxData){delete [] m_pxData;}

	int i,iC=xImage.size();
	m_pxData=new unsigned char[iC];
	int iStep=4;
	for(i=0;i<iC;i+=iStep){
		m_pxData[i]=xImage[i];
		m_pxData[i+1]=xImage[i+1];
		m_pxData[i+2]=xImage[i+2];
		m_pxData[i+3]=xImage[i+3];
	}
}

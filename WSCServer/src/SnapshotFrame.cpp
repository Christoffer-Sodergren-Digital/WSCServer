#include "SnapshotFrame.h"
#include <ctime>

SnapshotFrame::SnapshotFrame() : m_iTimestamp(0){
	m_uiDataSize=0;
}

SnapshotFrame::~SnapshotFrame(){}

//void SnapshotFrame::Add(std::stringstream *p_xOS){
//	time_t iTimestamp=time(NULL);
//	p_xOS->seekg(0,std::ios_base::end);
//	m_uiDataSize+=p_xOS->tellg();
//	p_xOS->clear();
//	p_xOS->seekg(0);
//
//	m_xBuffer.push_back(p_xOS);
//}

void SnapshotFrame::MakeFrame(){
	m_pucData=new unsigned char[m_uiDataSize+1];
	int iOffset=0;
	size_t iS=m_xBuffer.str().length();
	m_xBuffer.read((char*)m_pucData+iOffset,iS);
	iOffset+=(int)iS;

	Finalize();
}
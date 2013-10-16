#ifndef SNAPSHOT_FRAME_H
#define SNAPSHOT_FRAME_H

#include <sstream>
#include <vector>
#include "ApplicationDataFrame.h"


class SnapshotFrame : public WS::WSApplicationDataFrame{
public:
	SnapshotFrame();
	virtual ~SnapshotFrame();
	
//	void Add(std::stringstream * p_xOS);
	std::stringstream & Buffer(){return m_xBuffer;}
	void MakeFrame();

protected:
private:
	__int64 m_iTimestamp;
	std::stringstream m_xBuffer;
};


#endif
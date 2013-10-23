#pragma once
#include <string>
#include <ostream>

namespace Cager{

class IMapGenerator{
public:
	IMapGenerator(){}
	virtual ~IMapGenerator(){}

	virtual unsigned int *Generate()=0;

	virtual void Sparsity(int p_iSparsity)=0;
	virtual int Sparsity() const =0;

	virtual void Width(unsigned int p_uiW)=0;
	virtual unsigned int Width() const =0;

	virtual void Height(unsigned int p_uiH)=0;
	virtual unsigned int Height() const =0;
};

class MapGenerator : public IMapGenerator{
public:
	MapGenerator();
	virtual ~MapGenerator();

	virtual unsigned int *Generate();

	void Sparsity(int p_iSparsity){m_iSparsity=p_iSparsity;}
	int Sparsity() const {return m_iSparsity;}

	void Width(unsigned int p_uiW){m_uiWidth=p_uiW;}
	unsigned int Width() const{return m_uiWidth;}

	void Height(unsigned int p_uiH){m_uiHeight=p_uiH;}
	unsigned int Height() const{return m_uiHeight;}

private:
	int m_iSparsity;
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;

	unsigned int *m_puiMapData;
};

class SmallMap : public MapGenerator{
public:
	SmallMap();
	virtual ~SmallMap();

	virtual unsigned int *Generate();
};


class CagerMap
{
public:
	CagerMap(unsigned int p_uiSeed=0);
	~CagerMap(void);

	void Seed(unsigned int p_uiSeed){m_uiSeed=p_uiSeed;}

	void GenerateFromPNG(std::string p_sPath, unsigned int & p_ouiWidth, unsigned int & p_ouiHeight);
	void Generate(IMapGenerator *p_pxGenerator);

	unsigned char *MapData(){return m_pxData;}

	friend std::ostream & operator<<(std::ostream & p_xO,CagerMap & p_xM);
private:
	unsigned int m_uiSeed;
	unsigned char *m_pxData;
};

}
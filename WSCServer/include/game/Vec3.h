// Vec3.h

#pragma once
#ifndef VEC3_H_INCLUDED
#define VEC3_H_INCLUDED

#include <cmath>
#include <cstdlib>

namespace Rcr
{

class Vec3
{
public:
	float m_fX,m_fY,m_fZ;

	Vec3()
	{
		m_fX=m_fY=m_fZ=0.0f;
	};

	Vec3(float p_fX,float p_fY,float p_fZ)
	{
		m_fX=p_fX;
		m_fY=p_fY;
		m_fZ=p_fZ;
	};

	Vec3(const Vec3& p_xV)
	{
		m_fX=p_xV.m_fX;
		m_fY=p_xV.m_fY;
		m_fZ=p_xV.m_fZ;
	};

	Vec3& operator=(const Vec3& p_xV)
	{
		m_fX=p_xV.m_fX;
		m_fY=p_xV.m_fY;
		m_fZ=p_xV.m_fZ;
		return *this;
	};

	void Clear()									{(*(int*)&m_fX)=(*(int*)&m_fY)=(*(int*)&m_fZ)=0;};
	void SetXYZ(float p_fX,float p_fY,float p_fZ)	{m_fX=p_fX;m_fY=p_fY;m_fZ=p_fZ;};

	Vec3 operator+(const Vec3& p_kxrA) const {return Vec3(m_fX+p_kxrA.m_fX,m_fY+p_kxrA.m_fY,m_fZ+p_kxrA.m_fZ);};
	Vec3& operator+=(const Vec3& p_kxrA) {m_fX+=p_kxrA.m_fX;m_fY+=p_kxrA.m_fY;m_fZ+=p_kxrA.m_fZ;return *this;};
	Vec3 operator-(const Vec3& p_kxrA) const {return Vec3(m_fX-p_kxrA.m_fX,m_fY-p_kxrA.m_fY,m_fZ-p_kxrA.m_fZ);};
	Vec3& operator-=(const Vec3& p_kxrA) {m_fX-=p_kxrA.m_fX;m_fY-=p_kxrA.m_fY;m_fZ-=p_kxrA.m_fZ;return *this;};

	Vec3 operator-()const {return Vec3(-m_fX,-m_fY,-m_fZ);};

	Vec3 operator*(float p_fF) const {return Vec3(m_fX*p_fF,m_fY*p_fF,m_fZ*p_fF);};
	Vec3& operator*=(float p_fF) {m_fX*=p_fF;m_fY*=p_fF;m_fZ*=p_fF;return *this;};
	Vec3 operator/(float p_fF) const {return Vec3(m_fX/p_fF,m_fY/p_fF,m_fZ/p_fF);};
	Vec3& operator/=(float p_fF) {m_fX/=p_fF;m_fY/=p_fF;m_fZ/=p_fF;return *this;};

	float operator*(const Vec3& p_kxrA) const {return m_fX*p_kxrA.m_fX+m_fY*p_kxrA.m_fY+m_fZ*p_kxrA.m_fZ;};
	Vec3 operator^(const Vec3& p_kxrA) const {return Vec3(m_fY*p_kxrA.m_fZ-m_fZ*p_kxrA.m_fY,
		m_fZ*p_kxrA.m_fX-m_fX*p_kxrA.m_fZ,
		m_fX*p_kxrA.m_fY-m_fY*p_kxrA.m_fX);};

	bool operator==(const Vec3& p_kxrA)	const {return m_fX==p_kxrA.m_fX&&m_fY==p_kxrA.m_fY&&m_fZ==p_kxrA.m_fZ;};
	bool operator!=(const Vec3& p_kxrA)	const {return m_fX!=p_kxrA.m_fX||m_fY!=p_kxrA.m_fY||m_fZ!=p_kxrA.m_fZ;};

	float Dot(const Vec3& p_kxrV) const {return m_fX*p_kxrV.m_fX+m_fY*p_kxrV.m_fY+m_fZ*p_kxrV.m_fZ;};

	float Abs() const {return (sqrt(m_fX*m_fX+m_fY*m_fY+m_fZ*m_fZ));};
	float AbsSquare() const {return (m_fX*m_fX+m_fY*m_fY+m_fZ*m_fZ);};
	float AbsM() const {return (abs(m_fX))+abs(m_fY)+(abs(m_fZ));};

	Vec3 GetNormalized() const {float fD=Abs();if(fD<=0.0f)
	{return Vec3(1.0f,0.0f,0.0f);}else{return Vec3(m_fX/fD,m_fY/fD,m_fZ/fD);};};
	void Normalize() {float fD=Abs();if(fD<=0.0f)
	{SetXYZ(1.0f,0.0f,0.0f);}else{m_fX/=fD;m_fY/=fD;m_fZ/=fD;};};

};

}; // namespace Rcr

#endif // VEC3_H_INCLUDED

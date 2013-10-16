#ifndef VEC_2D_H
#define VEC_2D_H
#include <cmath>
#include <sstream>

class Vec2{
public:
	Vec2(){m_fX=m_fY=0;}
	Vec2(float p_fX,float p_fY):m_fX(p_fX),m_fY(p_fY){}
	virtual ~Vec2(){}

	Vec2(const Vec2 & p_xV){
		m_fX=p_xV.m_fX;
		m_fY=p_xV.m_fY;
	}
	Vec2 & operator=(const Vec2 & p_xV){
		m_fX=p_xV.m_fX;
		m_fY=p_xV.m_fY;
		return *this;
	}
	void Clear(){m_fX=0.0f;m_fY=0.0f;}
	void SetXY(float p_fX,float p_fY){
		m_fX=p_fX;m_fY=p_fY;
	}
	Vec2 operator+(const Vec2 & p_xV)const{return Vec2(m_fX+p_xV.m_fX,m_fY+p_xV.m_fY);}
	Vec2 & operator+=(const Vec2 & p_xV){m_fX+=p_xV.m_fX;m_fY+=p_xV.m_fY;return *this;}
	Vec2 operator-(const Vec2 & p_xV)const{return Vec2(m_fX-p_xV.m_fX,m_fY-p_xV.m_fY);}
	Vec2 & operator-=(const Vec2 & p_xV){m_fX-=p_xV.m_fX;m_fY-=p_xV.m_fY;return *this;}

	Vec2 operator-()const {return Vec2(-m_fX,-m_fY);}

	Vec2 operator*(float p_fF) const {return Vec2(m_fX*p_fF,m_fY*p_fF);}
	Vec2& operator*=(float p_fF) {m_fX*=p_fF;m_fY*=p_fF;return *this;}
	Vec2 operator/(float p_fF) const {return Vec2(m_fX/p_fF,m_fY/p_fF);}
	Vec2& operator/=(float p_fF) {m_fX/=p_fF;m_fY/=p_fF;return *this;}

	float operator*(const Vec2& p_kxrA) const {return m_fX*p_kxrA.m_fX+m_fY*p_kxrA.m_fY;}

	bool operator==(const Vec2& p_kxrA)	const {return m_fX==p_kxrA.m_fX&&m_fY==p_kxrA.m_fY;};
	bool operator!=(const Vec2& p_kxrA)	const {return m_fX!=p_kxrA.m_fX||m_fY!=p_kxrA.m_fY;};

	float Dot(const Vec2& p_kxrV) const {return m_fX*p_kxrV.m_fX+m_fY*p_kxrV.m_fY;};

	float Abs() const {return (sqrt(m_fX*m_fX+m_fY*m_fY));};
	float AbsSquare() const {return (m_fX*m_fX+m_fY*m_fY);};
	float AbsM() const {return (abs(m_fX))+abs(m_fY);};

	Vec2 GetNormalized() const {float fD=Abs();if(fD<=0.0f)
	{return Vec2(1.0f,0.0f);}else{return Vec2(m_fX/fD,m_fY/fD);};};
	void Normalize() {float fD=Abs();if(fD<=0.0f)
	{SetXY(1.0f,0.0f);}else{m_fX/=fD;m_fY/=fD;};};

	float m_fX;
	float m_fY;

	//friend std::stringstream & operator<<(std::stringstream & p_xSS,Vec2 & p_vV){
	//	p_xSS<<p_vV.m_fX<<p_vV.m_fY;
	//	return p_xSS;
	//}
};



#endif

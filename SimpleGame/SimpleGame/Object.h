#ifndef Object_h__
#define Object_h__

class Renderer;

class CObject
{
public:
	CObject(char* szName, int iHp, int iAtt);
	~CObject(void);

private:
	struct POS
	{
		float fX;
		float fY;
		float fZ;
	};

	struct COLOR
	{
		float fR;
		float fG;
		float fB;
		float fA;
	};

private:
	char	m_szName[100];
	int		m_iHp;
	int		m_iAtt;
	float	m_fSize;
	POS		m_Pos;
	COLOR	m_Color;

public:
	float	GetSize() { return m_fSize; }
	POS		GetPos() { return m_Pos; }
	COLOR	GetColor() { return m_Color; }

public:
	void	SetSize(float fSize);
	void	SetPos(float fX, float fY, float fZ = 0.f);
	void	SetColor(float fR, float fG, float fB, float fA);
};

#endif // Object_h__

// 어떤 방식으로 하는지
// 
// 
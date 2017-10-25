#ifndef Object_h__
#define Object_h__

class Renderer;

class CObject
{
public:
	CObject();
	CObject(char* szName, int iHp, int iAtt);
	~CObject();

private:
	struct VECTOR
	{
		float fX;
		float fY;
		float fZ;
	};

	struct RECT
	{
		float fTop;
		float fBottom;
		float fLeft;
		float fRight;
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
	float	m_fSpeed;
	bool	m_bLeftButtonDown;
	VECTOR	m_Pos;
	VECTOR	m_Dir;
	VECTOR	m_TargetPos;
	RECT	m_Rect;
	COLOR	m_Color;
	
public:
	void	Init();
	void	Update();

public:
	float	GetSize() { return m_fSize; }
	VECTOR	GetPos() { return m_Pos; }
	COLOR	GetColor() { return m_Color; }
	float	GetSpeed() { return m_fSpeed; }
	VECTOR	GetDir() { return m_Dir; }
	VECTOR	GetTargetPos() { return m_TargetPos; }
	bool	GetLeftButtonDown() { return m_bLeftButtonDown; }
	RECT	GetRect() { return m_Rect; }

public:
	void	SetSize(float fSize);
	void	SetPos(float fX, float fY, float fZ = 0.f);
	void	SetColor(float fR, float fG, float fB, float fA);
	void	SetSpeed(float fSpeed);
	void	SetDir(float fDirX, float fDirY, float fDirZ);
	void	SetTargetPos(float fTargetPosX, float fTargetPosY, float fTargetPosZ = 0.f);
	void	SetLeftButtonDown(bool bLeftButtonDown);
	void	SetRect(float fTop, float fBottom, float fLeft, float fRight);
};

#endif // Object_h__
#ifndef Object_h__
#define Object_h__

#include "SceneMgr.h"

class Renderer;

class CObject
{
public:
	CObject(OBJECT_TYPE ObjType, OBJECT_TEAM ObjTeam);
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
	int			m_iParentNum;
	float		m_fSize;
	float		m_fSpeed;
	float		m_fLife;
	float		m_fMaxLife;
	float		m_fLifeTime;
	float		m_fTimeTerm_Bullet;
	float		m_fTimeTerm_Arrow;
	VECTOR		m_Pos;
	VECTOR		m_Dir;
	VECTOR		m_TargetPos;
	RECT		m_Rect;
	COLOR		m_Color;
	OBJECT_TYPE	m_ObjType;
	OBJECT_TEAM m_ObjTeam;
	
public:
	void		Init();
	void		Update(float fElapsedTime);
	void		UpdateBuilding(float fTime);
	void		UpdateCharacter(float fTime);
	void		UpdateBullet(float fTime);
	void		UpdateArrow(float fTime);

public:
	void		DecreaseLife(float fDamage);

public:
	float		GetSize() { return m_fSize; }
	VECTOR		GetPos() { return m_Pos; }
	COLOR		GetColor() { return m_Color; }
	float		GetSpeed() { return m_fSpeed; }
	VECTOR		GetDir() { return m_Dir; }
	VECTOR		GetTargetPos() { return m_TargetPos; }
	RECT		GetRect() { return m_Rect; }
	float		GetMaxLife() { return m_fMaxLife; }
	float		GetLife() { return m_fLife; }
	OBJECT_TYPE	GetObjType() { return m_ObjType; }
	OBJECT_TEAM GetObjTeam() { return m_ObjTeam; }
	int			GetParentNum() { return m_iParentNum; }
	float		GetTimeTerm_Bullet() { return m_fTimeTerm_Bullet; }
	float		GetTimeTerm_Arrow() { return m_fTimeTerm_Arrow; }

public:
	void		SetSize(float fSize);
	void		SetPos(float fX, float fY, float fZ = 0.f);
	void		SetColor(float fR, float fG, float fB, float fA);
	void		SetSpeed(float fSpeed);
	void		SetDir(float fDirX, float fDirY, float fDirZ);
	void		SetTargetPos(float fTargetPosX, float fTargetPosY, float fTargetPosZ = 0.f);
	void		SetRect(float fTop, float fBottom, float fLeft, float fRight);
	void		SetLife(float fLife);
	void		SetMaxLife(float fMaxLife);
	void		SetDead();
	void		SetParentNum(int iParentNum);
	void		SetTimeTerm_Bullet(float fTime);
	void		SetTimeTerm_Arrow(float fTime);
};

#endif // Object_h__
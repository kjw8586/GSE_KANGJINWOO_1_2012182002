#include "stdafx.h"
#include "Object.h"
#include "SceneMgr.h"

CObject::CObject(OBJECT_TYPE ObjType, OBJECT_TEAM ObjTeam)
	: m_ObjType(ObjType)
	, m_ObjTeam(ObjTeam)
{

}

CObject::~CObject()
{

}

void CObject::Init()
{
	m_fSpeed = 0.f;
	m_fTimeTerm_Bullet = 0.f;
	m_fTimeTerm_Arrow = 0.f;

	do
	{
		m_Dir.fX = float(rand() % 3 - 1);
		m_Dir.fY = float(rand() % 3 - 1);
		m_Dir.fZ = 0.f;
	} while (m_Dir.fX == 0 && m_Dir.fY == 0);

	m_TargetPos.fX = 0.f;
	m_TargetPos.fY = 0.f;
	m_TargetPos.fZ = 0.f;
}

void CObject::Update(float fElapsedTime)
{
	float fTime = fElapsedTime / 1000.f;

	m_fTimeTerm_Bullet += fTime;
	m_fTimeTerm_Arrow += fTime;

	// RECT 갱신
	m_Rect.fTop = m_Pos.fY + m_fSize / 2.f;
	m_Rect.fBottom = m_Pos.fY - m_fSize / 2.f;
	m_Rect.fLeft = m_Pos.fX - m_fSize / 2.f;
	m_Rect.fRight = m_Pos.fX + m_fSize / 2.f;

	// Pos 갱신
	m_Pos.fX += m_fSpeed * m_Dir.fX * fTime;
	m_Pos.fY += m_fSpeed * m_Dir.fY * fTime;

	switch (m_ObjType)
	{
	case OBJECT_BUILDING:
		UpdateBuilding(fTime);
		break;
	case OBJECT_CHARACTER:
		UpdateCharacter(fTime);
		break;
	case OBJECT_BULLET:
		UpdateBullet(fTime);
		break;
	case OBJECT_ARROW:
		UpdateArrow(fTime);
		break;
	}
}

void CObject::UpdateBuilding(float fTime)
{

}

void CObject::UpdateCharacter(float fTime)
{
	// Dir 갱신
	if (m_Pos.fX < -WINCX / 2.f)
	{
		m_Pos.fX = -WINCX / 2.f;
		m_Dir.fX *= -1.f;
	}

	if (m_Pos.fX > WINCX / 2.f)
	{
		m_Pos.fX = WINCX / 2.f;
		m_Dir.fX *= -1.f;
	}

	if (m_Pos.fY < -WINCY / 2.f)
	{
		m_Pos.fY = -WINCY / 2.f;
		m_Dir.fY *= -1.f;
	}

	if (m_Pos.fY > WINCY / 2.f)
	{
		m_Pos.fY = WINCY / 2.f;
		m_Dir.fY *= -1.f;
	}
}

void CObject::UpdateBullet(float fTime)
{
	// 화면 밖으로 나가면 삭제
	if ((m_Pos.fX < -WINCX / 2.f - m_fSize / 2.f) ||
		(m_Pos.fX > WINCX / 2.f + m_fSize / 2.f) ||
		(m_Pos.fY < -WINCY / 2.f - m_fSize / 2.f) ||
		(m_Pos.fY > WINCY / 2.f + m_fSize / 2.f))
	{
		m_fLife = 0.f;
	}
}

void CObject::UpdateArrow(float fTime)
{
	// 화면 밖으로 나가면 삭제
	if ((m_Pos.fX < -WINCX / 2.f - m_fSize / 2.f) ||
		(m_Pos.fX > WINCX / 2.f + m_fSize / 2.f) ||
		(m_Pos.fY < -WINCY / 2.f - m_fSize / 2.f) ||
		(m_Pos.fY > WINCY / 2.f + m_fSize / 2.f))
	{
		m_fLife = 0.f;
	}
}

void CObject::DecreaseLife(float fDamage)
{
	m_fLife -= fDamage;
}

void CObject::SetSize(float fSize)
{
	m_fSize = fSize;
}

void CObject::SetPos(float fX, float fY, float fZ /*= 0.f*/)
{
	m_Pos.fX = fX;
	m_Pos.fY = fY;
	m_Pos.fZ = fZ;
}

void CObject::SetColor(float fR, float fG, float fB, float fA)
{
	m_Color.fR = fR;
	m_Color.fG = fG;
	m_Color.fB = fB;
	m_Color.fA = fA;
}

void CObject::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CObject::SetDir(float fDirX, float fDirY, float fDirZ)
{
	m_Dir.fX = fDirX;
	m_Dir.fY = fDirY;
	m_Dir.fZ = fDirZ;
}

void CObject::SetTargetPos(float fTargetPosX, float fTargetPosY, float fTargetPosZ /*=0*/)
{
	m_TargetPos.fX = fTargetPosX;
	m_TargetPos.fY = fTargetPosY;
	m_TargetPos.fZ = fTargetPosZ;
}

void CObject::SetRect(float fTop, float fBottom, float fLeft, float fRight)
{
	m_Rect.fTop = fTop;
	m_Rect.fBottom = fBottom;
	m_Rect.fLeft = fLeft;
	m_Rect.fRight = fRight;
}

void CObject::SetMaxLife(float fMaxLife)
{
	m_fMaxLife = fMaxLife;
}

void CObject::SetLife(float fLife)
{
	m_fLife = fLife;
}

void CObject::SetDead()
{
	m_fLife = 0.f;
}

void CObject::SetParentNum(int iParentNum)
{
	m_iParentNum = iParentNum;
}

void CObject::SetTimeTerm_Bullet(float fTime)
{
	m_fTimeTerm_Bullet = fTime;
}

void CObject::SetTimeTerm_Arrow(float fTime)
{
	m_fTimeTerm_Arrow = fTime;
}
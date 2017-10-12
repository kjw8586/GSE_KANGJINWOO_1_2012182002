#include "stdafx.h"
#include "Object.h"

CObject::CObject(char* szName, int iHp, int iAtt)
	: m_iHp(iHp)
	, m_iAtt(iAtt)
{
	strcpy_s(m_szName, sizeof(szName), szName);
}

CObject::~CObject(void)
{

}

void CObject::Init(void)
{
	m_Dir.fX = -1.f;
	m_Dir.fY = 0.f;
	m_Dir.fZ = 0.f;
}

void CObject::Update()
{
	float fTime = 0.2f;

	m_Pos.fX += m_Dir.fX * fTime;
	m_Pos.fY += m_Dir.fY * fTime;

	if (m_Pos.fX < -200.f)
	{
		m_Pos.fX = -200.f;

		m_Dir.fX = 0.f;
		m_Dir.fY = -1.f;

		SetColor(1.f, 0.f, 0.f, 1.f);
	}

	if (m_Pos.fX > 200.f)
	{
		m_Pos.fX = 200.f;

		m_Dir.fX = 0.f;
		m_Dir.fY = 1.f;

		SetColor(0.f, 0.f, 1.f, 1.f);
	}

	if (m_Pos.fY < -200.f)
	{
		m_Pos.fY = -200.f;

		m_Dir.fX = 1.f;
		m_Dir.fY = 0.f;

		SetColor(0.f, 1.f, 0.f, 1.f);
	}

	if (m_Pos.fY > 200.f)
	{
		m_Pos.fY = 200.f;

		m_Dir.fX = -1.f;
		m_Dir.fY = 0.f;

		SetColor(1.f, 1.f, 1.f, 1.f);
	}
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
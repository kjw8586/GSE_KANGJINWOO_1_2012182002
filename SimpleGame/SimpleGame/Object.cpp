#include "stdafx.h"
#include "Object.h"

CObject::CObject()
{

}

CObject::CObject(char* szName, int iHp, int iAtt)
	: m_iHp(iHp)
	, m_iAtt(iAtt)
{
	strcpy_s(m_szName, sizeof(szName), szName);
}

CObject::~CObject()
{

}

void CObject::Init()
{
	m_fSpeed = 1.f;

	m_Dir.fX = -1.f;
	m_Dir.fY = 1.f;
	m_Dir.fZ = 0.f;

	m_TargetPos.fX = 0.f;
	m_TargetPos.fY = 0.f;
	m_TargetPos.fZ = 0.f;

	m_bLeftButtonDown = false;
}

void CObject::Update()
{
	float fTime = 0.2f;

	// 마우스 연동
	//m_Dir.fX = m_TargetPos.fX - m_Pos.fX;
	//m_Dir.fY = m_TargetPos.fY - m_Pos.fY;

	//m_Dir.fX /= fSizeVector;
	//m_Dir.fY /= fSizeVector;

	m_Pos.fX += m_fSpeed * m_Dir.fX * fTime;
	m_Pos.fY += m_fSpeed * m_Dir.fY * fTime;



	if (m_Pos.fX < -250.f || m_Pos.fX > 250.f) 
	{
		m_Dir.fX *= -1.f;
	}

	if (m_Pos.fY < -250.f || m_Pos.fY > 250.f)
	{
		m_Dir.fY *= -1.f;
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

void CObject::SetTargetPos(float fTargetPosX, float fTargetPosY, float fTargetPosZ /*=0*/)
{
	m_TargetPos.fX = fTargetPosX;
	m_TargetPos.fY = fTargetPosY;
	m_TargetPos.fZ = fTargetPosZ;
}

void CObject::SetLeftButtonDown(bool bLeftButtonDown)
{
	m_bLeftButtonDown = bLeftButtonDown;
}
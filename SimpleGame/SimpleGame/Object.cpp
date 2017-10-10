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
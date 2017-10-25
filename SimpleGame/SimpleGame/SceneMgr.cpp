#include "stdafx.h"
#include "SceneMgr.h"
#include "Object.h"

CSceneMgr::CSceneMgr()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		m_Objects[i] = NULL;
	}
}

CSceneMgr::~CSceneMgr()
{
	for (int i = 0; i < m_ObjectsCount; ++i)
	{
		if (m_Objects[i] != NULL)
		{
			delete m_Objects[i];
			m_Objects[i] = NULL;
		}
	}
}

void CSceneMgr::Init()
{
	m_ObjectsCount = 50;

	for (int i = 0; i < m_ObjectsCount; ++i)
	{
		m_Objects[i] = new CObject();
		m_Objects[i]->Init();
		m_Objects[i]->SetPos(float(rand() % 500 - 250.f), float(rand() % 500 - 250.f), 0.f);
		m_Objects[i]->SetColor(1.f, 1.f, 1.f, 1.f);
		m_Objects[i]->SetSize(20.f);
	}
}

void CSceneMgr::Update()
{
	bool bCollision[MAX_OBJECTS_COUNT];

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		bCollision[i] = false;

	for (int i = 0; i < m_ObjectsCount; ++i)
	{
		for (int j = 0; j < m_ObjectsCount; ++j)
		{
			if (i == j)
				continue;

			if (CheckCollision(m_Objects[i], m_Objects[j]))
			{
				bCollision[i] = true;
				break;
			}
		}

		if (bCollision[i])
			m_Objects[i]->SetColor(1.f, 0.f, 0.f, 1.f);
		else
			m_Objects[i]->SetColor(1.f, 1.f, 1.f, 1.f);
	}

	for (int i = 0; i < m_ObjectsCount; ++i)
	{
		m_Objects[i]->Update();
	}
}

bool CSceneMgr::CheckCollision(CObject* objA, CObject* objB)
{
	if (objA->GetRect().fTop > objB->GetRect().fBottom &&
		objA->GetRect().fBottom < objB->GetRect().fTop &&
		objA->GetRect().fLeft < objB->GetRect().fRight &&
		objA->GetRect().fRight > objB->GetRect().fLeft)
	{
		return true;
	}
	else
		return false;
}
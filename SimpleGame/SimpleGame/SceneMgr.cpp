#include "stdafx.h"
#include "SceneMgr.h"
#include "Object.h"
#include "Renderer.h"

#define WINCX 500
#define WINCY 500

CSceneMgr::CSceneMgr()
{
	// Objects
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		m_Objects[i] = NULL;
	}

	// Renderer
	m_Renderer = NULL;
}

CSceneMgr::~CSceneMgr()
{
	// Objects
	for (int i = 0; i < m_ObjectsCount; ++i)
	{
		if (m_Objects[i] != NULL)
		{
			delete m_Objects[i];
			m_Objects[i] = NULL;
		}
	}

	// Renderer
	if (m_Renderer != NULL)
	{
		delete m_Renderer;
		m_Renderer = NULL;
	}
}

void CSceneMgr::Init()
{
	// Objects
	m_ObjectsCount = 0;

	for (int i = 0; i < m_ObjectsCount; ++i)
	{
		m_Objects[i] = new CObject();
		m_Objects[i]->Init();
		m_Objects[i]->SetPos(float(rand() % 500 - 250.f), float(rand() % 500 - 250.f), 0.f);
		m_Objects[i]->SetColor(1.f, 1.f, 1.f, 1.f);
		m_Objects[i]->SetSize(10.f);
	}

	// Renderer
	m_Renderer = new Renderer(WINCX, WINCY);

	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
}

void CSceneMgr::Update(float fElapsedTime)
{
	// Objects
	bool bCollision[MAX_OBJECTS_COUNT];

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		bCollision[i] = false;

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Objects[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Objects[j] == NULL)
				continue;

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

	CheckObjectsLife();

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Objects[i] == NULL)
			continue;

		m_Objects[i]->Update(fElapsedTime);
	}
}

void CSceneMgr::Render()
{
	// Renderer
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Objects[i] == NULL)
			continue;

		// x, y, z, size, r, g, b, a (왼쪽 위는 (-250, 250) 이다.)
		m_Renderer->DrawSolidRect(m_Objects[i]->GetPos().fX, m_Objects[i]->GetPos().fY, m_Objects[i]->GetPos().fZ, m_Objects[i]->GetSize(),
			m_Objects[i]->GetColor().fR, m_Objects[i]->GetColor().fG, m_Objects[i]->GetColor().fB, m_Objects[i]->GetColor().fA);
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

int g_Temp = 0;

void CSceneMgr::CreateObjects(float fX, float fY)
{
	m_Objects[g_Temp] = new CObject();
	m_Objects[g_Temp]->Init();
	m_Objects[g_Temp]->SetPos(fX, fY);
	m_Objects[g_Temp]->SetColor(1.f, 1.f, 1.f, 1.f);
	m_Objects[g_Temp]->SetSize(10.f);

	++g_Temp;
}

void CSceneMgr::IncreaseObjectsCount()
{
	++m_ObjectsCount;
}

void CSceneMgr::CheckObjectsLife()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Objects[i] == NULL)
			continue;

		if (m_Objects[i]->GetLife() <= 0.f)
		{
			delete m_Objects[i];
			m_Objects[i] = NULL;

			--m_ObjectsCount;
		}
	}
}
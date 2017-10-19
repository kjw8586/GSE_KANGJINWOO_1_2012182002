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
	for (int i = 0; i < 50; ++i)
	{
		delete m_Objects[i];
		m_Objects[i] = NULL;
	}
}

void CSceneMgr::Init()
{
	for (int i = 0; i < 50; ++i)
	{
		m_Objects[i] = new CObject();
		m_Objects[i]->Init();
		m_Objects[i]->SetPos(rand() % 500 - 250, rand() % 500 - 250, 0.f);
		m_Objects[i]->SetColor(1.f, 1.f, 1.f, 1.f);
		m_Objects[i]->SetSize(15.f);
	}
}

void CSceneMgr::Update()
{
	for (int i = 0; i < 50; ++i)
	{
		m_Objects[i]->Update();
	}
}

void CSceneMgr::CheckCollision()
{

}
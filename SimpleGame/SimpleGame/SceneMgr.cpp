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
		m_Building[i] = NULL;
		m_Character[i] = NULL;
		m_Bullet[i] = NULL;
		m_Arrow[i] = NULL;
	}

	// Renderer
	m_Renderer = NULL;
}

CSceneMgr::~CSceneMgr()
{
	// Objects
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] != NULL)
		{
			delete m_Building[i];
			m_Building[i] = NULL;
		}

		if (m_Character[i] != NULL)
		{
			delete m_Character[i];
			m_Character[i] = NULL;
		}

		if (m_Bullet[i] != NULL)
		{
			delete m_Bullet[i];
			m_Bullet[i] = NULL;
		}

		if (m_Arrow[i] != NULL)
		{
			delete m_Arrow[i];
			m_Arrow[i] = NULL;
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
	m_BuildingCount = 0;
	m_CharacterCount = 0;
	m_BulletCount = 0;
	m_ArrowCount = 0;

	// Objects
	CreateObjects(0.f, 0.f, OBJECT_BUILDING);

	// Renderer
	m_Renderer = new Renderer(WINCX, WINCY);

	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
}

void CSceneMgr::Update(float fElapsedTime)
{
	float fTime = fElapsedTime / 1000.f;

	static float fTimeTerm = 0.f;
	fTimeTerm += fTime;

	// Objects
	CollisionBuildingAndCharacter();
	CollisionCharacterAndBullet();

	CheckObjectsLife();

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] != NULL)
		{
			m_Building[i]->Update(fElapsedTime);

			if (fTimeTerm > 1.f)
			{
				float fX = m_Building[i]->GetPos().fX;
				float fY = m_Building[i]->GetPos().fY;

				CreateObjects(fX, fY, OBJECT_BULLET);

				fTimeTerm = 0.f;
			}
		}

		if (m_Character[i] != NULL)
			m_Character[i]->Update(fElapsedTime);

		if (m_Bullet[i] != NULL)
			m_Bullet[i]->Update(fElapsedTime);

		if (m_Arrow[i] != NULL)
			m_Arrow[i]->Update(fElapsedTime);
	}
}

void CSceneMgr::Render()
{
	// Renderer
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] != NULL)
		{
			m_Renderer->DrawSolidRect(m_Building[i]->GetPos().fX, m_Building[i]->GetPos().fY, m_Building[i]->GetPos().fZ, m_Building[i]->GetSize(),
				m_Building[i]->GetColor().fR, m_Building[i]->GetColor().fG, m_Building[i]->GetColor().fB, m_Building[i]->GetColor().fA);
		}

		if (m_Character[i] != NULL)
		{
			m_Renderer->DrawSolidRect(m_Character[i]->GetPos().fX, m_Character[i]->GetPos().fY, m_Character[i]->GetPos().fZ, m_Character[i]->GetSize(),
				m_Character[i]->GetColor().fR, m_Character[i]->GetColor().fG, m_Character[i]->GetColor().fB, m_Character[i]->GetColor().fA);
		}

		if (m_Bullet[i] != NULL)
		{
			m_Renderer->DrawSolidRect(m_Bullet[i]->GetPos().fX, m_Bullet[i]->GetPos().fY, m_Bullet[i]->GetPos().fZ, m_Bullet[i]->GetSize(),
				m_Bullet[i]->GetColor().fR, m_Bullet[i]->GetColor().fG, m_Bullet[i]->GetColor().fB, m_Bullet[i]->GetColor().fA);
		}

		if (m_Arrow[i] != NULL)
		{
			m_Renderer->DrawSolidRect(m_Arrow[i]->GetPos().fX, m_Arrow[i]->GetPos().fY, m_Arrow[i]->GetPos().fZ, m_Arrow[i]->GetSize(),
				m_Arrow[i]->GetColor().fR, m_Arrow[i]->GetColor().fG, m_Arrow[i]->GetColor().fB, m_Arrow[i]->GetColor().fA);
		}
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

void CSceneMgr::CollisionBuildingAndCharacter()
{
	bool bCollision[MAX_OBJECTS_COUNT];

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		bCollision[i] = false;

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Character[j] == NULL)
				continue;

			if (CheckCollision(m_Building[i], m_Character[j]))
			{
				float fDamage = m_Character[j]->GetLife();

				m_Building[i]->DecreaseLife(fDamage);
				m_Character[j]->SetDead();
				break;
			}
		}
	}
}

void CSceneMgr::CollisionCharacterAndBullet()
{
	bool bCollision[MAX_OBJECTS_COUNT];

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		bCollision[i] = false;

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Character[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Bullet[j] == NULL)
				continue;

			if (CheckCollision(m_Character[i], m_Bullet[j]))
			{
				float fDamage = m_Bullet[j]->GetLife();

				m_Character[i]->DecreaseLife(fDamage);
				m_Bullet[j]->SetDead();
				break;
			}
		}
	}
}

void CSceneMgr::CreateObjects(float fX, float fY, OBJECTTYPE ObjType)
{
	switch (ObjType)
	{
	case OBJECT_BUILDING:
		if (m_BuildingCount >= MAX_OBJECTS_COUNT)
			return;

		m_Building[m_BuildingCount] = new CObject(OBJECT_BUILDING);
		m_Building[m_BuildingCount]->Init();
		m_Building[m_BuildingCount]->SetPos(fX, fY);
		m_Building[m_BuildingCount]->SetColor(1.f, 1.f, 0.f, 1.f);
		m_Building[m_BuildingCount]->SetSize(50.f);
		m_Building[m_BuildingCount]->SetSpeed(0.f);
		m_Building[m_BuildingCount]->SetLife(500.f);

		++m_BuildingCount;
		break;
	case OBJECT_CHARACTER:
		if (m_CharacterCount >= MAX_OBJECTS_COUNT)
			return;

		m_Character[m_CharacterCount] = new CObject(OBJECT_CHARACTER);
		m_Character[m_CharacterCount]->Init();
		m_Character[m_CharacterCount]->SetPos(fX, fY);
		m_Character[m_CharacterCount]->SetColor(1.f, 1.f, 1.f, 1.f);
		m_Character[m_CharacterCount]->SetSize(10.f);
		m_Character[m_CharacterCount]->SetSpeed(300.f);
		m_Character[m_CharacterCount]->SetLife(10.f);

		++m_CharacterCount;
		break;
	case OBJECT_BULLET:
		if (m_BulletCount >= MAX_OBJECTS_COUNT)
			return;

		m_Bullet[m_BulletCount] = new CObject(OBJECT_BULLET);
		m_Bullet[m_BulletCount]->Init();
		m_Bullet[m_BulletCount]->SetPos(fX, fY);
		m_Bullet[m_BulletCount]->SetColor(1.f, 0.f, 0.f, 1.f);
		m_Bullet[m_BulletCount]->SetSize(5.f);
		m_Bullet[m_BulletCount]->SetSpeed(400.f);
		m_Bullet[m_BulletCount]->SetLife(20.f);

		++m_BulletCount;
		break;
	case OBJECT_ARROW:
		if (m_ArrowCount >= MAX_OBJECTS_COUNT)
			return;

		m_Arrow[m_ArrowCount] = new CObject(OBJECT_ARROW);
		m_Arrow[m_ArrowCount]->Init();
		m_Arrow[m_ArrowCount]->SetPos(fX, fY);
		m_Arrow[m_ArrowCount]->SetColor(0.f, 1.f, 0.f, 1.f);
		m_Arrow[m_ArrowCount]->SetSize(2.f);
		m_Arrow[m_ArrowCount]->SetSpeed(100.f);
		m_Arrow[m_ArrowCount]->SetLife(10.f);

		++m_ArrowCount;
		break;
	}
}

void CSceneMgr::CheckObjectsLife()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] != NULL)
		{
			if (m_Building[i]->GetLife() <= 0.f)
			{
				delete m_Building[i];
				m_Building[i] = NULL;

				--m_BuildingCount;
			}
		}

		if (m_Character[i] != NULL)
		{
			if (m_Character[i]->GetLife() <= 0.f)
			{
				delete m_Character[i];
				m_Character[i] = NULL;

				--m_CharacterCount;
			}
		}

		if (m_Bullet[i] != NULL)
		{
			if (m_Bullet[i]->GetLife() <= 0.f)
			{
				delete m_Bullet[i];
				m_Bullet[i] = NULL;

				--m_BulletCount;
			}
		}

		if (m_Arrow[i] != NULL)
		{
			if (m_Arrow[i]->GetLife() <= 0.f)
			{
				delete m_Arrow[i];
				m_Arrow[i] = NULL;

				--m_ArrowCount;
			}
		}
	}
}
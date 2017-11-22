#include "stdafx.h"
#include "SceneMgr.h"
#include "Object.h"
#include "Renderer.h"

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

	m_fTimeTerm_CreateCharacter_Team1 = 0.f;

	// Objects
	for (int i = 0; i < 3; ++i)
	{
		CreateObjects(-150.f + 150.f * i, 300.f, OBJECT_BUILDING, OBJECT_TEAM1);
		CreateObjects(-150.f + 150.f * i, -300.f, OBJECT_BUILDING, OBJECT_TEAM2);
	}

	// Renderer
	m_Renderer = new Renderer(WINCX, WINCY);

	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	m_texBuilding_Team1 = m_Renderer->CreatePngTexture("./Resources/Building_Team1.png");
	m_texBuilding_Team2 = m_Renderer->CreatePngTexture("./Resources/Building_Team2.png");
}

void CSceneMgr::Update(float fElapsedTime)
{
	float fTime = fElapsedTime / 1000.f;

	m_fTimeTerm_CreateCharacter_Team1 += fTime;

	// Objects
	CollisionBuildingAndCharacter();
	CollisionCharacterAndBullet();
	CollisionBuildingAndArrow();
	CollisionCharacterAndArrow();

	CheckObjectsLife();

	if (m_fTimeTerm_CreateCharacter_Team1 > 5.f)
	{
		float fRandomX = rand() % WINCX - 250.f;
		float fRandomY = rand() % (WINCY / 2);

		CreateObjects(fRandomX, fRandomY, OBJECT_CHARACTER, OBJECT_TEAM1);

		m_fTimeTerm_CreateCharacter_Team1 = 0.f;
	}

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] != NULL)
		{
			m_Building[i]->Update(fElapsedTime);

			if (m_Building[i]->GetTimeTerm_Bullet() > 10.f)
			{
				float fX = m_Building[i]->GetPos().fX;
				float fY = m_Building[i]->GetPos().fY;

				OBJECT_TEAM objTeam = m_Building[i]->GetObjTeam();

				CreateObjects(fX, fY, OBJECT_BULLET, objTeam, i);

				m_Building[i]->SetTimeTerm_Bullet(0.f);
			}
		}

		if (m_Character[i] != NULL)
		{
			m_Character[i]->Update(fElapsedTime);

			if (m_Character[i]->GetTimeTerm_Arrow() > 3.f)
			{
				float fX = m_Character[i]->GetPos().fX;
				float fY = m_Character[i]->GetPos().fY;

				OBJECT_TEAM objTeam = m_Character[i]->GetObjTeam();

				CreateObjects(fX, fY, OBJECT_ARROW, objTeam, i);

				m_Character[i]->SetTimeTerm_Arrow(0.f);
			}
		}

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
			if (m_Building[i]->GetObjTeam() == OBJECT_TEAM1)
			{
				m_Renderer->DrawTexturedRect(m_Building[i]->GetPos().fX, m_Building[i]->GetPos().fY, m_Building[i]->GetPos().fZ, m_Building[i]->GetSize(),
					m_Building[i]->GetColor().fR, m_Building[i]->GetColor().fG, m_Building[i]->GetColor().fB, m_Building[i]->GetColor().fA, m_texBuilding_Team1);
			}
			else
			{
				m_Renderer->DrawTexturedRect(m_Building[i]->GetPos().fX, m_Building[i]->GetPos().fY, m_Building[i]->GetPos().fZ, m_Building[i]->GetSize(),
					m_Building[i]->GetColor().fR, m_Building[i]->GetColor().fG, m_Building[i]->GetColor().fB, m_Building[i]->GetColor().fA, m_texBuilding_Team2);

			}
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
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Character[j] == NULL)
				continue;

			if (m_Character[j]->GetObjTeam() == m_Building[i]->GetObjTeam())
				continue;

			if (CheckCollision(m_Building[i], m_Character[j]))
			{
				float fDamage = m_Character[j]->GetLife();

				m_Building[i]->DecreaseLife(fDamage);
				m_Character[j]->SetDead();
				
				if (m_Building[i]->GetLife() <= 0.f)
					break;
			}
		}
	}
}

void CSceneMgr::CollisionCharacterAndBullet()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Character[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Bullet[j] == NULL)
				continue;

			if (m_Bullet[j]->GetObjTeam() == m_Character[i]->GetObjTeam())
				continue;

			if (CheckCollision(m_Character[i], m_Bullet[j]))
			{
				float fDamage = m_Bullet[j]->GetLife();

				m_Character[i]->DecreaseLife(fDamage);
				m_Bullet[j]->SetDead();

				if (m_Character[i]->GetLife() <= 0.f)
					break;
			}
		}
	}
}

void CSceneMgr::CollisionBuildingAndArrow()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Arrow[j] == NULL)
				continue;

			if (m_Arrow[j]->GetObjTeam() == m_Building[i]->GetObjTeam())
				continue;

			if (CheckCollision(m_Building[i], m_Arrow[j]))
			{
				float fDamage = m_Arrow[j]->GetLife();

				m_Building[i]->DecreaseLife(fDamage);
				m_Arrow[j]->SetDead();

				if (m_Building[i]->GetLife() <= 0.f)
					break;
			}
		}
	}
}

void CSceneMgr::CollisionCharacterAndArrow()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Character[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Arrow[j] == NULL)
				continue;

			if (m_Arrow[j]->GetParentNum() == i)			// 자신을 생성한 부모일 경우 충돌 처리 안함
				continue;

			if (m_Arrow[j]->GetObjTeam() == m_Character[i]->GetObjTeam())
				continue;

			if (CheckCollision(m_Character[i], m_Arrow[j]))
			{
				float fDamage = m_Arrow[j]->GetLife();

				m_Character[i]->DecreaseLife(fDamage);
				m_Arrow[j]->SetDead();

				if (m_Character[i]->GetLife() <= 0.f)
					break;
			}
		}
	}
}

void CSceneMgr::CreateObjects(float fX, float fY, OBJECT_TYPE ObjType, OBJECT_TEAM ObjTeam, int iParentNum /* = -1 */)
{
	int iNum = 0;

	switch (ObjType)
	{
	case OBJECT_BUILDING:
		if (m_BuildingCount >= MAX_OBJECTS_COUNT)
			return;

		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		{
			if (m_Building[i] == NULL)
			{
				iNum = i;
				break;
			}
		}

		m_Building[iNum] = new CObject(OBJECT_BUILDING, ObjTeam);
		m_Building[iNum]->Init();
		m_Building[iNum]->SetPos(fX, fY);
		m_Building[iNum]->SetSize(100.f);
		m_Building[iNum]->SetSpeed(0.f);
		m_Building[iNum]->SetLife(500.f);

		++m_BuildingCount;
		break;
	case OBJECT_CHARACTER:
		if (m_CharacterCount >= MAX_OBJECTS_COUNT)
			return;

		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		{
			if (m_Character[i] == NULL)
			{
				iNum = i;
				break;
			}
		}

		m_Character[iNum] = new CObject(OBJECT_CHARACTER, ObjTeam);
		m_Character[iNum]->Init();
		m_Character[iNum]->SetPos(fX, fY);
		m_Character[iNum]->SetSize(10.f);
		m_Character[iNum]->SetSpeed(300.f);
		m_Character[iNum]->SetLife(10.f);

		if (ObjTeam == OBJECT_TEAM1)
		{
			m_Character[iNum]->SetColor(1.f, 0.f, 0.f, 1.f);
		}
		else
		{
			m_Character[iNum]->SetColor(0.f, 0.f, 1.f, 1.f);
		}

		++m_CharacterCount;
		break;
	case OBJECT_BULLET:
		if (m_BulletCount >= MAX_OBJECTS_COUNT)
			return;

		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		{
			if (m_Bullet[i] == NULL)
			{
				iNum = i;
				break;
			}
		}

		m_Bullet[iNum] = new CObject(OBJECT_BULLET, ObjTeam);
		m_Bullet[iNum]->Init();
		m_Bullet[iNum]->SetPos(fX, fY);
		m_Bullet[iNum]->SetSize(2.f);
		m_Bullet[iNum]->SetSpeed(600.f);
		m_Bullet[iNum]->SetLife(20.f);
		m_Bullet[iNum]->SetParentNum(iParentNum);

		if (ObjTeam == OBJECT_TEAM1)
		{
			m_Bullet[iNum]->SetColor(1.f, 0.f, 0.f, 1.f);
		}
		else
		{
			m_Bullet[iNum]->SetColor(0.f, 0.f, 1.f, 1.f);
		}

		++m_BulletCount;
		break;
	case OBJECT_ARROW:
		if (m_ArrowCount >= MAX_OBJECTS_COUNT)
			return;

		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		{
			if (m_Arrow[i] == NULL)
			{
				iNum = i;
				break;
			}
		}

		m_Arrow[iNum] = new CObject(OBJECT_ARROW, ObjTeam);
		m_Arrow[iNum]->Init();
		m_Arrow[iNum]->SetPos(fX, fY);
		m_Arrow[iNum]->SetSize(2.f);
		m_Arrow[iNum]->SetSpeed(100.f);
		m_Arrow[iNum]->SetLife(10.f);
		m_Arrow[iNum]->SetParentNum(iParentNum);

		if (ObjTeam == OBJECT_TEAM1)
		{
			m_Arrow[iNum]->SetColor(0.5, 0.2, 0.7, 1.f);
		}
		else
		{
			m_Arrow[iNum]->SetColor(1.f, 1.f, 0.f, 1.f);
		}

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
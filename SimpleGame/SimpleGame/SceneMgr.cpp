#include "stdafx.h"
#include "SceneMgr.h"
#include "Object.h"
#include "Renderer.h"
#include "Sound.h"

#define RENDER_LEVEL_CHARACTER 0.f
#define RENDER_LEVEL_ARROW 0.1f
#define RENDER_LEVEL_BUILDING_TEAM2 0.3f
#define RENDER_LEVEL_INFANTRY 0.4f
#define RENDER_LEVEL_BUILDING_TEAM1 0.5f
#define RENDER_LEVEL_BULLET 0.6f
#define RENDER_LEVEL_BACKGROUND 0.7f

//#define RENDER_LEVEL_GOD 0.f
//#define RENDER_LEVEL_SKY 0.1f
//#define RENDER_LEVEL_GROUND 0.2f
//#define RENDER_LEVEL_UNDERGROUND 0.3f
//#define RENDER_LEVEL_BACKGROUND 0.4f

CSceneMgr::CSceneMgr()
{
	// Objects
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		m_Building[i] = NULL;
		m_Character[i] = NULL;
		m_Infantry[i] = NULL;
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

		if (m_Infantry[i] != NULL)
		{
			delete m_Infantry[i];
			m_Infantry[i] = NULL;
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
	m_InfantryCount = 0;
	m_BulletCount = 0;
	m_ArrowCount = 0;

	m_fTimeTerm_CreateCharacter_Team1 = 0.f;
	m_fTimeTerm_CreateInfantry_Team1 = 0.f;
	m_fTime_ClimateParticle = 0.f;

	// Objects
	for (int i = 0; i < 3; ++i)
	{
		CreateObjects(-150.f + 150.f * i, 250.f, OBJECT_BUILDING, OBJECT_TEAM1);
		CreateObjects(-150.f + 150.f * i, -250.f, OBJECT_BUILDING, OBJECT_TEAM2);
	}

	// Renderer
	m_Renderer = new Renderer(WINCX, WINCY);

	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	m_texBuilding_Team1 = m_Renderer->CreatePngTexture("./Resources/Building_Team1.png");
	m_texBuilding_Team2 = m_Renderer->CreatePngTexture("./Resources/Building_Team2.png");
	m_texBackground = m_Renderer->CreatePngTexture("./Resources/Background.png");
	m_texCharacter_Team1 = m_Renderer->CreatePngTexture("./Resources/Bird_Black.png");
	m_texCharacter_Team2 = m_Renderer->CreatePngTexture("./Resources/Bird_White.png");
	m_texCharacter_Reverse_Team1 = m_Renderer->CreatePngTexture("./Resources/Bird_Black_Reverse.png");
	m_texCharacter_Reverse_Team2 = m_Renderer->CreatePngTexture("./Resources/Bird_White_Reverse.png");
	m_texInfantry_Team1 = m_Renderer->CreatePngTexture("./Resources/Ogre.png");
	m_texInfantry_Team2 = m_Renderer->CreatePngTexture("./Resources/DarkSlayer.png");
	m_texInfantry_Attack_Team1 = m_Renderer->CreatePngTexture("./Resources/Ogre_Attack.png");
	m_texInfantry_Attack_Team2 = m_Renderer->CreatePngTexture("./Resources/DarkSlayer_Attack.png");
	m_texInfantry_Attack_Reverse_Team2 = m_Renderer->CreatePngTexture("./Resources/DarkSlayer_Attack_Reverse.png");
	m_texBullet_Team1 = m_Renderer->CreatePngTexture("./Resources/Bullet_Particle_Team1.png");
	m_texBullet_Team2 = m_Renderer->CreatePngTexture("./Resources/Bullet_Particle_Team2.png");
	m_texClimateParticle = m_Renderer->CreatePngTexture("./Resources/Snow_Particle.png");

	m_Sound = new Sound();
	m_soundBackground = m_Sound->CreateSound("./Dependencies/Sound/BackgroundMusic.mp3");
	m_Sound->PlaySound(m_soundBackground, true, 0.2f);
}

void CSceneMgr::Update(float fElapsedTime)
{
	float fTime = fElapsedTime / 1000.f;

	m_fTimeTerm_CreateCharacter_Team1 += fTime;
	m_fTimeTerm_CreateInfantry_Team1 += fTime;
	m_fTime_ClimateParticle += fTime;

	// Objects
	CollisionBuildingAndCharacter();
	CollisionBuildingAndBullet();
	CollisionCharacterAndBullet();
	CollisionBuildingAndArrow();
	CollisionCharacterAndArrow();
	InfantryAttackBuilding();
	CollisionInfantryAndBullet();
	CollisionInfantryAndArrow();

	CheckObjectsLife();

	if (m_fTimeTerm_CreateCharacter_Team1 > 1.5f)
	{
		float fRandomX = float(rand() % WINCX - 250.f);
		float fRandomY = float(rand() % (WINCY / 2));

		CreateObjects(fRandomX, fRandomY, OBJECT_CHARACTER, OBJECT_TEAM1);

		m_fTimeTerm_CreateCharacter_Team1 = 0.f;
	}

	if (m_fTimeTerm_CreateInfantry_Team1 > 2.f)
	{
		float fRandomX = float(rand() % WINCX - 250.f);
		float fRandomY = float(rand() % ((WINCY / 2) - 100));

		CreateObjects(fRandomX, fRandomY, OBJECT_INFANTRY, OBJECT_TEAM1);

		m_fTimeTerm_CreateInfantry_Team1 = 0.f;
	}

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] != NULL)
		{
			m_Building[i]->Update(fElapsedTime);

			if (m_Building[i]->GetTimeTerm_Bullet() > 2.f)
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

			if (m_Character[i]->GetTimeTerm_Arrow() > 0.5f)
			{
				float fX = m_Character[i]->GetPos().fX;
				float fY = m_Character[i]->GetPos().fY;

				OBJECT_TEAM objTeam = m_Character[i]->GetObjTeam();

				CreateObjects(fX, fY, OBJECT_ARROW, objTeam, i);

				m_Character[i]->SetTimeTerm_Arrow(0.f);
			}
		}

		if (m_Infantry[i] != NULL)
			m_Infantry[i]->Update(fElapsedTime);

		if (m_Bullet[i] != NULL)
			m_Bullet[i]->Update(fElapsedTime);

		if (m_Arrow[i] != NULL)
			m_Arrow[i]->Update(fElapsedTime);
	}
}

void CSceneMgr::Render()
{
	// Renderer
	m_Renderer->DrawTexturedRect(0.f, 0.f, 0.f, 800.f, 1.f, 1.f, 1.f, 1.f, m_texBackground, RENDER_LEVEL_BACKGROUND);

	m_Renderer->DrawParticleClimate(0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, -0.1f, -0.1f, m_texClimateParticle, m_fTime_ClimateParticle, 0.01f);

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] != NULL)
		{
			if (m_Building[i]->GetObjTeam() == OBJECT_TEAM1)
			{
				m_Renderer->DrawTexturedRect(m_Building[i]->GetPos().fX, m_Building[i]->GetPos().fY, m_Building[i]->GetPos().fZ, m_Building[i]->GetSize(),
					m_Building[i]->GetColor().fR, m_Building[i]->GetColor().fG, m_Building[i]->GetColor().fB, m_Building[i]->GetColor().fA, m_texBuilding_Team1, RENDER_LEVEL_BUILDING_TEAM1);

				m_Renderer->DrawSolidRectGauge(m_Building[i]->GetPos().fX, m_Building[i]->GetPos().fY + m_Building[i]->GetSize() / 2.f + m_Building[i]->GetSize() / 5.f, m_Building[i]->GetPos().fZ,
					m_Building[i]->GetSize() * 1.4f, m_Building[i]->GetSize() / 6.f, 1.f, 0.f, 0.f, 1.f, m_Building[i]->GetLife() / m_Building[i]->GetMaxLife(), RENDER_LEVEL_BUILDING_TEAM1);
			}
			else
			{
				m_Renderer->DrawTexturedRect(m_Building[i]->GetPos().fX, m_Building[i]->GetPos().fY, m_Building[i]->GetPos().fZ, m_Building[i]->GetSize(),
					m_Building[i]->GetColor().fR, m_Building[i]->GetColor().fG, m_Building[i]->GetColor().fB, m_Building[i]->GetColor().fA, m_texBuilding_Team2, RENDER_LEVEL_BUILDING_TEAM2);

				m_Renderer->DrawSolidRectGauge(m_Building[i]->GetPos().fX, m_Building[i]->GetPos().fY + m_Building[i]->GetSize() / 2.f + m_Building[i]->GetSize() / 5.f, m_Building[i]->GetPos().fZ,
					m_Building[i]->GetSize() * 1.4f, m_Building[i]->GetSize() / 6.f, 0.f, 0.f, 1.f, 1.f, m_Building[i]->GetLife() / m_Building[i]->GetMaxLife(), RENDER_LEVEL_BUILDING_TEAM2);
			}
		}

		if (m_Character[i] != NULL)
		{
			if (m_Character[i]->GetObjTeam() == OBJECT_TEAM1)
			{
				// 캐릭터가 움직일 때 애니메이션을 방향에 맞게 설정 (x축 양의 방향으로 움직일 때와 음의 방향으로 움직일 때 차이)
				if (m_Character[i]->GetDir().fX > 0)
				{
					m_Renderer->DrawTexturedRectSeq(m_Character[i]->GetPos().fX, m_Character[i]->GetPos().fY, m_Character[i]->GetPos().fZ, m_Character[i]->GetSize(),
						m_Character[i]->GetColor().fR, m_Character[i]->GetColor().fG, m_Character[i]->GetColor().fB, m_Character[i]->GetColor().fA, m_texCharacter_Team1, m_Character[i]->GetSpriteX(), 0, 8, 1, RENDER_LEVEL_CHARACTER);
				}
				else
				{
					m_Renderer->DrawTexturedRectSeq(m_Character[i]->GetPos().fX, m_Character[i]->GetPos().fY, m_Character[i]->GetPos().fZ, m_Character[i]->GetSize(),
						m_Character[i]->GetColor().fR, m_Character[i]->GetColor().fG, m_Character[i]->GetColor().fB, m_Character[i]->GetColor().fA, m_texCharacter_Reverse_Team1, m_Character[i]->GetSpriteX(), 0, 8, 1, RENDER_LEVEL_CHARACTER);
				}

				m_Renderer->DrawSolidRectGauge(m_Character[i]->GetPos().fX, m_Character[i]->GetPos().fY + m_Character[i]->GetSize() / 2.f + m_Character[i]->GetSize() / 5.f, m_Character[i]->GetPos().fZ,
					m_Character[i]->GetSize() * 1.4f, m_Character[i]->GetSize() / 6.f, 1.f, 0.f, 0.f, 1.f, m_Character[i]->GetLife() / m_Character[i]->GetMaxLife(), RENDER_LEVEL_CHARACTER);
			}
			else
			{
				// 캐릭터가 움직일 때 애니메이션을 방향에 맞게 설정 (x축 양의 방향으로 움직일 때와 음의 방향으로 움직일 때 차이) 
				if (m_Character[i]->GetDir().fX > 0)
				{
					m_Renderer->DrawTexturedRectSeq(m_Character[i]->GetPos().fX, m_Character[i]->GetPos().fY, m_Character[i]->GetPos().fZ, m_Character[i]->GetSize(),
						m_Character[i]->GetColor().fR, m_Character[i]->GetColor().fG, m_Character[i]->GetColor().fB, m_Character[i]->GetColor().fA, m_texCharacter_Team2, m_Character[i]->GetSpriteX(), 0, 8, 1, RENDER_LEVEL_CHARACTER);
				}
				else
				{
					m_Renderer->DrawTexturedRectSeq(m_Character[i]->GetPos().fX, m_Character[i]->GetPos().fY, m_Character[i]->GetPos().fZ, m_Character[i]->GetSize(),
						m_Character[i]->GetColor().fR, m_Character[i]->GetColor().fG, m_Character[i]->GetColor().fB, m_Character[i]->GetColor().fA, m_texCharacter_Reverse_Team2, m_Character[i]->GetSpriteX(), 0, 8, 1, RENDER_LEVEL_CHARACTER);
				}

				m_Renderer->DrawSolidRectGauge(m_Character[i]->GetPos().fX, m_Character[i]->GetPos().fY + m_Character[i]->GetSize() / 2.f + m_Character[i]->GetSize() / 5.f, m_Character[i]->GetPos().fZ,
					m_Character[i]->GetSize() * 1.4f, m_Character[i]->GetSize() / 6.f, 0.f, 0.f, 1.f, 1.f, m_Character[i]->GetLife() / m_Character[i]->GetMaxLife(), RENDER_LEVEL_CHARACTER);
			}
		}

		if (m_Infantry[i] != NULL)
		{
			if (m_Infantry[i]->GetObjTeam() == OBJECT_TEAM1)
			{
				if (m_Infantry[i]->GetState() == STATE_WALK)
				{
					m_Renderer->DrawTexturedRectSeq(m_Infantry[i]->GetPos().fX, m_Infantry[i]->GetPos().fY, m_Infantry[i]->GetPos().fZ, m_Infantry[i]->GetSize(),
						m_Infantry[i]->GetColor().fR, m_Infantry[i]->GetColor().fG, m_Infantry[i]->GetColor().fB, m_Infantry[i]->GetColor().fA, m_texInfantry_Team1, m_Infantry[i]->GetSpriteX(), 0, 12, 1, RENDER_LEVEL_INFANTRY);
				}
				else if (m_Infantry[i]->GetState() == STATE_ATTACK)
				{
					m_Renderer->DrawTexturedRectSeq(m_Infantry[i]->GetPos().fX, m_Infantry[i]->GetPos().fY, m_Infantry[i]->GetPos().fZ, m_Infantry[i]->GetSize(),
						m_Infantry[i]->GetColor().fR, m_Infantry[i]->GetColor().fG, m_Infantry[i]->GetColor().fB, m_Infantry[i]->GetColor().fA, m_texInfantry_Attack_Team1, m_Infantry[i]->GetSpriteX(), 0, 9, 1, RENDER_LEVEL_INFANTRY);
				}

				m_Renderer->DrawSolidRectGauge(m_Infantry[i]->GetPos().fX, m_Infantry[i]->GetPos().fY + m_Infantry[i]->GetSize() / 2.f + m_Infantry[i]->GetSize() / 5.f, m_Infantry[i]->GetPos().fZ,
					m_Infantry[i]->GetSize() * 1.4f, m_Infantry[i]->GetSize() / 6.f, 1.f, 0.f, 0.f, 1.f, m_Infantry[i]->GetLife() / m_Infantry[i]->GetMaxLife(), RENDER_LEVEL_INFANTRY);
			}
			else
			{
				if (m_Infantry[i]->GetState() == STATE_WALK)
				{
					m_Renderer->DrawTexturedRectSeq(m_Infantry[i]->GetPos().fX, m_Infantry[i]->GetPos().fY, m_Infantry[i]->GetPos().fZ, m_Infantry[i]->GetSize(),
						m_Infantry[i]->GetColor().fR, m_Infantry[i]->GetColor().fG, m_Infantry[i]->GetColor().fB, m_Infantry[i]->GetColor().fA, m_texInfantry_Team2, m_Infantry[i]->GetSpriteX(), 0, 12, 1, RENDER_LEVEL_INFANTRY);
				}
				else if (m_Infantry[i]->GetState() == STATE_ATTACK)
				{
					if (m_Infantry[i]->GetPos().fX > m_Infantry[i]->GetTargetPos().fX)
					{
						m_Renderer->DrawTexturedRectSeq(m_Infantry[i]->GetPos().fX, m_Infantry[i]->GetPos().fY, m_Infantry[i]->GetPos().fZ, m_Infantry[i]->GetSize(),
							m_Infantry[i]->GetColor().fR, m_Infantry[i]->GetColor().fG, m_Infantry[i]->GetColor().fB, m_Infantry[i]->GetColor().fA, m_texInfantry_Attack_Team2, m_Infantry[i]->GetSpriteX(), 0, 9, 1, RENDER_LEVEL_INFANTRY);
					}
					else
					{
						m_Renderer->DrawTexturedRectSeq(m_Infantry[i]->GetPos().fX, m_Infantry[i]->GetPos().fY, m_Infantry[i]->GetPos().fZ, m_Infantry[i]->GetSize(),
							m_Infantry[i]->GetColor().fR, m_Infantry[i]->GetColor().fG, m_Infantry[i]->GetColor().fB, m_Infantry[i]->GetColor().fA, m_texInfantry_Attack_Reverse_Team2, m_Infantry[i]->GetSpriteX(), 0, 9, 1, RENDER_LEVEL_INFANTRY);
					}
				}
				m_Renderer->DrawSolidRectGauge(m_Infantry[i]->GetPos().fX, m_Infantry[i]->GetPos().fY + m_Infantry[i]->GetSize() / 2.f + m_Infantry[i]->GetSize() / 5.f, m_Infantry[i]->GetPos().fZ,
					m_Infantry[i]->GetSize() * 1.4f, m_Infantry[i]->GetSize() / 6.f, 0.f, 0.f, 1.f, 1.f, m_Infantry[i]->GetLife() / m_Infantry[i]->GetMaxLife(), RENDER_LEVEL_INFANTRY);
			}
		}

		if (m_Bullet[i] != NULL)
		{
			if (m_Bullet[i]->GetObjTeam() == OBJECT_TEAM1)
			{
				m_Renderer->DrawParticle(m_Bullet[i]->GetPos().fX, m_Bullet[i]->GetPos().fY, m_Bullet[i]->GetPos().fZ, m_Bullet[i]->GetSize(),
					m_Bullet[i]->GetColor().fR, m_Bullet[i]->GetColor().fG, m_Bullet[i]->GetColor().fB, m_Bullet[i]->GetColor().fA, -(m_Bullet[i]->GetDir().fX), -(m_Bullet[i]->GetDir().fY), m_texBullet_Team1, m_Bullet[i]->GetParticleTime(), RENDER_LEVEL_BULLET);
			}
			else
			{
				m_Renderer->DrawParticle(m_Bullet[i]->GetPos().fX, m_Bullet[i]->GetPos().fY, m_Bullet[i]->GetPos().fZ, m_Bullet[i]->GetSize(),
					m_Bullet[i]->GetColor().fR, m_Bullet[i]->GetColor().fG, m_Bullet[i]->GetColor().fB, m_Bullet[i]->GetColor().fA, -(m_Bullet[i]->GetDir().fX), -(m_Bullet[i]->GetDir().fY), m_texBullet_Team2, m_Bullet[i]->GetParticleTime(), RENDER_LEVEL_BULLET);
			}
		}

		if (m_Arrow[i] != NULL)
		{
			m_Renderer->DrawSolidRect(m_Arrow[i]->GetPos().fX, m_Arrow[i]->GetPos().fY, m_Arrow[i]->GetPos().fZ, m_Arrow[i]->GetSize(),
				m_Arrow[i]->GetColor().fR, m_Arrow[i]->GetColor().fG, m_Arrow[i]->GetColor().fB, m_Arrow[i]->GetColor().fA, RENDER_LEVEL_ARROW);
		}
	}

	m_Renderer->DrawTextW(-80.f, 0.f, GLUT_BITMAP_TIMES_ROMAN_24, 1.f, 0.f, 0.f, "SELECT CHARACTER, PRESS 1 OR 2");
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

void CSceneMgr::CollisionInfantryAndBullet()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Infantry[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Bullet[j] == NULL)
				continue;

			if (m_Bullet[j]->GetObjTeam() == m_Infantry[i]->GetObjTeam())
				continue;

			if (CheckCollision(m_Infantry[i], m_Bullet[j]))
			{
				float fDamage = m_Bullet[j]->GetLife();

				m_Infantry[i]->DecreaseLife(fDamage);
				m_Bullet[j]->SetDead();

				if (m_Infantry[i]->GetLife() <= 0.f)
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

void CSceneMgr::CollisionInfantryAndArrow()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Infantry[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Arrow[j] == NULL)
				continue;

			if (m_Arrow[j]->GetParentNum() == i)			// 자신을 생성한 부모일 경우 충돌 처리 안함
				continue;

			if (m_Arrow[j]->GetObjTeam() == m_Infantry[i]->GetObjTeam())
				continue;

			if (CheckCollision(m_Infantry[i], m_Arrow[j]))
			{
				float fDamage = m_Arrow[j]->GetLife();

				m_Infantry[i]->DecreaseLife(fDamage);
				m_Arrow[j]->SetDead();

				if (m_Infantry[i]->GetLife() <= 0.f)
					break;
			}
		}
	}
}

void CSceneMgr::CollisionBuildingAndBullet()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Building[i] == NULL)
			continue;

		for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
		{
			if (m_Bullet[j] == NULL)
				continue;

			if (m_Bullet[j]->GetParentNum() == i)			// 자신을 생성한 부모일 경우 충돌 처리 안함
				continue;

			if (m_Bullet[j]->GetObjTeam() == m_Building[i]->GetObjTeam())
				continue;

			if (CheckCollision(m_Building[i], m_Bullet[j]))
			{
				float fDamage = m_Bullet[j]->GetLife();

				m_Building[i]->DecreaseLife(fDamage);
				m_Bullet[j]->SetDead();

				if (m_Building[i]->GetLife() <= 0.f)
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
		m_Building[iNum]->SetMaxLife(500.f);
		m_Building[iNum]->SetColor(1.f, 1.f, 1.f, 1.f);

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
		m_Character[iNum]->SetSize(50.f);
		m_Character[iNum]->SetSpeed(150.f);
		m_Character[iNum]->SetMaxLife(10.f);
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
	case OBJECT_INFANTRY:
		if (m_InfantryCount >= MAX_OBJECTS_COUNT)
			return;

		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		{
			if (m_Infantry[i] == NULL)
			{
				iNum = i;
				break;
			}
		}

		m_Infantry[iNum] = new CObject(OBJECT_INFANTRY, ObjTeam);
		m_Infantry[iNum]->Init();
		m_Infantry[iNum]->SetPos(fX, fY);
		m_Infantry[iNum]->SetSize(50.f);
		m_Infantry[iNum]->SetSpeed(40.f);
		m_Infantry[iNum]->SetMaxLife(10.f);
		m_Infantry[iNum]->SetLife(10.f);

		SetTarget(iNum);

		if (ObjTeam == OBJECT_TEAM1)
		{
			m_Infantry[iNum]->SetColor(1.f, 0.f, 0.f, 1.f);
		}
		else
		{
			m_Infantry[iNum]->SetColor(0.f, 0.f, 1.f, 1.f);
		}

		++m_InfantryCount;
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
		m_Bullet[iNum]->SetSize(6.f);
		m_Bullet[iNum]->SetSpeed(200.f);
		m_Bullet[iNum]->SetLife(20.f);
		m_Bullet[iNum]->SetMaxLife(20.f);
		m_Bullet[iNum]->SetParentNum(iParentNum);
		m_Bullet[iNum]->SetColor(1.f, 1.f, 1.f, 1.f);

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
		m_Arrow[iNum]->SetSize(4.f);
		m_Arrow[iNum]->SetSpeed(200.f);
		m_Arrow[iNum]->SetLife(10.f);
		m_Arrow[iNum]->SetMaxLife(10.f);
		m_Arrow[iNum]->SetParentNum(iParentNum);

		if (ObjTeam == OBJECT_TEAM1)
		{
			m_Arrow[iNum]->SetColor(0.5f, 0.2f, 0.7f, 1.f);
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
				OBJECT_TEAM ObjTeam = m_Building[i]->GetObjTeam();

				delete m_Building[i];
				m_Building[i] = NULL;

				--m_BuildingCount;

				ResetTarget();
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

		if (m_Infantry[i] != NULL)
		{
			if (m_Infantry[i]->GetLife() <= 0.f)
			{
				delete m_Infantry[i];
				m_Infantry[i] = NULL;

				--m_InfantryCount;
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

void CSceneMgr::ResetTarget()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Infantry[i] == NULL)
			continue;

		m_Infantry[i]->SetState(STATE_WALK);
		SetTarget(i);
	}
}

void CSceneMgr::SetTarget(int iNum)
{
	float fDirX = 0.f;
	float fDirY = 0.f;
	float fTargetPosX = 0.f;
	float fTargetPosY = 0.f;
	float fMinDist = 1000.f;

	if (m_Infantry[iNum]->GetObjTeam() == OBJECT_TEAM1)
	{
		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		{
			if (m_Building[i] == NULL)
				continue;

			if (m_Building[i]->GetObjTeam() == OBJECT_TEAM1)
				continue;

			float fX = m_Building[i]->GetPos().fX - m_Infantry[iNum]->GetPos().fX;
			float fY = m_Building[i]->GetPos().fY - m_Infantry[iNum]->GetPos().fY;
			float fDist = sqrtf(fX * fX + fY * fY);

			if (fDist < fMinDist)
			{
				fDirX = fX / fDist;
				fDirY = fY / fDist;

				fTargetPosX = m_Building[i]->GetPos().fX;
				fTargetPosY = m_Building[i]->GetPos().fY;

				fMinDist = fDist;
			}
		}

		if (fMinDist == 1000.f)
		{
			m_Infantry[iNum]->SetDir(0.f, -1.f, 0.f);
			m_Infantry[iNum]->SetTargetPos(-1000.f, 0.f, 0.f);
			return;
		}
	}
	else if (m_Infantry[iNum]->GetObjTeam() == OBJECT_TEAM2)
	{
		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		{
			if (m_Building[i] == NULL)
				continue;

			if (m_Building[i]->GetObjTeam() == OBJECT_TEAM2)
				continue;

			float fX = m_Building[i]->GetPos().fX - m_Infantry[iNum]->GetPos().fX;
			float fY = m_Building[i]->GetPos().fY - m_Infantry[iNum]->GetPos().fY;
			float fDist = sqrtf(fX * fX + fY * fY);

			if (fDist < fMinDist)
			{
				fDirX = fX / fDist;
				fDirY = fY / fDist;

				fTargetPosX = m_Building[i]->GetPos().fX;
				fTargetPosY = m_Building[i]->GetPos().fY;

				fMinDist = fDist;
			}
		}

		if (fMinDist == 1000.f)
		{
			m_Infantry[iNum]->SetDir(0.f, 1.f, 0.f);
			m_Infantry[iNum]->SetTargetPos(1000.f, 0.f, 0.f);
			return;
		}
	}

	m_Infantry[iNum]->SetDir(fDirX, fDirY, 0.f);
	m_Infantry[iNum]->SetTargetPos(fTargetPosX, fTargetPosY);
}

void CSceneMgr::InfantryAttackBuilding()
{
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (m_Infantry[i] == NULL)
			continue;

		if (m_Infantry[i]->GetObjTeam() == OBJECT_TEAM1)
		{
			for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
			{
				if (m_Building[j] == NULL)
					continue;

				if (m_Building[j]->GetObjTeam() == OBJECT_TEAM1)
					continue;

				if (m_Infantry[i]->GetTargetPos().fX == m_Building[j]->GetPos().fX)
				{
					if (m_Infantry[i]->GetState() == STATE_ATTACK &&
						m_Infantry[i]->GetSpriteX() >= 9)
					{
						m_Infantry[i]->SetSpriteX(0);

						float fDamage = m_Infantry[i]->GetLife();

						m_Building[j]->DecreaseLife(fDamage);

						if (m_Building[j]->GetLife() <= 0.f)
							break;
					}
				}
			}
		}
		else if (m_Infantry[i]->GetObjTeam() == OBJECT_TEAM2)
		{
			for (int j = 0; j < MAX_OBJECTS_COUNT; ++j)
			{
				if (m_Building[j] == NULL)
					continue;

				if (m_Building[j]->GetObjTeam() == OBJECT_TEAM2)
					continue;

				if (m_Infantry[i]->GetTargetPos().fX == m_Building[j]->GetPos().fX)
				{
					if (m_Infantry[i]->GetState() == STATE_ATTACK &&
						m_Infantry[i]->GetSpriteX() >= 9)
					{
						m_Infantry[i]->SetSpriteX(0);

						float fDamage = m_Infantry[i]->GetLife();

						m_Building[j]->DecreaseLife(fDamage);

						if (m_Building[j]->GetLife() <= 0.f)
							break;
					}
				}
			}
		}
	}
}
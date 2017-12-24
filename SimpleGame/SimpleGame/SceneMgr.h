#ifndef SceneMgr_h__
#define SceneMgr_h__

#define WINCX 500
#define WINCY 700

#define MAX_OBJECTS_COUNT 1000

enum OBJECT_TYPE
{
	OBJECT_BUILDING,
	OBJECT_CHARACTER,
	OBJECT_INFANTRY,
	OBJECT_BULLET,
	OBJECT_ARROW
};

enum OBJECT_TEAM
{
	OBJECT_TEAM1,
	OBJECT_TEAM2
};

enum OBJECT_STATE
{
	STATE_WALK,
	STATE_ATTACK
};

class CObject;
class Renderer;
class Sound;

class CSceneMgr
{
public:
	CSceneMgr();
	~CSceneMgr();	

private:
	int			m_BuildingCount;
	int			m_CharacterCount;
	int			m_InfantryCount;
	int			m_BulletCount;
	int			m_ArrowCount;
	int			m_texBackground;
	int			m_texBuilding_Team1;
	int			m_texBuilding_Team2;
	int			m_texCharacter_Team1;
	int			m_texCharacter_Team2;
	int			m_texCharacter_Reverse_Team1;
	int			m_texCharacter_Reverse_Team2;
	int			m_texInfantry_Team1;
	int			m_texInfantry_Attack_Team1;
	int			m_texInfantry_Attack_Reverse_Team1;
	int			m_texInfantry_Team2;
	int			m_texInfantry_Attack_Team2;
	int			m_texInfantry_Attack_Reverse_Team2;
	int			m_texBullet_Team1;
	int			m_texBullet_Team2;
	int			m_texArrow_Team1;
	int			m_texArrow_Team2;
	int			m_texClimateParticle;
	int			m_soundBackground;
	float		m_fTimeTerm_CreateCharacter_Team1;
	float		m_fTimeTerm_CreateInfantry_Team1;
	float		m_fTime_ClimateParticle;

private:
	CObject*	m_Building[MAX_OBJECTS_COUNT];
	CObject*	m_Character[MAX_OBJECTS_COUNT];
	CObject*	m_Infantry[MAX_OBJECTS_COUNT];
	CObject*	m_Bullet[MAX_OBJECTS_COUNT];
	CObject*	m_Arrow[MAX_OBJECTS_COUNT];
	Renderer*	m_Renderer;
	Sound*		m_Sound;

public:
	void		Init();
	void		Update(float fElapsedTime);
	void		Render();
	bool		CheckCollision(CObject* objA, CObject* objB);
	void		CollisionBuildingAndCharacter();
	void		CollisionCharacterAndBullet();
	void		CollisionBuildingAndArrow();
	void		CollisionCharacterAndArrow();
	void		CollisionBuildingAndBullet();
	void		InfantryAttackBuilding();
	void		CollisionInfantryAndBullet();
	void		CollisionInfantryAndArrow();
	void		SetTarget(int iNum);
	void		ResetTarget();

public:
	int			GetBuildingCount() { return m_BuildingCount; }
	int			GetCharacterCount() { return m_CharacterCount; }
	int			GetBulletCount() { return m_BulletCount; }
	int			GetArrowCount() { return m_ArrowCount; }

public:
	void		CreateObjects(float fX, float fY, OBJECT_TYPE ObjType, OBJECT_TEAM ObjTeam, int iParentNum = -1);
	void		CheckObjectsLife();
};

#endif // SceneMgr_h__

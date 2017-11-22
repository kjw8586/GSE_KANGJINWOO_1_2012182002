#ifndef SceneMgr_h__
#define SceneMgr_h__

#define WINCX 500
#define WINCY 700

#define MAX_OBJECTS_COUNT 1000

enum OBJECT_TYPE
{
	OBJECT_BUILDING,
	OBJECT_CHARACTER,
	OBJECT_BULLET,
	OBJECT_ARROW
};

enum OBJECT_TEAM
{
	OBJECT_TEAM1,
	OBJECT_TEAM2
};

class CObject;
class Renderer;

class CSceneMgr
{
public:
	CSceneMgr();
	~CSceneMgr();	

private:
	int			m_BuildingCount;
	int			m_CharacterCount;
	int			m_BulletCount;
	int			m_ArrowCount;
	int			m_texBuilding_Team1;
	int			m_texBuilding_Team2;
	int			m_texCharacter;
	int			m_texBullet;
	int			m_texArrow;
	float		m_fTimeTerm_CreateCharacter_Team1;
private:
	CObject*	m_Building[MAX_OBJECTS_COUNT];
	CObject*	m_Character[MAX_OBJECTS_COUNT];
	CObject*	m_Bullet[MAX_OBJECTS_COUNT];
	CObject*	m_Arrow[MAX_OBJECTS_COUNT];
	Renderer*	m_Renderer;

public:
	void		Init();
	void		Update(float fElapsedTime);
	void		Render();
	bool		CheckCollision(CObject* objA, CObject* objB);
	void		CollisionBuildingAndCharacter();
	void		CollisionCharacterAndBullet();
	void		CollisionBuildingAndArrow();
	void		CollisionCharacterAndArrow();

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

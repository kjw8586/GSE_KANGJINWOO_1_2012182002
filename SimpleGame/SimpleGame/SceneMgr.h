#ifndef SceneMgr_h__
#define SceneMgr_h__

#define MAX_OBJECTS_COUNT 1000

enum OBJECTTYPE
{
	OBJECT_BUILDING,
	OBJECT_CHARACTER,
	OBJECT_BULLET,
	OBJECT_ARROW
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
	int			m_texBuilding;
	int			m_texCharacter;
	int			m_texBullet;
	int			m_texArrow;

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
	void		CreateObjects(float fX, float fY, OBJECTTYPE ObjType, int iParentNum = -1);
	void		CheckObjectsLife();
};

#endif // SceneMgr_h__

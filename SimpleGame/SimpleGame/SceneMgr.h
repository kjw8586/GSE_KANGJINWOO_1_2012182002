#ifndef SceneMgr_h__
#define SceneMgr_h__

#define MAX_OBJECTS_COUNT 100
class CObject;
class Renderer;

class CSceneMgr
{
public:
	CSceneMgr();
	~CSceneMgr();

private:
	int			m_ObjectsCount;

private:
	CObject*	m_Objects[MAX_OBJECTS_COUNT];
	Renderer*	m_Renderer;

public:
	void		Init();
	void		Update(float fElapsedTime);
	void		Render();
	bool		CheckCollision(CObject* objA, CObject* objB);

public:
	CObject*	GetObjects(int iObjectNum) { return m_Objects[iObjectNum]; }
	int			GetObjectsCount() { return m_ObjectsCount; }

public:
	void		CreateObjects(float fX, float fY);
	void		IncreaseObjectsCount();
	void		CheckObjectsLife();
};

#endif // SceneMgr_h__

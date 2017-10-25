#ifndef SceneMgr_h__
#define SceneMgr_h__

#define MAX_OBJECTS_COUNT 100
class CObject;

class CSceneMgr
{
public:
	CSceneMgr();
	~CSceneMgr();

private:
	int m_ObjectsCount;

private:
	CObject* m_Objects[MAX_OBJECTS_COUNT];

public:
	void Init();
	void Update();
	bool CheckCollision(CObject* objA, CObject* objB);

public:
	CObject* GetObjects(int iObjectNum) { return m_Objects[iObjectNum]; }
};

#endif // SceneMgr_h__

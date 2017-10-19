#ifndef SceneMgr_h__
#define SceneMgr_h__

#define MAX_OBJECTS_COUNT 100
class CObject;

class CSceneMgr
{
public:
	CSceneMgr();
	~CSceneMgr();

public:
	void Init();
	void Update();
	void CheckCollision();

public:
	CObject* GetObjects(int iObjectNum) { return m_Objects[iObjectNum]; }

private:
	CObject* m_Objects[MAX_OBJECTS_COUNT];
};

#endif // SceneMgr_h__

#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>

#include "Nodes.h"

class nuralScene : public Scene
{
public:
	nuralScene(const std::string& friendly_name);
	virtual ~nuralScene();

	virtual void OnInitializeScene()	 override;
	virtual void OnCleanupScene()		 override;
	virtual void OnUpdateScene(float dt) override;

	void setSpawnPoint(int i) { i = m_spawnPoint; };

	Object* getPlayer() { return m_pPlayer; };
	Object* getAI() { return m_pAI; };

protected:
	bool training = false;
	int loading;
	bool lcomplete;

	int m_spawnPoint;
	float m_AccumTime;
	Object* m_pPlayer;
	Vector3 m_pvPlayer;
	Object* m_pAI;
	Vector3 m_pvAI;
	Object* m_floor[30][30];
	Object* m_walls[30];
};
#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>

class planetScene : public Scene
{
public:
	planetScene(const std::string& friendly_name);
	virtual ~planetScene();

	virtual void OnInitializeScene()	 override;
	virtual void OnCleanupScene()		 override;
	virtual void OnUpdateScene(float dt) override;

protected:
	float m_AccumTime;
	int score;
	Object* m_pPlayer;
	Object* target;
	Object* planet;
};
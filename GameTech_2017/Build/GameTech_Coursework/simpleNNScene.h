#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>

#include "Nodes.h"

class simpleNNScene : public Scene
{
public:
	simpleNNScene(const std::string& friendly_name);
	virtual ~simpleNNScene();

	virtual void OnInitializeScene()	 override;
	virtual void OnCleanupScene()		 override;
	virtual void OnUpdateScene(float dt) override;


protected:
	bool training = false;

	float m_AccumTime;
};


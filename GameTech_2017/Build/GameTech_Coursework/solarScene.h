#pragma once

#include <ncltech\NetworkBase.h>

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\DistConTooClose.h>


class solarScene : public Scene
{
public:
	solarScene(const std::string& friendly_name);
	virtual ~solarScene();

	virtual void OnInitializeScene()	 override;
	virtual void OnCleanupScene()		 override;
	virtual void OnUpdateScene(float dt) override;

	void ProcessNetworkEvent(const ENetEvent& evnt);

protected:
	float m_AccumTime;
	Object* m_pPlayer;
	Object* sun;
	Object* planet1;
	Object* planet2;
	Object* planet3;
	Object* planet4;
	Object* planet5;
	Object* m_pObj;

	Vector3 destPos;
	Vector3 pos;

	DistConTooClose* constraint;

	bool hasTarget = false;
	bool inMotion = false;
	bool reachedTarget = false;

	int currentTarget;

	NetworkBase m_Network;
	ENetPeer*	m_pServerConnection;
};
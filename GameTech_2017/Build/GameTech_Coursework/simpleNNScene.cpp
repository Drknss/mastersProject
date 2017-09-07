#include "simpleNNScene.h"

#include <nclgl\Vector4.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>
using namespace CommonUtils;

simpleNNScene::simpleNNScene(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_AccumTime(0.0f)
{
}

simpleNNScene::~simpleNNScene()
{

}

void simpleNNScene::OnInitializeScene()
{
	//Disable the physics engine (We will be starting this later!)
	PhysicsEngine::Instance()->SetPaused(false);
	SceneManager::Instance()->SetVsyncEnabled(false);

	//Set the camera position
	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(15.0f, 10.0f, -15.0f));
	SceneManager::Instance()->GetCamera()->SetYaw(140.f);
	SceneManager::Instance()->GetCamera()->SetPitch(-20.f);

	m_AccumTime = 0.0f;

	//Example usage of Log and LogE inside NCLDebug functionality
	//NCLDebug::Log("This is a log entry");
	//NCLERROR("THIS IS AN ERROR!");



	//Create Walls
	Object* wall = BuildCuboidObject("RearWall", Vector3(-15.0f, 1.0f, 0.5f), Vector3(0.5f, 1.0f, 15.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	this->AddGameObject(wall);
}

void simpleNNScene::OnCleanupScene()
{
	//Just delete all created game objects 
	//  - this is the default command on any Scene instance so we don't really need to override this function here.
	Scene::OnCleanupScene();
}

void simpleNNScene::OnUpdateScene(float dt)
{
	m_AccumTime += dt;
	
}
#include "planetScene.h"

#include <nclgl\Vector4.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>

using namespace CommonUtils;

planetScene::planetScene(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_AccumTime(0.0f)
	, m_pPlayer(NULL)
{
}

planetScene::~planetScene()
{

}


void planetScene::OnInitializeScene()
{
	//Disable the physics engine (We will be starting this later!)
	PhysicsEngine::Instance()->SetPaused(true);

	//Set the camera position
	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(15.0f, 10.0f, -15.0f));
	SceneManager::Instance()->GetCamera()->SetYaw(140.f);
	SceneManager::Instance()->GetCamera()->SetPitch(-20.f);

	m_AccumTime = 0.0f;

	//Example usage of Log and LogE inside NCLDebug functionality
	NCLDebug::Log("This is a log entry");
	NCLERROR("THIS IS AN ERROR!");

	float scale = 2.25f * 0.85f;
	float distScale = 2.0f*1.5f;

	//<--- SCENE CREATION --->

	Vector3 pos = Vector3();
	planet = BuildSphereObject(
		"",						// Optional: Name
		pos,					// Position
		15.0f,					// Half-Dimensions
		true,					// Physics Enabled?
		0.000001f,				// Physical Mass (must have physics enabled)
		true,					// Physically Collidable (has collision shape)
		false,					// Dragable by user?
		Vector4(1, 1, 1, 1),	// Render colour
		true,					// In space?
		1,						// Gravity Type; 0 Planer, 1 Relative, 2 None
		4);
	planet->Physics()->SetAngularVelocity(Vector3(0, -1.0f, 0));
	this->AddGameObject(planet);

	Object* moon = BuildSphereObject(
		"",								// Optional: Name
		Vector3(20.0f*distScale, 2.0f*distScale, 0),	// Position
		1.0f,							// Half-Dimensions
		true,							// Physics Enabled?
		0.1f,							// Physical Mass (must have physics enabled)
		true,							// Physically Collidable (has collision shape)
		false,							// Dragable by user?
		Vector4(1, 1, 1, 1),			// Render colour
		true,							// In space?
		1,								// Gravity Type; 0 Planer, 1 Relative, 2 None
		9);

	moon->Physics()->SetAngularVelocity(Vector3(0, 1.0f, 0));
	moon->Physics()->SetLinearVelocity(Vector3(0, 0, 20.0f*scale));
	this->AddGameObject(moon);

	target = BuildCuboidObject(
		"",								// Optional: Name
		Vector3(16.0f, 0, 0),			// Position
		Vector3(0.1f,2.0f,2.0f),							// Half-Dimensions
		true,							// Physics Enabled?
		0.0f,							// Physical Mass (must have physics enabled)
		true,							// Physically Collidable (has collision shape)
		false,							// Dragable by user?
		Vector4(1, 1, 1, 1),			// Render colour
		false,							// In space?
		2,								// Gravity Type; 0 Planer, 1 Relative, 2 None
		8);

	target->Physics()->SetOnCollisionCallback([&](PhysicsObject* self, PhysicsObject* other_object)
	{
		/*bool temp = */
		if(!other_object->Hit()) {
			NCLDebug::Log("YOU HIT THE TARGET!!");
			score += 10;
			other_object->SetHit(true);
		}
		return false;
	});
	//Target->Physics()->SetLinearVelocity(Vector3(10.0f,0.0f,0.0f));
	this->AddGameObject(target);

	//DistanceConstraint* constraint = new DistanceConstraint(
	//	planet->Physics(),					//Physics Object A
	//	Target->Physics(),					//Physics Object B
	//	planet->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
	//	Target->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
	//PhysicsEngine::Instance()->AddConstraint(constraint);
}

	void planetScene::OnCleanupScene()
	{
		//Just delete all created game objects 
		//  - this is the default command on any Scene instance so we don't really need to override this function here.
		Scene::OnCleanupScene();
	}

	void planetScene::OnUpdateScene(float dt)
	{
		m_AccumTime += dt;

		Vector3 radius = Vector3(16.f, 0.f, 0.f);
		Matrix3 rot = planet->Physics()->GetOrientation().ToMatrix3();

		target->Physics()->SetOrientation(planet->Physics()->GetOrientation());
		target->Physics()->SetPosition(rot * radius);
		planet->Physics()->SetPosition(Vector3(0.f, 0.f, 0.f));
		//NCLDebug::DrawPoint(Vector3(0, 1.0, 0), 2.f);


		// You can add status entries to the top left from anywhere in the program
		NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Current Score");
		NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "Score = %i", score);

		// You can print text using 'printf' formatting
		bool donkeys = false;
		NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 The %s in this scene are dragable", donkeys ? "donkeys" : "cubes");


		// Lets sun a little bit...
		Vector3 invLightDir = Matrix4::Rotation(15.f * dt, Vector3(0.0f, 1.0f, 0.0f)) * SceneManager::Instance()->GetInverseLightDirection();
		SceneManager::Instance()->SetInverseLightDirection(invLightDir);


		//Or move our car around the scene..
		{
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Y))
			{
				Vector3 pos = SceneManager::Instance()->GetCamera()->GetPosition();
				Object* sphere = BuildSphereObject(
					"",					// Optional: Name
					pos,				// Position
					0.5f,				// Half-Dimensions
					true,				// Physics Enabled?
					0.5f,				// Physical Mass (must have physics enabled)
					true,				// Physically Collidable (has collision shape)
					false,				// Dragable by user?
					Vector4(1, 0, 0, 1),// Render colour
					true,				//In Space?
					1);				 

				Matrix3 view = Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix());
				Vector3 forward = Vector3(-view._13, -view._23, -view._33);

				sphere->Physics()->SetLinearVelocity(forward*10.0f);
				this->AddGameObject(sphere);
			}
		}
	}
#include "nuralScene.h"

#include <nclgl\Vector4.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>
using namespace CommonUtils;

nuralScene::nuralScene(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_AccumTime(0.0f)
	, m_pPlayer(NULL)
{
	m_spawnPoint = 0;
}

nuralScene::~nuralScene()
{

}


void nuralScene::OnInitializeScene()
{
	//Disable the physics engine (We will be starting this later!)
	PhysicsEngine::Instance()->SetPaused(false);

	loading = 0;
	lcomplete = false;

	//Set the camera position
	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(15.0f, 10.0f, -15.0f));
	SceneManager::Instance()->GetCamera()->SetYaw(140.f);
	SceneManager::Instance()->GetCamera()->SetPitch(-20.f);

	m_AccumTime = 0.0f;

	//Example usage of Log and LogE inside NCLDebug functionality
	NCLDebug::Log("This is a log entry");
	NCLERROR("THIS IS AN ERROR!");



	//<--- SCENE CREATION --->
	//Create Ground
	//this->AddGameObject(BuildCuboidObject("Ground", Vector3(0.0f, -1.0f, 0.0f), Vector3(20.0f, 1.0f, 20.0f), true, 0.0f, true, false, Vector4(0.2f, 0.5f, 1.0f, 1.0f)));
	auto create_floor_tiles = [&](const Vector3& offset, float cubewidth)
	{
		const Vector3 halfdims = Vector3(cubewidth, cubewidth, cubewidth) * 0.5f;
		for (int x = -15; x < 15; ++x)
		{
			for (int z = -15; z < 15; ++z)
			{
				Vector4 colour = Vector4(0.2f, 0.5f, 1.0f, 1.0f);
				Vector3 pos = offset + Vector3(-x * cubewidth + 0.0001, -1 * cubewidth, -cubewidth * z + 0.0001);

				Object* cube = BuildCuboidObject(
					"",						// Optional: Name
					pos,					// Position
					halfdims,				// Half-Dimensions
					true,					// Physics Enabled?
					0.0f,					// Physical Mass (must have physics enabled)
					true,					// Physically Collidable (has collision shape)
					false,					// Dragable by user?
					colour,
					false,
					0);						// Render colour
				//cube->Physics()->SetAtRest(true);
				m_floor[x + 15][z + 15] = cube;
				this->AddGameObject(cube);
			}
		}
	};

	//Create Walls
	m_walls[0] = BuildCuboidObject("RearWall", Vector3(-15.0f, 1.0f, 0.5f), Vector3(0.5f, 1.0f, 15.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[1] = BuildCuboidObject("FrontWall", Vector3(16.0f, 1.0f, 0.5f), Vector3(0.5f, 1.0f, 15.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[2] = BuildCuboidObject("LeftWall", Vector3(0.5f, 1.0f, 16.0f), Vector3(15.0f, 1.0f, 0.5f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[3] = BuildCuboidObject("RightWall", Vector3(0.5f, 1.0f, -15.0f), Vector3(15.0f, 1.0f, 0.5f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	//Horizontal Walls
	m_walls[4] = BuildCuboidObject("", Vector3(13.5f, 1.0f, -7.5f), Vector3(2.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[5] = BuildCuboidObject("", Vector3(13.5f, 1.0f, 8.5f), Vector3(2.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[6] = BuildCuboidObject("", Vector3(7.5f, 1.0f, -8.5f), Vector3(4.0f, 1.0f, 2.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 2.0f));
	m_walls[7] = BuildCuboidObject("", Vector3(7.5f, 1.0f, -0.5f), Vector3(4.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[8] = BuildCuboidObject("", Vector3(7.5f, 1.0f, 10.5f), Vector3(4.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[9] = BuildCuboidObject("", Vector3(-4.5f, 1.0f, 10.5f), Vector3(4.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[10] = BuildCuboidObject("", Vector3(-4.5f, 1.0f, 0.5f), Vector3(4.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[11] = BuildCuboidObject("", Vector3(-4.5f, 1.0f, -9.5f), Vector3(4.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[12] = BuildCuboidObject("", Vector3(-12.5f, 1.0f, -7.5f), Vector3(2.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[13] = BuildCuboidObject("", Vector3(-12.5f, 1.0f, 5.5f), Vector3(2.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	//Vertical Walls
	m_walls[14] = BuildCuboidObject("", Vector3(-9.5f, 1.0f, -4.5f), Vector3(1.0f, 1.0f, 6.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[15] = BuildCuboidObject("", Vector3(-9.5f, 1.0f, 8.0f), Vector3(1.0f, 1.0f, 3.5f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[16] = BuildCuboidObject("", Vector3(-3.5f, 1.0f, 7.0f), Vector3(1.0f, 1.0f, 2.5f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[17] = BuildCuboidObject("", Vector3(-1.5f, 1.0f, -3.0f), Vector3(1.0f, 1.0f, 2.5f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[18] = BuildCuboidObject("", Vector3(4.5f, 1.0f, 5.0f), Vector3(1.0f, 1.0f, 4.5f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[19] = BuildCuboidObject("", Vector3(10.5f, 1.0f, 7.5f), Vector3(1.0f, 1.0f, 2.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	m_walls[20] = BuildCuboidObject("", Vector3(10.5f, 1.0f, 1.5f), Vector3(1.0f, 1.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	
	int w = 0;
	while (m_walls[w] != NULL) {
		this->AddGameObject(m_walls[w]);
		w++;
	}

	switch (m_spawnPoint)
	{
	case 0:
		m_pvAI =		Vector3(-12, 1, 7.5);
		m_pvPlayer = Vector3(-12, 1, 9.5);
		break;
	case 1:
		m_pvAI = Vector3(-12,1,-5.5);
		m_pvPlayer = Vector3(-12,1,-3.5);
		break;
	case 2:
		m_pvAI = Vector3(-12,1,-10);
		m_pvPlayer = Vector3(-12,1,-11.5);
		break;
	case 3:
		m_pvAI = Vector3(-6.5,1,7.5);
		m_pvPlayer = Vector3(-6.5,1,5.5);
		break;
	case 4:
		m_pvAI = Vector3(-6.5,1,-2.5);
		m_pvPlayer = Vector3(-6.5,1,-4.5);
		break;
	case 5:
		m_pvAI = Vector3(7,1,7);
		m_pvPlayer = Vector3(7,1,5);
		break;
	case 6:
		m_pvAI = Vector3(7,1,1);
		m_pvPlayer = Vector3(7,1,3);
		break;
	case 7:
		m_pvAI = Vector3(13,1,10);
		m_pvPlayer = Vector3(13,1,12);
		break;
	case 8:
		m_pvAI = Vector3(13.5,1,-9.5);
		m_pvPlayer = Vector3(13.5,1,-11.5);
		break;
	default:
		break;
	}

	//Create AI (See OnUpdateScene)
	m_pAI = BuildCuboidObject(
		"AI",						// Optional: Name
		m_pvAI,					// Position
		Vector3(0.5f, 0.5f, 0.5f),	// Half-Dimensions
		true,						// Physics Enabled?
		0.1f,						// Physical Mass (must have physics enabled)
		true,						// Physically Collidable (has collision shape)
		false,						// Dragable by user?
		Vector4(0.1f, 0.1f, 0.1f, 1.0f),
		false,
		0); // Render colour
	m_pAI->Physics()->SetRestEnabled(false);
	this->AddGameObject(m_pAI);


	//Create Player (See OnUpdateScene)
	m_pPlayer = BuildCuboidObject(
		"Player1",					// Optional: Name
		m_pvPlayer,				// Position
		Vector3(0.5f, 0.5f, 0.5f),	// Half-Dimensions
		true,						// Physics Enabled?
		0.1f,						// Physical Mass (must have physics enabled)
		true,						// Physically Collidable (has collision shape)
		false,						// Dragable by user?
		Vector4(0.1f, 0.1f, 0.1f, 1.0f),
		false,
		0); // Render colour
	m_pPlayer->Physics()->SetRestEnabled(false);
	this->AddGameObject(m_pPlayer);


	//Create Floor Tiles
	create_floor_tiles(Vector3(0.0f, 0.5f, 0.0f), 1.0f);

}

void nuralScene::OnCleanupScene()
{
	//Just delete all created game objects 
	//  - this is the default command on any Scene instance so we don't really need to override this function here.
	Scene::OnCleanupScene();
}

void nuralScene::OnUpdateScene(float dt)
{
	if (!lcomplete) {
		if (loading < 60) {
			NCLDebug::Log("Scene Still Loading...");
			loading++;
		}
		else if (loading >= 60)
		{
			NCLDebug::Log("Loading Complete!!");
			lcomplete = true;
		}
	}


	m_AccumTime += dt;
	m_pPlayer->Physics()->SetAtRest(false);
	
	for (int x = 0; x < 30; ++x)
	{
		for (int z = 0; z < 30; ++z)
		{
			if ( m_floor[x][z]->Physics()->GetPosition().x - 0.4f < m_pPlayer->Physics()->GetPosition().x && m_pPlayer->Physics()->GetPosition().x < m_floor[x][z]->Physics()->GetPosition().x + 0.4f) {
				if (m_floor[x][z]->Physics()->GetPosition().z - 0.4f < m_pPlayer->Physics()->GetPosition().z && m_pPlayer->Physics()->GetPosition().z < m_floor[x][z]->Physics()->GetPosition().z + 0.4f) {
					m_floor[x][z]->SetColour(Vector4(1.0, 0.0f, 0.0f, 1.0f));
				}				
			}
		}
	}
	

	
	// You can add status entries to the top left from anywhere in the program
	//NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Welcome to the Game Tech Framework!");
	//NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 You can move the black car with the arrow keys");

	// You can print text using 'printf' formatting
	//bool donkeys = false;
	//NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 The %s in this scene are dragable", donkeys ? "donkeys" : "cubes");


	// Lets sun a little bit...
	Vector3 invLightDir = Matrix4::Rotation(15.f * dt, Vector3(0.0f, 1.0f, 0.0f)) * SceneManager::Instance()->GetInverseLightDirection();
	SceneManager::Instance()->SetInverseLightDirection(invLightDir);


	//Or move our car around the scene..
	{
		const float mv_speed = 10.f * dt;			//Motion: Meters per second
		const float rot_speed = 90.f * dt;			//Rotation: Degrees per second

		bool boosted = false;

		PhysicsObject* pobj = m_pPlayer->Physics();		
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))
		{
			pobj->SetPosition(pobj->GetPosition() +
				pobj->GetOrientation().ToMatrix3() * Vector3(0.0f, 0.0f, -mv_speed));
			boosted = true;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))
		{
			pobj->SetPosition(pobj->GetPosition() +
				pobj->GetOrientation().ToMatrix3()* Vector3(0.0f, 0.0f, mv_speed / 2.f));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT))
		{
			pobj->SetOrientation(pobj->GetOrientation() *
				Quaternion::AxisAngleToQuaterion(Vector3(0.0f, 1.0f, 0.0f), rot_speed));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT))
		{
			pobj->SetOrientation(pobj->GetOrientation() *
				Quaternion::AxisAngleToQuaterion(Vector3(0.0f, 1.0f, 0.0f), -rot_speed));
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Y))
		{
			Vector3 pos = SceneManager::Instance()->GetCamera()->GetPosition();
			Object* sphere = BuildSphereObject(
				"",					// Optional: Name
				pos,				// Position
				0.5f,				// Half-Dimensions
				true,				// Physics Enabled?
				0.1f,				// Physical Mass (must have physics enabled)
				true,				// Physically Collidable (has collision shape)
				false,				// Dragable by user?
				Vector4(1, 0, 0, 1));// Render colour

			Matrix3 view = Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix());
			Vector3 forward = Vector3(-view._13, -view._23, -view._33);

			sphere->Physics()->SetLinearVelocity(forward*10.0f);
			this->AddGameObject(sphere);
		}


		// Also (and importantly), as the projMatrix/viewMatrix is all abstracted away
		//  we can now use debug draw tools to render geometry in world-space from anywhere
		//  in the program. Very useful for debugging!
		if (boosted)
		{
			//Draw the rocket booster on the car using NCLDebug
			Vector3 backward_dir = pobj->GetOrientation().ToMatrix3() * Vector3(0, 0, 1);
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir, 0.3f, Vector4(1.f, 0.7f, 0.0f, 1.0f));
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir * 1.333f, 0.26f, Vector4(0.9f, 0.5f, 0.0f, 1.0f));
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir * 1.666f, 0.23f, Vector4(0.8f, 0.3f, 0.0f, 1.0f));
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir * 2.f, 0.2f, Vector4(0.7f, 0.2f, 0.0f, 1.0f));
		}
	}
}
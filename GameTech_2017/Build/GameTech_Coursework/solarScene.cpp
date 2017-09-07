#include "solarScene.h"

#include <nclgl\Vector4.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>
#include <random>

using namespace CommonUtils;

#define update_rate (1.0f/30.0f)

enum packetType {
	REQUEST,
	MOTION,
	ARRIVED
};

solarScene::solarScene(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_AccumTime(0.0f)
	, m_pPlayer(NULL)
{
}

solarScene::~solarScene()
{
	m_pObj = NULL;

	enet_peer_disconnect_now(m_pServerConnection, 0);

	m_Network.Release();
	m_pServerConnection = NULL;
}


void solarScene::OnInitializeScene()
{
	//Disable the physics engine (We will be starting this later!)
	PhysicsEngine::Instance()->SetPaused(true);

	//Set the camera position
	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(15.0f, 10.0f, -15.0f));
	SceneManager::Instance()->GetCamera()->SetYaw(140.f);
	SceneManager::Instance()->GetCamera()->SetPitch(-20.f);

	m_AccumTime = 0.0f;
	hasTarget = false;

	//Example usage of Log and LogE inside NCLDebug functionality
	NCLDebug::Log("This is a log entry");
	NCLERROR("THIS IS AN ERROR!");

	float scale = 2.25f * 0.85f;
	float distScale = 2.0f*1.5f;

	//Connect to Server
	if (m_Network.Initialize(0))
	{
		NCLDebug::Log("Network: Initialized!");

		//Attempt to connect to the server on localhost:1234
		m_pServerConnection = m_Network.ConnectPeer(127, 0, 0, 1, 1234);
		NCLDebug::Log("Network: Attempting to connect to server.");
	}

	//<--- SCENE CREATION --->

	Vector3 pos = Vector3();
	sun = BuildSphereObject(
		"",						// Optional: Name
		pos,					// Position
		15.0f,					// Half-Dimensions
		true,					// Physics Enabled?
		0.000001f,				// Physical Mass (must have physics enabled)
		true,					// Physically Collidable (has collision shape)
		false,					// Dragable by user?
		Vector4(1, 1, 1, 0.9),	// Render colour
		true,					// In space?
		1,						// Gravity Type; 0 Planer, 1 Relative, 2 None
		1);
	sun->Physics()->SetAngularVelocity(Vector3(0, -10.0f, 0));
	//sun->Physics()->
	this->AddGameObject(sun);

	planet1 = BuildSphereObject(
		"",								// Optional: Name
		Vector3(10.0f*distScale, 0, 0),	// Position
		1.0f,							// Half-Dimensions
		true,							// Physics Enabled?
		0.1f,							// Physical Mass (must have physics enabled)
		true,							// Physically Collidable (has collision shape)
		false,							// Dragable by user?
		Vector4(1, 1, 1, 1),			// Render colour
		true,							// In space?
		1,								// Gravity Type; 0 Planer, 1 Relative, 2 None
		2);

	planet1->Physics()->SetAngularVelocity(Vector3(0, 1.0f, 0));
	planet1->Physics()->SetLinearVelocity(Vector3(0, 0, 30.0f*scale));
	this->AddGameObject(planet1);

	planet2 = BuildSphereObject(
		"",								// Optional: Name
		Vector3(20.0f*distScale, 0, 0),	// Position
		1.5f,							// Half-Dimensions
		true,							// Physics Enabled?
		0.02f,							// Physical Mass (must have physics enabled)
		true,							// Physically Collidable (has collision shape)
		false,							// Dragable by user?
		Vector4(1, 1, 1, 1),			// Render colour
		true,							// In space?
		1,								// Gravity Type; 0 Planer, 1 Relative, 2 None
		3);

	planet2->Physics()->SetAngularVelocity(Vector3(0, 1.0f, 0));
	planet2->Physics()->SetLinearVelocity(Vector3(0, 0, 21.0f*scale));
	this->AddGameObject(planet2);

	planet3 = BuildSphereObject(
		"",								// Optional: Name
		Vector3(25.0f*distScale, 0, 0),	// Position
		1.75f,							// Half-Dimensions
		true,							// Physics Enabled?
		0.015f,							// Physical Mass (must have physics enabled)
		true,							// Physically Collidable (has collision shape)
		false,							// Dragable by user?
		Vector4(1, 1, 1, 1),			// Render colour
		true,							// In space?
		1,								// Gravity Type; 0 Planer, 1 Relative, 2 None
		4);

	planet3->Physics()->SetAngularVelocity(Vector3(0, 1.0f, 0));
	planet3->Physics()->SetLinearVelocity(Vector3(0, 0, 20.0f*scale));
	this->AddGameObject(planet3);

	planet4 = BuildSphereObject(
		"",								// Optional: Name
		Vector3(35.0f*distScale, 0, 0),	// Position
		1.5f,							// Half-Dimensions
		true,							// Physics Enabled?
		0.02f,							// Physical Mass (must have physics enabled)
		true,							// Physically Collidable (has collision shape)
		false,							// Dragable by user?
		Vector4(1, 1, 1, 1),			// Render colour
		true,							// In space?
		1,								// Gravity Type; 0 Planer, 1 Relative, 2 None
		5);

	planet4->Physics()->SetAngularVelocity(Vector3(0, 1.0f, 0));
	planet4->Physics()->SetLinearVelocity(Vector3(0, 0, 17.0f*scale));
	this->AddGameObject(planet4);

	//astaroid belt

	for (int i = 0; i < 100; i++) {

		float angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (2 * PI));

		Object* roid = BuildSphereObject(
			"",								// Optional: Name
			Vector3(40.0f * sin(angle) *distScale, (float(rand() / float(RAND_MAX)) * 4) - 2, 40.0f * cos(angle)*distScale),	// Position
			0.2f,							// Half-Dimensions
			true,							// Physics Enabled?
			0.999f,							// Physical Mass (must have physics enabled)
			true,							// Physically Collidable (has collision shape)
			false,							// Dragable by user?
			Vector4(1, 1, 1, 1),			// Render colour
			true,							// In space?
			1,								// Gravity Type; 0 Planer, 1 Relative, 2 None
			7);

		roid->Physics()->SetAngularVelocity(Vector3(0, 1.0f, 0));
		float velocity = 16.5f*scale;
		roid->Physics()->SetLinearVelocity(Vector3(velocity * sin(angle - (0.5*PI)), 0, velocity * cos(angle - (0.5*PI))));
		roid->Physics()->SetElasticity(0.0f);
		this->AddGameObject(roid);
	}

	planet5 = BuildSphereObject(
		"",								// Optional: Name
		Vector3(60.0f*distScale, 0, 0),	// Position
		4.0f,							// Half-Dimensions
		true,							// Physics Enabled?
		0.0001f,						// Physical Mass (must have physics enabled)
		true,							// Physically Collidable (has collision shape)
		false,							// Dragable by user?
		Vector4(1, 1, 1, 1),			// Render colour
		true,							// In space?
		1,								// Gravity Type; 0 Planer, 1 Relative, 2 None
		6);

	planet5->Physics()->SetAngularVelocity(Vector3(0, 1.0f, 0));
	planet5->Physics()->SetLinearVelocity(Vector3(0, 0, 14.0f*scale));
	this->AddGameObject(planet5);

	m_pObj = CommonUtils::BuildCuboidObject(
		"Server",
		Vector3(30.0f, 20.0f, 0.0f),
		Vector3(0.5f, 0.5f, 0.5f),
		true,									//Physics Enabled here Purely to make setting position easier via Physics()->SetPosition()
		1.0f,
		true,
		false,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),
		true,
		1);
	this->AddGameObject(m_pObj);

		constraint = new DistConTooClose(
			m_pObj->Physics(),					//Physics Object A
			sun->Physics(),						//Physics Object B
			m_pObj->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
			sun->Physics()->GetPosition());		//Attachment Position on Object B	-> Currently the centre  
		PhysicsEngine::Instance()->AddConstraint(constraint);
}

void solarScene::OnCleanupScene()
{
	//Just delete all created game objects 
	//  - this is the default command on any Scene instance so we don't really need to override this function here.
	Scene::OnCleanupScene();
}

void solarScene::OnUpdateScene(float dt)
{
	m_AccumTime += dt;

	auto callback = std::bind(
		&solarScene::ProcessNetworkEvent,	// Function to call
		this,								// Associated class instance
		std::placeholders::_1);				// Where to place the first parameter
	m_Network.ServiceNetwork(dt, callback);

	uint8_t ip1 = m_pServerConnection->address.host & 0xFF;
	uint8_t ip2 = (m_pServerConnection->address.host >> 8) & 0xFF;
	uint8_t ip3 = (m_pServerConnection->address.host >> 16) & 0xFF;
	uint8_t ip4 = (m_pServerConnection->address.host >> 24) & 0xFF;

	NCLDebug::DrawTextWs(m_pObj->Physics()->GetPosition() + Vector3(0.f, 0.5f, 0.f), 14.f, TEXTALIGN_CENTRE, Vector4(),
		"Peer: %u.%u.%u.%u:%u", ip1, ip2, ip3, ip4, m_pServerConnection->address.port);

	Vector4 status_color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	NCLDebug::AddStatusEntry(status_color, "Network Traffic");
	NCLDebug::AddStatusEntry(status_color, "    Incoming: %5.2fKbps", m_Network.m_IncomingKb);
	NCLDebug::AddStatusEntry(status_color, "    Outgoing: %5.2fKbps", m_Network.m_OutgoingKb);

	//NCLDebug::DrawPoint(Vector3(0, 1.0, 0), 2.f);


	// You can add status entries to the top left from anywhere in the program
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Welcome to the Game Tech Framework!");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 You can move the black car with the arrow keys");

	// You can print text using 'printf' formatting
	bool donkeys = false;
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 The %s in this scene are dragable", donkeys ? "donkeys" : "cubes");


	// Lets sun a little bit...
	Vector3 invLightDir = Matrix4::Rotation(15.f * dt, Vector3(0.0f, 1.0f, 0.0f)) * SceneManager::Instance()->GetInverseLightDirection();
	SceneManager::Instance()->SetInverseLightDirection(invLightDir);


	//Fire Object into scene
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
				true);				//In Space? 

			Matrix3 view = Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix());
			Vector3 forward = Vector3(-view._13, -view._23, -view._33);

			sphere->Physics()->SetLinearVelocity(forward*10.0f);
			this->AddGameObject(sphere);
		}
	}

	if (m_AccumTime >= update_rate)
	{

		//Packet data
		// - At the moment this is just a position update that rotates around the origin of the world
		//   though this can be any variable, structure or class you wish. Just remember that everything 
		//   you send takes up valuable network bandwidth so no sending every PhysicsObject struct each frame ;)
		m_AccumTime = 0.0f;


		//Create the packet and broadcast it (unreliable transport) to all clients

		if (currentTarget == 0) {
			destPos = planet1->Physics()->GetPosition();
		}
		if (currentTarget == 1) {
			destPos = planet2->Physics()->GetPosition();
		}
		if (currentTarget == 2) {
			destPos = planet3->Physics()->GetPosition();
		}
		if (currentTarget == 3) {
			destPos = planet4->Physics()->GetPosition();
		}
		if (currentTarget == 4) {
			destPos = planet5->Physics()->GetPosition();
		}

		if (!hasTarget) {
			int d = REQUEST;
			ENetPacket* update = enet_packet_create(&d, sizeof(int), 0);
			enet_peer_send(m_pServerConnection, 0, update);
		}
		else {
			inMotion = true;
			pos = m_pObj->Physics()->GetPosition();

			Matrix3 data;

			data._11 = pos.x;
			data._12 = pos.y;
			data._13 = pos.z;
			data._21 = destPos.x;
			data._22 = destPos.y;
			data._23 = destPos.z;

			ENetPacket* accelUpdate = enet_packet_create(&data, sizeof(Matrix3), 0);
			enet_peer_send(m_pServerConnection, 0, accelUpdate);

		}

		Vector3 ab = m_pObj->Physics()->GetPosition() - sun->Physics()->GetPosition();
		float m_Distance = ab.LengthSquared();

		//if (m_Distance < (18.0f * 18.0f)) {

		//}
		//else
		//{
		//	PhysicsEngine::Instance()->
		//}

		Vector3 cd = m_pObj->Physics()->GetPosition() - destPos;
		m_Distance = cd.LengthSquared();

		if (m_Distance < (5.0f * 5.0f)) {
			hasTarget = false;
		}
	}
}

void solarScene::ProcessNetworkEvent(const ENetEvent& evnt)
{
	switch (evnt.type)
	{
		//New connection request or an existing peer accepted our connection request
	case ENET_EVENT_TYPE_CONNECT:
	{
		if (evnt.peer == m_pServerConnection)
		{
			NCLDebug::Log("Network: Successfully connected to server!");

			//Send a 'hello' packet
			/*char* text_data = "Hellooo!";
			ENetPacket* packet = enet_packet_create(text_data, strlen(text_data) + 1, 0);
			enet_peer_send(m_pServerConnection, 0, packet);*/
		}
	}
	break;


	//Server has sent us a new packet
	case ENET_EVENT_TYPE_RECEIVE:
	{
		if (evnt.packet->dataLength == sizeof(int))
		{
			int temp;
			memcpy(&temp, evnt.packet->data, sizeof(int));
			currentTarget = temp;
			hasTarget = true;
		}
		else  if (evnt.packet->dataLength == sizeof(Vector3)) {
			Vector3 temp;
			memcpy(&temp, evnt.packet->data, sizeof(Vector3));
			m_pObj->Physics()->SetLinearVelocity(temp);
		}
		else
		{
			NCLERROR("Recieved Invalid Network Packet!");
		}

	}
	break;


	//Server has disconnected
	case ENET_EVENT_TYPE_DISCONNECT:
	{
		NCLDebug::Log("Network: Server has disconnected!");
	}
	break;
	}
}
#include <enet/enet.h>

#include <nclgl\Window.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\SceneManager.h>
#include <ncltech\NCLDebug.h>
#include <ncltech\PerfTimer.h>

#include "nuralScene.h"
#include "neuralNet.h"
#include "simpleNNScene.h"
#include "AStar.h"
#include "Testing.h"

#include "Testing.h"

const Vector4 status_colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 status_colour_header = Vector4(0.8f, 0.9f, 1.0f, 1.0f);

bool show_perf_metrics = false;
PerfTimer timer_total, timer_physics, timer_update, timer_render, timer_neuralNet;
uint shadowCycleKey = 4;

bool isTraining = false;

simpleNNScene* mainScene = new simpleNNScene("GameThesis #1 - NN Scene");
nuralScene* specialScene = new nuralScene("GameThesis #2 - NN Scene");
AStar*		pathFind = new AStar();


// Program De constructor
//  - Releases all global components and memory
//  - Optionally prints out an error message and
//    stalls the runtime if requested.
void Quit(bool error = false, const string &reason = "") {
	//Release Singletons
	SceneManager::Release();
	PhysicsEngine::Release();
	enet_deinitialize();  //!!!NEW!!!!!
	Window::Destroy();
	
	//Show console reason before exit
	if (error) {
		std::cout << reason << std::endl;
		system("PAUSE");
		exit(-1);
	}
}


// Program Initialise
//  - Generates all program wide components and enqueues all scenes
//    for the SceneManager to display
void Initialize()
{
	//Initialise the Window
	if (!Window::Initialise("Game Technologies", 1280, 800, false))
		Quit(true, "Window failed to initialise!");

	//Initialise ENET for networking  //!!!NEW!!!!!
	if (enet_initialize() != 0)
	{
		Quit(true, "ENET failed to initialize!");
	}

	//Initialise the PhysicsEngine
	PhysicsEngine::Instance();

	//Initialize Renderer
	SceneManager::Instance()->InitializeOGLContext(Window::GetWindow());
	if (!SceneManager::Instance()->HasInitialised())
		Quit(true, "Renderer failed to initialise!");

	//Randomize Spawn
	srand(time(NULL));
	int v1 = rand() % 9;
	specialScene->setSpawnPoint(v1);

	//Enqueue All Scenes
	SceneManager::Instance()->EnqueueScene(mainScene);
	SceneManager::Instance()->EnqueueScene(specialScene);


}

// Print Debug Info
//  - Prints a list of status entries to the top left
//    hand corner of the screen each frame.
void PrintStatusEntries()
{
	//Print Engine Options
	NCLDebug::AddStatusEntry(status_colour_header, "NCLTech Settings");
	NCLDebug::AddStatusEntry(status_colour, "     Physics Engine: %s (Press P to toggle)", PhysicsEngine::Instance()->IsPaused() ? "Paused  " : "Enabled ");
	NCLDebug::AddStatusEntry(status_colour, "     Monitor V-Sync: %s (Press V to toggle)", SceneManager::Instance()->GetVsyncEnabled() ? "Enabled " : "Disabled");
	NCLDebug::AddStatusEntry(status_colour, "");

	//Print Current Scene Name
	NCLDebug::AddStatusEntry(status_colour_header, "[%d/%d]: %s",
		SceneManager::Instance()->GetCurrentSceneIndex() + 1,
		SceneManager::Instance()->SceneCount(),
		SceneManager::Instance()->GetCurrentScene()->GetSceneName().c_str()
		);
	NCLDebug::AddStatusEntry(status_colour, "     \x01 Q/E to cycle or R to reload scene");

	//Print Performance Timers
	NCLDebug::AddStatusEntry(status_colour, "     FPS: %5.2f  (Press G for %s info)", 1000.f / timer_total.GetAvg(), show_perf_metrics ? "less" : "more");
	if (show_perf_metrics)
	{
		timer_total.PrintOutputToStatusEntry(status_colour, "       Total Time        :");
		timer_update.PrintOutputToStatusEntry(status_colour, "       Scene Update      :");
		timer_physics.PrintOutputToStatusEntry(status_colour, "       Physics Update    :");
		timer_render.PrintOutputToStatusEntry(status_colour, "       Render Scene      :");
		timer_neuralNet.PrintOutputToStatusEntry(status_colour, "       Neural Net Update:");
		
	}
	NCLDebug::AddStatusEntry(status_colour, "");
}


// Process Input
//  - Handles all program wide keyboard inputs for
//    things such toggling the physics engine and 
//    cycling through scenes.
void HandleKeyboardInputs()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
		PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F))
		PhysicsEngine::Instance()->SetAtRestFlag(!PhysicsEngine::Instance()->GetAtRestFlag());

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_V))
		SceneManager::Instance()->SetVsyncEnabled(!SceneManager::Instance()->GetVsyncEnabled());

	uint sceneIdx = SceneManager::Instance()->GetCurrentSceneIndex();
	uint sceneMax = SceneManager::Instance()->SceneCount();
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_E))
		SceneManager::Instance()->JumpToScene((sceneIdx + 1) % sceneMax);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Q))
		SceneManager::Instance()->JumpToScene((sceneIdx == 0 ? sceneMax : sceneIdx) - 1);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
		SceneManager::Instance()->JumpToScene(sceneIdx);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_G))
		show_perf_metrics = !show_perf_metrics;
}

// Program Entry Point
int main()
{


	//Thorough Testing Stuff
	Testing nNet;

	int cSubRes = 2;
	int maxSubRes = 100;
	int iCounter = 0;

	int fCount = 0;
	string inp1;
	string inp2;
	string inp5;
	bool tt = false;
	int inp3;
	int inp4;

	

	cout << "Please enter whether we are Training a NN or Loading a pre-created NN? T/Training or L/Load" << endl;
	cin >> inp1;
	if (inp1 == "T" || inp1 == "Training" || inp1 == "t") {
		isTraining = true;
		cout << "Please enter the secondary resolution, THIS NUMBER MUST BE A FACTOR OF 2!! \n This number will be squared!!" << endl;
		cin >> inp3;
		cout << "Please enter the number of training cycles you wish the program to complete, this is recommended to be a factor of 100" << endl;
		cin >> inp4;
		cout << "Thorough Testing? Continue increasing secondary res, or single test? T/Thorough or S/Single" << endl;
		cin >> inp5;
		if (inp5 == "T" || inp5 == "Thorough" || inp5 == "t") {
			tt = false;
		}
		else {
			tt = true;
		}

	}
	if (inp1._Equal("L") || inp1._Equal("Load")) {
		cout << "Please enter the file name of the NN to Load" << endl;
		cin >> inp2;
	}
	
	cSubRes = inp3;
	nNet.setSubRes(inp3);
	nNet.setTraining(true);
	nNet.Initialize();

	float ft = 0.f;
	float nt = 0.f;
	float ttt = 0.f;

	//Initialize our Window, Physics, Scenes etc
	Initialize();

	Window::GetWindow().GetTimer()->GetTimedMS();	


	//Create main game-loop
	while (Window::GetWindow().UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		//Start Timing
		float dt = Window::GetWindow().GetTimer()->GetTimedMS() * 0.001f;	//How many milliseconds since last update?
		timer_total.BeginTimingSection();

		//Print Status Entries
		PrintStatusEntries();

		//Handle Keyboard Inputs
		HandleKeyboardInputs();

		//Update Performance Timers (Show results every second)
		timer_total.UpdateRealElapsedTime(dt);
		timer_physics.UpdateRealElapsedTime(dt);
		timer_update.UpdateRealElapsedTime(dt);
		timer_render.UpdateRealElapsedTime(dt);
		timer_neuralNet.UpdateRealElapsedTime(dt);

		//Update Scene
		timer_update.BeginTimingSection();
		SceneManager::Instance()->UpdateScene(dt);
		timer_update.EndTimingSection();

		//Update Physics
		timer_physics.BeginTimingSection();
		PhysicsEngine::Instance()->Update(dt);
		timer_physics.EndTimingSection();

		//Render Scene
		timer_render.BeginTimingSection();
		SceneManager::Instance()->RenderScene();
		//cout << SceneManager::Instance()->GetCamera()->GetPosition();
		{
			//Forces synchronisation if vsync is disabled
			// - This is solely to allow accurate estimation of render time
			// - We should NEVER normally lock our render or game loop!		
			glClientWaitSync(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, NULL), GL_SYNC_FLUSH_COMMANDS_BIT, 1000000);
		}
		timer_render.EndTimingSection();
		
		//if (fCount > 20 && SceneManager::Instance()->GetCurrentScene() == specialScene) {
		//	cout << pathFind->calcDistance(specialScene->getPlayer()->Physics()->GetPosition(), Vector3(13, 1.1, -9.5)) << endl;
		//	fCount = 0;
		//}
		//fCount++;
		
		timer_neuralNet.BeginTimingSection();
		if (isTraining) {
			if (cSubRes < maxSubRes) {				
				if (iCounter < inp4) {
					nNet.Update(inp4, iCounter, ft, nt);
					iCounter++;
				}
				else if (iCounter >= inp4)
				{
					if (!tt) {
						cSubRes += 2;
						iCounter = 0;
						//nNet.~Testing();
						nNet.createdNN = false;
						nNet.setSubRes(cSubRes);
						nNet.setTraining(true);
						nNet.Initialize();
					}
					else {
						iCounter = 0;
						nNet.setTraining(false);
					}
				}				
			}
		}
		else {
			//nNet.updateDataToFile(0, ft, ttt);
		}
		timer_neuralNet.EndTimingSection();
		//Finish Timing
		timer_total.EndTimingSection();

		ft = 1000.f / timer_total.GetAvg();
		nt = timer_neuralNet.GetAvg();
		ttt = timer_total.GetAvg();

		//Let other programs on the computer have some CPU time
		Sleep(0);
	}

	//Cleanup
	Quit();
	return 0;
}
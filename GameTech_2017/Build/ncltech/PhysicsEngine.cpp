#include "PhysicsEngine.h"
#include "Object.h"
#include "CollisionDetectionSAT.h"
#include "NCLDebug.h"
#include <nclgl\Window.h>
#include <omp.h>
#include <algorithm>

using namespace std;

#define G = 0.1f;


void PhysicsEngine::SetDefaults()
{
	m_DebugDrawFlags = NULL;
	m_IsPaused = false;
	m_UpdateTimestep = 1.0f / 60.f;
	m_UpdateAccum = 0.0f;
	m_Gravity = Vector3(0.0f, -9.81f, 0.0f);
	m_DampingFactor = 0.999f;
	m_SpaceDampingFactor = 1.0f;
}

PhysicsEngine::PhysicsEngine()
{
	SetDefaults();
}

PhysicsEngine::~PhysicsEngine()
{
	RemoveAllPhysicsObjects();
}

void PhysicsEngine::AddPhysicsObject(PhysicsObject* obj)
{
	m_PhysicsObjects.push_back(obj);
}

void PhysicsEngine::RemovePhysicsObject(PhysicsObject* obj)
{
	//Lookup the object in question
	auto found_loc = std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), obj);

	//If found, remove it from the list
	if (found_loc != m_PhysicsObjects.end())
	{
		m_PhysicsObjects.erase(found_loc);
	}
}

void PhysicsEngine::RemoveAllPhysicsObjects()
{
	//Delete and remove all constraints/collision manifolds
	for (Constraint* c : m_vpConstraints)
	{
		delete c;
	}
	m_vpConstraints.clear();

	for (Manifold* m : m_vpManifolds)
	{
		delete m;
	}
	m_vpManifolds.clear();


	//Delete and remove all physics objects
	// - we also need to inform the (possible) associated game-object
	//   that the physics object no longer exists
	for (PhysicsObject* obj : m_PhysicsObjects)
	{
		if (obj->m_pParent != NULL) obj->m_pParent->m_pPhysicsObject = NULL;
		delete obj;
	}
	m_PhysicsObjects.clear();
}


void PhysicsEngine::Update(float deltaTime)
{
	const int max_updates_per_frame = 4;

	if (!m_IsPaused)
	{
		m_UpdateAccum += deltaTime;
		for (int i = 0; (m_UpdateAccum >= m_UpdateTimestep) && i < max_updates_per_frame; ++i)
		{
			m_UpdateAccum -= m_UpdateTimestep;
			if (!m_IsPaused) UpdatePhysics(); //Additional check here incase physics was paused mid-update and the contents of the physics need to be displayed
		}

		if (m_UpdateAccum >= m_UpdateTimestep)
		{
			NCLERROR("Physics too slow to run in real time!");
			//Drop Time in the hope that it can continue to run in real-time
			m_UpdateAccum = 0.0f;
		}
	}
}


void PhysicsEngine::UpdatePhysics()
{
	for (Manifold* m : m_vpManifolds)
	{
		delete m;
	}
	m_vpManifolds.clear();

	//Check for collisions
	BroadPhaseCollisions();
	NarrowPhaseCollisions();

	//Solve collision constraints
	SolveConstraints();

	//Update movement
	for (PhysicsObject* obj : m_PhysicsObjects)
	{
		UpdatePhysicsObject(obj);
	}
}


void PhysicsEngine::SolveConstraints()
{
	//Optional step to allow constraints to 
	// precompute values based off current velocities 
	// before they are updated in the main loop below.
	for (Manifold* m : m_vpManifolds)		m->PreSolverStep(m_UpdateTimestep);
	for (Constraint* c : m_vpConstraints)	c->PreSolverStep(m_UpdateTimestep);

	// Solve all Constraints and Collision Manifolds
	for (size_t i = 0; i < SOLVER_ITERATIONS; ++i)
	{
		for (Manifold * m : m_vpManifolds)
		{
			m->ApplyImpulse();
		}

		for (Constraint * c : m_vpConstraints)
		{
			c->ApplyImpulse();
		}
	}
}


void PhysicsEngine::UpdatePhysicsObject(PhysicsObject* obj)
{
	/* TUTORIAL 2 */

	
	if (obj->m_Gravity == 0) {
		if (obj->m_InvMass > 0.0f) {
			obj->m_LinearVelocity += m_Gravity * m_UpdateTimestep;
		}
	}
	else if (obj->m_Gravity == 1) {
		Vector3 gravF = Vector3();
		if (obj->m_InvMass > 0.f)
		{
			for each(PhysicsObject* obj2 in m_PhysicsObjects) {
				if (obj != obj2 && obj2->m_InvMass > 0.0f) {
					Vector3 dist = obj2->GetPosition() - obj->GetPosition();
					Vector3 distNorm = dist;
					distNorm.Normalise();
					gravF += distNorm * 0.1f / (obj->GetInverseMass() * obj2->GetInverseMass() * Vector3::Dot(dist, dist));
					obj->m_Force = gravF;
				}
			}
		}
	}

	//Semi-Implicit
	obj->m_LinearVelocity += obj->m_Force * obj->m_InvMass * m_UpdateTimestep;

	//Apply Velocity Damping
	if (obj->m_InSpace) {
		obj->m_LinearVelocity = obj->m_LinearVelocity * m_SpaceDampingFactor;
	}
	else {
		obj->m_LinearVelocity = obj->m_LinearVelocity * m_DampingFactor;
	}
	

	//Update Position
	obj->m_Position += obj->m_LinearVelocity * m_UpdateTimestep;

	//Angular Rotation
	obj->m_AngularVelocity += obj->m_InvInertia * obj->m_Torque * m_UpdateTimestep;
	
	//Apply Velocity Damping	
	if (obj->m_InSpace) {
		obj->m_AngularVelocity = obj->m_AngularVelocity * m_SpaceDampingFactor;
	}
	else {
		obj->m_AngularVelocity = obj->m_AngularVelocity * m_DampingFactor;
	}

	//Update Orientation
	obj->m_Orientation = obj->m_Orientation + obj->m_Orientation * (obj->m_AngularVelocity * m_UpdateTimestep * 0.5f);
	obj->m_Orientation.Normalise();

	obj->m_wsTransformInvalidated = true;
}

bool sortByYAxis(PhysicsObject* lhs, PhysicsObject* rhs) {
	Vector3 *m_MinY, *m_MaxY, *m_MinX, *m_MaxX, *m_MinZ, *m_MaxZ;
	m_MinY = &Vector3();
	m_MaxY = &Vector3();
	m_MinX = &Vector3();
	m_MaxX = &Vector3();
	m_MinZ = &Vector3();
	m_MaxZ = &Vector3();
	lhs->GetCollisionShape()->GetMinMaxVertexOnAxis(lhs, Vector3(0, 1, 0), m_MinY, m_MaxY);
	lhs->SetMinY(*m_MinY);
	lhs->SetMaxY(*m_MaxY);
	lhs->GetCollisionShape()->GetMinMaxVertexOnAxis(lhs, Vector3(1, 0, 0), m_MinX, m_MaxX);
	lhs->SetMinX(*m_MinX);
	lhs->SetMaxX(*m_MaxX);
	lhs->GetCollisionShape()->GetMinMaxVertexOnAxis(lhs, Vector3(0, 0, 1), m_MinZ, m_MaxZ);
	lhs->SetMinZ(*m_MinZ);
	lhs->SetMaxZ(*m_MaxZ);
	//rhs->GetCollisionShape()->GetMinMaxVertexOnAxis(rhs, Vector3(0, 1, 0), m_MinY, m_MaxY);
	//rhs->SetMinY(*m_MinY);
	//rhs->SetMaxY(*m_MaxY);
	rhs->GetCollisionShape()->GetMinMaxVertexOnAxis(rhs, Vector3(1, 0, 0), m_MinX, m_MaxX);
	rhs->SetMinX(*m_MinX);
	rhs->SetMaxX(*m_MaxX);
	/*rhs->GetCollisionShape()->GetMinMaxVertexOnAxis(rhs, Vector3(0, 0, 1), m_MinZ, m_MaxZ);
	rhs->SetMinZ(*m_MinZ);
	rhs->SetMaxZ(*m_MaxZ);*/

	return  lhs->GetMinX().x < rhs->GetMinX().x;
}

double approxRollingAverage(double avg, double new_sample) {

	avg -= avg / 15;
	avg += new_sample / 15;

	return avg;
}

void PhysicsEngine::BroadPhaseCollisions()
{
	m_BroadphaseCollisionPairs.clear();

	PhysicsObject *m_pObj1, *m_pObj2;
	//	The broadphase needs to build a list of all potentially colliding objects in the world,
	//	which then get accurately assesed in narrowphase. If this is too coarse then the system slows down with
	//	the complexity of narrowphase collision checking, if this is too fine then collisions may be missed.


	//	Brute force approach.
	//  - For every object A, assume it could collide with every other object.. 
	//    even if they are on the opposite sides of the world.
	if (m_PhysicsObjects.size() > 0)
	{
		sort(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), sortByYAxis);

		for each (PhysicsObject* p in m_PhysicsObjects) {
			if (p->GetGravityType() == 1) { break; }
			if (!p->IsRestStateEnabled()) { continue; }

			p->SetAngularVelocityAvg(approxRollingAverage(p->GetAngularVelocityAvg(), p->GetAngularVelocity().LengthSquared()));
			p->SetLinearVelocityAvg(approxRollingAverage(p->GetLinearVelocityAvg(),p->GetLinearVelocity().LengthSquared()));

			if (p->GetAngularVelocityAvg() < 0.001 && p->GetLinearVelocityAvg() < 0.001) {
				p->SetAtRest(true);
				p->SetAngularVelocity(Vector3());
				p->SetLinearVelocity(Vector3());
				p->SetGravityType(2);
				if (m_restStateFlag) {
					p->GetAssociatedObject()->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1.0f));
				}
			}
			else {
				p->SetAtRest(false);
				p->SetGravityType(0);
				if (m_restStateFlag) {
					p->GetAssociatedObject()->SetColour(Vector4(0.9f, 0.9f, 0.9f, 1.0f));
				}
			}
		}

		for (size_t i = 0; i < m_PhysicsObjects.size() - 1; ++i)
		{
			for (size_t j = i + 1; j < m_PhysicsObjects.size(); ++j)
			{
				m_pObj1 = m_PhysicsObjects[i];
				m_pObj2 = m_PhysicsObjects[j];

				if (m_pObj1 == m_pObj2) { continue; }

				if (m_pObj1->m_AtRest && m_pObj2->m_AtRest) { continue; }
				//Check they both atleast have collision shapes
				if (m_pObj1->GetCollisionShape() != NULL
					&& m_pObj2->GetCollisionShape() != NULL)
				{
					if (m_pObj1->GetMaxX().x < m_pObj2->GetMinX().x ) { break; }

					if (m_pObj1->GetMaxY().y > m_pObj2->GetMinY().y) {
						if (m_pObj1->GetMaxZ().z > m_pObj2->GetMinZ().z) {
							CollisionPair cp;
							cp.pObjectA = m_pObj1;
							cp.pObjectB = m_pObj2;
							m_BroadphaseCollisionPairs.push_back(cp);
						}
					}					
				}
			}
		}
	}
}


void PhysicsEngine::NarrowPhaseCollisions()
{
	if (m_BroadphaseCollisionPairs.size() > 0)
	{
		//Collision data to pass between detection and manifold generation stages.
		CollisionData colData;				

		//Collision Detection Algorithm to use
		CollisionDetectionSAT colDetect;	

		// Iterate over all possible collision pairs and perform accurate collision detection
		for (size_t i = 0; i < m_BroadphaseCollisionPairs.size(); ++i)
		{
			CollisionPair& cp = m_BroadphaseCollisionPairs[i];

			CollisionShape *shapeA = cp.pObjectA->GetCollisionShape();
			CollisionShape *shapeB = cp.pObjectB->GetCollisionShape();

			colDetect.BeginNewPair(
				cp.pObjectA,
				cp.pObjectB,
				cp.pObjectA->GetCollisionShape(),
				cp.pObjectB->GetCollisionShape());

			//--TUTORIAL 4 CODE--
			// Detects if the objects are colliding - Seperating Axis Theorem
			if (colDetect.AreColliding(&colData))
			{
				//Draw collision data to the window if requested
				// - Have to do this here as colData is only temporary. 
				if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_COLLISIONNORMALS)
				{
					NCLDebug::DrawPointNDT(colData._pointOnPlane, 0.1f, Vector4(0.5f, 0.5f, 1.0f, 1.0f));
					NCLDebug::DrawThickLineNDT(colData._pointOnPlane, colData._pointOnPlane - colData._normal * colData._penetration, 0.05f, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
				}

				//Check to see if any of the objects have collision callbacks that dont want the objects to physically collide
				bool okA = cp.pObjectA->FireOnCollisionEvent(cp.pObjectA, cp.pObjectB);
				bool okB = cp.pObjectB->FireOnCollisionEvent(cp.pObjectB, cp.pObjectA);

				if (okA && okB)
				{
					//-- TUTORIAL 5 CODE --
					// Build full collision manifold that will also handle the collision response between the two objects in the solver stage
					Manifold* manifold = new Manifold();
					manifold->Initiate(cp.pObjectA, cp.pObjectB);

					// Construct contact points that form the perimeter of the collision manifold
					colDetect.GenContactPoints(manifold);

					// Add to list of manifolds that need solving
					m_vpManifolds.push_back(manifold);
				}
			}
		}

	}
}


void PhysicsEngine::DebugRender()
{
	// Draw all collision manifolds
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_MANIFOLD)
	{
		for (Manifold* m : m_vpManifolds)
		{
			m->DebugDraw();
		}
	}

	// Draw all constraints
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_CONSTRAINT)
	{
		for (Constraint* c : m_vpConstraints)
		{
			c->DebugDraw();
		}
	}

	// Draw all associated collision shapes
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_COLLISIONVOLUMES)
	{
		for (PhysicsObject* obj : m_PhysicsObjects)
		{
			if (obj->GetCollisionShape() != NULL)
			{
				obj->GetCollisionShape()->DebugDraw(obj);
			}
		}
	}
}
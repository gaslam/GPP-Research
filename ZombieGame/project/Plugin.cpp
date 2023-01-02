#include "stdafx.h"
#include "Plugin.h"
#include "IExamInterface.h"
#include "EBlackboard.h"
#include "SteeringBehaviors.h"
#include "EBehaviorTree.h"
#include "Behaviors.h"
#include "CombinedSteeringBehaviors.h"
#include "InventoryManager.h"

using namespace std;

//Called only once, during initialization
void Plugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);

	//Bit information about the plugin
	//Please fill this in!!
	info.BotName = "MinionExam";
	info.Student_FirstName = "Gaspard";
	info.Student_LastName = "Lammertyn";
	info.Student_Class = "2DAE08";

	m_pInventoryManager = new InventoryManager();

	m_pBehaviors->pWander = new Wander{};
	m_pBehaviors->pSeek = new Seek{};
	m_pBehaviors->pArrive = new Arrive{};
	m_pBehaviors->pEvade = new Evade{};

	m_pBehaviors->pWanderAndSeek = new BlendedSteering{ {{m_pBehaviors->pWander,0.6f},{m_pBehaviors->pSeek,1.f}} };

	m_pBehaviors->pSelectedSteering = m_pBehaviors->pWanderAndSeek;

	m_pBlackboard = new Elite::Blackboard{};
	m_pBlackboard->AddData("Interface", m_pInterface);
	m_pBlackboard->AddData("Behaviors", m_pBehaviors);
	m_pBlackboard->AddData("SelectedBehavior", m_pBehaviors->pSelectedSteering);
	m_pBlackboard->AddData("AgentFleeTarget", Elite::Vector2());
	m_pBlackboard->AddData("Target", m_Target);

	BehaviorSequence* pMoveToTarget{
		new BehaviorSequence({
			new BehaviorAction(BT_Actions::ChangeToSeekAndWander),
		new BehaviorConditional(BT_Conditions::IsTargetInRadius),
		new BehaviorAction(BT_Actions::ChangeToArrive)
	}) };

	BehaviorSequence* pEvadeEnemy{ new BehaviorSequence({
			new BehaviorConditional(BT_Conditions::IsEnemyInFov),
			new BehaviorAction(BT_Actions::ChangeToEvade)
		}) };

	m_pBehaviors->pDecisionMaking = new Elite::BehaviorTree{ m_pBlackboard,
		new BehaviorSelector(
			{
				pEvadeEnemy,
				pMoveToTarget
			}
		)
	};
}


Plugin::~Plugin()
{
	SAFE_DELETE(m_pBlackboard);
	SAFE_DELETE(m_pBehaviors->pWanderAndSeek);
	SAFE_DELETE(m_pBehaviors->pWander);
	SAFE_DELETE(m_pBehaviors->pSeek);
	SAFE_DELETE(m_pBehaviors->pArrive);
	//SAFE_DELETE(m_pInterface);
	//SAFE_DELETE(m_pSteeringBehavior);
	//SAFE_DELETE(m_pDecisionMaking);
}
//Called only once
void Plugin::DllInit()
{
	//Called when the plugin is loaded
}

//Called only once
void Plugin::DllShutdown()
{
	//Called wheb the plugin gets unloaded
}

//Called only once, during initialization
void Plugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
	params.SpawnEnemies = true; //Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 20; //How many enemies? (Default = 20)
	params.GodMode = false; //GodMode > You can't die, can be useful to inspect certain behaviors (Default = false)
	params.LevelFile = "GameLevel.gppl";
	params.AutoGrabClosestItem = true; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
	params.StartingDifficultyStage = 1;
	params.InfiniteStamina = false;
	params.SpawnDebugPistol = true;
	params.SpawnDebugShotgun = true;
	params.SpawnPurgeZonesOnMiddleClick = true;
	params.PrintDebugMessages = true;
	params.ShowDebugItemNames = true;
	params.Seed = 36;
}

//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void Plugin::Update(float dt)
{
	//Demo Event Code
	//In the end your AI should be able to walk around without external input
	if (m_pInterface->Input_IsMouseButtonUp(Elite::InputMouseButton::eLeft))
	{
		//Update target based on input
		Elite::MouseData mouseData = m_pInterface->Input_GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		const Elite::Vector2 pos = Elite::Vector2(static_cast<float>(mouseData.X), static_cast<float>(mouseData.Y));
		m_Target = m_pInterface->Debug_ConvertScreenToWorld(pos);
		m_pBlackboard->ChangeData("Target", m_Target);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Space))
	{
		m_CanRun = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Left))
	{
		m_AngSpeed -= Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Right))
	{
		m_AngSpeed += Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyUp(Elite::eScancode_Space))
	{
		m_CanRun = false;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Delete))
	{
		m_pInterface->RequestShutdown();
	}

	m_pInventoryManager->Update(m_pInterface);
}

//Update
//This function calculates the new SteeringOutput, called once per frame
SteeringPlugin_Output Plugin::UpdateSteering(float dt)
{

	//Use the Interface (IAssignmentInterface) to 'interface' with the AI_Framework
	auto agentInfo = m_pInterface->Agent_GetInfo();
	m_pBehaviors->pDecisionMaking->Update(dt);

	auto nextTargetPos = m_pInterface->NavMesh_GetClosestPathPoint(m_Target);
	ISteeringBehavior* pBehavior;
	if (!m_pBlackboard->GetData("SelectedBehavior", pBehavior))
	{
		return{};
	}
	auto steering = pBehavior->CalculateSteering(dt, agentInfo);

	//Use the navmesh to calculate the next navmesh point
	//auto nextTargetPos = m_pInterface->NavMesh_GetClosestPathPoint(checkpointLocation);

	//OR, Use the mouse target
	//Uncomment this to use mouse position as guidance

	auto vHousesInFOV = GetHousesInFOV();//uses m_pInterface->Fov_GetHouseByIndex(...)
	auto vEntitiesInFOV = GetEntitiesInFOV(); //uses m_pInterface->Fov_GetEntityByIndex(...)

	for (auto& e : vEntitiesInFOV)
	{
		if (e.Type == eEntityType::PURGEZONE)
		{
			PurgeZoneInfo zoneInfo;
			m_pInterface->PurgeZone_GetInfo(e, zoneInfo);
			//std::cout << "Purge Zone in FOV:" << e.Location.x << ", "<< e.Location.y << "---Radius: "<< zoneInfo.Radius << std::endl;
		}
	}

	//INVENTORY USAGE DEMO
	//********************
	const UINT inventorySlot{ m_pInventoryManager->GetInventorySlot() };
	if (m_pInventoryManager->CanGrabItem())
	{
		ItemInfo item;
		//Item_Grab > When DebugParams.AutoGrabClosestItem is TRUE, the Item_Grab function returns the closest item in range
		//Keep in mind that DebugParams are only used for debugging purposes, by default this flag is FALSE
		//Otherwise, use GetEntitiesInFOV() to retrieve a vector of all entities in the FOV (EntityInfo)
		//Item_Grab gives you the ItemInfo back, based on the passed EntityHash (retrieved by GetEntitiesInFOV)
		GetEntitiesInFOV();
		if (m_pInterface->Item_Grab({}, item))
		{
			//Once grabbed, you can add it to a specific inventory slot
			//Slot must be empty
			m_pInterface->Inventory_AddItem(inventorySlot, item);
		}
	}

	if (m_pInventoryManager->CanUseItem())
	{
		//Use an item (make sure there is an item at the given inventory slot)
		m_pInterface->Inventory_UseItem(inventorySlot);
	}

	if (m_pInventoryManager->CanRemoveItem())
	{
		//Remove an item from a inventory slot
		m_pInterface->Inventory_RemoveItem(inventorySlot);
	}

	//Simple Seek Behaviour (towards Target)
	//steering.LinearVelocity = nextTargetPos - agentInfo.Position; //Desired Velocity
	//steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	//steering.LinearVelocity *= agentInfo.MaxLinearSpeed; //Rescale to Max Speed

	//if (Distance(nextTargetPos, agentInfo.Position) < 2.f)
	//{
	//	steering.LinearVelocity = Elite::ZeroVector2;
	//}

	//steering.AngularVelocity = m_AngSpeed; //Rotate your character to inspect the world while walking
	steering.AutoOrient = true; //Setting AutoOrient to TRue overrides the AngularVelocity

	steering.RunMode = m_CanRun; //If RunMode is True > MaxLinSpd is increased for a limited time (till your stamina runs out)

	//SteeringPlugin_Output is works the exact same way a SteeringBehaviour output
	m_pInventoryManager->SetGrabItem(false);
	m_pInventoryManager->SetRemoveItem(false);
	m_pInventoryManager->SetUseItem(false);

	return steering;
}

//This function should only be used for rendering debug elements
void Plugin::Render(float dt) const
{
	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Target, .7f, { 0,0 }, { 1, 0, 0 });
}

vector<HouseInfo> Plugin::GetHousesInFOV() const
{
	vector<HouseInfo> vHousesInFOV = {};

	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetHouseByIndex(i, hi))
		{
			vHousesInFOV.push_back(hi);
			continue;
		}

		break;
	}

	return vHousesInFOV;
}

vector<EntityInfo> Plugin::GetEntitiesInFOV() const
{
	vector<EntityInfo> vEntitiesInFOV = {};
	vector<EnemyInfo> vEnemiesInFOV = {};

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetEntityByIndex(i, ei))
		{
			vEntitiesInFOV.push_back(ei);
			EnemyInfo info = {};
			continue;
		}

		break;
	}
	return vEntitiesInFOV;
}

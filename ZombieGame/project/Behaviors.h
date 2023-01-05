/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// Behaviors.h: Implementation of certain reusable behaviors for the BT version of the Agario Game
/*=============================================================================*/
#ifndef ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
#define ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "../inc/EliteMath/EMath.h"
#include "EBehaviorTree.h"
#include <IExamInterface.h>
#include <Exam_HelperStructs.h>
#include "SteeringBehaviors.h"
#include "CombinedSteeringBehaviors.h"
#include "InventoryManager.h"

using namespace Elite;

//-----------------------------------------------------------------
// Behaviors
//-----------------------------------------------------------------

namespace BT_Actions
{
	inline BehaviorState ChangeToSeekAndWander(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{nullptr};
		ISteeringBehavior* pSteering{ nullptr };
		SteeringBehaviors* pBehaviors{nullptr};
		Vector2 target{};

		if (!pBlackboard->GetData("SelectedBehavior", pSteering) || pSteering == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Behaviors", pBehaviors) || pBehaviors == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Target", target))
		{
			return BehaviorState::Failure;
		}

		Vector2 nextTarget{ pInterface->NavMesh_GetClosestPathPoint(target) };
		pBehaviors->pSeek->SetTarget(nextTarget);

		pSteering = pBehaviors->pWanderAndSeek;

		if (!pBlackboard->ChangeData("SelectedBehavior", pSteering))
		{
			return BehaviorState::Failure;
		}

		return BehaviorState::Success;
	}

	inline BehaviorState ChangeToArrive(Blackboard* pBlackboard)
	{
		ISteeringBehavior* pSteering{ nullptr };
		SteeringBehaviors* pBehaviors{ nullptr };
		IExamInterface* pInterface{};
		Vector2 target;
		if (!pBlackboard->GetData("SelectedBehavior", pSteering) || pSteering == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Behaviors", pBehaviors) || pBehaviors == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Target", target))
		{
			return BehaviorState::Failure;
		}

		auto agentInfo = pInterface->Agent_GetInfo();

		pBehaviors->pArrive->SetSlowRadius(agentInfo.GrabRange);
		pBehaviors->pArrive->SetTargetRadius(agentInfo.GrabRange / 4.f);
		pBehaviors->pArrive->SetTarget(target);

		pSteering = pBehaviors->pArrive;

		if (!pBlackboard->ChangeData("SelectedBehavior", pSteering))
		{
			return BehaviorState::Failure;
		}

		return BehaviorState::Success;
	}

	//Needs to be changed

	inline BehaviorState ChangeToEvade(Blackboard* pBlackboard)
	{
		ISteeringBehavior* pSteering{ nullptr };
		SteeringBehaviors* pBehaviors{ nullptr };
		Vector2 target;

		if (!pBlackboard->GetData("SelectedBehavior", pSteering) || pSteering == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Behaviors", pBehaviors) || pBehaviors == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("AgentFleeTarget", target))
		{
			return BehaviorState::Failure;
		}

		pBehaviors->pEvade->SetTarget(target);
		pBehaviors->pEvade->SetRadius(30.f);
		pSteering = pBehaviors->pEvade;

		if (!pBlackboard->ChangeData("SelectedBehavior", pSteering))
		{
			return BehaviorState::Failure;
		}

		return BehaviorState::Success;
	}

	//Shoot = Face the enemy to allow the player to shoot
	inline BehaviorState ChangeToEvadeAndShoot(Blackboard* pBlackboard)
	{
		ISteeringBehavior* pSteering{ nullptr };
		SteeringBehaviors* pBehaviors{ nullptr };
		Vector2 target;

		if (!pBlackboard->GetData("SelectedBehavior", pSteering) || pSteering == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Behaviors", pBehaviors) || pBehaviors == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("AgentFleeTarget", target))
		{
			return BehaviorState::Failure;
		}

		if (pBehaviors->pEvade == nullptr || pBehaviors->pFace == nullptr || pBehaviors->pEvadeAndFace == nullptr)
		{
			return BehaviorState::Failure;
		}

		pBehaviors->pEvade->SetTarget(target);
		pBehaviors->pEvade->SetRadius(7.5f);
		pBehaviors->pFace->SetTarget(target);
		pSteering = pBehaviors->pEvadeAndFace;

		if (!pBlackboard->ChangeData("SelectedBehavior", pSteering))
		{
			return BehaviorState::Failure;
		}

		return BehaviorState::Success;
	}

	inline BehaviorState ChangeToFace(Blackboard* pBlackboard)
	{
		ISteeringBehavior* pSteering{ nullptr };
		SteeringBehaviors* pBehaviors{ nullptr };
		Vector2 target;

		if (!pBlackboard->GetData("SelectedBehavior", pSteering) || pSteering == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Behaviors", pBehaviors) || pBehaviors == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("AgentFleeTarget", target))
		{
			return BehaviorState::Failure;
		}

		if (pBehaviors->pEvade == nullptr || pBehaviors->pFace == nullptr || pBehaviors->pEvadeAndFace == nullptr)
		{
			return BehaviorState::Failure;
		}

		pBehaviors->pFace->SetTarget(target);
		pSteering = pBehaviors->pFace;

		if (!pBlackboard->ChangeData("SelectedBehavior", pSteering))
		{
			return BehaviorState::Failure;
		}

		return BehaviorState::Success;
	}

	inline BehaviorState ChangeToFlee(Blackboard* pBlackboard)
	{
		ISteeringBehavior* pSteering{ nullptr };
		SteeringBehaviors* pBehaviors{ nullptr };
		Vector2 target;

		if (!pBlackboard->GetData("SelectedBehavior", pSteering) || pSteering == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Behaviors", pBehaviors) || pBehaviors == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("AgentFleeTarget", target))
		{
			return BehaviorState::Failure;
		}

		if (pBehaviors->pFlee == nullptr)
		{
			return BehaviorState::Failure;
		}

		pBehaviors->pFlee->SetTarget(target);
		pSteering = pBehaviors->pFlee;

		if (!pBlackboard->ChangeData("SelectedBehavior", pSteering))
		{
			return BehaviorState::Failure;
		}

		return BehaviorState::Success;
	}
}

namespace BT_Conditions
{

	inline bool IsTargetInRadius(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{};
		Vector2 target{};

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("Target", target))
		{
			return false;
		}

		auto agentInfo = pInterface->Agent_GetInfo();
		
		float totalRadAgentGrabPlusTarget{ (agentInfo.GrabRange * 2.f) };
		totalRadAgentGrabPlusTarget *= totalRadAgentGrabPlusTarget;

		if ((target - agentInfo.Position).MagnitudeSquared() <= totalRadAgentGrabPlusTarget)
		{
			return true;
		}
		return false;
	}

	inline bool DoesPlayerHaveUsableWeapon(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{};
		InventoryManager* pManager{};
		Vector2 target{};

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("InventoryManager", pManager) || pManager == nullptr)
		{
			return false;
		}

		if (!pManager->PlayerUsesWeapon())
		{
			return false;
		}

		ItemInfo info = {};

		auto test = pManager->ReturnPlayerSelectedItemInfo(pInterface);

		if (info.Type != eItemType::SHOTGUN && info.Type != eItemType::PISTOL)
		{
			return false;
		}



		const int ammo = pInterface->Weapon_GetAmmo(test);

		if (ammo <= 0)
		{
			return false;
		}

		return true;

	}


	bool enemyAttacked{ false };
	inline bool IsEnemyInFov(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo> enemiesInFov{};
		IExamInterface* pInterface{};

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}


		EntityInfo ei = {};
		for (int i = 0;; ++i)
		{
			if (pInterface->Fov_GetEntityByIndex(i, ei))
			{
				if (ei.Type == eEntityType::ENEMY)
				{
					enemiesInFov.push_back(ei);
					enemyAttacked = false;
				}
				continue;
			}
			break;
		}

		if (enemiesInFov.size() == 0)
		{
			return false;
		}

		EntityInfo closestEnemy{ enemiesInFov[0] };
		auto agentInfo = pInterface->Agent_GetInfo();
		float closestDistance{ agentInfo.Position.DistanceSquared(closestEnemy.Location) };
		for (auto& enemy : enemiesInFov)
		{
			if (agentInfo.Position.DistanceSquared(enemy.Location) < closestDistance)
			{
				closestEnemy = enemy;
				closestDistance = agentInfo.Position.DistanceSquared(closestEnemy.Location);
			}
		}

		if (!pBlackboard->ChangeData("AgentFleeTarget", closestEnemy.Location))
		{
			return false;
		}
		return true;
	}

	inline bool IsPlayerBitten(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		InventoryManager* pManager{ nullptr };
		SteeringBehaviors* pBehaviors{ nullptr };
		Vector2 target{};

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("AgentFleeTarget",target))
		{
			return false;
		}

		auto agentInfo = pInterface->Agent_GetInfo();
		target = agentInfo.Position;

		if (agentInfo.Bitten && !enemyAttacked)
		{
			enemyAttacked = true;
			target = agentInfo.Position + ( - agentInfo.LinearVelocity);

			if (!pBlackboard->ChangeData("AgentFleeTarget", target))
			{
				return false;
			}
		}
		if (!enemyAttacked)
		{
			return false;
		}

		if (!pBlackboard->GetData("Behaviors", pBehaviors) || pBehaviors == nullptr)
		{
			return false;
		}

		return true;
	}

	inline bool IsPlayerInPurgeZone(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringBehaviors* pBehaviors{ nullptr };

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("Behaviors", pBehaviors) || pBehaviors == nullptr)
		{
			return false;
		}

		EntityInfo ei = {};
		for (int i = 0;; ++i)
		{
			if (pInterface->Fov_GetEntityByIndex(i, ei))
			{
				if (ei.Type == eEntityType::PURGEZONE)
				{
					PurgeZoneInfo zoneInfo{};
					pInterface->PurgeZone_GetInfo(ei, zoneInfo);

					pBehaviors->pFlee->SetRadius(zoneInfo.Radius);
					if (!pBlackboard->ChangeData("AgentFleeTarget", zoneInfo.Center))
					{
						return false;
					}
					return true;
				}
				continue;
			}

			break;
		}
		return false;
	}
}










#endif
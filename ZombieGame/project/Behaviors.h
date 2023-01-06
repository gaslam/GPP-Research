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
		IExamInterface* pInterface{ nullptr };
		ISteeringBehavior* pSteering{ nullptr };
		SteeringBehaviors* pBehaviors{ nullptr };
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

	inline BehaviorState ChangeToArriveAndFace(Blackboard* pBlackboard)
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

		pBehaviors->pFace->SetTarget(target);

		pSteering = pBehaviors->pArriveAndFace;

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

	inline BehaviorState GrabItem(Blackboard* pBlackboard)
	{
		InventoryManager* pInventory;
		IExamInterface* pInterface;
		EntityInfo entity;

		if (!pBlackboard->GetData("InventoryManager", pInventory) || pInventory == nullptr)
		{
			return BehaviorState::Failure;
		}


		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("EntityToPick", entity))
		{
			return BehaviorState::Failure;
		}

		if (!pInventory->CanGrabItem())
		{
			return BehaviorState::Failure;
		}

		ItemInfo item{};
		if (!pInventory->GetItemInfo(pInterface, entity, item))
		{
			return BehaviorState::Failure;
		}

		if (!pInventory->GrabItem(pInterface, entity, item))
		{
			return BehaviorState::Failure;
		}

		return BehaviorState::Success;
	}

	inline BehaviorState UseMedkitIfPossible(Blackboard* pBlackboard)
	{
		InventoryManager* pInventory;
		IExamInterface* pInterface;

		if (!pBlackboard->GetData("InventoryManager", pInventory) || pInventory == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return BehaviorState::Failure;
		}

		ItemInfo item{};
		int slot{};

		if (!pInventory->GetItem(pInterface, eItemType::MEDKIT, item, slot))
		{
			return BehaviorState::Failure;
		}

		if (!pInventory->UseItem(pInterface, slot))
		{
			return BehaviorState::Failure;
		}

		pInventory->RemoveItem(pInterface, slot);

		return BehaviorState::Success;
	}

	inline BehaviorState UseFoodIfPossible(Blackboard* pBlackboard)
	{
		InventoryManager* pInventory;
		IExamInterface* pInterface;

		if (!pBlackboard->GetData("InventoryManager", pInventory) || pInventory == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return BehaviorState::Failure;
		}

		ItemInfo item{};
		int slot{};

		if (!pInventory->GetItem(pInterface, eItemType::FOOD, item, slot))
		{
			return BehaviorState::Failure;
		}

		if (!pInventory->UseItem(pInterface, slot))
		{
			return BehaviorState::Failure;
		}

		pInventory->RemoveItem(pInterface, slot);

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

		//if (!pManager->PlayerUsesWeapon())
		//{
		//	return false;
		//}

		ItemInfo info = {};

		if (info.Type != eItemType::SHOTGUN && info.Type != eItemType::PISTOL)
		{
			return false;
		}



		//const int ammo = pInterface->Weapon_GetAmmo(test);

		//if (ammo <= 0)
		//{
		//	return false;
		//}

		return false;

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

		if (!pBlackboard->GetData("AgentFleeTarget", target))
		{
			return false;
		}

		auto agentInfo = pInterface->Agent_GetInfo();
		target = agentInfo.Position;

		if (agentInfo.Bitten && !enemyAttacked)
		{
			enemyAttacked = true;
			target = agentInfo.Position + (-agentInfo.LinearVelocity);

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

	inline bool IsUsableItemInFov(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringBehaviors* pBehaviors{ nullptr };
		InventoryManager* pInventory{ nullptr };
		Vector2 target{};

		if (!pBlackboard->GetData("InventoryManager", pInventory) || pInventory == nullptr)
		{
			return false;
		}

		if (pInventory->IsFull())
		{
			return false;
		}

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("Behaviors", pBehaviors) || pBehaviors == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("Target", target))
		{
			return false;
		}

		EntityInfo ei = {};
		float closestDistance{ FLT_MAX };
		bool hasItem{ false };

		auto agentInfo = pInterface->Agent_GetInfo();

		for (int i = 0;; ++i)
		{
			if (pInterface->Fov_GetEntityByIndex(i, ei))
			{
				if (ei.Type == eEntityType::ITEM)
				{
					ItemInfo item;
					if (pInterface->Item_GetInfo(ei, item) && item.Type != eItemType::GARBAGE)
					{
						hasItem = true;
						const float newDistance{ agentInfo.Position.DistanceSquared(item.Location) };
						if (newDistance < closestDistance)
						{
							target = item.Location;
							closestDistance = newDistance;

							if (!pBlackboard->ChangeData("EntityToPick", ei))
							{
								return false;
							}
						}
					}
				}
				continue;
			}
			break;
		}

		if (!hasItem)
		{
			return false;
		}

		if (!pBlackboard->ChangeData("Target", target))
		{
			return false;
		}

		return true;
	}

	inline bool CanPlayerGrabItem(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		InventoryManager* pInventory{ nullptr };

		EntityInfo entityToPick{};

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}


		if (!pBlackboard->GetData("EntityToPick", entityToPick))
		{
			return false;
		}

		if (!pBlackboard->GetData("InventoryManager", pInventory) || pInventory == nullptr)
		{
			return false;
		}

		auto agentInfo = pInterface->Agent_GetInfo();

		if ((agentInfo.Position - entityToPick.Location).MagnitudeSquared() <= agentInfo.GrabRange * agentInfo.GrabRange)
		{
			pInventory->SetGrabItem(true);
			return true;
		}
		pInventory->SetGrabItem(false);
		return false;
	}

	inline bool IsPlayerHealthLow(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		InventoryManager* pInventory{ nullptr };
		const int maxHealth{ 10 };

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("InventoryManager", pInventory) || pInventory == nullptr)
		{
			return false;
		}

		if (pInventory->IsEmpty())
		{
			return false;
		}

		ItemInfo item{};
		int slot{};

		if (!pInventory->GetItem(pInterface, eItemType::MEDKIT, item, slot))
		{
			return false;
		}

		auto health = pInterface->Medkit_GetHealth(item);

		auto agentInfo = pInterface->Agent_GetInfo();

		if (agentInfo.Health + health <= maxHealth)
		{
			return true;
		}

		return false;
	}

	inline bool IsPlayerEnergyLow(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		InventoryManager* pInventory{ nullptr };
		const int maxEnergy{ 10 };

		if (!pBlackboard->GetData("Interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("InventoryManager", pInventory) || pInventory == nullptr)
		{
			return false;
		}

		if (pInventory->IsEmpty())
		{
			return false;
		}

		ItemInfo item{};
		int slot{};

		if (!pInventory->GetItem(pInterface, eItemType::FOOD, item, slot))
		{
			return false;
		}

		int energy = pInterface->Food_GetEnergy(item);

		auto agentInfo = pInterface->Agent_GetInfo();

		if (agentInfo.Energy + energy <= maxEnergy)
		{
			return true;
		}

		return false;
	}
}










#endif
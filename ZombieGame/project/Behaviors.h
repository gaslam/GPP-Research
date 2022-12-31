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

using namespace Elite;

//-----------------------------------------------------------------
// Behaviors
//-----------------------------------------------------------------

namespace BT_Actions
{
	inline BehaviorState ChangeToSeekAndWander(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{};
		ISteeringBehavior* pSteering{ nullptr };
		BlendedSteering* pWanderAndSeek{nullptr};
		Seek* pSeek{ nullptr };
		Vector2 target{};

		if (!pBlackboard->GetData("SteeringBehavior", pSteering) || pSteering == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("WanderAndSeek", pWanderAndSeek) || pWanderAndSeek == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Seek", pSeek) || pSeek == nullptr)
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

		Vector2 nextTarget{ pInterface->NavMesh_GetClosestPathPoint(target) };
		pSeek->SetTarget(nextTarget);

		pSteering = pWanderAndSeek;

		if (!pBlackboard->ChangeData("SteeringBehavior", pSteering))
		{
			return BehaviorState::Failure;
		}

		return BehaviorState::Success;
	}

	inline BehaviorState ChangeToArrive(Blackboard* pBlackboard)
	{
		ISteeringBehavior* pSteering{ nullptr };
		Arrive* pArrive{ nullptr };
		IExamInterface* pInterface{};
		Vector2 target;
		if (!pBlackboard->GetData("SteeringBehavior", pSteering) || pSteering == nullptr)
		{
			return BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("Arrive", pArrive) || pArrive == nullptr)
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

		pArrive->SetSlowRadius(agentInfo.GrabRange * 2.f);
		pArrive->SetTargetRadius(agentInfo.GrabRange);
		pArrive->SetTarget(target);

		pSteering = pArrive;

		if (!pBlackboard->ChangeData("SteeringBehavior", pSteering))
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
}










#endif
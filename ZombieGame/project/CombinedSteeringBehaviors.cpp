#include "stdafx.h"
#include "CombinedSteeringBehaviors.h"
#include <algorithm>
#include "../inc/Exam_HelperStructs.h"

BlendedSteering::BlendedSteering(std::vector<WeightedBehavior> weightedBehaviors)
	:m_WeightedBehaviors(weightedBehaviors)
{
};

//****************
//BLENDED STEERING
SteeringPlugin_Output BlendedSteering::CalculateSteering(float deltaT, AgentInfo& pAgent)
{
	SteeringPlugin_Output blendedSteering = {};
	auto totalWeight = 0.f;

	for (auto weightedBehavior : m_WeightedBehaviors)
	{
		auto steering = weightedBehavior.pBehavior->CalculateSteering(deltaT, pAgent);
		blendedSteering.LinearVelocity += weightedBehavior.weight * steering.LinearVelocity;
		blendedSteering.AngularVelocity += weightedBehavior.weight * steering.AngularVelocity;

		totalWeight += weightedBehavior.weight;
	}

	if (totalWeight > 0.f)
	{
		auto scale = 1.f / totalWeight;
		blendedSteering *= scale;
	}

	//if (pAgent->CanRenderBehavior())
	//	DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), blendedSteering.LinearVelocity, 7, { 0, 1, 1 }, 0.40f);

	return blendedSteering;
}

//*****************
//PRIORITY STEERING
SteeringPlugin_Output PrioritySteering::CalculateSteering(float deltaT, AgentInfo& pAgent)
{
	SteeringPlugin_Output steering = {};

	for (auto pBehavior : m_PriorityBehaviors)
	{
		steering = pBehavior->CalculateSteering(deltaT, pAgent);

		if (steering.IsValid)
			break;
	}

	//If non of the behavior return a valid output, last behavior is returned
	return steering;
}
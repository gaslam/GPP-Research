//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"

//SEEK
//****
SteeringPlugin_Output_Extended Seek::CalculateSteering(float deltaT, AgentInfo& agent)
{
	SteeringPlugin_Output_Extended steering = {};
	const auto agentPos{ agent.Position };
	steering.LinearVelocity = m_Target.Position - agentPos;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent.MaxLinearSpeed;

	//if (agent->CanRenderBehavior())
	//	DEBUGRENDERER2D->DrawDirection(agentPos, steering.LinearVelocity, 5, Elite::Color{ 0,1,0 });

	//DEBUGRENDERER2D->DrawSegment(agentPos, m_Target.Position, { 0,1,0 });

	return steering;
}

SteeringPlugin_Output_Extended Flee::CalculateSteering(float deltaT, AgentInfo& agent)
{

	SteeringPlugin_Output_Extended steering = {};

	const Elite::Vector2 agentPos{ agent.Position };

	//if (agent->CanRenderBehavior())
	//{
	//	DEBUGRENDERER2D->DrawDirection(agentPos, steering.LinearVelocity, 5, Elite::Color{ 0,1,0 });
	//	DEBUGRENDERER2D->DrawCircle(agentPos, m_Radius, { 1,0,0 }, 0.f);
	//}

	FleeFromTarget(agent, steering, m_Target.Position);

	return steering;
}

void Flee::FleeFromTarget(AgentInfo& agent, SteeringPlugin_Output_Extended& output, const Elite::Vector2 targetPos) const
{
	const Elite::Vector2 direction{ agent.Position - targetPos };
	const float distanceSquared{ direction.MagnitudeSquared() };
	if (distanceSquared > m_Radius * m_Radius)
	{
		output.IsValid = false;
	}

	else if (direction.MagnitudeSquared() < m_Radius * m_Radius)
	{
		output.LinearVelocity = direction;
		output.LinearVelocity.Normalize();
		output.LinearVelocity *= agent.MaxLinearSpeed;
	}
}

SteeringPlugin_Output_Extended Arrive::CalculateSteering(float deltaT, AgentInfo& agent)
{
	SteeringPlugin_Output_Extended steering{  };

	const Elite::Vector2 agentPos{ agent.Position };

	//if (agent->CanRenderBehavior())
	//{
	//	DEBUGRENDERER2D->DrawCircle(agentPos, m_Radius, { 0,1,0 }, 0.f);
	//	DEBUGRENDERER2D->DrawCircle(agentPos, m_SlowRadius, { 1,0,0 }, 0.f);
	//}

	const Elite::Vector2 direction{ m_Target.Position - agentPos };
	const float distance{ direction.MagnitudeSquared() };

	if (distance < m_Radius * m_Radius)
	{
		steering.LinearVelocity = direction;
		steering.LinearVelocity.Normalize();
		steering.LinearVelocity *= m_SlowSpeed;
		return steering;
	}

	if (distance < m_SlowRadius * m_SlowRadius)
	{
		steering.LinearVelocity.Normalize();
		steering.LinearVelocity *= m_SlowSpeed;
		return steering;
	}

	steering.LinearVelocity = direction;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent.MaxLinearSpeed;

	return steering;
}

SteeringPlugin_Output_Extended Face::CalculateSteering(float deltaT, AgentInfo& agent)
{
	SteeringPlugin_Output_Extended steering{};
	const float rotation{ agent.Orientation};
	const Elite::Vector2 agentPos{ agent.Position };
	const Elite::Vector2 directionAgent{ cosf(rotation), sinf(rotation) };
	const Elite::Vector2 directionTarget{ agentPos - m_Target.Position };


	steering.AutoOrient = false;
	steering.AngularVelocity = Elite::AngleBetween(directionTarget, directionAgent);
	steering.IsValid = true;

	return steering;
}

SteeringPlugin_Output_Extended Wander::CalculateSteering(float deltaT, AgentInfo& agent)
{
	SteeringPlugin_Output_Extended steering{  };
	const float rotation{ agent.Orientation };
	const Elite::Vector2 agentPos{ agent.Position };
	Elite::Vector2 circleCenter{ agentPos + Elite::Vector2(cosf(rotation) * m_OffsetDistance, sinf(rotation) * m_OffsetDistance) };

	//Intended purpose:
	//Generate wander angle
	//add random float the wander angle
	//add random offset
	//calculate position
	//m_WanderAngle =  rotation - (m_MaxAngleChange / 2);
	m_WanderAngle += float(rand()) / float((RAND_MAX)) * m_MaxAngleChange - (m_MaxAngleChange / 2.f);

	Elite::Vector2 newRotation{ cosf(m_WanderAngle) * m_Radius, sinf(m_WanderAngle) * m_Radius };
	Elite::Vector2 pointOnCircle{ circleCenter + newRotation };
	steering.LinearVelocity = pointOnCircle - agentPos;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent.MaxLinearSpeed;

	return steering;
}


SteeringPlugin_Output_Extended Pursuit::CalculateSteering(float deltaT, AgentInfo& agent)
{
	SteeringPlugin_Output_Extended steering{};
	const Elite::Vector2 targetPos{ m_Target.Position };
	const Elite::Vector2 agentPos{ agent.Position };
	const float distance{ (targetPos - agentPos).MagnitudeSquared() };
	const float speed{ (m_Target.GetDirection() * agent.MaxLinearSpeed).MagnitudeSquared() };
	const float time{ distance / speed };

	Elite::Vector2 predictedPos{ (time * m_Target.LinearVelocity) + m_Target.Position };

	steering.LinearVelocity = predictedPos - agentPos;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent.MaxLinearSpeed;

	return steering;
}


//Set point in the middle to avoid target from moving in between the agent
SteeringPlugin_Output_Extended Evade::CalculateSteering(float deltaT, AgentInfo& agent)
{
	SteeringPlugin_Output_Extended steering{};
	const Elite::Vector2 targetPos{ m_Target.Position };
	const Elite::Vector2 agentPos{ agent.Position };
	const float distance{ (targetPos - agentPos).MagnitudeSquared() };
	const float speed{ (m_Target.GetDirection() * agent.MaxLinearSpeed).MagnitudeSquared() };
	const float time{ distance / speed };

	Elite::Vector2 predictedPos{ ((time * m_Target.LinearVelocity) + m_Target.Position) };

	Elite::Vector2 middlePos{ m_Target.Position + (predictedPos - m_Target.Position) / 2 };
	Elite::Vector2 middleToAgent{ agentPos - middlePos };

	//if (agent->CanRenderBehavior())
	//{
	//	DEBUGRENDERER2D->DrawCircle(predictedPos, 0.5f, { 1,1,0 }, 0.f);
	//	DEBUGRENDERER2D->DrawCircle(middlePos, m_Radius, { 1,1,0 }, 0.f);
	//}

	if (middleToAgent.MagnitudeSquared() <= ((m_Radius * m_Radius)) )
	{

		steering.LinearVelocity = middleToAgent;
		steering.LinearVelocity.Normalize();
		steering.LinearVelocity *= agent.MaxAngularSpeed;
		steering.IsValid = true;
	}
	else
	{
		steering.IsValid = false;
	}
	return steering;
}


SteeringPlugin_Output_Extended Turn::CalculateSteering(float deltaT, AgentInfo& agent)
{
	SteeringPlugin_Output_Extended steering{};
	steering.AutoOrient = false;

	steering.AngularVelocity = agent.MaxAngularSpeed * 3.f;

	steering.LinearVelocity = agent.LinearVelocity;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent.MaxLinearSpeed;
	return steering;
}

/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "SteeringHelpers.h"
struct AgentInfo;
struct SteeringPlugin_Output;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& pAgent) = 0;

	//Seek Functions
	void SetTarget(const TargetData& target) { m_Target = target; }

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }

protected:
	TargetData m_Target;
};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& pAgent) override;
};

class Flee : public ISteeringBehavior
{
public:
	explicit Flee() = default;
	virtual ~Flee() = default;

	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& pAgent) override;
private:
	float m_Radius = 20.f;

protected:
	void FleeFromTarget(AgentInfo& pAgent, SteeringPlugin_Output& output, const Elite::Vector2 targetPos) const;
};

class Arrive : public ISteeringBehavior
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;
	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& pAgent) override;
	void SetSlowRadius(float radius) { m_SlowRadius = radius; }
	void SetTargetRadius(float radius) { m_Radius = radius; }

private:
	float m_Radius{8.f};
	float m_SlowRadius{15.f};
	float m_CurrentSpeed{};
	float m_SlowSpeed{5.f};

	bool m_IsCurrentSpeedSet{false};
};

class Face : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() = default;
	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& pAgent) override;
private:
};

class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behavior
	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& pAgent) override;

	void SetWanderOffset(float offset) { m_OffsetDistance = offset; }
	void SetWanderRadius(float radius) { m_Radius = radius; }
	void SetMaxAngleChange(float rad) { m_MaxAngleChange = rad; }

protected:
	float m_OffsetDistance{ 6.f };
	float m_Radius{ 4.f };
	float m_MaxAngleChange{ Elite::ToRadians(45.f) };
	float m_WanderAngle{ 0.f };
};

class Pursuit : public ISteeringBehavior
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;
	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& pAgent) override;

};

class Evade : public ISteeringBehavior
{
public:
	Evade() = default;
	virtual ~Evade() = default;
	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& pAgent) override;
	void SetRadius(float radius) { m_Radius = radius; }
protected:
	float m_Radius{ 15.f };
};

#endif



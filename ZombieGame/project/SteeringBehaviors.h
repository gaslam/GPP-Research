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
struct SteeringPlugin_Output_Extended;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringPlugin_Output_Extended CalculateSteering(float deltaT, AgentInfo& agent) = 0;

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
	SteeringPlugin_Output_Extended CalculateSteering(float deltaT, AgentInfo& agent) override;
};

class Flee : public ISteeringBehavior
{
public:
	explicit Flee() = default;
	virtual ~Flee() = default;

	SteeringPlugin_Output_Extended CalculateSteering(float deltaT, AgentInfo& agent) override;
	void SetRadius(float radius) { m_Radius = radius; };
private:
	float m_Radius = 20.f;

protected:
	void FleeFromTarget(AgentInfo& agent, SteeringPlugin_Output_Extended& output, const Elite::Vector2 targetPos) const;
};

class Arrive : public ISteeringBehavior
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;
	SteeringPlugin_Output_Extended CalculateSteering(float deltaT, AgentInfo& agent) override;
	void SetSlowRadius(float radius) { m_SlowRadius = radius; }
	void SetTargetRadius(float radius) { m_Radius = radius; }

private:
	float m_Radius{8.f};
	float m_SlowRadius{15.f};
	float m_SlowSpeed{5.f};
};

class Face : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() = default;
	SteeringPlugin_Output_Extended CalculateSteering(float deltaT, AgentInfo& agent) override;
private:
};

class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behavior
	SteeringPlugin_Output_Extended CalculateSteering(float deltaT, AgentInfo& agent) override;

	void SetWanderOffset(float offset) { m_OffsetDistance = offset; }
	void SetWanderRadius(float radius) { m_Radius = radius; }
	void SetMaxAngleChange(float rad) { m_MaxAngleChange = rad; }

protected:
	float m_OffsetDistance{ 4.5f };
	float m_Radius{ 4.f };
	float m_MaxAngleChange{ Elite::ToRadians(45.f) };
	float m_WanderAngle{ 0.f };
};

class Pursuit : public ISteeringBehavior
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;
	SteeringPlugin_Output_Extended CalculateSteering(float deltaT, AgentInfo& agent) override;

};

class Evade : public ISteeringBehavior
{
public:
	Evade() = default;
	virtual ~Evade() = default;
	SteeringPlugin_Output_Extended CalculateSteering(float deltaT, AgentInfo& agent) override;
	void SetRadius(float radius) { m_Radius = radius; }
protected:
	float m_Radius{ 15.f };
};

class Turn : public ISteeringBehavior
{
public:
	Turn() = default;
	virtual ~Turn() = default;

	SteeringPlugin_Output_Extended CalculateSteering(float deltaT, AgentInfo& agent);
};

#endif



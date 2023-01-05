#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include "EBlackboard.h"
#include "Behaviors.h"

class IBaseInterface;
class IExamInterface;
class ISteeringBehavior;
class InventoryManager;
class Grid;
class Plugin :public IExamPlugin
{
public:
	Plugin() {};
	virtual ~Plugin();

	void Initialize(IBaseInterface* pInterface, PluginInfo& info) override;
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void Update(float dt) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

private:
	//Interface, used to request data from/perform actions with the AI Framework

	Elite::Blackboard* m_pBlackboard = nullptr;
	IExamInterface* m_pInterface = nullptr;

	SteeringBehaviors* m_pBehaviors{new SteeringBehaviors()};
	InventoryManager* m_pInventoryManager{nullptr};
	Grid* m_pGrid{};

	Elite::Vector2 m_Target = {};
	bool m_CanRun = false; //Demo purpose
	float m_AngSpeed = 0.f; //Demo purpose

};

//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register()
	{
		return new Plugin();
	}
}
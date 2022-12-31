#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include "EBlackboard.h"
#include "Behaviors.h"

class IBaseInterface;
class IExamInterface;
class ISteeringBehavior;
class BlendedSteering;

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
	BlendedSteering* m_pWanderAndSeek = nullptr;
	Wander* m_pWander = nullptr;
	Seek* m_pSeek = nullptr;
	Arrive* m_pArrive = nullptr;
	Elite::IDecisionMaking* m_pDecisionMaking{ nullptr };

	ISteeringBehavior* m_pSteeringBehavior{};
	std::vector<HouseInfo> GetHousesInFOV() const;
	std::vector<EntityInfo> GetEntitiesInFOV() const;

	Elite::Vector2 m_Target = {};
	bool m_CanRun = false; //Demo purpose
	bool m_GrabItem = false; //Demo purpose
	bool m_UseItem = false; //Demo purpose
	bool m_RemoveItem = false; //Demo purpose
	float m_AngSpeed = 0.f; //Demo purpose

	UINT m_InventorySlot = 0;
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
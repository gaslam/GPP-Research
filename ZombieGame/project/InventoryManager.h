#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"

class IExamInterface;
class InventoryManager
{
public:
	InventoryManager() = default;
	bool GrabItem(IExamInterface* pInterface, EntityInfo& entity, ItemInfo& item);
	bool UseItem(IExamInterface* pInterface,int slot);
	bool RemoveItem(IExamInterface* pInterface,int slot);
	void SetGrabItem(const bool canGrabItem) { m_GrabItem = canGrabItem; };
	bool CanGrabItem() const { return m_GrabItem; };
	bool IsFull() const { return m_IsFull; };

	int GetItemAmount(eItemType type, IExamInterface* pInterface) const;

	bool GetItem(IExamInterface* pInterface, eItemType type, ItemInfo& item, int& slot);

	static bool GetItemInfo(IExamInterface* pInterface, EntityInfo& entity, ItemInfo& item);
private:
	bool m_IsFull = false;
	bool m_GrabItem = false;
	const static int m_TotalSlots = 5;
	int m_AmountsPerItem[m_TotalSlots]{};
	int m_AmountOfItems{};
};


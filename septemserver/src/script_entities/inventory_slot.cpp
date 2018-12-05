#include "stdafx.h"
#include "script_entities/inventory_slot.h"

inventory_slot::inventory_slot()
{
}
inventory_slot::inventory_slot(InventorySlot slot, unsigned int maxItems, ItemSize maxItemSize)
    : m_slot(slot)
    , m_maxItems(maxItems)
    , m_maxSize(maxItemSize)
{
}

InventorySlot inventory_slot::get_inventorySlot()
{
    return m_slot;
}

unsigned int inventory_slot::get_maxItems()
{
    return m_maxItems;
}

ItemSize inventory_slot::get_maxItemSize()
{
    return m_maxSize;
}

void inventory_slot::set_maxItems(unsigned int i)
{
    m_maxItems = i;
}

void inventory_slot::set_itemCount(unsigned int i)
{
    m_itemCount = i;
}

unsigned int inventory_slot::get_itemCount()
{
    return m_itemCount;
}

void inventory_slot::incrementItemCount()
{
    m_itemCount++;
}

#ifndef INVENTORY_SLOT_H_
#define INVENTORY_SLOT_H_

#include "script_entities/itemobj.h"
enum InventorySlot {
        NONE                        = 0,
        BODY                        = 1<<0,
        BACK                        = 1<<1,
        WAIST                       = 1<<2,
        LIKE_HEAD_ARMOR             = 1<<3,
        OVER_SHOULDER               = 1<<4,
        LIKE_PANTS                  = 1<<5,
        LIKE_SHIRT                  = 1<<6,
        ON_WRIST                    = 1<<7,
        ON_FINGER                   = 1<<8,
        ON_FEET                     = 1<<9,
        NECK                        = 1<<10,
        BELT                        = 1<<11,
        LIKE_ARM_ARMOR              = 1<<12,
        LIKE_LEG_ARMOR              = 1<<13,
        ON_AN_EAR                   = 1<<14,
        ON_BOTH_EARS                = 1<<15,
        ANKLE                       = 1<<16,
        HANDS                       = 1<<17,
        TAIL                        = 1<<18,
        THIGH                       = 1<<19,
        UPPER_ARM                   = 1<<20,
        NOSE                        = 1<<21,
        OVER_LEFT_EYE               = 1<<22,
        OVER_RIGHT_EYE              = 1<<23,
        ON_HAIR                     = 1<<24,
        IN_HAIR                     = 1<<25,
        HEAD                        = 1<<26
    };
    enum class InventorySlotError { SLOT_OK, NO_SLOT, ITEM_COUNT_EXCEEDED, ITEM_SIZE_EXCEEDED };


struct inventory_slot
{
    
    inventory_slot()
    ;

    inventory_slot( InventorySlot slot, unsigned int maxItems,  ItemSize maxItemSize );
    
    InventorySlot get_inventorySlot()
    ;
    
    unsigned int get_maxItems()
    ;
    
    ItemSize get_maxItemSize()
    ;
    
    void set_maxItems( unsigned int i )
    ;
    
    void set_itemCount( unsigned int i )
    ;
    
    unsigned int get_itemCount()
    ;
    
    void incrementItemCount()
    ;
    
private:
    InventorySlot m_slot;
    unsigned int m_maxItems = 1; // default is 1 item
    ItemSize m_maxSize = ItemSize::COLOSSAL; // biggest item that can fit
    unsigned int m_itemCount = 0;
};

#endif
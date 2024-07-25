#ifndef __EQUIPMENT_FACTORY_H__
#define __EQUIPMENT_FACTORY_H__

#include "cocos2d.h"
class BaseEquipment;
class EquipmentFactory
{
public:
    enum equipmentType {
        CHIPPED_SWORD,
        SHORT_SWORD,
        EXCALIBUR
    };
    static BaseEquipment *createEquipment(std::string, int);
};

#endif // __EQUIPMENT_FACTORY_H__
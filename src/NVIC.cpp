#include "stm32f10x_tim.h"
#include "NVIC.h"

void Nvic::SetPriorityGrouping(PriorityGroup priorityGroup)
{
    NVIC_PriorityGroupConfig(static_cast<uint32_t>(priorityGroup));
}
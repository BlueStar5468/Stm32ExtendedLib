/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#include "stm32f1xx_ll_tim.h"
#include "NVIC.h"

void Nvic::SetPriorityGrouping(Nvic::Config::PriorityGroup priorityGroup)
{
    NVIC_SetPriorityGrouping(static_cast<uint32_t>(priorityGroup));
}

Nvic::NVICInitTypeDef Nvic::GetDefaultConfig()
{
    NVICInitTypeDef defaultConfig;
    defaultConfig.NVIC_IRQChannel = WWDG_IRQn;
    defaultConfig.NVIC_IRQChannelPreemptionPriority = 0;
    defaultConfig.NVIC_IRQChannelSubPriority = 0;
    defaultConfig.NVIC_IRQChannelCmd = DISABLE;
    return defaultConfig;
}

void Nvic::ChannelControl(NVICInitTypeDef* config)
{
    //配置指定通道的优先级
    NVIC_SetPriority(
        config->NVIC_IRQChannel,
        NVIC_EncodePriority(
            NVIC_GetPriorityGrouping(),
            config->NVIC_IRQChannelPreemptionPriority,
            config->NVIC_IRQChannelSubPriority)
    );
    //启动或关闭指定的中断通道
    if (config->NVIC_IRQChannelCmd)
    {
        NVIC_EnableIRQ(config->NVIC_IRQChannel);
    }
    else
    {
        NVIC_DisableIRQ(config->NVIC_IRQChannel);
    }
}

void Nvic::ChannelControl(IRQn_Type channel, uint8_t preemptionPriority, uint8_t subPriority, bool isEnabled)
{
    //配置指定通道的优先级
    NVIC_SetPriority(
        channel,
        NVIC_EncodePriority(
            NVIC_GetPriorityGrouping(),
            preemptionPriority,
            subPriority)
    );
    //启动或关闭指定的中断通道
    if (isEnabled)
    {
        NVIC_EnableIRQ(channel);
    }
    else
    {
        NVIC_DisableIRQ(channel);
    }
}

//NVIC对象实例化
Nvic nvic;

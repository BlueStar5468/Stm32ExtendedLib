/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#include "stm32f10x_tim.h"
#include "NVIC.h"

void Nvic::SetPriorityGrouping(Nvic::Config::PriorityGroup priorityGroup)
{
    NVIC_PriorityGroupConfig(static_cast<uint32_t>(priorityGroup));
}

Nvic::NvicConfig Nvic::GetDefaltConfig()
{
    NVIC_InitTypeDef defaultConfig;
    defaultConfig.NVIC_IRQChannel = 0;
    defaultConfig.NVIC_IRQChannelPreemptionPriority = 0;
    defaultConfig.NVIC_IRQChannelSubPriority = 0;
    defaultConfig.NVIC_IRQChannelCmd = DISABLE;
    return defaultConfig;
}

void Nvic::ChannelControl(NvicConfig* config)
{
    NVIC_Init(config);
}

void Nvic::ChannelControl(uint8_t channel, uint8_t preemptionPriority, uint8_t subPriority, bool isEnabled)
{
    NVIC_InitTypeDef config;
    config.NVIC_IRQChannel = channel;
    config.NVIC_IRQChannelPreemptionPriority = preemptionPriority;
    config.NVIC_IRQChannelSubPriority = subPriority;
    config.NVIC_IRQChannelCmd = isEnabled ? ENABLE : DISABLE;
    NVIC_Init(&config);
}

void Nvic::SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{
    NVIC_SetVectorTable(NVIC_VectTab, Offset);
}

void Nvic::SetPowerMode(Nvic::Config::PowerMode mode)
{
    if (mode == Nvic::Config::Sleep)
    {
        //进入睡眠模式
        NVIC_SystemLPConfig(NVIC_LP_SEVONPEND, ENABLE);
    }
    else if (mode == Nvic::Config::Stop)
    {
        //进入停止模式
        NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP, ENABLE);
    }
    else if (mode == Nvic::Config::Standby)
    {
        //进入待机模式
        NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT, ENABLE);
    }
    else
    {
        //退出睡眠模式
        NVIC_SystemLPConfig(NVIC_LP_SEVONPEND, DISABLE);
    }
}

//NVIC对象实例化
Nvic nvic;

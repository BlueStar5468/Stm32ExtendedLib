/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#include "stm32f10x_exti.h"
#include "EXTI.h"

EXTI_InitTypeDef Exti::GetDefaultInitConfig()
{
    EXTI_InitTypeDef defaultConfig;
    defaultConfig.EXTI_Line = 0;
    defaultConfig.EXTI_Mode = EXTI_Mode_Interrupt;
    defaultConfig.EXTI_Trigger = EXTI_Trigger_Rising;
    defaultConfig.EXTI_LineCmd = DISABLE;
    return defaultConfig;
}

void Exti::DeInit()
{
    EXTI_DeInit();
}

void Exti::Init(Config::EXTILine line, Config::Mode mode, Config::Trigger trigger, bool isEnabled)
{
    EXTI_InitTypeDef config;
    config.EXTI_Line = static_cast<uint32_t>(line);
    config.EXTI_Mode = static_cast<EXTIMode_TypeDef>(mode);
    config.EXTI_Trigger = static_cast<EXTITrigger_TypeDef>(trigger);
    config.EXTI_LineCmd = isEnabled ? ENABLE : DISABLE;
    EXTI_Init(&config);
}

void Exti::Init(InitConfig* config)
{
    EXTI_Init(config);
}

void Exti::GenerateIT(Config::EXTILine line)
{
    EXTI_GenerateSWInterrupt(static_cast<uint32_t>(line));
}

bool Exti::GetITStatus(Config::EXTILine line)
{
    return EXTI_GetITStatus(static_cast<uint32_t>(line)) == SET;
}

bool Exti::GetFlagStatus(Config::EXTILine line)
{
    return EXTI_GetFlagStatus(static_cast<uint32_t>(line)) == SET;
}

void Exti::ClearITStatus(Config::EXTILine line)
{
    EXTI_ClearITPendingBit(static_cast<uint32_t>(line));
}

void Exti::ClearFlagStatus(Config::EXTILine line)
{
    EXTI_ClearFlag(static_cast<uint32_t>(line));
}

//全局EXTI对象
Exti exti(EXTI);


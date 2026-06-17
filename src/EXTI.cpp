/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#include "stm32f1xx_ll_exti.h"
#include "EXTI.h"

LL_EXTI_InitTypeDef Exti::GetDefaultInitConfig()
{
    LL_EXTI_InitTypeDef defaultConfig;
    defaultConfig.Line_0_31 = LL_EXTI_LINE_0;
    defaultConfig.Mode = LL_EXTI_MODE_IT;
    defaultConfig.Trigger = LL_EXTI_TRIGGER_RISING;
    defaultConfig.LineCommand = DISABLE;
    return defaultConfig;
}

void Exti::DeInit()
{
    LL_EXTI_DeInit();
}

void Exti::Init(Config::EXTILine line, Config::Mode mode, Config::Trigger trigger, bool isEnabled)
{
    LL_EXTI_InitTypeDef config;
    config.Line_0_31 = line;
    config.Mode = mode;
    config.Trigger = trigger;
    config.LineCommand = isEnabled ? ENABLE : DISABLE;
    LL_EXTI_Init(&config);
}

void Exti::Init(InitConfig* config)
{
    LL_EXTI_Init(config);
}

void Exti::GenerateIT(Config::EXTILine line)
{
    LL_EXTI_GenerateSWI_0_31(line);
}

bool Exti::GetFlagStatus(Config::EXTILine line)
{
    return LL_EXTI_IsActiveFlag_0_31(line) == SET;
}

void Exti::ClearFlagStatus(Config::EXTILine line)
{
    LL_EXTI_ClearFlag_0_31(line);
}

//全局EXTI对象
Exti exti(EXTI);


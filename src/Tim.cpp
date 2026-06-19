/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_bus.h"
#include "Nvic.h"
#include "Tim.h"

void Tim::InterruptSwitch(bool isEnabled, Configs::ITMode it)
{
    if (isEnabled)
    {
        switch (it)
        {
        case Configs::ITMode::Update:
            LL_TIM_EnableIT_UPDATE(self);
            break;
        case Configs::ITMode::CC1:
            LL_TIM_EnableIT_CC1(self);
            break;
        case Configs::ITMode::CC2:
            LL_TIM_EnableIT_CC2(self);
            break;
        case Configs::ITMode::CC3:
            LL_TIM_EnableIT_CC3(self);
            break;
        case Configs::ITMode::CC4:
            LL_TIM_EnableIT_CC4(self);
            break;
        case Configs::ITMode::COM:
            LL_TIM_EnableIT_COM(self);
            break;
        case Configs::ITMode::Trigger:
            LL_TIM_EnableIT_TRIG(self);
            break;
        case Configs::ITMode::Break:
            LL_TIM_EnableIT_BRK(self);
            break;
        default:
            return;
        }
    }
    else
    {
        switch (it)
        {
        case Configs::ITMode::Update:
            LL_TIM_DisableIT_UPDATE(self);
            break;
        case Configs::ITMode::CC1:
            LL_TIM_DisableIT_CC1(self);
            break;
        case Configs::ITMode::CC2:
            LL_TIM_DisableIT_CC2(self);
            break;
        case Configs::ITMode::CC3:
            LL_TIM_DisableIT_CC3(self);
            break;
        case Configs::ITMode::CC4:
            LL_TIM_DisableIT_CC4(self);
            break;
        case Configs::ITMode::COM:
            LL_TIM_DisableIT_COM(self);
            break;
        case Configs::ITMode::Trigger:
            LL_TIM_DisableIT_TRIG(self);
            break;
        case Configs::ITMode::Break:
            LL_TIM_DisableIT_BRK(self);
            break;
        default:
            return;
        }
    }
}

void Tim::Switch(State state)
{
    if (state == ENABLE)
        LL_TIM_EnableCounter(self);
    else
        LL_TIM_DisableCounter(self);
}

Tim::TimeBaseInitConfig Tim::GetDefaultTimeBaseInitConfig()
{
    LL_TIM_InitTypeDef defaultConfig;
    LL_TIM_StructInit(&defaultConfig);
    return defaultConfig;
}

void Tim::DeInit()
{
    LL_TIM_DeInit(self);
}

void Tim::TimeBaseInit(TimeBaseInitConfig* config)
{
    LL_TIM_Init(self, config);
}

void Tim::SetClockMode(Configs::TIMClockMode mode)
{
    if (mode == Configs::TIMClockMode::InternalClock)
    {
        LL_TIM_SetClockSource(self, LL_TIM_CLOCKSOURCE_INTERNAL);
    }
    else if (mode == Configs::TIMClockMode::ExternalClockMode1)
    {
        LL_TIM_SetClockSource(self, LL_TIM_CLOCKSOURCE_EXT_MODE1);
    }
    else if (mode == Configs::TIMClockMode::ExternalClockMode2)
    {
        LL_TIM_SetClockSource(self, LL_TIM_CLOCKSOURCE_EXT_MODE2);
    }
}

void Tim::SetPrescaler(uint16_t prescaler, Configs::PSCReloadMode reloadMode)
{
    LL_TIM_SetPrescaler(self, prescaler);
    if (reloadMode == Configs::PSCReloadMode::Immediate)
    {
        LL_TIM_GenerateEvent_UPDATE(self); //立即更新预分频器的值
    }
}

void Tim::SetCounterMode(Configs::CounterMode mode)
{
    LL_TIM_SetCounterMode(self, mode);
}

void Tim::ARReloadConfig(State state)
{
    if (state == ENABLE)
    {
        LL_TIM_EnableARRPreload(self);
    }
    else
    {
        LL_TIM_DisableARRPreload(self);
    }
}

void Tim::SetCounter(uint16_t counter)
{
    LL_TIM_SetCounter(self, counter);
}

void Tim::SetAutoReload(uint16_t autoReload)
{
    LL_TIM_SetAutoReload(self, autoReload);
}

uint16_t Tim::GetCounter()
{
    return LL_TIM_GetCounter(self);
}

uint16_t Tim::GetPrescaler()
{
    return LL_TIM_GetPrescaler(self);
}

uint16_t Tim::GetAutoReload()
{
    return LL_TIM_GetAutoReload(self);
}

bool Tim::GetFlagStatus(Configs::Flags flag)
{
    bool ret;
    switch (flag)
    {
    case Configs::Flags::UpdateFlag:
        ret = LL_TIM_IsActiveFlag_UPDATE(self);
        break;
    case Configs::Flags::CC1Flag:
        ret = LL_TIM_IsActiveFlag_CC1(self);
        break;
    case Configs::Flags::CC2Flag:
        ret = LL_TIM_IsActiveFlag_CC2(self);
        break;
    case Configs::Flags::CC3Flag:
        ret = LL_TIM_IsActiveFlag_CC3(self);
        break;
    case Configs::Flags::CC4Flag:
        ret = LL_TIM_IsActiveFlag_CC4(self);
        break;
    case Configs::Flags::COMFlag:
        ret = LL_TIM_IsActiveFlag_COM(self);
        break;
    case Configs::Flags::TriggerFlag:
        ret = LL_TIM_IsActiveFlag_TRIG(self);
        break;
    case Configs::Flags::BreakFlag:
        ret = LL_TIM_IsActiveFlag_BRK(self);
        break;
    default:
        ret = false;
    }
    return ret;
}

void Tim::ClearFlag(Configs::Flags flag)
{
    switch (flag)
    {
    case Configs::Flags::UpdateFlag:
        LL_TIM_ClearFlag_UPDATE(self);
        break;
    case Configs::Flags::CC1Flag:
        LL_TIM_ClearFlag_CC1(self);
        break;
    case Configs::Flags::CC2Flag:
        LL_TIM_ClearFlag_CC2(self);
        break;
    case Configs::Flags::CC3Flag:
        LL_TIM_ClearFlag_CC3(self);
        break;
    case Configs::Flags::CC4Flag:
        LL_TIM_ClearFlag_CC4(self);
        break;
    case Configs::Flags::COMFlag:
        LL_TIM_ClearFlag_COM(self);
        break;
    case Configs::Flags::TriggerFlag:
        LL_TIM_ClearFlag_TRIG(self);
        break;
    case Configs::Flags::BreakFlag:
        LL_TIM_ClearFlag_BRK(self);
        break;
    default: break;
    }
}

void Tim::PeripheralClockControl(State state)
{
    //TIM1和TIM8属于高级定时器 挂载于APB2总线
    if (self == TIM1)
    {
        if (state == ENABLE)
            LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
        else LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM1);
    }
#if defined(TIM8)
    else if (self == TIM8)
    {
        if (state == ENABLE)
            LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);
        else LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM8);
    }
#endif
    else //其他定时器挂载于APB1总线
    {
        //根据计时器地址判断是哪个定时器
        if (self == TIM2)
        {
            if (state == ENABLE)
                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
            else LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM2);
        }
        else if (self == TIM3)
        {
            if (state == ENABLE)
                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
            else
                LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM3);
        }
        else if (self == TIM4)
        {
            if (state == ENABLE)
                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
            else
                LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM4);
        }
#if defined(TIM5)
        else if (self == TIM5)
        {
            if (state == ENABLE)
                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
            else
                LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM5);
        }
#endif
#if defined(TIM6)
        else if (self == TIM6)
        {
            if (state == ENABLE)
                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);
            else
                LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM6);
        }
#endif
#if defined(TIM7)
        else if (self == TIM7)
        {
            if (state == ENABLE)
                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);
            else
                LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM7);
        }
#endif
    }
}

void Tim::SelectSubModeTrigger(Tim::Configs::TimTrigger trigger)
{
    LL_TIM_SetTriggerInput(self, trigger);
}

void Tim::SelectMainModeOutputTrigger(Configs::InternalTrigger trigger)
{
    LL_TIM_SetTriggerOutput(self, trigger);
}

void Tim::SelectSubMode(Tim::Configs::SlaveMode mode)
{
    LL_TIM_SetSlaveMode(self, mode);
}

void Tim::OneClickToStart(uint16_t seconds)
{
    Tim tim = *this;
    //启动自己的外设时钟
    tim.PeripheralClockControl(State::ENABLE);

    //配置TIM为基本定时器模式 以1Hz的频率计数
    tim.SetClockMode(Tim::Configs::TIMClockMode::InternalClock); //使用内部时钟
    LL_TIM_InitTypeDef timeBaseConfig;
    timeBaseConfig.Prescaler = 7200 - 1; //预分频器设置为7200-1, 使计数频率为10kHz
    timeBaseConfig.CounterMode = LL_TIM_COUNTERMODE_UP; //向上计数模式
    timeBaseConfig.Autoreload = 10000 - 1; //自动重装载值设置为10000-1, 使计数周期为1秒
    timeBaseConfig.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1; //时钟分频设置为1
    timeBaseConfig.RepetitionCounter = 0; //重复计数器值设置为0(不使用)
    tim.TimeBaseInit(&timeBaseConfig); //初始化TIM的时基单元
    tim.ClearFlag(Tim::Configs::Flags::UpdateFlag); //清除更新中断挂起位

    //配置TIM的更新中断
    tim.InterruptSwitch(Tim::State::ENABLE, Tim::Configs::ITMode::Update); //使能TIM的更新中断
    IRQn_Type irq;
    if (tim.self == TIM1)
    {
        irq = TIM1_UP_IRQn; //TIM1更新中断
    }
    else if (tim.self == TIM2)
    {
        irq = TIM2_IRQn; //TIM2全局中断
    }
    else if (tim.self == TIM3)
    {
        irq = TIM3_IRQn; //TIM3全局中断
    }
    else if (tim.self == TIM4)
    {
        irq = TIM4_IRQn; //TIM4全局中断
    }
#if defined(TIM5)
    else if (tim.self == TIM5)
    {
        irq = TIM5_IRQn; //TIM5全局中断
    }
#endif
#if defined(TIM6)
    else if (tim.self == TIM6)
    {
        irq = TIM6_IRQn; //TIM6全局中断
    }
#endif
#if defined(TIM7)
    else if (tim.self == TIM7)
    {
        irq = TIM7_IRQn; //TIM7全局中断
    }
#endif
#if defined(TIM8)
    else if (tim.self == TIM8)
    {
        irq = TIM8_UP_IRQn; //TIM8更新中断
    }
#endif
    nvic.ChannelControl(irq, 0, 0, true);

    tim.Switch(Tim::State::ENABLE); //启动TIM2
}

void Tim::OutputCompare::Init(uint16_t Pluse, bool Polarity, bool NPolarity, OCMode mode, bool isEnabled, bool isNEnabled, uint8_t channel)
{
    //获取默认的配置
    Tim::OutputCompare::InitConfig config;
    config = GetDefaultInitConfig();

    config.OCMode = mode;
    config.OCPolarity = (Polarity) ? LL_TIM_OCPOLARITY_HIGH : LL_TIM_OCPOLARITY_LOW;
    config.OCNPolarity = (NPolarity) ? LL_TIM_OCPOLARITY_HIGH : LL_TIM_OCPOLARITY_LOW;
    config.OCState = (isEnabled) ? LL_TIM_OCSTATE_ENABLE : LL_TIM_OCSTATE_DISABLE;
    config.OCNState = (isNEnabled) ? LL_TIM_OCSTATE_ENABLE : LL_TIM_OCSTATE_DISABLE;
    config.CompareValue = Pluse;

    switch (channel)
    {
    case 0:
        LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH1, &config);
        LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH2, &config);
        LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH3, &config);
        LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH4, &config);
        break;
    case 1:LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH1, &config);break;
    case 2:LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH2, &config);break;
    case 3:LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH3, &config);break;
    case 4:LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH4, &config);break;
    default:return;
    }
}

void Tim::OutputCompare::Init(InitConfig* config, uint8_t channel)
{
    switch (channel)
    {
    case 1:LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH1, config);break;
    case 2:LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH2, config);break;
    case 3:LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH3, config);break;
    case 4:LL_TIM_OC_Init(self, LL_TIM_CHANNEL_CH4, config);break;
    default: return;
    }
}

void Tim::OutputCompare::EnableCCChannelOutput(uint8_t channel, bool isEnabled)
{
    if (isEnabled)
    {
        switch (channel)
        {
        case 1: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH1); break;
        case 2: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH2); break;
        case 3: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH3); break;
        case 4: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH4); break;
        default: return;
        }
    }
    else
    {
        switch (channel)
        {
        case 1: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH1); break;
        case 2: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH2); break;
        case 3: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH3); break;
        case 4: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH4); break;
        default: return;
        }
    }
}

void Tim::OutputCompare::EnableCCNChannelOutput(uint8_t channel, bool isEnabled)
{
    if (isEnabled)
    {
        switch (channel)
        {
        case 1: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH1N); break;
        case 2: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH2N); break;
        case 3: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH3N); break;
        default: return;
        }
    }
    else
    {
        switch (channel)
        {
        case 1: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH1N); break;
        case 2: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH2N); break;
        case 3: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH3N); break;
        default: return;
        }
    }
}

void Tim::OutputCompare::CCRPreloadConfig(uint8_t channel, bool isEnabled)
{
    if (isEnabled)
    {
        switch (channel)
        {
        case 1: LL_TIM_OC_EnablePreload(self, LL_TIM_CHANNEL_CH1); break;
        case 2: LL_TIM_OC_EnablePreload(self, LL_TIM_CHANNEL_CH2); break;
        case 3: LL_TIM_OC_EnablePreload(self, LL_TIM_CHANNEL_CH3); break;
        case 4: LL_TIM_OC_EnablePreload(self, LL_TIM_CHANNEL_CH4); break;
        default: return;
        }
    }
    else
    {
        switch (channel)
        {
        case 1: LL_TIM_OC_DisablePreload(self, LL_TIM_CHANNEL_CH1); break;
        case 2: LL_TIM_OC_DisablePreload(self, LL_TIM_CHANNEL_CH2); break;
        case 3: LL_TIM_OC_DisablePreload(self, LL_TIM_CHANNEL_CH3); break;
        case 4: LL_TIM_OC_DisablePreload(self, LL_TIM_CHANNEL_CH4); break;
        default: return;
        }
    }
}

void Tim::OutputCompare::SetOutputPolarity(uint8_t channel, bool polarity)
{
    switch (channel)
    {
    case 1: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH1, (polarity) ? LL_TIM_OCPOLARITY_HIGH : LL_TIM_OCPOLARITY_LOW); break;
    case 2: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH2, (polarity) ? LL_TIM_OCPOLARITY_HIGH : LL_TIM_OCPOLARITY_LOW); break;
    case 3: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH3, (polarity) ? LL_TIM_OCPOLARITY_HIGH : LL_TIM_OCPOLARITY_LOW); break;
    case 4: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH4, (polarity) ? LL_TIM_OCPOLARITY_HIGH : LL_TIM_OCPOLARITY_LOW); break;
    default: return;
    }
}

void Tim::OutputCompare::SetNOutputPolarity(uint8_t channel, bool polarity)
{
    switch (channel)
    {
    case 1: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH1N, (polarity) ? LL_TIM_OCPOLARITY_HIGH : LL_TIM_OCPOLARITY_LOW); break;
    case 2: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH2N, (polarity) ? LL_TIM_OCPOLARITY_HIGH : LL_TIM_OCPOLARITY_LOW); break;
    case 3: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH3N, (polarity) ? LL_TIM_OCPOLARITY_HIGH : LL_TIM_OCPOLARITY_LOW); break;
    default: return;
    }
}

void Tim::OutputCompare::SetOutputState(uint8_t channel, bool isEnabled)
{
#if defined(TIM6)
    if (self == TIM6) return;
#endif
#if defined(TIM7)
    if (self == TIM7) return;
#endif //基本定时器没有输出比较功能
    if (isEnabled)
    {
        switch (channel)
        {
        case 1: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH1); break;
        case 2: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH2); break;
        case 3: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH3); break;
        case 4: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH4); break;
        default: return;
        }
    }
    else
    {
        switch (channel)
        {
        case 1: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH1); break;
        case 2: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH2); break;
        case 3: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH3); break;
        case 4: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH4); break;
        default: return;
        }
    }
}

void Tim::OutputCompare::SetNOutputState(uint8_t channel, bool isEnabled)
{
#if defined(TIM6)
    if (self == TIM6) return;
#endif
#if defined(TIM7)
    if (self == TIM7) return;
#endif //基本定时器没有输出比较功能
    if (isEnabled)
    {
        switch (channel)
        {
        case 1: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH1N); break;
        case 2: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH2N); break;
        case 3: LL_TIM_CC_EnableChannel(self, LL_TIM_CHANNEL_CH3N); break;
        default: return;
        }
    }
    else
    {
        switch (channel)
        {
        case 1: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH1N); break;
        case 2: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH2N); break;
        case 3: LL_TIM_CC_DisableChannel(self, LL_TIM_CHANNEL_CH3N); break;
        default: return;
        }
    }
}

void Tim::OutputCompare::EnablePWMOutput(bool isEnabled)
{
    //只有TIM1和TIM8需要调用这个函数来启用PWM输出,其他定时器不需要设置PWM输出功能
    if (self != TIM1) return;
#if defined(TIM8)
    if (self != TIM8) return;
#endif
    if (isEnabled)
    {
        LL_TIM_EnableAllOutputs(self);
    }
}

void Tim::OutputCompare::SetOCMode(uint8_t channel, OCMode mode)
{
    switch (channel)
    {
    case 1: LL_TIM_OC_SetMode(self, LL_TIM_CHANNEL_CH1, mode); break;
    case 2: LL_TIM_OC_SetMode(self, LL_TIM_CHANNEL_CH2, mode); break;
    case 3: LL_TIM_OC_SetMode(self, LL_TIM_CHANNEL_CH3, mode); break;
    case 4: LL_TIM_OC_SetMode(self, LL_TIM_CHANNEL_CH4, mode); break;
    default: return;
    }
}

void Tim::OutputCompare::SetCCR(uint8_t channel, uint16_t ccr)
{
#if defined(TIM6)
    if (self == TIM6) return;
#endif
#if defined(TIM7)
    if (self == TIM7) return;
#endif //基本定时器没有输出比较功能
    switch (channel)
    {
    case 1: LL_TIM_OC_SetCompareCH1(self, ccr); break;
    case 2: LL_TIM_OC_SetCompareCH2(self, ccr); break;
    case 3: LL_TIM_OC_SetCompareCH3(self, ccr); break;
    case 4: LL_TIM_OC_SetCompareCH4(self, ccr); break;
    default: return;
    }
}

Tim::OutputCompare::InitConfig Tim::OutputCompare::GetDefaultInitConfig()
{
    InitConfig config;
    LL_TIM_OC_StructInit(&config);
    return config;
}

Tim::InputCapture::InitConfig Tim::InputCapture::GetDefaultInitConfig()
{
    InitConfig config;
    LL_TIM_IC_StructInit(&config);
    return config;
}

void Tim::InputCapture::Init(Tim::InputCapture::InitConfig* config, uint8_t channel)
{
    switch (channel)
    {
    case 0:
        LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH1, config);
        LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH2, config);
        LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH3, config);
        LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH4, config);
        break;
    case 1: LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH1, config); break;
    case 2: LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH2, config); break;
    case 3: LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH3, config); break;
    case 4: LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH4, config); break;
    default: return;
    }
}

void Tim::InputCapture::Init(ICPolarity polarity, ICActiveInput activeInput, ICPrescaler prescaler, ICFilter filter, uint8_t channel)
{
    Tim::InputCapture::InitConfig config;
    config = GetDefaultInitConfig();

    config.ICPolarity = polarity;
    config.ICActiveInput = activeInput;
    config.ICPrescaler = prescaler;
    config.ICFilter = filter;

    switch (channel)
    {
    case 0:
        LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH1, &config);
        LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH2, &config);
        LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH3, &config);
        LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH4, &config);
        break;
    case 1: LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH1, &config); break;
    case 2: LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH2, &config); break;
    case 3: LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH3, &config); break;
    case 4: LL_TIM_IC_Init(self, LL_TIM_CHANNEL_CH4, &config); break;
    default: return;
    }
}

void Tim::InputCapture::SetICActiveInput(uint8_t channel, Tim::InputCapture::ICActiveInput activeInput)
{
    switch (channel)
    {
    case 1: LL_TIM_IC_SetActiveInput(self, LL_TIM_CHANNEL_CH1, activeInput); break;
    case 2: LL_TIM_IC_SetActiveInput(self, LL_TIM_CHANNEL_CH2, activeInput); break;
    case 3: LL_TIM_IC_SetActiveInput(self, LL_TIM_CHANNEL_CH3, activeInput); break;
    case 4: LL_TIM_IC_SetActiveInput(self, LL_TIM_CHANNEL_CH4, activeInput); break;
    default: return;
    }
}

void Tim::InputCapture::SetICFilter(uint8_t channel, Tim::InputCapture::ICFilter filter)
{
    switch (channel)
    {
    case 1: LL_TIM_IC_SetFilter(self, LL_TIM_CHANNEL_CH1, filter); break;
    case 2: LL_TIM_IC_SetFilter(self, LL_TIM_CHANNEL_CH2, filter); break;
    case 3: LL_TIM_IC_SetFilter(self, LL_TIM_CHANNEL_CH3, filter); break;
    case 4: LL_TIM_IC_SetFilter(self, LL_TIM_CHANNEL_CH4, filter); break;
    default: return;
    }
}

void Tim::InputCapture::SetICPolarity(uint8_t channel, Tim::InputCapture::ICPolarity polarity)
{
    switch (channel)
    {
    case 1: LL_TIM_IC_SetPolarity(self, LL_TIM_CHANNEL_CH1, polarity); break;
    case 2: LL_TIM_IC_SetPolarity(self, LL_TIM_CHANNEL_CH2, polarity); break;
    case 3: LL_TIM_IC_SetPolarity(self, LL_TIM_CHANNEL_CH3, polarity); break;
    case 4: LL_TIM_IC_SetPolarity(self, LL_TIM_CHANNEL_CH4, polarity); break;
    default: return;
    }
}

void Tim::InputCapture::SetICPrescaler(uint8_t channel, Tim::InputCapture::ICPrescaler prescaler)
{
    switch (channel)
    {
    case 1: LL_TIM_IC_SetPrescaler(self, LL_TIM_CHANNEL_CH1, prescaler); break;
    case 2: LL_TIM_IC_SetPrescaler(self, LL_TIM_CHANNEL_CH2, prescaler); break;
    case 3: LL_TIM_IC_SetPrescaler(self, LL_TIM_CHANNEL_CH3, prescaler); break;
    case 4: LL_TIM_IC_SetPrescaler(self, LL_TIM_CHANNEL_CH4, prescaler); break;
    default: return;
    }
}

uint32_t Tim::InputCapture::GetCaptureValue(uint8_t channel)
{
    switch (channel)
    {
    case 1: return LL_TIM_IC_GetCaptureCH1(self);
    case 2: return LL_TIM_IC_GetCaptureCH2(self);
    case 3: return LL_TIM_IC_GetCaptureCH3(self);
    case 4: return LL_TIM_IC_GetCaptureCH4(self);
    default: return 0;
    }
}

void Tim::InputCapture::InitPWMI(ICPolarity polarity, ICPrescaler prescaler, ICFilter filter, PWMIGroup group)
{
    switch (group)
    {
    case PWMIGroup::Group1Target1:
        this->Init((polarity == ICPolarity::Rising) ? ICPolarity::Rising : ICPolarity::Falling,
                    ICActiveInput::Direct,
                    prescaler,
                    filter,
                    1);
        this->Init((polarity == ICPolarity::Rising) ? ICPolarity::Falling : ICPolarity::Rising,
                    ICActiveInput::InDirect,
                    prescaler,
                    filter,
                    2);
        break;
    case PWMIGroup::Group1Target2:
        this->Init((polarity == ICPolarity::Rising) ? ICPolarity::Rising : ICPolarity::Falling,
                    ICActiveInput::InDirect,
                    prescaler,
                    filter,
                    1);
        this->Init((polarity == ICPolarity::Rising) ? ICPolarity::Falling : ICPolarity::Rising,
                    ICActiveInput::Direct,
                    prescaler,
                    filter,
                    2);
        break;
    case PWMIGroup::Group2Target1:
        this->Init((polarity == ICPolarity::Rising) ? ICPolarity::Rising : ICPolarity::Falling,
                    ICActiveInput::Direct,
                    prescaler,
                    filter,
                    3);
        this->Init((polarity == ICPolarity::Rising) ? ICPolarity::Falling : ICPolarity::Rising,
                    ICActiveInput::InDirect,
                    prescaler,
                    filter,
                    4);
        break;
    case PWMIGroup::Group2Target2:
        this->Init((polarity == ICPolarity::Rising) ? ICPolarity::Rising : ICPolarity::Falling,
                    ICActiveInput::InDirect,
                    prescaler,
                    filter,
                    3);
        this->Init((polarity == ICPolarity::Rising) ? ICPolarity::Falling : ICPolarity::Rising,
                    ICActiveInput::Direct,
                    prescaler,
                    filter,
                    4);
        break;
    }
}




Tim tim1(TIM1); //APB2 高级定时器
Tim tim2(TIM2); //APB1 通用定时器
Tim tim3(TIM3); //APB1 通用定时器
Tim tim4(TIM4); //APB1 通用定时器
#if defined(TIM5)
Tim tim5(TIM5); //APB1 通用定时器
#endif
#if defined(TIM6)
Tim tim6(TIM6); //APB1 基本定时器
#endif
#if defined(TIM7)
Tim tim7(TIM7); //APB1 基本定时器
#endif
#if defined(TIM8)
Tim tim8(TIM8); //APB2 高级定时器
#endif

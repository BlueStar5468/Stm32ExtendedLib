/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_bus.h"
#include "Nvic.h"
#include "Tim.h"

/// @brief 切换计时器的中断使能状态
    /// @param state - 需要切换到的状态
    /// @param it - 需要切换状态的中断类型(默认为更新中断)
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

/// @brief 切换计时器的使能状态
    /// @param state - 需要切换到的状态
void Tim::Switch(State state)
{
    if (state == ENABLE)
        LL_TIM_EnableCounter(self);
    else
        LL_TIM_DisableCounter(self);
}

/// @brief 获取一个默认配置的计数器初始化结构体
/// @return 默认的计数器初始化结构体
Tim::TimeBaseInitConfig Tim::GetDefaultTimeBaseInitConfig()
{
    LL_TIM_InitTypeDef defaultConfig;
    LL_TIM_StructInit(&defaultConfig);
    return defaultConfig;
}

/// @brief 恢复计时器的缺省配置
void Tim::DeInit()
{
    LL_TIM_DeInit(self);
}

/// @brief 初始化计时器的时基单元
    /// @param config - 时基单元的配置结构体指针
void Tim::TimeBaseInit(TimeBaseInitConfig* config)
{
    LL_TIM_Init(self, config);
}

/// @brief 设置计时器的时钟模式(不含输入捕获的极性和通道以及滤波设置)
/// @param mode - 需要设置的时钟模式
/// @param trigger - 触发器选择(默认值为IT0, 具体查阅TimTrigger枚举，注意部分触发器可能在某些模式下无效)
/// @note 内部时钟模式:RCC------------------->内部时钟模式 -> PSC预分频器 -> CNT计数器
/// @note 其他定时器输入:ITRx----->选择器------>外部时钟模式1 -> PSC预分频器 -> CNT计数器
/// @note 外部时钟模式1:GPIO------>选择器------>外部时钟模式1 -> PSC预分频器 -> CNT计数器
/// @note TIx捕获模式(时钟1):GPIO--->选择器---->外部时钟模式1 -> PSC预分频器 -> CNT计数器
/// @note 外部时钟模式2:GPIO------------------>外部时钟模式2 -> PSC预分频器 -> CNT计数器
/// @note TIx捕获模式:GPIO------------------->编码器输入 -> PSC预分频器 -> CNT计数器
/// @note TIx为外部引脚(GPIO) EXT来源于专用外部时钟引脚 而ITRx在芯片内部链接,常用于定时器级联
void Tim::SetClockMode(Configs::TIMClockMode mode, Configs::TimTrigger trigger)
{
    if (mode == Configs::TIMClockMode::InternalClock)
    {
        LL_TIM_SetClockSource(self, LL_TIM_CLOCKSOURCE_INTERNAL);
    }
    else if (mode == Configs::TIMClockMode::ExternalClockMode1)
    {
        LL_TIM_SetClockSource(self, LL_TIM_CLOCKSOURCE_EXT_MODE1);
        LL_TIM_SetTriggerInput(self, trigger);
    }
    else if (mode == Configs::TIMClockMode::ExternalClockMode2)
    {
        LL_TIM_SetClockSource(self, LL_TIM_CLOCKSOURCE_EXT_MODE2);
    }
}

/// @brief 设置计时器的预分频器
/// @param prescaler 预分频器的值(必须在0x0000到0xFFFF之间)
/// @param reloadMode 预分频器更新模式(单次有效)(默认值为Wait)
void Tim::SetPrescaler(uint16_t prescaler, Configs::PSCReloadMode reloadMode)
{
    LL_TIM_SetPrescaler(self, prescaler);
    if (reloadMode == Configs::PSCReloadMode::Immediate)
    {
        LL_TIM_GenerateEvent_UPDATE(self); //立即更新预分频器的值
    }
}

/// @brief 设置计时器计数模式
/// @param mode 修改的目标模式
void Tim::SetCounterMode(Configs::CounterMode mode)
{
    LL_TIM_SetCounterMode(self, mode);
}

/// @brief 使能或失能ARR预装载(是否启用ARR的影子寄存器)
/// @param state 需要设置的状态
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

/// @brief 很简单的函数 给计数器设置一个值
/// @param counter 要设置的值(必须在0x0000到0xFFFF之间)
void Tim::SetCounter(uint16_t counter)
{
    LL_TIM_SetCounter(self, counter);
}

/// @brief 给ARR设置一个值(注意ARR预装载的状态会影响这个函数的生效时机)
/// @param autoReload 要设置的值(必须在0x0000到0xFFFF之间) 
/// @note 如果想设置ARR预装载 请使用ARReloadConfig(State)函数
void Tim::SetAutoReload(uint16_t autoReload)
{
    LL_TIM_SetAutoReload(self, autoReload);
}

/// @brief 获取计数器当前的值
/// @return 16位计数器的值
uint16_t Tim::GetCounter()
{
    return LL_TIM_GetCounter(self);
}

/// @brief 获取当前预分频器的值
/// @return 预分频器的值
uint16_t Tim::GetPrescaler()
{
    return LL_TIM_GetPrescaler(self);
}

/// @brief 获取当前ARR的值 
/// @return 当前ARR寄存器的值
uint16_t Tim::GetAutoReload()
{
    return LL_TIM_GetAutoReload(self);
}

/// @brief 获取指定的标志位的状态
/// @param flag 要获取的标志位
/// @return 标志位的状态
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

/// @brief 清除指定的标志位
/// @param flag 要清除的标志位
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

/// @brief 使能或失能计时器的外设时钟
/// @param state 需要设置的状态
/// @note 也可以使用RCC控制时钟,函数等效。
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

/// @brief 你是否因为繁杂的计时器配置而感到抓狂? 这个函数为你量身定做! 你只需要输入时间长度,他就会自动配置好计时器和中断!
/// @param seconds 计时器触发时间的长度(单位为秒,必须在0x0000到0xFFFF之间)
/// @note 记得写中断函数哦 如果不写会调到默认中断里死循环的!
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

/// @brief 通过参数初始化OC通道
/// @param Pluse CCR寄存器的值(必须在0x0000到0xFFFF之间)
/// @param Polarity 正向通道输出 true高电平 false低电平
/// @param NPolarity 反向通道输出(仅高级定时器支持) true高电平 false低电平
/// @param mode OC的模式
/// @param isEnabled 是否启用正向通道
/// @param isNEnabled 是否启用反向通道(仅高级定时器支持)
/// @param channel 要初始化的OC通道(每个定时器有4个1-4,只能同时选择一个,输入0代表全部通道)
/// @note 以上配置不一定对所有定时器都适用,请根据实际情况调整参数以避免不可预期的行为
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

/// @brief 通过初始化结构体初始化OC通道
/// @param config 初始化结构体
/// @param channel 需要初始化的通道(1-4)
/// @note 在使用高级定时器时,确保config结构体的所有函数均有值,否则可能会导致不可预期的行为
/// @note 不推荐自建结构体,可使用函数GetDefaultOCInitConfig()获取一个默认配置的结构体,然后修改其中的成员来满足需求
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

/// @brief 是否启用OCR寄存器的预装载(是否启用OCR的影子寄存器)
/// @param isEnabled true启用 false失能
void Tim::OutputCompare::PreloadConfig(uint8_t channel, bool isEnabled)
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

/// @brief 设置通道的输出极性
/// @param channel 要设置的通道(1-4)
/// @param polarity 输出极性 true高电平 false低电平
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


/// @brief 设置反向通道的输出极性
/// @param channel 要设置的通道(1-3) 反向通道只有1-3有 4没有
/// @param isToggled 是否反转输出极性 true反转 false不反转
void Tim::OutputCompare::SetNOutputPolarity(uint8_t channel, bool isToggled)
{
    switch (channel)
    {
    case 1: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH1N, (isToggled) ? LL_TIM_OCPOLARITY_LOW : LL_TIM_OCPOLARITY_HIGH); break;
    case 2: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH2N, (isToggled) ? LL_TIM_OCPOLARITY_LOW : LL_TIM_OCPOLARITY_HIGH); break;
    case 3: LL_TIM_OC_SetPolarity(self, LL_TIM_CHANNEL_CH3N, (isToggled) ? LL_TIM_OCPOLARITY_LOW : LL_TIM_OCPOLARITY_HIGH); break;
    default: return;
    }
}

/// @brief 设置目标通道的输出比较是否启用
/// @param channel 目标通道
/// @param isEnabled 是否启用输出比较 true使能 false失能
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

/// @brief 设置目标通道的反向输出比较是否启用
/// @param channel 目标通道
/// @param isEnabled 是否启用输出比较 true使能 false失能
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

/// @brief 启用高级定时器的PWM输出(通用定时器不需要设置PWM输出功能,默认开启PWM输出)
/// @param isEnabled 是否启用PWM输出 true启用 false失能
/// @note 只有TIM1和TIM8需要调用这个函数来启用PWM输出,其他定时器不需要设置PWM输出功能
/// @note 高级定时器:TIM1和TIM8 | TIM 15 16 17(本库不支持)
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

/// @brief 用于设置输出比较模式的函数
/// @param channel 要设置的通道
/// @param mode 目标模式
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

/// @brief 设置CCR寄存器的值 从而改变PWM的占空比或输出比较的比较值
/// @param channel 要设置的通道
/// @param ccr CCR寄存器的值(必须在0x0000到0xFFFF之间)
/// @note 对TIM6和TIM7无效 因为基本定时器没有输出比较功能
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

/// @brief 获取一个默认配置的输出比较初始化结构体
/// @return 默认配置的输出比较初始化结构体
Tim::OutputCompare::InitConfig Tim::OutputCompare::GetDefaultInitConfig()
{
    InitConfig config;
    LL_TIM_OC_StructInit(&config);
    return config;
}

Tim TIM_1(TIM1); //APB2 高级定时器
Tim TIM_2(TIM2); //APB1 通用定时器
Tim TIM_3(TIM3); //APB1 通用定时器
Tim TIM_4(TIM4); //APB1 通用定时器
#if defined(TIM5)
Tim TIM_5(TIM5); //APB1 通用定时器
#endif
#if defined(TIM6)
Tim TIM_6(TIM6); //APB1 基本定时器
#endif
#if defined(TIM7)
Tim TIM_7(TIM7); //APB1 基本定时器
#endif
#if defined(TIM8)
Tim TIM_8(TIM8); //APB2 高级定时器
#endif

/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#include "stm32f10x_tim.h"
#include "Tim.h"

/// @brief 切换计时器的中断使能状态
    /// @param state - 需要切换到的状态
    /// @param it - 需要切换状态的中断类型(默认为更新中断)
void Tim::InterruptSwitch(State state, Configs::ITMode it)
{
    //使用库函数配置
    if (state == State::ENABLE)
    {
        TIM_ITConfig(self, static_cast<uint16_t>(it), ENABLE);
    }
    else
    {
        TIM_ITConfig(self, static_cast<uint16_t>(it), DISABLE);
    }
}

/// @brief 切换计时器的使能状态
    /// @param state - 需要切换到的状态
void Tim::Switch(State state)
{
    TIM_Cmd(self, state);
}

/// @brief 获取一个默认配置的计数器初始化结构体
/// @return 默认的计数器初始化结构体
Tim::TimeBaseInitConfig Tim::GetDefaultTimeBaseInitConfig()
{
    TIM_TimeBaseInitTypeDef defaultConfig;
    TIM_TimeBaseStructInit(&defaultConfig);
    return defaultConfig;
}

/// @brief 恢复计时器的缺省配置
void Tim::DeInit()
{
    TIM_DeInit(self);
}

/// @brief 初始化计时器的时基单元
    /// @param config - 时基单元的配置结构体指针
void Tim::TimeBaseInit(TimeBaseInitConfig* config)
{
    TIM_TimeBaseInit(self, config);
}

/// @brief 设置计时器的时钟模式
/// @param mode - 需要设置的时钟模式
/// @param trigger - 内部触发器选择(默认值为IT0, 仅在OtherTIM模式下有效)
/// @param extPrescaler - 外部时钟预分频选择(默认值为OFF, 仅在ExternalClockMode1和ExternalClockMode2模式下有效)
/// @param tixChannel - TIx通道选择(默认值为TI1, 仅在TIxClock模式下有效)
/// @param Polarity - TIx和ETR外部时钟极性选择(默认值为Rising)
/// @param tixFilter - TIx输入滤波器配置(默认值为0),注意此值必须处于0x0到0xF之间
/// @note 内部时钟模式:RCC------------------->内部时钟模式 -> PSC预分频器 -> CNT计数器
/// @note 其他定时器输入:ITRx----->选择器------>外部时钟模式1 -> PSC预分频器 -> CNT计数器
/// @note 外部时钟模式1:GPIO------>选择器------>外部时钟模式1 -> PSC预分频器 -> CNT计数器
/// @note TIx捕获模式(时钟1):GPIO--->选择器---->外部时钟模式1 -> PSC预分频器 -> CNT计数器
/// @note 外部时钟模式2:GPIO------------------>外部时钟模式2 -> PSC预分频器 -> CNT计数器
/// @note TIx捕获模式:GPIO------------------->编码器输入 -> PSC预分频器 -> CNT计数器
/// @note TIx为外部引脚(GPIO) EXT来源于专用外部时钟引脚 而ITRx在芯片内部链接,常用于定时器级联
void Tim::SetClockMode(Configs::TIMClockMode mode, Configs::InternalTrigger trigger, Configs::ExternalPrescaler extPrescaler, Configs::TIxChannel tixChannel, Configs::Polarity polarity, uint16_t tixFilter)
{
    if (mode == Configs::TIMClockMode::InternalClock)
    {
        TIM_InternalClockConfig(self);
    }
    else if (mode == Configs::TIMClockMode::OtherTIM)
    {
        TIM_ITRxExternalClockConfig(self, static_cast<uint16_t>(trigger));
    }
    else if (mode == Configs::TIMClockMode::TIxClock)
    {
        TIM_TIxExternalClockConfig(self, static_cast<uint16_t>(tixChannel), static_cast<uint16_t>(polarity), tixFilter);
    }
    else if (mode == Configs::TIMClockMode::ExternalClockMode1)
    {
        if (polarity == Configs::Polarity::Rising)
        {
            TIM_ETRClockMode1Config(self, extPrescaler, TIM_ExtTRGPolarity_NonInverted, tixFilter);
        }
        else
        {
            TIM_ETRClockMode1Config(self, extPrescaler, TIM_ExtTRGPolarity_Inverted, tixFilter);
        }
    }
    else if (mode == Configs::TIMClockMode::ExternalClockMode2)
    {
        if (polarity == Configs::Polarity::Rising)
        {
            TIM_ETRClockMode2Config(self, extPrescaler, TIM_ExtTRGPolarity_NonInverted, tixFilter);
        }
        else
        {
            TIM_ETRClockMode2Config(self, extPrescaler, TIM_ExtTRGPolarity_Inverted, tixFilter);
        }
    }
}

/// @brief 设置计时器的预分频器
/// @param prescaler 预分频器的值(必须在0x0000到0xFFFF之间)
/// @param reloadMode 预分频器更新模式(默认值为Wait)
void Tim::SetPrescaler(uint16_t prescaler, Configs::PSCReloadMode reloadMode)
{
    TIM_PrescalerConfig(self, prescaler, static_cast<uint16_t>(reloadMode));
}

/// @brief 设置计时器计数模式
/// @param mode 修改的目标模式
void Tim::SetCounterMode(Configs::CounterMode mode)
{
    TIM_CounterModeConfig(self, static_cast<uint16_t>(mode));
}

/// @brief 使能或失能ARR预装载(是否启用ARR的影子寄存器)
/// @param state 需要设置的状态
void Tim::ARReloadConfig(State state)
{
    TIM_ARRPreloadConfig(self, state);
}

/// @brief 很简单的函数 给计数器设置一个值
/// @param counter 要设置的值(必须在0x0000到0xFFFF之间)
void Tim::SetCounter(uint16_t counter)
{
    TIM_SetCounter(self, counter);
}

/// @brief 给ARR设置一个值(注意ARR预装载的状态会影响这个函数的生效时机)
/// @param autoReload 要设置的值(必须在0x0000到0xFFFF之间) 
/// @note 如果想设置ARR预装载 请使用ARReloadConfig(State)函数
void Tim::SetAutoReload(uint16_t autoReload)
{
    TIM_SetAutoreload(self, autoReload);
}

/// @brief 获取计数器当前的值
/// @return 16位计数器的值
uint16_t Tim::GetCounter()
{
    return TIM_GetCounter(self);
}

/// @brief 获取当前预分频器的值
/// @return 预分频器的值
uint16_t Tim::GetPrescaler()
{
    return self->PSC;
}

/// @brief 获取当前ARR的值 
/// @return 当前ARR寄存器的值
uint16_t Tim::GetAutoReload()
{
    return self->ARR;
}

/// @brief 获取指定的标志位的状态
/// @param flag 要获取的标志位
/// @return 标志位的状态
bool Tim::GetFlagStatus(Configs::Flags flag)
{
    return TIM_GetFlagStatus(self, static_cast<uint16_t>(flag)) == SET;
}

/// @brief 清除指定的标志位
/// @param flag 要清除的标志位
void Tim::ClearFlag(Configs::Flags flag)
{
    TIM_ClearFlag(self, static_cast<uint16_t>(flag));
}

/// @brief 获取指定的中断状态
/// @param it 要获取的中断模式
/// @return 中断状态
bool Tim::GetITStatus(Configs::ITMode it)
{
    return TIM_GetITStatus(self, static_cast<uint16_t>(it)) == SET;
}

/// @brief 清除指定的中断挂起位
/// @param it 要清除的中断模式
void Tim::ClearITPendingBit(Configs::ITMode it)
{
    TIM_ClearITPendingBit(self, static_cast<uint16_t>(it));
}

/// @brief 使能或失能计时器的外设时钟
/// @param state 需要设置的状态
/// @note 也可以使用RCC控制时钟,函数等效。
void Tim::PeripheralClockControl(State state)
{
    //TIM1和TIM8属于高级定时器 挂载于APB2总线
    if (self == TIM1 || self == TIM8)
    {
        RCC_APB2PeriphClockCmd((self == TIM1) ? RCC_APB2Periph_TIM1 : RCC_APB2Periph_TIM8, state);
    }
    else //其他定时器挂载于APB1总线
    {
        //根据计时器地址判断是哪个定时器
        if (self == TIM2)
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, state);
        }
        else if (self == TIM3)
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, state);
        }
        else if (self == TIM4)
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, state);
        }
        else if (self == TIM5)
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, state);
        }
        else if (self == TIM6)
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, state);
        }
        else if (self == TIM7)
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, state);
        }
    }
}

/// @brief 你是否因为繁杂的计时器配置而感到抓狂? 这个函数为你量身定做! 你只需要输入时间长度,他就会自动配置好计时器和中断!
/// @param seconds 计时器触发事件
/// @note 记得写中断函数哦 如果不写会调到默认中断里死循环的!
void Tim::Foolish::Start(Tim tim, uint16_t seconds)
{
    //启动自己的外设时钟
    tim.PeripheralClockControl(State::ENABLE);

    //配置TIM2为基本定时器模式 以1Hz的频率计数
    tim.SetClockMode(Tim::Configs::TIMClockMode::InternalClock); //使用内部时钟
    TIM_TimeBaseInitTypeDef timeBaseConfig;
    timeBaseConfig.TIM_Prescaler = 7200 - 1; //预分频器设置为7200-1, 使计数频率为10kHz
    timeBaseConfig.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    timeBaseConfig.TIM_Period = 10000 - 1; //自动重装载值设置为10000-1, 使计数周期为1秒
    timeBaseConfig.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频设置为1
    timeBaseConfig.TIM_RepetitionCounter = 0; //重复计数器值设置为0(不使用)
    tim.TimeBaseInit(&timeBaseConfig); //初始化TIM的时基单元
    tim.ClearITPendingBit(Tim::Configs::ITMode::Update); //清除更新中断挂起位


    //配置TIM的更新中断
    tim.InterruptSwitch(Tim::State::ENABLE, Tim::Configs::ITMode::Update); //使能TIM的更新中断
    NVIC_InitTypeDef nvicConfig;
    if (tim.self == TIM1)
    {
        nvicConfig.NVIC_IRQChannel = TIM1_UP_IRQn; //TIM1更新中断
    }
    else if (tim.self == TIM2)
    {
        nvicConfig.NVIC_IRQChannel = TIM2_IRQn; //TIM2全局中断
    }
    else if (tim.self == TIM3)
    {
        nvicConfig.NVIC_IRQChannel = TIM3_IRQn; //TIM3全局中断
    }
    else if (tim.self == TIM4)
    {
        nvicConfig.NVIC_IRQChannel = TIM4_IRQn; //TIM4全局中断
    }
#ifdef STM32F10X_HD
    else if (tim.self == TIM5)
    {
        nvicConfig.NVIC_IRQChannel = TIM5_IRQn; //TIM5全局中断
    }
    else if (tim.self == TIM6)
    {
        nvicConfig.NVIC_IRQChannel = TIM6_IRQn; //TIM6全局中断
    }
    else if (tim.self == TIM7)
    {
        nvicConfig.NVIC_IRQChannel = TIM7_IRQn; //TIM7全局中断
    }
    else if (tim.self == TIM8)
    {
        nvicConfig.NVIC_IRQChannel = TIM8_UP_IRQn; //TIM8更新中断
    }
#endif
    else {}
    nvicConfig.NVIC_IRQChannelPreemptionPriority = 0;
    nvicConfig.NVIC_IRQChannelSubPriority = 0;
    nvicConfig.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicConfig);

    tim.Switch(Tim::State::ENABLE); //启动TIM2
}

/// @brief 通过参数初始化OC通道
/// @param Pluse CCR寄存器的值(必须在0x0000到0xFFFF之间)
/// @param Polarity 正向通道输出 true高电平 false低电平
/// @param NPolarity 反向通道输出(仅高级定时器支持) true高电平 false低电平
/// @param mode OC的模式
/// @param isEnabled 是否启用正向通道
/// @param isNEnabled 是否启用反向通道(仅高级定时器支持)
/// @param channel 要初始化的OC通道(每个定时器有4个1-4)
/// @note 以上配置不一定对所有定时器都适用,请根据实际情况调整参数以避免不可预期的行为
void Tim::OutputCompare::Init(uint16_t Pluse, bool Polarity, bool NPolarity, OCMode mode, bool isEnabled, bool isNEnabled, uint8_t channel)
{
    //获取默认的配置
    Tim::OutputCompare::InitConfig config;
    config = GetDefaultInitConfig();

    config.TIM_OCMode = static_cast<uint16_t>(mode);
    config.TIM_OCPolarity = (Polarity) ? TIM_OCPolarity_High : TIM_OCPolarity_Low;
    config.TIM_OCNPolarity = (NPolarity) ? TIM_OCNPolarity_High : TIM_OCNPolarity_Low;
    config.TIM_OutputState = (isEnabled) ? TIM_OutputState_Enable : TIM_OutputState_Disable;
    config.TIM_OutputNState = (isNEnabled) ? TIM_OutputNState_Enable : TIM_OutputNState_Disable;
    config.TIM_Pulse = Pluse;

    switch (channel)
    {
    case 1:TIM_OC1Init(self, &config);break;
    case 2:TIM_OC2Init(self, &config);break;
    case 3:TIM_OC3Init(self, &config);break;
    case 4:TIM_OC4Init(self, &config);break;
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
    case 1:TIM_OC1Init(self, config);break;
    case 2:TIM_OC2Init(self, config);break;
    case 3:TIM_OC3Init(self, config);break;
    case 4:TIM_OC4Init(self, config);break;
    }
}

/// @brief 设置通道的强制输出状态
/// @param isActive true强制输出高电平 false强制输出低电平
void Tim::OutputCompare::ForcedOutput(uint8_t channel, bool isActive)
{
    switch (channel)
    {
    case 1: TIM_ForcedOC1Config(self, (isActive) ? TIM_ForcedAction_Active : TIM_ForcedAction_InActive); break;
    case 2: TIM_ForcedOC2Config(self, (isActive) ? TIM_ForcedAction_Active : TIM_ForcedAction_InActive); break;
    case 3: TIM_ForcedOC3Config(self, (isActive) ? TIM_ForcedAction_Active : TIM_ForcedAction_InActive); break;
    case 4: TIM_ForcedOC4Config(self, (isActive) ? TIM_ForcedAction_Active : TIM_ForcedAction_InActive); break;
    }
}

/// @brief 是否启用OCR寄存器的预装载(是否启用OCR的影子寄存器)
/// @param isEnabled true启用 false失能
void Tim::OutputCompare::PreloadConfig(uint8_t channel, bool isEnabled)
{
    switch (channel)
    {
    case 1: TIM_OC1PreloadConfig(self, (isEnabled) ? TIM_OCPreload_Enable : TIM_OCPreload_Disable); break;
    case 2: TIM_OC2PreloadConfig(self, (isEnabled) ? TIM_OCPreload_Enable : TIM_OCPreload_Disable); break;
    case 3: TIM_OC3PreloadConfig(self, (isEnabled) ? TIM_OCPreload_Enable : TIM_OCPreload_Disable); break;
    case 4: TIM_OC4PreloadConfig(self, (isEnabled) ? TIM_OCPreload_Enable : TIM_OCPreload_Disable); break;
    }
}

/// @brief 设置通道的输出极性
/// @param channel 要设置的通道
/// @param isToggled 是否反转输出极性 true反转 false不反转
void Tim::OutputCompare::SetOutputPolarity(uint8_t channel, bool isToggled)
{
    switch (channel)
    {
    case 1: TIM_OC1PolarityConfig(self, (isToggled) ? TIM_OCPolarity_Low : TIM_OCPolarity_High); break;
    case 2: TIM_OC2PolarityConfig(self, (isToggled) ? TIM_OCPolarity_Low : TIM_OCPolarity_High); break;
    case 3: TIM_OC3PolarityConfig(self, (isToggled) ? TIM_OCPolarity_Low : TIM_OCPolarity_High); break;
    case 4: TIM_OC4PolarityConfig(self, (isToggled) ? TIM_OCPolarity_Low : TIM_OCPolarity_High); break;
    }
}


/// @brief 设置反向通道的输出极性
/// @param channel 要设置的通道(1-3) 反向通道只有1-3有 4没有
/// @param isToggled 是否反转输出极性 true反转 false不反转
void Tim::OutputCompare::SetNOutputPolarity(uint8_t channel, bool isToggled)
{
    switch (channel)
    {
    case 1: TIM_OC1NPolarityConfig(self, (isToggled) ? TIM_OCPolarity_Low : TIM_OCPolarity_High); break;
    case 2: TIM_OC2NPolarityConfig(self, (isToggled) ? TIM_OCPolarity_Low : TIM_OCPolarity_High); break;
    case 3: TIM_OC3NPolarityConfig(self, (isToggled) ? TIM_OCPolarity_Low : TIM_OCPolarity_High); break;
    }
}

/// @brief 设置目标通道的输出比较是否启用
/// @param channel 目标通道
/// @param isEnabled 是否启用输出比较 true使能 false失能
void Tim::OutputCompare::SetOutputState(uint8_t channel, bool isEnabled)
{
    if (self == TIM6 || self == TIM7) return; //基本定时器没有输出比较功能
    switch (channel)
    {
    case 1: TIM_CCxCmd(self, TIM_Channel_1, (isEnabled) ? TIM_CCx_Enable : TIM_CCx_Disable); break;
    case 2: TIM_CCxCmd(self, TIM_Channel_2, (isEnabled) ? TIM_CCx_Enable : TIM_CCx_Disable); break;
    case 3: TIM_CCxCmd(self, TIM_Channel_3, (isEnabled) ? TIM_CCx_Enable : TIM_CCx_Disable); break;
    case 4: TIM_CCxCmd(self, TIM_Channel_4, (isEnabled) ? TIM_CCx_Enable : TIM_CCx_Disable); break;
    }
}

/// @brief 设置目标通道的反向输出比较是否启用
/// @param channel 目标通道
/// @param isEnabled 是否启用输出比较 true使能 false失能
void Tim::OutputCompare::SetNOutputState(uint8_t channel, bool isEnabled)
{
    if (self == TIM6 || self == TIM7) return; //基本定时器没有输出比较功能
    switch (channel)
    {
    case 1: TIM_CCxNCmd(self, TIM_Channel_1, (isEnabled) ? TIM_CCxN_Enable : TIM_CCxN_Disable); break;
    case 2: TIM_CCxNCmd(self, TIM_Channel_2, (isEnabled) ? TIM_CCxN_Enable : TIM_CCxN_Disable); break;
    case 3: TIM_CCxNCmd(self, TIM_Channel_3, (isEnabled) ? TIM_CCxN_Enable : TIM_CCxN_Disable); break;
    }
}

/// @brief 启用高级定时器的PWM输出(通用定时器不许呀设置PWM输出功能,默认开启PWM输出)
/// @param isEnabled 是否启用PWM输出 true启用 false失能
/// @note 只有TIM1和TIM8需要调用这个函数来启用PWM输出,其他定时器不需要设置PWM输出功能
/// @note 高级定时器:TIM1和TIM8 | TIM 15 16 17(本库不支持)
void Tim::OutputCompare::EnablePWMOutput(bool isEnabled)
{
    TIM_CtrlPWMOutputs(self, (isEnabled) ? ENABLE : DISABLE);
}

/// @brief 用于设置输出比较模式的函数
/// @param channel 要设置的通道
/// @param mode 目标模式
void Tim::OutputCompare::SetOCMode(uint8_t channel, OCMode mode)
{
    TIM_SelectOCxM(self, channel, static_cast<uint16_t>(mode));
}

/// @brief 设置CCR寄存器的值 从而改变PWM的占空比或输出比较的比较值
/// @param channel 要设置的通道
/// @param ccr CCR寄存器的值(必须在0x0000到0xFFFF之间)
/// @note 对TIM6和TIM7无效 因为基本定时器没有输出比较功能
void Tim::OutputCompare::SetCCR(uint8_t channel, uint16_t ccr)
{
    if (self == TIM6 || self == TIM7) return; //基本定时器没有输出比较功能
    switch (channel)
    {
    case 1: TIM_SetCompare1(self, ccr); break;
    case 2: TIM_SetCompare2(self, ccr); break;
    case 3: TIM_SetCompare3(self, ccr); break;
    case 4: TIM_SetCompare4(self, ccr); break;
    }
}

/// @brief 获取一个默认配置的输出比较初始化结构体
/// @return 默认配置的输出比较初始化结构体
Tim::OutputCompare::InitConfig Tim::OutputCompare::GetDefaultInitConfig()
{
    InitConfig config;
    TIM_OCStructInit(&config);
    return config;
}

Tim TIM_1(TIM1); //APB2 高级定时器
Tim TIM_2(TIM2); //APB1 通用定时器
Tim TIM_3(TIM3); //APB1 通用定时器
Tim TIM_4(TIM4); //APB1 通用定时器
Tim TIM_5(TIM5); //APB1 通用定时器
Tim TIM_6(TIM6); //APB1 基本定时器
Tim TIM_7(TIM7); //APB1 基本定时器
Tim TIM_8(TIM8); //APB2 高级定时器


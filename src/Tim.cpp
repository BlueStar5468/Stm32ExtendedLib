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

Tim TIM_1(TIM1); //APB2 高级定时器
Tim TIM_2(TIM2); //APB1 通用定时器
Tim TIM_3(TIM3); //APB1 通用定时器
Tim TIM_4(TIM4); //APB1 通用定时器
Tim TIM_5(TIM5); //APB1 通用定时器
Tim TIM_6(TIM6); //APB1 基本定时器
Tim TIM_7(TIM7); //APB1 基本定时器
Tim TIM_8(TIM8); //APB2 高级定时器


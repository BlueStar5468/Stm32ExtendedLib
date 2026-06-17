/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/
#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f1xx_ll_tim.h"
//stm32f10x的计时器
//该类提供了对stm32f10x计时器的操作接口
//计时器速查表:
//高级计时器(APB2):TIM1, TIM8
//通用计时器(APB1):TIM2, TIM3, TIM4, TIM5
//基本计时器(APB1):TIM6, TIM7
/// @note 计时器初始化基本步骤:
/// @note 1.使能计时器的外设时钟
/// @note 2.配置计时器的时基单元(计数周期和计数模式)
/// @note 3.配置计时器的时钟模式(内部时钟/外部时钟/其他定时器/编码器模式等)
/// @note 4.配置计时器的中断(可选)
class Tim
{
public:
    /// @brief 计时器时基单元的配置结构体(请以指针形式使用)
    /// @param TIM_Prescaler - 计时器预分频器, 范围为0x0000到0xFFFF
    /// @param TIM_CounterMode - 计数器模式
    /// @param TIM_Autoreload - 自动重装载值, 范围为0,达到此值即触发更新.
    /// @param TIM_ClockDivision - 时钟分频(用于滤波器采样时钟,基础频率为72MHz) 使用Tim::Configs::ClockDivision枚举
    /// @param TIM_RepetitionCounter - 重复计数器值, 范围为0x00到0xFF(只适用于高级计时器TIM1和TIM8)
    /// @note 计时器周期可由 ((重装值 + 1)/72MHz) * (预分频器值 + 1) 计算得出
    typedef LL_TIM_InitTypeDef TimeBaseInitConfig;
    /// @brief 计时器的使能状态枚举
    /// @param ENABLE - 使能计时器
    /// @param DISABLE - 禁用计时器
    typedef FunctionalState State;

    struct Configs
    {
        /*
        中断模式表
        */
        typedef enum ITMode
        {
            Update, //更新中断
            CC1, //捕获/比较1中断
            CC2, //捕获/比较2中断
            CC3, //捕获/比较3中断
            CC4, //捕获/比较4中断
            COM, //通信中断
            Trigger, //触发中断
            Break, //死区中断
        } ITMode;

        /*
        时钟模式表
        */
        typedef enum TIMClockMode
        {
            InternalClock,      //内部时钟
            ExternalClockMode1, //外部时钟模式1
            ExternalClockMode2, //外部时钟模式2
        } TIMClockMode;

        //计时器触发器表
        typedef enum TimTrigger
        {
            //内部触发器列表 用于定时器间的级联触发

            IT0 = LL_TIM_TS_ITR0, //内部触发器0
            IT1 = LL_TIM_TS_ITR1, //内部触发器1
            IT2 = LL_TIM_TS_ITR2, //内部触发器2
            IT3 = LL_TIM_TS_ITR3, //内部触发器3

            //TIx通道列表 用于外部时钟模式1的时钟源选择和TIx捕获模式的输入选择

            TI1Edge = LL_TIM_TS_TI1F_ED, //TI1通道边缘检测
            TI1 = LL_TIM_TS_TI1FP1,       //TI1通道
            TI2 = LL_TIM_TS_TI2FP2,       //TI2通道

            //外部引脚列表 用于外部时钟模式1的时钟源选择

            ETRF = LL_TIM_TS_ETRF   //外部触发输入

        } TimTrigger;

        //外部引脚极性
        typedef enum Polarity
        {
            Rising = LL_TIM_ETR_POLARITY_NONINVERTED,   //上升沿触发
            Falling = LL_TIM_ETR_POLARITY_INVERTED, //下降沿触发
        } Polarity;


        //外部预分频器
        typedef enum ExternalPrescaler
        {
            OFF = LL_TIM_ETR_PRESCALER_DIV1, //不分频
            Div2 = LL_TIM_ETR_PRESCALER_DIV2, //分频2
            Div4 = LL_TIM_ETR_PRESCALER_DIV4, //分频4
            Div8 = LL_TIM_ETR_PRESCALER_DIV8, //分频8
        } ExternalPrescaler;

        //计数器模式
        typedef enum CounterMode
        {
            Up = LL_TIM_COUNTERMODE_UP, //向上计数
            Down = LL_TIM_COUNTERMODE_DOWN, //向下计数
            CenterAligned1 = LL_TIM_COUNTERMODE_CENTER_UP, //中心对齐向上
            CenterAligned2 = LL_TIM_COUNTERMODE_CENTER_DOWN, //中心对齐向下
            CenterAligned3 = LL_TIM_COUNTERMODE_CENTER_UP_DOWN, //中心对齐向上向下
        } CounterMode;

        //PSC更新设置
        //表示预分频器值的更新时机,是单次有效的设置,在调用SetPrescaler函数时使用
        typedef enum PSCReloadMode
        {
            Wait,           //启用ARR预装载,计数器完成当前周期后更新PSC
            Immediate,    //不启用ARR预装载,PSC立即更新,并重置计数器
        } PSCReloadMode;

        //计时器标志位
        typedef enum Flags
        {
            UpdateFlag,      //更新标志
            CC1Flag,         //捕获/比较1标志
            CC2Flag,         //捕获/比较2标志
            CC3Flag,         //捕获/比较3标志
            CC4Flag,         //捕获/比较4标志
            COMFlag,         //通信标志
            TriggerFlag,      //触发标志
            BreakFlag,       //死区标志
        } Flags;

        //滤波采样器分频
        typedef enum ClockDivision
        {
            ClockDiv1 = LL_TIM_CLOCKDIVISION_DIV1, //不分频
            ClockDiv2 = LL_TIM_CLOCKDIVISION_DIV2, //分频2
            ClockDiv4 = LL_TIM_CLOCKDIVISION_DIV4, //分频4
        } ClockDivision;

    };

    //对象硬件地址
    TIM_TypeDef* self;

    /// @brief 计时器对象的构造函数
    Tim(TIM_TypeDef* timDef) : self(timDef), OC(timDef) {}

    void DeInit();
    void TimeBaseInit(TimeBaseInitConfig* config);
    TimeBaseInitConfig GetDefaultTimeBaseInitConfig();
    void Switch(State state);
    void InterruptSwitch(bool isEnabled, Configs::ITMode it = Configs::ITMode::Update);
    void SetClockMode(Configs::TIMClockMode mode, Tim::Configs::TimTrigger trigger = Configs::TimTrigger::IT0);
    void SetPrescaler(uint16_t prescaler, Configs::PSCReloadMode reloadMode = Configs::PSCReloadMode::Wait);
    void SetCounterMode(Configs::CounterMode mode);
    void ARReloadConfig(State state);
    void SetCounter(uint16_t counter);
    void SetAutoReload(uint16_t autoReload);
    uint16_t GetCounter();
    uint16_t GetPrescaler();
    uint16_t GetAutoReload();

    //标志位相关函数

    bool GetFlagStatus(Configs::Flags flag);
    void ClearFlag(Configs::Flags flag);

    //时钟控制

    void PeripheralClockControl(State state);

    //傻瓜配置函数

    void OneClickToStart(uint16_t seconds);


    // 输出比较部分
    /// @note 此部分为动态结构体,需要使用对象调用
    /// @note PWM频率:CK_PSC / (PSC + 1) / (ARR + 1)  ; PSC - 预分频器 
    /// @note PWM占空比:CCR / (ARR + 1) 
    /// @note PWM步长(百分比) : 1 / (ARR + 1)
    /// @note 对于高级定时器 使用PWM需要调用EnablePWMOutput函数使能PWM输出
    struct OutputCompare
    {
        //初始化结构体
        /// @param TIM_OCMode - 输出比较模式
        /// @param TIM_OutputState - 输出比较状态
        typedef LL_TIM_OC_InitTypeDef InitConfig;
        //硬件地址
        TIM_TypeDef* self;
        //构造函数
        OutputCompare(TIM_TypeDef* timDef) : self(timDef) {}
        //输出比较模式表
        typedef enum OCMode
        {
            Frozen = LL_TIM_OCMODE_FROZEN,         //冻结模式 CNT与CCR无关 保持输出
            Active = LL_TIM_OCMODE_ACTIVE,         //CNT=CCR时 输出高电平
            Inactive = LL_TIM_OCMODE_INACTIVE,     //CNT=CCR时 输出低电平
            Toggle = LL_TIM_OCMODE_TOGGLE,         //CNT与CCR匹配时 翻转输出
            PWM1 = LL_TIM_OCMODE_PWM1,             //PWM模式1: 向上计数时 CNT < CCR时输出高电平 否则输出低电平; 向下计数时 CNT > CCR时输出高电平 否则输出低电平
            PWM2 = LL_TIM_OCMODE_PWM2,             //PWM模式2: 向上计数时 CNT > CCR时输出高电平 否则输出低电平; 向下计数时 CNT < CCR时输出高电平 否则输出低电平
            ForcedActive = LL_TIM_OCMODE_FORCED_ACTIVE,   //强制输出高电平
            ForcedInactive = LL_TIM_OCMODE_FORCED_INACTIVE, //强制输出低电平
        } OCMode;

        void Init(uint16_t Pluse, bool Polarity, bool NPolarity, OCMode mode, bool isEnabled, bool isNEnabled, uint8_t channel);
        void Init(InitConfig* config, uint8_t channel);
        void PreloadConfig(uint8_t channel, bool isEnabled);
        void SetOutputPolarity(uint8_t channel, bool isToggled);
        void SetNOutputPolarity(uint8_t channel, bool isToggled);
        void SetOutputState(uint8_t channel, bool isEnabled);
        void SetNOutputState(uint8_t channel, bool isEnabled);
        void EnablePWMOutput(bool isEnabled);
        void SetOCMode(uint8_t channel, OCMode mode);
        void SetCCR(uint8_t channel, uint16_t ccr);
        InitConfig GetDefaultInitConfig();
    } OC;

};

/*
定时器对象表
*/

extern Tim TIM_1; //APB2 高级定时器
extern Tim TIM_2; //APB1 通用定时器
extern Tim TIM_3; //APB1 通用定时器
extern Tim TIM_4; //APB1 通用定时器
extern Tim TIM_5; //APB1 通用定时器
extern Tim TIM_6; //APB1 基本定时器
extern Tim TIM_7; //APB1 基本定时器
extern Tim TIM_8; //APB2 高级定时器

#endif

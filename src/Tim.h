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

        //计时器内部触发器表
        typedef enum InternalTrigger
        {
            Reset = LL_TIM_TRGO_RESET, //重置
            Enable = LL_TIM_TRGO_ENABLE, //使能
            Update = LL_TIM_TRGO_UPDATE, //更新事件
            OC1REF = LL_TIM_TRGO_OC1REF, //比较OC1REF信号(仅TIM1和TIM8支持)
            OC2REF = LL_TIM_TRGO_OC2REF, //比较OC2REF信号(仅TIM1和TIM8支持)
            OC3REF = LL_TIM_TRGO_OC3REF, //比较OC3REF信号(仅TIM1和TIM8支持)
            OC4REF = LL_TIM_TRGO_OC4REF, //比较脉冲OC4REF信号(仅TIM1和TIM8支持)
            CC1IF = LL_TIM_TRGO_CC1IF, //滤波定时器输入1事件
        } InternalTrigger;

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

        //从模式指令表
        typedef enum SlaveMode
        {
            Reset = LL_TIM_SLAVEMODE_RESET, //重置模式
            Gated = LL_TIM_SLAVEMODE_GATED, //门控模式
            Trigger = LL_TIM_SLAVEMODE_TRIGGER, //触发模式
            Disabled = LL_TIM_SLAVEMODE_DISABLED, //从模式禁用
        } SlaveMode;
    };

    //对象硬件地址
    TIM_TypeDef* self;

    /// @brief 计时器对象的构造函数
    Tim(TIM_TypeDef* timDef) : self(timDef), OC(timDef), IC(timDef) {}

    /// @brief 恢复计时器的缺省配置
    void DeInit();
    /// @brief 初始化计时器的时基单元
    /// @param config - 时基单元的配置结构体指针
    void TimeBaseInit(TimeBaseInitConfig* config);
    /// @brief 获取一个默认配置的计数器初始化结构体
    /// @return 默认的计数器初始化结构体
    TimeBaseInitConfig GetDefaultTimeBaseInitConfig();
    /// @brief 切换计时器的使能状态
    /// @param state - 需要切换到的状态
    void Switch(State state);
    /// @brief 切换计时器的中断使能状态
    /// @param state - 需要切换到的状态
    /// @param it - 需要切换状态的中断类型(默认为更新中断)
    void InterruptSwitch(bool isEnabled, Configs::ITMode it = Configs::ITMode::Update);
    /// @brief 设置计时器的时钟模式(不含输入捕获的极性和通道以及滤波设置)
    /// @brief 在设置外部时钟模式1时,触发器请使用SelectSubModeTrigger函数设置TRGI信号源
    /// @param mode - 需要设置的时钟模式
    /// @param trigger - 触发器选择(默认值为IT0, 具体查阅TimTrigger枚举，注意部分触发器可能在某些模式下无效)
    /// @note 内部时钟模式:RCC------------------->内部时钟模式 -> PSC预分频器 -> CNT计数器
    /// @note 其他定时器输入:ITRx----->选择器------>外部时钟模式1 -> PSC预分频器 -> CNT计数器
    /// @note 外部时钟模式1:GPIO------>选择器------>外部时钟模式1 -> PSC预分频器 -> CNT计数器
    /// @note TIx捕获模式(时钟1):GPIO--->选择器---->外部时钟模式1 -> PSC预分频器 -> CNT计数器
    /// @note 外部时钟模式2:GPIO------------------>外部时钟模式2 -> PSC预分频器 -> CNT计数器
    /// @note TIx捕获模式:GPIO------------------->编码器输入 -> PSC预分频器 -> CNT计数器
    /// @note TIx为外部引脚(GPIO) EXT来源于专用外部时钟引脚 而ITRx在芯片内部链接,常用于定时器级联
    void SetClockMode(Configs::TIMClockMode mode);
    /// @brief 设置计时器的预分频器
    /// @param prescaler 预分频器的值(必须在0x0000到0xFFFF之间)
    /// @param reloadMode 预分频器更新模式(单次有效)(默认值为Wait)
    void SetPrescaler(uint16_t prescaler, Configs::PSCReloadMode reloadMode = Configs::PSCReloadMode::Wait);
    /// @brief 设置计时器计数模式
    /// @param mode 修改的目标模式
    void SetCounterMode(Configs::CounterMode mode);
    /// @brief 使能或失能ARR预装载(是否启用ARR的影子寄存器)
    /// @param state 需要设置的状态
    void ARReloadConfig(State state);
    /// @brief 很简单的函数 给计数器设置一个值
    /// @param counter 要设置的值(必须在0x0000到0xFFFF之间)
    void SetCounter(uint16_t counter);
    /// @brief 给ARR设置一个值(注意ARR预装载的状态会影响这个函数的生效时机)
    /// @param autoReload 要设置的值(必须在0x0000到0xFFFF之间) 
    /// @note 如果想设置ARR预装载 请使用ARReloadConfig(State)函数
    void SetAutoReload(uint16_t autoReload);
    /// @brief 获取计数器当前的值
    /// @return 16位计数器的值
    uint16_t GetCounter();
    /// @brief 获取当前预分频器的值
    /// @return 预分频器的值
    uint16_t GetPrescaler();
    /// @brief 获取当前ARR的值 
    /// @return 当前ARR寄存器的值
    uint16_t GetAutoReload();
    /// @brief 选择从模式触发源
    /// @param trigger 触发源
    /// @note 此设置每个Timer只有一个
    void SelectSubModeTrigger(Tim::Configs::TimTrigger trigger);
    /// @brief 选择主模式输出触发源 这将会将这个tim的信号输出到它的ITRx上 ITRx的具体链接在不同tim间不同 请查手册
    /// @param trigger 此Timer内的信号源
    void SelectMainModeOutputTrigger(Configs::InternalTrigger trigger);
    /// @brief 设置此定时器的从模式指令
    /// @param mode 要设置的模式
    /// @note 此函数不包含触发器选择 如需选择使用SelectSubModeTrigger函数设置触发器
    void SelectSubMode(Configs::SlaveMode mode);

    //标志位相关函数

    /// @brief 获取指定的标志位的状态
    /// @param flag 要获取的标志位
    /// @return 标志位的状态
    bool GetFlagStatus(Configs::Flags flag);
    /// @brief 清除指定的标志位
    /// @param flag 要清除的标志位
    void ClearFlag(Configs::Flags flag);

    //时钟控制

    /// @brief 使能或失能计时器的外设时钟
    /// @param state 需要设置的状态
    /// @note 也可以使用RCC控制时钟,函数等效。
    void PeripheralClockControl(State state);

    //傻瓜配置函数

    /// @brief 你是否因为繁杂的计时器配置而感到抓狂? 这个函数为你量身定做! 你只需要输入时间长度,他就会自动配置好计时器和中断!
    /// @param seconds 计时器触发时间的长度(单位为秒,必须在0x0000到0xFFFF之间)
    /// @note 记得写中断函数哦 如果不写会调到默认中断里死循环的!
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

        /// @brief 通过参数初始化OC通道
        /// @param Pluse CCR寄存器的值(必须在0x0000到0xFFFF之间)
        /// @param Polarity 正向通道输出 true高电平 false低电平
        /// @param NPolarity 反向通道输出(仅高级定时器支持) true高电平 false低电平
        /// @param mode OC的模式
        /// @param isEnabled 是否启用正向通道
        /// @param isNEnabled 是否启用反向通道(仅高级定时器支持)
        /// @param channel 要初始化的OC通道(每个定时器有4个1-4,只能同时选择一个,输入0代表全部通道)
        /// @note 以上配置不一定对所有定时器都适用,请根据实际情况调整参数以避免不可预期的行为
        void Init(uint16_t Pluse, bool Polarity, bool NPolarity, OCMode mode, bool isEnabled, bool isNEnabled, uint8_t channel);
        /// @brief 通过初始化结构体初始化OC通道
        /// @param config 初始化结构体
        /// @param channel 需要初始化的通道(1-4)
        /// @note 在使用高级定时器时,确保config结构体的所有函数均有值,否则可能会导致不可预期的行为
        /// @note 不推荐自建结构体,可使用函数GetDefaultOCInitConfig()获取一个默认配置的结构体,然后修改其中的成员来满足需求
        void Init(InitConfig* config, uint8_t channel);
        /// @brief 启用或禁用CC正向通道
        /// @param channel 要设置的通道(1-4) 
        /// @param isEnabled 是否启用 true启用 false失能
        /// @note 在初始化OC后通道输出默认处于开启状态,如果需要关闭请调用这个函数
        void EnableCCChannelOutput(uint8_t channel, bool isEnabled);
        /// @brief 启用或禁用CC反向通道
        /// @param channel 要设置的通道(1-3)
        /// @param isEnabled 是否启用 true启用 false失能
        /// @note 在初始化OC后通道输出默认处于开启状态,如果需要关闭请调用这个函数
        void EnableCCNChannelOutput(uint8_t channel, bool isEnabled);
        /// @brief 启用高级定时器的PWM输出(通用定时器不需要设置PWM输出功能,默认开启PWM输出)
        /// @param isEnabled 是否启用PWM输出 true启用 false失能
        /// @note 只有TIM1和TIM8需要调用这个函数来启用PWM输出,其他定时器不需要设置PWM输出功能
        /// @note 高级定时器:TIM1和TIM8 | TIM 15 16 17(本库不支持)
        void EnablePWMOutput(bool isEnabled);
        /// @brief 是否启用CCR寄存器的预装载(是否启用CCR的影子寄存器)
        /// @param isEnabled true启用 false失能
        /// @note 对TIM6和TIM7无效 因为基本定时器没有输出比较功能
        /// @note CCR寄存器由OC和IC共享
        void CCRPreloadConfig(uint8_t channel, bool isEnabled);
        /// @brief 设置CCR寄存器的值 从而改变PWM的占空比或输出比较的比较值
        /// @param channel 要设置的通道
        /// @param ccr CCR寄存器的值(必须在0x0000到0xFFFF之间)
        /// @note 对TIM6和TIM7无效 因为基本定时器没有输出比较功能
        /// @note CCR寄存器由OC和IC共享
        void SetCCR(uint8_t channel, uint16_t ccr);
        /// @brief 设置通道的输出极性
        /// @param channel 要设置的通道(1-4)
        /// @param polarity 输出极性 true高电平 false低电平
        void SetOutputPolarity(uint8_t channel, bool isToggled);
        /// @brief 设置反向通道的输出极性
        /// @param channel 要设置的通道(1-3) 反向通道只有1-3有 4没有
        /// @param polarity 反转输出极性 true高电平 false低电平
        void SetNOutputPolarity(uint8_t channel, bool isToggled);
        /// @brief 设置目标通道的输出比较是否启用
        /// @param channel 目标通道
        /// @param isEnabled 是否启用输出比较 true使能 false失能
        void SetOutputState(uint8_t channel, bool isEnabled);
        /// @brief 设置目标通道的反向输出比较是否启用
        /// @param channel 目标通道
        /// @param isEnabled 是否启用输出比较 true使能 false失能
        void SetNOutputState(uint8_t channel, bool isEnabled);
        /// @brief 用于设置输出比较模式的函数
        /// @param channel 要设置的通道
        /// @param mode 目标模式
        void SetOCMode(uint8_t channel, OCMode mode);
        /// @brief 获取一个默认配置的输出比较初始化结构体
        /// @return 默认配置的输出比较初始化结构体
        InitConfig GetDefaultInitConfig();
    } OC;

    /// @brief 输入捕获部分
    /// @note 此部分为动态结构体,需要使用对象调用
    /// @note 在IC触发时,CNT寄存器会被锁存至CCR寄存器中,如需在此时处理事件,你可以打开对应通道的IT或Event
    struct InputCapture
    {
        //硬件地址
        TIM_TypeDef* self;
        //构造函数
        InputCapture(TIM_TypeDef* timDef) : self(timDef) {}

        /// @brief 输入捕获边沿检测的极性
        /// @note 此边沿检测极性在FPx通道交叉之后 也就是来自同组另一个CH的边沿信号可在当前通道进行独立的边沿检测
        typedef enum ICPolarity
        {
            Rising = LL_TIM_IC_POLARITY_RISING, //上升沿捕获
            Falling = LL_TIM_IC_POLARITY_FALLING //下降沿捕获
        } ICPolarity;

        /// @brief 选择接入后方分频器的信号来源,即调整后方分频器前数据选择器
        /// @param Direct - 直接来自于当前通道的输入信号
        /// @param InDirect - 来自于另外一个通道的输入信号
        /// @param TRC - 来自于TRC引脚的信号
        /// @note 一般Ch1和Ch2为一组 Ch3和Ch4为一组 使用FPx是同组第x个后端引脚的边沿检测信号
        /// @note 如Ch1后方配置FP2 则边沿检测信号来自于CH2的输入
        /// @note 由此可实现两个输入比较同时检测一个CH输入 可用于实现PWMI模式
        typedef enum ICActiveInput
        {
            Direct = LL_TIM_ACTIVEINPUT_DIRECTTI, //FP1通道
            InDirect = LL_TIM_ACTIVEINPUT_INDIRECTTI, //FP2通道
            TRC = LL_TIM_ACTIVEINPUT_TRC //触发输入
        } ICActiveInput;

        /// @brief 后端处理的预分频器 此分频器设置会在每次边沿检测信号到达时进行分频
        /// @brief 若不分频 则在每次边沿检测信号到达时都会触发后方输入捕获动作
        /// @note 此分频器在从模式信号输出的后方 设置此分频器并不能对从模式信号输出分频
        /// @note 从模式信号:FP1ED FP1等
        typedef enum ICPrescaler
        {
            Div1 = LL_TIM_ICPSC_DIV1, //不分频
            Div2 = LL_TIM_ICPSC_DIV2, //分频2
            Div4 = LL_TIM_ICPSC_DIV4, //分频4
            Div8 = LL_TIM_ICPSC_DIV8, //分频8
        } ICPrescaler;

        /// @brief 这是边沿检测前的滤波器设置 用于对输入信号进行数字滤波以消除毛刺干扰
        /// @note 参数配置为 FD x N y 其中x表示采样频率的分频系数 其实际采样频率为 计时器频率/x 采样次数为y
        /// @note 表示以Fck/x采样 只有连续y次采样均是高/低电平时才往边沿检测器传递高/低电平信号
        typedef enum ICFilter
        {
            FD1N1 = LL_TIM_IC_FILTER_FDIV1,
            FD1N2 = LL_TIM_IC_FILTER_FDIV1_N2,
            FD1N4 = LL_TIM_IC_FILTER_FDIV1_N4,
            FD1N8 = LL_TIM_IC_FILTER_FDIV1_N8,
            FD2N6 = LL_TIM_IC_FILTER_FDIV2_N6,
            FD2N8 = LL_TIM_IC_FILTER_FDIV2_N8,
            FD4N6 = LL_TIM_IC_FILTER_FDIV4_N6,
            FD4N8 = LL_TIM_IC_FILTER_FDIV4_N8,
            FD8N6 = LL_TIM_IC_FILTER_FDIV8_N6,
            FD8N8 = LL_TIM_IC_FILTER_FDIV8_N8,
            FD16N5 = LL_TIM_IC_FILTER_FDIV16_N5,
            FD16N6 = LL_TIM_IC_FILTER_FDIV16_N6,
            FD16N8 = LL_TIM_IC_FILTER_FDIV16_N8,
            FD32N5 = LL_TIM_IC_FILTER_FDIV32_N5,
            FD32N6 = LL_TIM_IC_FILTER_FDIV32_N6,
            FD32N8 = LL_TIM_IC_FILTER_FDIV32_N8
        } ICFilter;

        /// @brief PWMI模式的分组表 用于选择两个通道的输入捕获信号来源关系
        typedef enum PWMIGroup
        {
            Group1Target1, //CH1和CH2为一组,CH1链接至CH1和CH2输入捕获
            Group1Target2, //CH1和CH2为一组,CH2链接至CH1和CH2输入捕获
            Group2Target1, //CH3和CH4为一组,CH3链接至CH3和CH4输入捕获
            Group2Target2, //CH3和CH4为一组,CH4链接至CH3和CH4输入捕获
        };

        /// 输入捕获配置结构体
        /// @note  ###############|-> FP1ED#####|-> FP1 可用于从模式信号
        /// @note  GPIO -- > 滤波器和边沿检测 --> FP1 --> 预分频器 --> 执行捕获
        /// @note  ######################|-> FP2 --> 另外一个通道的预分频器 --> 执行另外一个通道捕获
        /// @param ICPolarity - 输入捕获边沿选择
        /// @param ICActiveInput - 输入捕获输入选择(直连或交叉另外一个通道)
        /// @param ICPrescaler - 输入捕获预分频器
        /// @param ICFilter - 输入捕获滤波器
        /// @note 边沿检测器同时输出上升沿和下降沿 选择游ICPolarity控制的数据选择器选择 FP1ED来自FP1上升和下降的或
        typedef LL_TIM_IC_InitTypeDef InitConfig;

        /// @brief 获取一个默认配置的输入捕获初始化结构体
        /// @return 默认配置的输入捕获初始化结构体
        LL_TIM_IC_InitTypeDef GetDefaultInitConfig();
        /// @brief 初始化输入捕获
        /// @param config 初始化结构体
        /// @param channel 选择通道(1-4) 0代表全部通道 只能选择一个
        void Init(InitConfig* config, uint8_t channel);
        /// @brief 使用参数初始化输入捕获
        /// @param polarity 边沿检测触发的边沿
        /// @param activeInput 输入捕获输入选择(直连或交叉另外一个通道)
        /// @param prescaler 输入捕获预分频器
        /// @param filter 输入捕获滤波器
        /// @param channel 选择通道(1-4) 0代表全部通道 只能选择一个
        void Init(ICPolarity polarity, ICActiveInput activeInput, ICPrescaler prescaler, ICFilter filter, uint8_t channel);

        /// @brief 便捷配置PWMI捕获 将会同时占用两个捕获通道
        /// @param polarity 同一分组内前一个通道的边沿检测触发极性，后一个通道与其相反
        /// @param prescaler 两个通道的后端分频器
        /// @param filter 捕获通道的边沿检测前滤波器(在此配置模式下仅目标捕获通道的滤波器有效)
        /// @param group PWMI捕获通道分组 详见Tim::InputCapture::PWMIGroup枚举
        void InitPWMI(ICPolarity polarity, ICPrescaler prescaler, ICFilter filter, PWMIGroup group);

        /// @brief 设置输入捕获边沿
        /// @param channel 选择通道(1-4) 0代表全部通道 只能选择一个
        /// @param polarity 边沿检测触发的边沿
        void SetICPolarity(uint8_t channel, ICPolarity polarity);
        /// @brief 设置输入捕获输入选择
        /// @param channel 选择通道(1-4) 0代表全部通道 只能选择一个
        /// @param activeInput 输入捕获输入选择(直连或交叉另外一个通道)
        void SetICActiveInput(uint8_t channel, ICActiveInput activeInput);
        /// @brief 设置输入捕获预分频器
        /// @param channel 选择通道(1-4) 0代表全部通道 只能选择一个
        /// @param prescaler 输入捕获预分频器
        void SetICPrescaler(uint8_t channel, ICPrescaler prescaler);
        /// @brief 设置边沿检测前滤波器的模式
        /// @param channel 选择通道(1-4) 0代表全部通道 只能选择一个
        /// @param filter 输入捕获滤波器
        void SetICFilter(uint8_t channel, ICFilter filter);
        /// @brief 读取指定通道的CCR值
        /// @param channel 要读取的通道
        /// @return 返回的CCR寄存器的值 最大0xFFFF
        uint32_t GetCaptureValue(uint8_t channel);
    } IC;

};

/*
定时器对象表
*/
extern Tim tim1; //APB2 高级定时器
extern Tim tim2; //APB1 通用定时器
extern Tim tim3; //APB1 通用定时器
extern Tim tim4; //APB1 通用定时器
#if defined(TIM5)
extern Tim tim5; //APB1 通用定时器
#endif
#if defined(TIM6)
extern Tim tim6; //APB1 基本定时器
#endif
#if defined(TIM7)
extern Tim tim7; //APB1 基本定时器
#endif
#if defined(TIM8)
extern Tim tim8; //APB2 高级定时器
#endif

#endif

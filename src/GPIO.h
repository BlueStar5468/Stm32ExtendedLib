/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f10x_gpio.h"

/// @brief GPIO对象
///该类提供了对stm32f10x GPIO的操作接口
/// @note GPIO简介
/// @note 挂载总线:APB2 ; 片上资源:GPIOA,GPIOB
/// @note 若要配置GPIO中断 需要先配置GPIO的EXTI链接(通往AFIO) 再配置AFIO链接至EXTI 配置EXTI 最后在NVIC中使能对应的中断通道
/// @note 链接图类似:
/// @note GPIOA --16-> | AFIO | --16-> EXTI --Nvic::Channel-> NVIC
/// @note GPIOB --16-> | AFIO |
class Gpio
{
public:
    struct Config
    {
        //输入引脚模式
        struct In
        {
            typedef enum Mode
            {
                Floating = GPIO_Mode_IN_FLOATING,   //浮空输入
                PullUp = GPIO_Mode_IPU,             //上拉输入
                PullDown = GPIO_Mode_IPD,           //下拉输入
                Analog = GPIO_Mode_AIN,             //模拟输入
            } Mode;
        };

        //输出引脚模式
        struct Out
        {
            typedef enum Mode
            {
                PushPull = GPIO_Mode_Out_PP,        //推挽输出
                OpenDrain = GPIO_Mode_Out_OD,       //开漏输出(低电平强驱动)
                AF_PushPull = GPIO_Mode_AF_PP,      //复用推挽输出
                AF_OpenDrain = GPIO_Mode_AF_OD,     //复用开漏输出
            } Mode;
        };

        //引脚速度配置
        typedef enum Speed
        {
            _2M = GPIO_Speed_2MHz,   //2MHz速率
            _10M = GPIO_Speed_10MHz, //10MHz速率
            _50M = GPIO_Speed_50MHz, //50MHz速率
        } Speed;

        //引脚定义
        typedef enum Pins
        {
            Pin0 = GPIO_Pin_0,   //引脚0
            Pin1 = GPIO_Pin_1,   //引脚1
            Pin2 = GPIO_Pin_2,   //引脚2
            Pin3 = GPIO_Pin_3,   //引脚3
            Pin4 = GPIO_Pin_4,   //引脚4
            Pin5 = GPIO_Pin_5,   //引脚5
            Pin6 = GPIO_Pin_6,   //引脚6
            Pin7 = GPIO_Pin_7,   //引脚7
            Pin8 = GPIO_Pin_8,   //引脚8
            Pin9 = GPIO_Pin_9,   //引脚9
            Pin10 = GPIO_Pin_10, //引脚10
            Pin11 = GPIO_Pin_11, //引脚11
            Pin12 = GPIO_Pin_12, //引脚12
            Pin13 = GPIO_Pin_13, //引脚13
            Pin14 = GPIO_Pin_14, //引脚14
            Pin15 = GPIO_Pin_15, //引脚15
            ALL = GPIO_Pin_All,   //所有引脚
        } Pins;

        //AFIO线路定义
        // 链接图 --GPIOLine--> AFIO --EXTILine --> EXTI
        typedef enum AFIOLine
        {
            Line0 = GPIO_PinSource0, //0号外部中断线
            Line1 = GPIO_PinSource1, //1号外部中断线
            Line2 = GPIO_PinSource2, //2号外部中断线
            Line3 = GPIO_PinSource3, //3号外部中断线
            Line4 = GPIO_PinSource4, //4号外部中断线
            Line5 = GPIO_PinSource5, //5号外部中断线
            Line6 = GPIO_PinSource6, //6号外部中断线
            Line7 = GPIO_PinSource7, //7号外部中断线
            Line8 = GPIO_PinSource8, //8号外部中断线
            Line9 = GPIO_PinSource9, //9号外部中断线
            Line10 = GPIO_PinSource10, //10号外部中断线
            Line11 = GPIO_PinSource11, //11号外部中断线
            Line12 = GPIO_PinSource12, //12号外部中断线
            Line13 = GPIO_PinSource13, //13号外部中断线
            Line14 = GPIO_PinSource14, //14号外部中断线
            Line15 = GPIO_PinSource15, //15号外部中断线
        } AFIOLine;
    };

    //设备硬件地址
    GPIO_TypeDef* self;
    //构造函数
    Gpio(GPIO_TypeDef* gpioDef) : self(gpioDef) {}
    /// @brief 初始化结构体参数
    /// @param GPIO_Pin - 需要配置的GPIO引脚, 详见Config::Pins (可以使用按位或"|"组合多个引脚)
    /// @param GPIO_Speed - GPIO引脚的速率, 详见Config::Speed
    /// @param GPIO_Mode - GPIO引脚的工作模式, 详见Config::In::Mode和Config::Out::Mode
    typedef GPIO_InitTypeDef InitConfig;

    /// @brief 初始化GPIO引脚(结构体参数模式)
    /// @param config - GPIO初始化结构体指针, 具体参考Gpio::InitConfig
    void Init(InitConfig* config);
    /// @brief 直接使用参数配置GPIO引脚
    /// @param pins 需要配置的GPIO引脚, 详见Config::Pins (可以使用按位或"|"组合多个引脚)
    /// @param GPIO_Speed GPIO引脚的速率, 详见Config::Speed
    /// @param GPIO_Mode GPIO引脚的工作模式, 详见Config::In::Mode和Config::Out::Mode
    void Init(Config::Pins pins, Config::Speed GPIO_Speed, uint32_t GPIO_Mode);
    /// @brief 复位整个GPIO至缺省状态(所有引脚复位为输入模式)
    void DeInit();
    /// @brief 控制GPIO输出状态
    /// @param pins 需要控制的GPIO引脚, 详见Config::Pins (可以使用按位或"|"组合多个引脚)
    /// @param state 输出状态 true为高电平 false为低电平
    /// @note 若引脚配置为输入状态,此函数操作无效
    void OutPutControl(Config::Pins pins, bool state);

    //寄存器操作

    /// @brief 获取GPIO单个引脚的输入
    /// @param pin 要获取的引脚,不可多选
    /// @return 引脚状态
    uint8_t GetInputDataBit(Config::Pins pin);
    /// @brief 获取GPIO单个引脚的理论输出(即ODR寄存器的值,不考虑引脚实际配置和外部电路)
    /// @param pin 要获取的引脚,不可多选
    /// @return 引脚理论状态
    uint8_t GetOutputDataBit(Config::Pins pin);
    /// @brief 获取所有引脚的输入状态
    /// @return 输入寄存器的值
    uint16_t GetInputData();
    /// @brief 获取所有引脚的理论输出状态(不考虑实际外部电路和引脚配置)
    /// @return 输出寄存器的值
    uint16_t GetOutputData();

    //时钟控制

    /// @brief 控制是否启用GPIO的外设时钟(也可在RCC中控制时钟,函数等效)
    /// @param isEnabled 是否启用时钟
    void PeripheralClockControl(bool isEnabled);

    //中断配置
    struct Afio
    {
        /// @brief 恢复目标gpio的AFIO线路链接至默认状态(即不链接)
        /// @note 由于AFIO只有一个 此操作会重置所有GPIO的AFIO链接状态
        static void Deinit();
        /// @brief 将目标gpio的指定引脚链接至AFIO的指定线路
        /// @param gpio 目标gpio
        /// @param line AFIO线路, 详见Config::AFIOLine
        /// @note 连接只能将同样编号的线连在一起,比如PA0只能连到Line0(line=Config::AFIOLine::Line0) PA1只能连到Line1(line=Config::AFIOLine::Line1) 
        static void Connect(Gpio gpio, Config::AFIOLine line);
        /// @brief AFIO时钟控制
        /// @param isEnabled 是否使能AFIO时钟
        static void ClockControl(bool isEnabled);
    };

    //快速中断操作

    /// @brief 厌倦了一遍又一遍配置GPIO中断了吗?这个函数为你量身打造! 只需一步调用即可将所选引脚连接至一个优先级为(0,0)中断!
    /// @param pin 需要配置的引脚,详见Config::Pins(不可多选)
    /// @param isEnabled 是否使能该引脚的中断(默认为true)
    /// @note 记得实现中断处理函数!
    /// @note 函数会自动连接AFIO线路和EXTI线路并配置EXTI为上升沿触发的中断模式, 最后在NVIC中使能对应的中断通道(优先级为0, 0)
    void FastITControl(Config::Pins pin, bool isEnabled);

};

//GPIO对象实例化
extern Gpio gpioA; //GPIOA端口
extern Gpio gpioB; //GPIOB端口

#endif

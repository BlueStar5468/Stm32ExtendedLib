/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#ifndef __EXTI_H__
#define __EXTI_H__

#include "stm32f10x_exti.h"

/// @brief 边缘中断控制器
/// 该类提供了对stm32f10x EXTI的操作接口
class Exti
{
public:
    struct Config
    {
        /// @brief EXTI触发方式
        typedef enum Trigger
        {
            Rising = EXTI_Trigger_Rising, //上升沿触发
            Falling = EXTI_Trigger_Falling, //下降沿触发
            Both = EXTI_Trigger_Rising_Falling, //双边沿触发

        } Trigger;

        /// @brief EXTI工作模式
        typedef enum Mode
        {
            Interrupt = EXTI_Mode_Interrupt, //中断模式
            Event = EXTI_Mode_Event, //事件模式
        } Mode;

        /// @brief EXTI线定义
        /// @note EXTI0-15对应AFIO线路0-15,剩余引脚请参考芯片手册
        typedef enum EXTILine
        {
            Line0 = EXTI_Line0,     //外部中断线0
            Line1 = EXTI_Line1,     //外部中断线1
            Line2 = EXTI_Line2,     //外部中断线2
            Line3 = EXTI_Line3,     //外部中断线3
            Line4 = EXTI_Line4,     //外部中断线4
            Line5 = EXTI_Line5,     //外部中断线5
            Line6 = EXTI_Line6,     //外部中断线6
            Line7 = EXTI_Line7,     //外部中断线7
            Line8 = EXTI_Line8,     //外部中断线8
            Line9 = EXTI_Line9,     //外部中断线9
            Line10 = EXTI_Line10,    //外部中断线10
            Line11 = EXTI_Line11,    //外部中断线11
            Line12 = EXTI_Line12,    //外部中断线12
            Line13 = EXTI_Line13,    //外部中断线13
            Line14 = EXTI_Line14,    //外部中断线14
            Line15 = EXTI_Line15,    //外部中断线15

            Line16 = EXTI_Line16,    //RTC Alarm事件
            Line17 = EXTI_Line17,    //USB OTG FS Wakeup事件
            Line18 = EXTI_Line18,    //Ethernet Wakeup事件
            Line19 = EXTI_Line19,    //USB OTG HS Wakeup事件
        } EXTILine;
    };

    //设备硬件地址
    EXTI_TypeDef* self;
    //构造函数
    Exti(EXTI_TypeDef* extiDef) : self(extiDef) {}
    /// @brief EXTI初始化结构体
    /// @param EXTI_Line - 需要配置的EXTI线路,详见Config::EXTILine (可以使用按位或"|"组合多个线路)
    /// @param EXTI_Mode - EXTI的工作模式,详见Config::Mode
    /// @param EXTI_Trigger - EXTI的触发方式,详见Config::Trigger
    /// @param EXTI_LineCmd - 是否使能该EXTI线路 ENABLE或DISABLE
    typedef EXTI_InitTypeDef InitConfig;

    /// @brief 获取一个默认配置的EXTI初始化结构体
    /// @return 默认的EXTI初始化结构体
    EXTI_InitTypeDef GetDefaultInitConfig();
    /// @brief 恢复EXTI的缺省配置
    void DeInit();
    /// @brief 使用参数初始化EXTI线
    /// @param line 要设置的EXTI线路,详见Config::EXTILine
    /// @param mode 要设置的EXTI工作模式,详见Config::Mode
    /// @param trigger 要设置的EXTI触发方式,详见Config::Trigger
    void Init(Config::EXTILine line, Config::Mode mode, Config::Trigger trigger, bool isEnabled = true);
    /// @brief 使用初始化结构体指针初始化EXTI线
    /// @param config 初始化结构体指针
    void Init(InitConfig* config);
    /// @brief 手动触发EXTI线的中断(仅当该线配置为中断模式且已使能时有效)
    /// @param line 要在哪个EXTI线上触发中断,详见Config::EXTILine
    void GenerateIT(Config::EXTILine line);
    /// @brief 获取EXTI线的中断状态
    /// @param line 要查询的EXTI线路,详见Config::EXTILine
    /// @return 中断状态
    bool GetITStatus(Config::EXTILine line);
    /// @brief 获取EXTI线的中断标志状态
    /// @param line 要查询的EXTI线路,详见Config::EXTILine
    /// @return 标志状态
    bool GetFlagStatus(Config::EXTILine line);
    /// @brief 清除EXTI线的中断状态
    /// @param line 要清除的EXTI线路,详见Config::EXTILine
    void ClearITStatus(Config::EXTILine line);
    /// @brief 清除EXTI线的中断标志状态
    /// @param line 要清除的EXTI线路,详见Config::EXTILine
    void ClearFlagStatus(Config::EXTILine line);
};

//全局EXTI对象
extern Exti exti;

#endif

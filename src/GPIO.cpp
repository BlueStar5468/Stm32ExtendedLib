/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#include "stm32f10x_gpio.h"
#include "GPIO.h"
#include "EXTI.h"
#include "NVIC.h"

void Gpio::Init(Gpio::InitConfig* config)
{
    GPIO_Init(self, config);
}

void Gpio::Init(Config::Pins pins, Config::Speed GPIO_Speed, uint32_t GPIO_Mode)
{
    InitConfig config;
    config.GPIO_Pin = static_cast<uint16_t>(pins);
    config.GPIO_Speed = static_cast<GPIOSpeed_TypeDef>(GPIO_Speed);
    config.GPIO_Mode = static_cast<GPIOMode_TypeDef>(GPIO_Mode);
    GPIO_Init(self, &config);
}

void Gpio::DeInit()
{
    GPIO_DeInit(self);
}

void Gpio::OutPutControl(Config::Pins pins, bool state)
{
    if (state)
    {
        GPIO_SetBits(self, static_cast<uint16_t>(pins));
    }
    else
    {
        GPIO_ResetBits(self, static_cast<uint16_t>(pins));
    }
}

uint8_t Gpio::GetInputDataBit(Config::Pins pin)
{
    return GPIO_ReadInputDataBit(self, static_cast<uint16_t>(pin));
}

uint8_t Gpio::GetOutputDataBit(Config::Pins pin)
{
    return GPIO_ReadOutputDataBit(self, static_cast<uint16_t>(pin));
}

uint16_t Gpio::GetInputData()
{
    return GPIO_ReadInputData(self);
}

uint16_t Gpio::GetOutputData()
{
    return GPIO_ReadOutputData(self);
}

void Gpio::PeripheralClockControl(bool isEnabled)
{
    //根据GPIO端口地址判断是哪个GPIO
    if (self == GPIOA)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, isEnabled ? ENABLE : DISABLE);
    }
    else if (self == GPIOB)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, isEnabled ? ENABLE : DISABLE);
    }
}

void Gpio::Afio::Deinit()
{
    GPIO_AFIODeInit();
}

void Gpio::Afio::Connect(Gpio gpio, Config::AFIOLine line)
{
    uint8_t source;
    if (gpio.self == GPIOA)
    {
        source = GPIO_PortSourceGPIOA;
    }
    else if (gpio.self == GPIOB)
    {
        source = GPIO_PortSourceGPIOB;
    }
    else
    {
        return; //无效GPIO
    }
    GPIO_EXTILineConfig(source, static_cast<uint8_t>(line));
}

void Gpio::Afio::ClockControl(bool isEnabled)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, isEnabled ? ENABLE : DISABLE);

};

void Gpio::FastITControl(Config::Pins pin, bool isEnabled)
{
    //使能时钟(EXTI和NVIC时钟默认开启)
    Afio::ClockControl(true);

    //连接AFIO线路
    Gpio::Config::AFIOLine line;
    switch (pin)
    {
    case Gpio::Config::Pins::Pin0: line = Gpio::Config::AFIOLine::Line0; break;
    case Gpio::Config::Pins::Pin1: line = Gpio::Config::AFIOLine::Line1; break;
    case Gpio::Config::Pins::Pin2: line = Gpio::Config::AFIOLine::Line2; break;
    case Gpio::Config::Pins::Pin3: line = Gpio::Config::AFIOLine::Line3; break;
    case Gpio::Config::Pins::Pin4: line = Gpio::Config::AFIOLine::Line4; break;
    case Gpio::Config::Pins::Pin5: line = Gpio::Config::AFIOLine::Line5; break;
    case Gpio::Config::Pins::Pin6: line = Gpio::Config::AFIOLine::Line6; break;
    case Gpio::Config::Pins::Pin7: line = Gpio::Config::AFIOLine::Line7; break;
    case Gpio::Config::Pins::Pin8: line = Gpio::Config::AFIOLine::Line8; break;
    case Gpio::Config::Pins::Pin9: line = Gpio::Config::AFIOLine::Line9; break;
    case Gpio::Config::Pins::Pin10: line = Gpio::Config::AFIOLine::Line10; break;
    case Gpio::Config::Pins::Pin11: line = Gpio::Config::AFIOLine::Line11; break;
    case Gpio::Config::Pins::Pin12: line = Gpio::Config::AFIOLine::Line12; break;
    case Gpio::Config::Pins::Pin13: line = Gpio::Config::AFIOLine::Line13; break;
    case Gpio::Config::Pins::Pin14: line = Gpio::Config::AFIOLine::Line14; break;
    case Gpio::Config::Pins::Pin15: line = Gpio::Config::AFIOLine::Line15; break;
    default: return; //无效引脚
    }
    Gpio::Afio::Connect(*this, line);
    //配置EXTI
    Exti::Config::EXTILine extiLine;
    switch (pin)
    {
    case Gpio::Config::Pins::Pin0: extiLine = Exti::Config::EXTILine::Line0; break;
    case Gpio::Config::Pins::Pin1: extiLine = Exti::Config::EXTILine::Line1; break;
    case Gpio::Config::Pins::Pin2: extiLine = Exti::Config::EXTILine::Line2; break;
    case Gpio::Config::Pins::Pin3: extiLine = Exti::Config::EXTILine::Line3; break;
    case Gpio::Config::Pins::Pin4: extiLine = Exti::Config::EXTILine::Line4; break;
    case Gpio::Config::Pins::Pin5: extiLine = Exti::Config::EXTILine::Line5; break;
    case Gpio::Config::Pins::Pin6: extiLine = Exti::Config::EXTILine::Line6; break;
    case Gpio::Config::Pins::Pin7: extiLine = Exti::Config::EXTILine::Line7; break;
    case Gpio::Config::Pins::Pin8: extiLine = Exti::Config::EXTILine::Line8; break;
    case Gpio::Config::Pins::Pin9: extiLine = Exti::Config::EXTILine::Line9; break;
    case Gpio::Config::Pins::Pin10: extiLine = Exti::Config::EXTILine::Line10; break;
    case Gpio::Config::Pins::Pin11: extiLine = Exti::Config::EXTILine::Line11; break;
    case Gpio::Config::Pins::Pin12: extiLine = Exti::Config::EXTILine::Line12; break;
    case Gpio::Config::Pins::Pin13: extiLine = Exti::Config::EXTILine::Line13; break;
    case Gpio::Config::Pins::Pin14: extiLine = Exti::Config::EXTILine::Line14; break;
    case Gpio::Config::Pins::Pin15: extiLine = Exti::Config::EXTILine::Line15; break;
    default: return; //无效引脚
    }
    exti.Init(extiLine, Exti::Config::Mode::Interrupt, Exti::Config::Trigger::Rising, isEnabled);
    //配置NVIC
    uint8_t irqChannel;
    if (pin < Gpio::Config::Pins::Pin5)
    {
        switch (pin)
        {
        case Gpio::Config::Pins::Pin0: irqChannel = EXTI0_IRQn; break;
        case Gpio::Config::Pins::Pin1: irqChannel = EXTI1_IRQn; break;
        case Gpio::Config::Pins::Pin2: irqChannel = EXTI2_IRQn; break;
        case Gpio::Config::Pins::Pin3: irqChannel = EXTI3_IRQn; break;
        case Gpio::Config::Pins::Pin4: irqChannel = EXTI4_IRQn; break;
        default: return; //无效引脚
        }
    }
    else if (pin < Gpio::Config::Pins::Pin10)
    {
        irqChannel = EXTI9_5_IRQn;
    }
    else
    {
        irqChannel = EXTI15_10_IRQn;
    }
    nvic.ChannelControl(irqChannel, 0, 0, isEnabled);
}

void Gpio::Afio::RemapPin(Config::RemapPin remapPin, bool isEnabled)
{
    GPIO_PinRemapConfig(static_cast<uint32_t>(remapPin), isEnabled ? ENABLE : DISABLE);
}


//对象实例化
Gpio gpioA(GPIOA);
Gpio gpioB(GPIOB);

/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "GPIO.h"
#include "EXTI.h"
#include "NVIC.h"

void Gpio::Init(Gpio::InitConfig* config)
{
    LL_GPIO_InitTypeDef gpioConfig;
    gpioConfig.Pin = config->Pin;
    gpioConfig.Speed = config->Speed;
    switch (config->Mode)
    {
    case Config::In::Mode::Floating:
        gpioConfig.Mode = LL_GPIO_MODE_FLOATING;
        break;
    case Config::In::Mode::PullUp:
        gpioConfig.Mode = LL_GPIO_MODE_INPUT;
        gpioConfig.Pull = LL_GPIO_PULL_UP;
        break;
    case Config::In::Mode::PullDown:
        gpioConfig.Mode = LL_GPIO_MODE_INPUT;
        gpioConfig.Pull = LL_GPIO_PULL_DOWN;
        break;
    case Config::In::Mode::Analog:
        gpioConfig.Mode = LL_GPIO_MODE_ANALOG;
        break;
    case Config::Out::Mode::PushPull:
        gpioConfig.Mode = LL_GPIO_MODE_OUTPUT;
        gpioConfig.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        break;
    case Config::Out::Mode::OpenDrain:
        gpioConfig.Mode = LL_GPIO_MODE_OUTPUT;
        gpioConfig.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
        break;
    case Config::Out::Mode::AltPushPull:
        gpioConfig.Mode = LL_GPIO_MODE_ALTERNATE;
        gpioConfig.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        break;
    case Config::Out::Mode::AltOpenDrain:
        gpioConfig.Mode = LL_GPIO_MODE_ALTERNATE;
        gpioConfig.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
        break;
    }
    LL_GPIO_Init(self, &gpioConfig);
}

void Gpio::Init(Config::Pins pins, Config::Speed GPIO_Speed, uint8_t GPIO_Mode)
{
    InitConfig config;
    config.Pin = pins;
    config.Speed = GPIO_Speed;
    config.Mode = GPIO_Mode;
    this->Init(&config);
}

void Gpio::DeInit()
{
    LL_GPIO_DeInit(self);
}

void Gpio::OutPutControl(Config::Pins pins, bool state)
{
    if (state)
    {
        LL_GPIO_SetOutputPin(self, pins);
    }
    else
    {
        LL_GPIO_ResetOutputPin(self, pins);
    }
}

bool Gpio::GetInputDataBit(Config::Pins pin)
{
    return LL_GPIO_IsInputPinSet(self, pin);
}

bool Gpio::GetOutputDataBit(Config::Pins pin)
{
    return LL_GPIO_IsOutputPinSet(self, pin);
}

uint16_t Gpio::GetInputData()
{
    return LL_GPIO_ReadInputPort(self);
}

uint16_t Gpio::GetOutputData()
{
    return LL_GPIO_ReadOutputPort(self);
}

void Gpio::PeripheralClockControl(bool isEnabled)
{
    if (self == GPIOA)
    {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    }
    else if (self == GPIOB)
    {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    }
    else if (self == GPIOC)
    {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    }
    else if (self == GPIOD)
    {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
    }
    else if (self == GPIOE)
    {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOE);
    }
}

void Gpio::Afio::Connect(Gpio gpio, Config::AFIOLine line)
{
    uint8_t source;
    if (gpio.self == GPIOA)
    {
        source = LL_GPIO_AF_EXTI_PORTA;
    }
    else if (gpio.self == GPIOB)
    {
        source = LL_GPIO_AF_EXTI_PORTB;
    }
    else if (gpio.self == GPIOC)
    {
        source = LL_GPIO_AF_EXTI_PORTC;
    }
    else if (gpio.self == GPIOD)
    {
        source = LL_GPIO_AF_EXTI_PORTD;
    }
    else if (gpio.self == GPIOE)
    {
        source = LL_GPIO_AF_EXTI_PORTE;
    }
    else return; //无效GPIO
    LL_GPIO_AF_SetEXTISource(source, static_cast<uint8_t>(line));
}

void Gpio::Afio::ClockControl(bool isEnabled)
{
    if (isEnabled)
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
    else
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_AFIO);
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
    IRQn_Type irqChannel;
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
    if (isEnabled)
    {
        switch (remapPin)
        {
        case Config::RemapPin::CAN1_1:
            LL_GPIO_AF_RemapPartial1_CAN1();
            break;
        case Config::RemapPin::CAN1_2:
            LL_GPIO_AF_RemapPartial2_CAN1();
            break;
        case Config::RemapPin::I2C1_Partial:
            LL_GPIO_AF_EnableRemap_I2C1();
            break;
        case Config::RemapPin::SWJ_ALLDisable:
            LL_GPIO_AF_DisableRemap_SWJ();
            break;
        case Config::RemapPin::SWJ_JTAGDisable:
            LL_GPIO_AF_Remap_SWJ_NOJTAG();
            break;
        case Config::RemapPin::TIM1_Full:
            LL_GPIO_AF_EnableRemap_TIM1();
            break;
        case Config::RemapPin::TIM1_Partial:
            LL_GPIO_AF_RemapPartial_TIM1();
            break;
        case Config::RemapPin::TIM2_Full:
            LL_GPIO_AF_EnableRemap_TIM2();
            break;
        case Config::RemapPin::TIM2_Partial_1:
            LL_GPIO_AF_RemapPartial1_TIM2();
            break;
        case Config::RemapPin::TIM2_Partial_2:
            LL_GPIO_AF_RemapPartial2_TIM2();
            break;
        case Config::RemapPin::TIM3_Full:
            LL_GPIO_AF_EnableRemap_TIM3();
            break;
        case Config::RemapPin::TIM3_Partial:
            LL_GPIO_AF_RemapPartial_TIM3();
            break;
        case Config::RemapPin::TIM4_Full:
            LL_GPIO_AF_EnableRemap_TIM4();
            break;
        case Config::RemapPin::USART1_Partial:
            LL_GPIO_AF_EnableRemap_USART1();
            break;
        case Config::RemapPin::USART2_Partial:
            LL_GPIO_AF_EnableRemap_USART2();
            break;
        case Config::RemapPin::USART3_Partial:
            LL_GPIO_AF_RemapPartial_USART3();
            break;
        case Config::RemapPin::USART3_Full:
            LL_GPIO_AF_EnableRemap_USART3();
            break;
        default: return; //无效重映射选项
        }
    }
    else
    {
        switch (remapPin)
        {
        case Config::RemapPin::CAN1_1:
            LL_GPIO_AF_RemapPartial1_CAN1();
            break;
        case Config::RemapPin::CAN1_2:
            LL_GPIO_AF_RemapPartial2_CAN1();
            break;
        case Config::RemapPin::I2C1_Partial:
            LL_GPIO_AF_DisableRemap_I2C1();
            break;
        case Config::RemapPin::SWJ_ALLDisable:
        case Config::RemapPin::SWJ_JTAGDisable:
            LL_GPIO_AF_EnableRemap_SWJ();
            break;
        case Config::RemapPin::TIM1_Full:
            LL_GPIO_AF_DisableRemap_TIM1();
            break;
        case Config::RemapPin::TIM1_Partial:
            LL_GPIO_AF_DisableRemap_TIM1();
            break;
        case Config::RemapPin::TIM2_Full:
            LL_GPIO_AF_DisableRemap_TIM2();
            break;
        case Config::RemapPin::TIM2_Partial_1:
            LL_GPIO_AF_DisableRemap_TIM2();
            break;
        case Config::RemapPin::TIM2_Partial_2:
            LL_GPIO_AF_DisableRemap_TIM2();
            break;
        case Config::RemapPin::TIM3_Full:
            LL_GPIO_AF_DisableRemap_TIM3();
            break;
        case Config::RemapPin::TIM3_Partial:
            LL_GPIO_AF_DisableRemap_TIM3();
            break;
        case Config::RemapPin::TIM4_Full:
            LL_GPIO_AF_DisableRemap_TIM4();
            break;
        case Config::RemapPin::USART1_Partial:
            LL_GPIO_AF_DisableRemap_USART1();
            break;
        case Config::RemapPin::USART2_Partial:
            LL_GPIO_AF_DisableRemap_USART2();
            break;
        case Config::RemapPin::USART3_Partial:
            LL_GPIO_AF_DisableRemap_USART3();
            break;
        case Config::RemapPin::USART3_Full:
            LL_GPIO_AF_DisableRemap_USART3();
            break;
        default: return; //无效重映射选项
        }
    }
}


//对象实例化
Gpio gpioA(GPIOA);
Gpio gpioB(GPIOB);
Gpio gpioC(GPIOC);
Gpio gpioD(GPIOD);
Gpio gpioE(GPIOE);

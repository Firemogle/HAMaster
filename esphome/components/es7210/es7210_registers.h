#pragma once

namespace esphome {
namespace es7210 {

static const uint8_t ES7210_REG00_RESET = 0x00;
static const uint8_t ES7210_REG01_CLOCK_OFF = 0x01;
static const uint8_t ES7210_REG02_MAIN_CLK = 0x02;
static const uint8_t ES7210_REG03_MASTER_CLK = 0x03;
static const uint8_t ES7210_REG04_LRCK_DIVH = 0x04;
static const uint8_t ES7210_REG05_LRCK_DIVL = 0x05;
static const uint8_t ES7210_REG06_PWR_DOWN = 0x06;
static const uint8_t ES7210_REG07_OSR = 0x07;
static const uint8_t ES7210_REG08_MODE_CONFIG = 0x08;
static const uint8_t ES7210_REG09_TIME_CONTROL0 = 0x09;
static const uint8_t ES7210_REG0A_TIME_CONTROL1 = 0x0A;
static const uint8_t ES7210_REG0B_CHIP_STATUS = 0x0B;
static const uint8_t ES7210_REG0C_INTR_CNTRL = 0x0C;
static const uint8_t ES7210_REG0D_MISC_CNTRL = 0x0D;
static const uint8_t ES7210_REG10_DMIC_CNTRL = 0x10;
static const uint8_t ES7210_REG11_SDP_INTERFACE1 = 0x11;
static const uint8_t ES7210_REG12_SDP_INTERFACE2 = 0x12;
static const uint8_t ES7210_REG13_ADC_AUTOMUTE = 0x13;
static const uint8_t ES7210_REG14_ADC34_MUTE = 0x14;
static const uint8_t ES7210_REG15_ADC12_MUTE = 0x15;
static const uint8_t ES7210_REG16_ALC_SEL = 0x16;
static const uint8_t ES7210_REG17_ALC_COMMON1 = 0x17;
static const uint8_t ES7210_REG18_ADC34_ALC_LVL = 0x18;
static const uint8_t ES7210_REG19_ADC12_ALC_LVL = 0x19;
static const uint8_t ES7210_REG1A_ALC_COMMON_2 = 0x1A;
static const uint8_t ES7210_REG1B_ADC4_MAX_GAIN = 0x1B;
static const uint8_t ES7210_REG1C_ADC3_MAX_GAIN = 0x1C;
static const uint8_t ES7210_REG1D_ADC2_MAX_GAIN = 0x1D;
static const uint8_t ES7210_REG1E_ADC1_MAX_GAIN = 0x1E;
static const uint8_t ES7210_REG20_ADC34_HPF2 = 0x20;
static const uint8_t ES7210_REG21_ADC34_HPF1 = 0x21;
static const uint8_t ES7210_REG22_ADC12_HPF1 = 0x22;
static const uint8_t ES7210_REG23_ADC12_HPF2 = 0x23;
static const uint8_t ES7210_REG3D_CHIP_ID1 = 0x3D;
static const uint8_t ES7210_REG3E_CHIP_ID0 = 0x3E;
static const uint8_t ES7210_REG3F_CHIP_VERSION = 0x3F;
static const uint8_t ES7210_REG40_ANALOG = 0x40;
static const uint8_t ES7210_REG41_MIC12_BIAS = 0x41;
static const uint8_t ES7210_REG42_MIC34_BIAS = 0x42;
static const uint8_t ES7210_REG43_MIC1_GAIN = 0x43;
static const uint8_t ES7210_REG44_MIC2_GAIN = 0x44;
static const uint8_t ES7210_REG45_MIC3_GAIN = 0x45;
static const uint8_t ES7210_REG46_MIC4_GAIN = 0x46;
static const uint8_t ES7210_REG47_MIC1_LOW_PWR = 0x47;
static const uint8_t ES7210_REG48_MIC2_LOW_PWR = 0x48;
static const uint8_t ES7210_REG49_MIC3_LOW_PWR = 0x49;
static const uint8_t ES7210_REG4A_MIC4_LOW_PWR = 0x4A;
static const uint8_t ES7210_REG4B_MIC12_PWR_DOWN = 0x4B;
static const uint8_t ES7210_REG4C_MIC34_PWR_DOWN = 0x4C;

}  // namespace es7210
}  // namespace esphome

#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace es7210 {

// I2S interface format for ES7210
enum ES7210Format {
  ES7210_FORMAT_I2S = 0x00,
  ES7210_FORMAT_LJ = 0x01,
  ES7210_FORMAT_DSP_A = 0x03,
  ES7210_FORMAT_DSP_B = 0x13
};

// I2S bit width for ES7210
enum ES7210Resolution {
  ES7210_RESOLUTION_16 = 16,
  ES7210_RESOLUTION_18 = 18,
  ES7210_RESOLUTION_20 = 20,
  ES7210_RESOLUTION_24 = 24,
  ES7210_RESOLUTION_32 = 32
};

// Microphone gain for ES7210
enum ES7210MicGain {
  ES7210_MIC_GAIN_0DB = 0,
  ES7210_MIC_GAIN_3DB = 1,
  ES7210_MIC_GAIN_6DB = 2,
  ES7210_MIC_GAIN_9DB = 3,
  ES7210_MIC_GAIN_12DB = 4,
  ES7210_MIC_GAIN_15DB = 5,
  ES7210_MIC_GAIN_18DB = 6,
  ES7210_MIC_GAIN_21DB = 7,
  ES7210_MIC_GAIN_24DB = 8,
  ES7210_MIC_GAIN_27DB = 9,
  ES7210_MIC_GAIN_30DB = 10,
  ES7210_MIC_GAIN_33DB = 11,
  ES7210_MIC_GAIN_34_5DB = 12,
  ES7210_MIC_GAIN_36DB = 13,
  ES7210_MIC_GAIN_37_5DB = 14
};

// Microphone bias for ES7210
enum ES7210MicBias {
  ES7210_MIC_BIAS_2V18 = 0x00,
  ES7210_MIC_BIAS_2V26 = 0x10,
  ES7210_MIC_BIAS_2V36 = 0x20,
  ES7210_MIC_BIAS_2V45 = 0x30,
  ES7210_MIC_BIAS_2V55 = 0x40,
  ES7210_MIC_BIAS_2V66 = 0x50,
  ES7210_MIC_BIAS_2V78 = 0x60,
  ES7210_MIC_BIAS_2V87 = 0x70
};

// ES7210 codec config struct
struct ES7210Config {
  uint32_t sample_rate_hz;     // Sample rate in Hz, common values are supported
  uint32_t mclk_ratio;         // MCLK-to-Sample-rate clock ratio, typically 256
  ES7210Format i2s_format;     // I2S format of ES7210's output, can be any value in es7210_i2s_fmt_t
  ES7210Resolution bit_width;  // I2S bit width of ES7210's output, can be any value in es7210_i2s_bits_t
  ES7210MicBias mic_bias;      // Bias voltage of analog MIC, please refer to your MIC's datasheet
  ES7210MicGain mic_gain;      // Gain of analog MIC, please adjust according to your MIC's sensitivity
  uint32_t tdm_enable;         // Choose whether to enable TDM mode
};

// Clock coefficient structure
struct ES7210Coefficient {
  uint32_t mclk;     // mclk frequency
  uint32_t lrck;     // lrck
  uint8_t ss_ds;     // not used
  uint8_t adc_div;   // adcclk divider
  uint8_t dll;       // dll_bypass
  uint8_t doubler;   // doubler enable
  uint8_t osr;       // adc osr
  uint8_t mclk_src;  // select mclk  source
  uint32_t lrck_h;   // The high 4 bits of lrck
  uint32_t lrck_l;   // The low 8 bits of lrck
};

class ES7210Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::LATE - 1; }
  void dump_config() override;

  void set_volume(int8_t volume);

 protected:
  static const ES7210Coefficient *get_coefficient(uint32_t mclk, uint32_t rate);

  void config_codec_(const ES7210Config *codec_conf);
  void set_i2s_format_(ES7210Format i2s_format, ES7210Resolution bit_width, bool tdm_enable);
  void set_i2s_sample_rate_(uint32_t sample_rate_hz, uint32_t mclk_ratio);
  void set_mic_gain_(ES7210MicGain mic_gain);
  void set_mic_bias_(ES7210MicBias mic_bias);
};

}  // namespace es7210
}  // namespace esphome

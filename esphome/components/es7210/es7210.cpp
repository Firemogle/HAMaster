#include "es7210.h"
#include "es7210_registers.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace es7210 {

static const char *const TAG = "es7210";

#define ES7210_ERROR_CHECK(func) \
  if (!(func)) { \
    this->mark_failed(); \
    return; \
  }
#define ES7210_READ_BYTE(reg, value) ES7210_ERROR_CHECK(this->read_byte(reg, value));
#define ES7210_WRITE_BYTE(reg, value) ES7210_ERROR_CHECK(this->write_byte(reg, value));

void ES7210Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ES7210...");

  ES7210Config codec_conf = {.sample_rate_hz = 16000,
                             .mclk_ratio = 256,
                             .i2s_format = ES7210_FORMAT_I2S,
                             .bit_width = ES7210_RESOLUTION_16,
                             .mic_bias = ES7210_MIC_BIAS_2V87,
                             .mic_gain = ES7210_MIC_GAIN_30DB,
                             .tdm_enable = false};

  this->config_codec_(&codec_conf);
  this->set_volume(0);
}

void ES7210Component::dump_config() {
  ESP_LOGCONFIG(TAG, "ES7210 Audio Codec:");
  if (this->is_failed()) {
    ESP_LOGCONFIG(TAG, "  Failed to initialize!");
    return;
  }
  ESP_LOGV(TAG, "  Register Values:");
  for (uint8_t reg = 0; reg <= 0x4C; reg++) {
    uint8_t value;
    ES7210_READ_BYTE(reg, &value);
    ESP_LOGV(TAG, "    %02x = %02x", reg, value);
  }
}

void ES7210Component::config_codec_(const ES7210Config *codec_conf) {
  // Perform software reset
  ES7210_WRITE_BYTE(ES7210_REG00_RESET, 0xFF);
  ES7210_WRITE_BYTE(ES7210_REG00_RESET, 0x32);
  // Set the initialization time when device powers up
  ES7210_WRITE_BYTE(ES7210_REG09_TIME_CONTROL0, 0x30);
  ES7210_WRITE_BYTE(ES7210_REG0A_TIME_CONTROL1, 0x30);
  // Configure HPF for ADC1-4
  ES7210_WRITE_BYTE(ES7210_REG22_ADC12_HPF1, 0x2A);
  ES7210_WRITE_BYTE(ES7210_REG23_ADC12_HPF2, 0x0A);
  ES7210_WRITE_BYTE(ES7210_REG21_ADC34_HPF1, 0x2A);
  ES7210_WRITE_BYTE(ES7210_REG20_ADC34_HPF2, 0x0A);
  // Set bits per sample, data protocol and TDM
  set_i2s_format_(codec_conf->i2s_format, codec_conf->bit_width, codec_conf->tdm_enable);
  // Configure analog power and VMID voltage
  ES7210_WRITE_BYTE(ES7210_REG40_ANALOG, 0xC3);
  // Set mic bias
  set_mic_bias_(codec_conf->mic_bias);
  // Set mic gain
  set_mic_gain_(codec_conf->mic_gain);
  // Power on
  ES7210_WRITE_BYTE(ES7210_REG47_MIC1_LOW_PWR, 0x08);
  ES7210_WRITE_BYTE(ES7210_REG48_MIC2_LOW_PWR, 0x08);
  ES7210_WRITE_BYTE(ES7210_REG49_MIC3_LOW_PWR, 0x08);
  ES7210_WRITE_BYTE(ES7210_REG4A_MIC4_LOW_PWR, 0x08);
  // Set ADC sample rate
  set_i2s_sample_rate_(codec_conf->sample_rate_hz, codec_conf->mclk_ratio);
  // Power down DLL
  ES7210_WRITE_BYTE(ES7210_REG06_PWR_DOWN, 0x04);
  // Power on MIC1-4 bias & ADC1-4 & PGA1-4 Power
  ES7210_WRITE_BYTE(ES7210_REG4B_MIC12_PWR_DOWN, 0x0F);
  ES7210_WRITE_BYTE(ES7210_REG4C_MIC34_PWR_DOWN, 0x0F);
  // Enable device
  ES7210_WRITE_BYTE(ES7210_REG00_RESET, 0x71);
  ES7210_WRITE_BYTE(ES7210_REG00_RESET, 0x41);
}

void ES7210Component::set_volume(int8_t volume_db) {
  /*
   * reg_val: 0x00 represents -95.5dB, 0xBF represents 0dB (default after reset),
   * and 0xFF represents +32dB, with a 0.5dB step
   */
  uint8_t reg_val = 191 + volume_db * 2;

  ES7210_WRITE_BYTE(ES7210_REG1E_ADC1_MAX_GAIN, reg_val);
  ES7210_WRITE_BYTE(ES7210_REG1D_ADC2_MAX_GAIN, reg_val);
  ES7210_WRITE_BYTE(ES7210_REG1C_ADC3_MAX_GAIN, reg_val);
  ES7210_WRITE_BYTE(ES7210_REG1B_ADC4_MAX_GAIN, reg_val);
}

void ES7210Component::set_i2s_format_(ES7210Format i2s_format, ES7210Resolution bit_width, bool tdm_enable) {
  uint8_t reg_val = 0;

  switch (bit_width) {
    case ES7210_RESOLUTION_16:
      reg_val = 0x60;
      break;
    case ES7210_RESOLUTION_18:
      reg_val = 0x40;
      break;
    case ES7210_RESOLUTION_20:
      reg_val = 0x20;
      break;
    case ES7210_RESOLUTION_24:
      reg_val = 0x00;
      break;
    case ES7210_RESOLUTION_32:
      reg_val = 0x80;
      break;
    default:
      ESP_LOGE(TAG, "Invalid bit width");
      this->mark_failed();
      return;
  }
  ES7210_WRITE_BYTE(ES7210_REG11_SDP_INTERFACE1, i2s_format | reg_val);

  const char *mode_str = NULL;
  switch (i2s_format) {
    case ES7210_FORMAT_I2S:
      reg_val = 0x02;
      mode_str = "standard i2s";
      break;
    case ES7210_FORMAT_LJ:
      reg_val = 0x02;
      mode_str = "left justify";
      break;
    case ES7210_FORMAT_DSP_A:
      reg_val = 0x01;
      mode_str = "DSP-A";
      break;
    case ES7210_FORMAT_DSP_B:
      reg_val = 0x01;
      mode_str = "DSP-B";
      break;
    default:
      ESP_LOGE(TAG, "Invalid I2S mode");
      this->mark_failed();
      return;
  }

  if (tdm_enable) {  // enable 1xFS TDM
    ES7210_WRITE_BYTE(ES7210_REG12_SDP_INTERFACE2, reg_val);
  } else {
    ES7210_WRITE_BYTE(ES7210_REG12_SDP_INTERFACE2, 0x00);
  }

  ESP_LOGI(TAG, "format: %s, bit width: %d, tdm mode %s", mode_str, bit_width, tdm_enable ? "enabled" : "disabled");
}

void ES7210Component::set_i2s_sample_rate_(uint32_t sample_rate_hz, uint32_t mclk_ratio) {
  uint32_t mclk_freq_hz = sample_rate_hz * mclk_ratio;
  const ES7210Coefficient *coeff_div = get_coefficient(mclk_freq_hz, sample_rate_hz);
  if (!coeff_div) {
    ESP_LOGE(TAG, "Unable to set %d Hz sample rate with %d Hz MCLK", sample_rate_hz, mclk_freq_hz);
    this->mark_failed();
    return;
  }
  // Set osr
  ES7210_WRITE_BYTE(ES7210_REG07_OSR, coeff_div->osr);
  // Set adc_div & doubler & dll
  ES7210_WRITE_BYTE(ES7210_REG02_MAIN_CLK, (coeff_div->adc_div) | (coeff_div->doubler << 6) | (coeff_div->dll << 7));
  // Set lrck
  ES7210_WRITE_BYTE(ES7210_REG04_LRCK_DIVH, coeff_div->lrck_h);
  ES7210_WRITE_BYTE(ES7210_REG05_LRCK_DIVL, coeff_div->lrck_l);

  ESP_LOGI(TAG, "sample rate: %d Hz, mclk frequency: %d Hz", sample_rate_hz, mclk_freq_hz);
}

void ES7210Component::set_mic_gain_(ES7210MicGain mic_gain) {
  ES7210_WRITE_BYTE(ES7210_REG43_MIC1_GAIN, mic_gain | 0x10);
  ES7210_WRITE_BYTE(ES7210_REG44_MIC2_GAIN, mic_gain | 0x10);
  ES7210_WRITE_BYTE(ES7210_REG45_MIC3_GAIN, mic_gain | 0x10);
  ES7210_WRITE_BYTE(ES7210_REG46_MIC4_GAIN, mic_gain | 0x10);
}

void ES7210Component::set_mic_bias_(ES7210MicBias mic_bias) {
  ES7210_WRITE_BYTE(ES7210_REG41_MIC12_BIAS, mic_bias);
  ES7210_WRITE_BYTE(ES7210_REG42_MIC34_BIAS, mic_bias);
}

}  // namespace es7210
}  // namespace esphome

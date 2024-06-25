import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import i2c
from esphome.const import CONF_ID

es7210_ns = cg.esphome_ns.namespace("es7210")
ES7210Component = es7210_ns.class_("ES7210Component", cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = (
    cv.Schema({cv.GenerateID(): cv.declare_id(ES7210Component)})
    .extend(i2c.i2c_device_schema(0x40))
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan, output
from esphome.const import CONF_OSCILLATION_OUTPUT, CONF_OUTPUT, \
    CONF_OUTPUT_ID, CONF_SPEED, CONF_LOW, CONF_MEDIUM, CONF_HIGH
#from .. import speed_ns

fan_ns = cg.esphome_ns.namespace('binaryspeed')
BinarySpeedFan = fan_ns.class_('BinarySpeedFan', cg.Component)

#IS_PLATFORM_COMPONENT = True
print(BinarySpeedFan)

CONFIG_SCHEMA = fan.FAN_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(BinarySpeedFan),
    cv.Required(CONF_OUTPUT): cv.Schema({
        cv.Optional(CONF_LOW): cv.use_id(output.BinaryOutput),
        cv.Optional(CONF_MEDIUM): cv.use_id(output.BinaryOutput),
        cv.Optional(CONF_HIGH): cv.use_id(output.BinaryOutput),
    }),
    cv.Optional(CONF_OSCILLATION_OUTPUT): cv.use_id(output.BinaryOutput),
}).extend(cv.COMPONENT_SCHEMA)

print(CONFIG_SCHEMA)


def to_code(config):
    # get outputs
    outputs = config[CONF_OUTPUT]
    low_output_ = yield cg.get_variable(outputs[CONF_LOW])
    medium_output_ = yield cg.get_variable(outputs[CONF_MEDIUM])
    high_output_ = yield cg.get_variable(outputs[CONF_HIGH])
    
    # state for the fan component itself
    fan_ = yield fan.create_fan_state(config)
    
    # register me as a component
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID], fan_, low_output_, medium_output_, high_output_)
    yield cg.register_component(var, config)
    
    # add the speeds to the template
    # cg.add(var.set_outputs(low_output_, medium_output_, high_output_))

    if CONF_OSCILLATION_OUTPUT in config:
        oscillation_output = yield cg.get_variable(config[CONF_OSCILLATION_OUTPUT])
        cg.add(var.set_oscillating(oscillation_output))
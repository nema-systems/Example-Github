def compute_powertrain_efficiency() -> float:

    # @nema:get:https://app.nemasystems.io/dev/default/motor-example/data/1
    motor_efficiency = 0.92

    # @nema:get:https://app.nemasystems.io/dev/default/motor-example/data/2
    motor_controller_efficiency = 0.92

    return motor_efficiency * motor_controller_efficiency

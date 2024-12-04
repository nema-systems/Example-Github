def compute_powertrain_efficiency() -> float:

    # @nema:get:https://app.nemasystems.io/dev/default/motor-example/data/1
    motor_efficiency = 0.82

    # @nema:get:https://app.nemasystems.io/dev/default/motor-example/data/2
    motor_controller_efficiency = 0.95

    return motor_efficiency * motor_controller_efficiency

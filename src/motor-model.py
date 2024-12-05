def compute_powertrain_efficiency() -> float:
    """
    Compute the efficiency of the powertrain.
    """

    # @nema:get:https://app.nemasystems.io/dev/default/motor-example/data/1
    motor_efficiency = 0.89

    # @nema:get:https://app.nemasystems.io/dev/default/motor-example/data/2
    motor_controller_efficiency = 0.97

    return motor_efficiency * motor_controller_efficiency

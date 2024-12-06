"""
This module contains the control algorithm used for controlling the robot in production.
"""

from dataclasses import dataclass


@dataclass
class ControlParameters:
    Kp: float
    Ki: float
    Kd: float


CONTROL_PARAMETERS = ControlParameters(
    # @nema:get:https://app.nemasystems.io/nema-demo/default/differential-drive-example/data/2
    Kp=3.664,
    # @nema:get:https://app.nemasystems.io/nema-demo/default/differential-drive-example/data/5
    Ki=0.73,
    # @nema:get:https://app.nemasystems.io/nema-demo/default/differential-drive-example/data/3
    Kd=0.5,
)


def compute_theta_control(
    theta_error: float, previous_theta_error: float, integral_error: float, dt: float
) -> tuple[float]:
    """
    Compute the control signal for the angular velocity of the robot.

    Args:
        theta_error (float): Error in orientation
        previous_theta_error (float): Error in orientation at the previous time step
        integral_error (float): Integral of the error in orientation
        dt (float): Time step

    Returns:
        tuple[float]: Tuple containing the control signal and the new integral error
    """

    P = CONTROL_PARAMETERS.Kp * theta_error
    I = CONTROL_PARAMETERS.Ki * integral_error
    D = CONTROL_PARAMETERS.Kd * (theta_error - previous_theta_error) / dt

    new_integral_error = integral_error + theta_error * dt

    return P + I + D, new_integral_error

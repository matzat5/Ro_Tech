import lib.initial_parameters as ip
import lib.atmosphere as atm
import math


def do_thrust(t):
    tk = - ip.MASS_FUEL / ip.DM_DT
    if t <= tk:
        return ip.THRUST
    else:
        return 0


def force_x_a(velocity, height):
    dencity = atm.dencity(height)
    return velocity ** 2 * dencity * ip.S_M * ip.C_X_A / 2


def force_y_a(velocity, height):
    dencity = atm.dencity(height)
    return velocity ** 2 * dencity * ip.S_M * ip.C_Y_A / 2


def momentum_z(velocity, height, alpha):
    dencity = atm.dencity(height)
    return velocity ** 2 * dencity * ip.S_M * ip.LENGHT * ip.M_Z_KOEF * alpha


def do_alpha(pitch, tetta_c):
    # return 0.1
    return pitch - tetta_c


def dv_dt(velocity, height, tetta_c, alpha, m, t):
    x_a = force_x_a(velocity, height)
    return (do_thrust(t) * math.cos(alpha) - x_a) / m - atm.acceleration_g(height) * math.sin(tetta_c)


def dtetta_c_dt(velocity, height, tetta_c, alpha, m, t):
    if height <= ip.HEIGHT_GUIDE * math.sin(ip.TETTA_C_0):
        return 0
    else:
        y_a = force_y_a(velocity, height)
        return (y_a + do_thrust(t) * math.sin(alpha)) / (m * velocity) + atm.acceleration_g(
            height) * math.cos(tetta_c) / velocity
    # TODO по результатам работы проги тетта мало меняется от 90 градусов - подумать, почему (ракета не ложится
    #  набок, хотя из-за вращетального движения должна


def domega_z_dt(velocity, height, alpha):
    return momentum_z(velocity, height, alpha) / ip.INERTION_MOMENTUM


def dpitch_dt(omega_z):
    return omega_z


def dx_dt(velocity, tetta_c):
    return velocity * math.cos(tetta_c)


def dy_dt(velocity, tetta_c):
    return velocity * math.sin(tetta_c)


def dm_dt(t):
    tk = - ip.MASS_FUEL / ip.DM_DT
    if t <= tk:
        return ip.DM_DT
    else:
        return 0

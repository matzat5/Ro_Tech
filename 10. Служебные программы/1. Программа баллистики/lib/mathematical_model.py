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


def dv_dt(velocity, height, tetta_c, m, t):
    x_a = force_x_a(velocity, height)
    return (do_thrust(t) - x_a) / m - atm.acceleration_g(height) * math.sin(tetta_c)


def dtetta_c_dt(velocity, height, tetta_c, m):
    if height <= ip.HEIGHT_GUIDE * math.sin(ip.TETTA_C_0):
        return 0
    else:
        y_a = force_y_a(velocity, height)
        return y_a / (m * velocity) + atm.acceleration_g(height) * math.cos(tetta_c) / velocity


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

# TODO добавить вращение вокруг ЦМ

import lib.mathematical_model as mm
import lib.initial_parameters as ip
import math


def runge_kutta(arr_parameters, t, dt):
    v_old = arr_parameters[0]
    tetta_c_old = arr_parameters[1]
    x_old = arr_parameters[2]
    y_old = arr_parameters[3]
    m_old = arr_parameters[4]

    k_v_1 = dt * mm.dv_dt(v_old, y_old, tetta_c_old, m_old, t)
    k_tetta_c_1 = dt * mm.dtetta_c_dt(v_old, y_old, tetta_c_old, m_old)
    k_x_1 = dt * mm.dx_dt(v_old, tetta_c_old)
    k_y_1 = dt * mm.dy_dt(v_old, tetta_c_old)
    k_m_1 = dt * mm.dm_dt(t)

    k_v_2 = dt * mm.dv_dt(v_old + k_v_1 / 2, y_old + k_y_1 / 2, tetta_c_old + k_tetta_c_1 / 2, m_old + k_m_1 / 2,
                          t + dt / 2)
    k_tetta_c_2 = dt * mm.dtetta_c_dt(v_old + k_v_1 / 2, y_old + k_y_1 / 2, tetta_c_old + k_tetta_c_1 / 2,
                                      m_old + k_m_1 / 2)
    k_x_2 = dt * mm.dx_dt(v_old + k_v_1 / 2, tetta_c_old + k_tetta_c_1 / 2)
    k_y_2 = dt * mm.dy_dt(v_old + k_v_1 / 2, tetta_c_old + k_tetta_c_1 / 2)
    k_m_2 = dt * mm.dm_dt(t)

    k_v_3 = dt * mm.dv_dt(v_old + k_v_2 / 2, y_old + k_y_2 / 2, tetta_c_old + k_tetta_c_2 / 2, m_old + k_m_2 / 2,
                          t + dt / 2)
    k_tetta_c_3 = dt * mm.dtetta_c_dt(v_old + k_v_2 / 2, y_old + k_y_2 / 2, tetta_c_old + k_tetta_c_2 / 2,
                                      m_old + k_m_2 / 2)
    k_x_3 = dt * mm.dx_dt(v_old + k_v_2 / 2, tetta_c_old + k_tetta_c_2 / 2)
    k_y_3 = dt * mm.dy_dt(v_old + k_v_2 / 2, tetta_c_old + k_tetta_c_2 / 2)
    k_m_3 = dt * mm.dm_dt(t)

    k_v_4 = dt * mm.dv_dt(v_old + k_v_3, y_old + k_y_3, tetta_c_old + k_tetta_c_3, m_old + k_m_3, t + dt)
    k_tetta_c_4 = dt * mm.dtetta_c_dt(v_old + k_v_3, y_old + k_y_3, tetta_c_old + k_tetta_c_3, m_old + k_m_3)
    k_x_4 = dt * mm.dx_dt(v_old + k_v_3, tetta_c_old + k_tetta_c_3)
    k_y_4 = dt * mm.dy_dt(v_old + k_v_3, tetta_c_old + k_tetta_c_3)
    k_m_4 = dt * mm.dm_dt(t)

    v_new = v_old + (k_v_1 + 2 * k_v_2 + 2 * k_v_3 + k_v_4) / 6
    tetta_c_new = tetta_c_old + (k_tetta_c_1 + 2 * k_tetta_c_2 + 2 * k_tetta_c_3 + k_tetta_c_4) / 6
    x_new = x_old + (k_x_1 + 2 * k_x_2 + 2 * k_x_3 + k_x_4) / 6
    y_new = y_old + (k_y_1 + 2 * k_y_2 + 2 * k_y_3 + k_y_4) / 6
    m_new = m_old + (k_m_1 + 2 * k_m_2 + 2 * k_m_3 + k_m_4) / 6

    return [v_new, tetta_c_new, x_new, y_new, m_new]


def calculate():
    print('%-5s %-8s %-7s %-8s %-7s %-7s' % ('t, c', 'V, m/c', 'θ, °',
                                             'x, m', 'y, m', 'm, kg'))

    parameters = [ip.V_0, ip.TETTA_C_0, ip.X_0, ip.Y_0, ip.MASS_0]
    dt = ip.DELTA_T
    t = ip.T_0
    t_prev = t
    parameters_prev = parameters
    flg_guide = False

    while True:
        # TODO добавить четко точку схода с направляющей
        print('%-5.1f %-8.3f %-7.2f %-8.2f %-7.2f %-7.2f' % (t, parameters[0], math.degrees(parameters[1]),
                                                             parameters[2], parameters[3], parameters[4]))

        parameters_prev = parameters
        parameters = runge_kutta(parameters, t, dt)
        t += dt

        if parameters_prev[3] > parameters[3]:
            break

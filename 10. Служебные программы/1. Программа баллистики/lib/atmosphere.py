import math


# Функции атмосферы
def geopotencial_h(h):
    H = (r * h) / (r + h)  # Геопотенциальная высота
    return H


def acceleration_g(h):
    g = g0 * (r / (r + h)) ** 2  # Ускорение свободного падения
    return g


def parametry_t(h):
    H = geopotencial_h(h)
    # Параметры, для определения температуры и давления
    if (H >= -2001) and (H < 0):  # 3-4 стр. ГОСТа
        Hx = -2000
        batm = -0.0065
        Tx = 301.15
        px = 127774
    elif (H >= 0) and (H <= 11000):  # 5-15 стр. ГОСТа
        Hx = 0
        batm = -0.0065
        Tx = 288.15
        px = 101325
    elif (H > 11000) and (H < 20000):  # 15-24 стр. ГОСТа
        Hx = 11000
        Tx = 216.65
        batm = 0
        px = 22632
    elif (H >= 20000) and (H < 32000):  # 25-36 стр. ГОСТа
        Hx = 20000
        batm = 0.001
        Tx = 216.65
        px = 5474.87
    elif (H >= 32000) and (H < 47000):  # 37-43 стр. ГОСТа
        Hx = 32000
        batm = 0.0028
        Tx = 228.65
        px = 868.014
    elif (H >= 47000) and (H < 51000):  # 43-46 стр. ГОСТа
        Hx = 47000
        Tx = 270.65
        batm = 0
        px = 110.906
    elif (H >= 51000) and (H < 71000):  # 45-52 стр. ГОСТа
        Hx = 51000
        batm = -0.0028
        Tx = 270.650
        px = 66.9384
    elif (H >= 71000) and (H < 85000):  # 51-53 и 159 стр. ГОСТа
        Hx = 71000
        batm = -0.002
        Tx = 214.65
        px = 3.95639
    return dict(Hx=Hx, batm=batm, Tx=Tx, px=px)


def temperature(h):
    p = parametry_t(h)
    H = geopotencial_h(h)
    T = p["Tx"] + p["batm"] * (H - p["Hx"])  # Температура
    return T


def pressure(h):
    p = parametry_t(h)
    H = geopotencial_h(h)
    T = temperature(h)
    if p["batm"] != 0:  # Давление
        p = 10 ** (math.log10(p["px"]) - (g0 / (p["batm"] * R)) * math.log10(T / p["Tx"]))
    else:
        p = 10 ** (math.log10(p["px"]) - ((0.434294 * g0 * (H - p["Hx"])) / (T * R)))
    return p


def dencity(h):
    p = pressure(h)
    T = temperature(h)
    ro = p / (R * T)  # Плотность
    return ro


def sound_velocity(h):
    T = temperature(h)
    a = 20.046796 * math.sqrt(T)  # Скорость звука
    return a


def vivod(h):
    H = geopotencial_h(h)
    g = acceleration_g(h)
    T = temperature(h)
    p = pressure(h)
    ro = dencity(h)
    a = sound_velocity(h)
    print("Геометрическая высота: " + "{:.0f}".format(h) + ' м')
    print("Геопотенциальная высота: " + "{:.0f}".format(H) + ' м')
    print("Ускорение свободного падения: " + str(round(g, 3)) + ' м/с^2')
    print("Температура: " + str(round(T / 100, 3)) + ' * 10^2 K')
    print("Давление: " + str(round(p / 10000, 3)) + ' * 10^4' + ' Па')
    print('Плотность: ' + str(round(ro * 10, 3)) + ' * 10^-1' + ' кг/м^3')
    print("Скорость звука: " + str(round(a / 100, 3)) + ' * 10^2 м/с')
    print(' ')


r = 6356767
g0 = 9.80665
R = 287.05287

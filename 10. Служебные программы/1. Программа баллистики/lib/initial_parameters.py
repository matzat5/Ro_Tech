# Характеристики ракеты
import math

THRUST = 337                    # Тяга
MASS_0 = 27                     # Стартовая масса
MASS_FUEL = 9.777                   # Масса топлива
DM_DT = -0.175                    # Массовый расход
S_M = 0.1                       # Мидалево сечение корпуса
C_X_A = 0.3                     # Коэффициент силы лобового сопротивления
C_Y_A = 0.14                     # Коэффициент подъемной силы. Сейчас выполняет имитацию легкого опрокидывания набок
HEIGHT_GUIDE = 10               # Длина направляющей

# Параметры интегрирования
T_0 = 0                         # Время начала интегрирования
X_0 = 0                         # Стартовые координаты по X
Y_0 = 0                         # Стартовые координаты по Y
TETTA_C_0 = math.radians(90)    # Стартовый угол старта (в градусах)
V_0 = 0                         # Начальная скорость
DELTA_T = 0.1                   # Шаг интегрирования
# T_K

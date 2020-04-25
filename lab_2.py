from math import sqrt, pi, exp

import scipy.stats as st
from PIL import Image, ImageDraw
from pylab import *
from scipy import linalg

MEAN_VECTOR_R = []
MEAN_VECTOR_G = []
MEAN_VECTOR_B = []

VAR_VECTOR_R = []
VAR_VECTOR_G = []
VAR_VECTOR_B = []

SKEW_VECTOR_R = []
SKEW_VECTOR_G = []
SKEW_VECTOR_B = []

KURT_VECTOR_R = []
KURT_VECTOR_G = []
KURT_VECTOR_B = []


def get_array(n):
    array = [[0 for i in range(n)] for i in range(n)]
    return array


def f(x, mu, sigma):
    return 1 / (sigma * sqrt(2 * pi)) * exp((-(x - mu) ** 2) / (2 * sigma ** 2))


def get_RGB(image):
    width = image.size[0]  # Определяем ширину.
    height = image.size[1]  # Определяем высоту.
    pix = image.load()  # Выгружаем значения пикселей.
    RGB = [[], [], []]
    for i in range(width):
        for j in range(height):
            a = pix[i, j][0]
            b = pix[i, j][1]
            c = pix[i, j][2]
            RGB[0].append(a)
            RGB[1].append(b)
            RGB[2].append(c)
    return RGB


def get_description(x):
    result = [np.mean(x), np.var(x), st.skew(x), st.kurtosis(x)]
    return tuple(result)


def main():
    # --------Gauss------------
    print('------------------Task ------------------')
    for j in range(1, 21):
        image = Image.open('mirflickr/im{}.jpg'.format(j))
        RGB = get_RGB(image)

        mean0, var0, skew0, kurt0 = get_description(RGB[0])
        mean1, var1, skew1, kurt1 = get_description(RGB[1])
        mean2, var2, skew2, kurt2 = get_description(RGB[2])

        MEAN_VECTOR_R.append(mean0)
        MEAN_VECTOR_G.append(mean1)
        MEAN_VECTOR_B.append(mean2)

        VAR_VECTOR_R.append(var0)
        VAR_VECTOR_G.append(var1)
        VAR_VECTOR_B.append(var2)

        SKEW_VECTOR_R.append(skew0)
        SKEW_VECTOR_G.append(skew1)
        SKEW_VECTOR_B.append(skew2)

        KURT_VECTOR_R.append(kurt0)
        KURT_VECTOR_G.append(kurt1)
        KURT_VECTOR_B.append(kurt2)

    M = np.cov(np.vstack((MEAN_VECTOR_R, MEAN_VECTOR_G, MEAN_VECTOR_B)))
    M_D = np.cov(np.vstack((MEAN_VECTOR_R, MEAN_VECTOR_G, MEAN_VECTOR_B, VAR_VECTOR_R, VAR_VECTOR_G, VAR_VECTOR_B)))
    M_D_Skew = np.cov(np.vstack((MEAN_VECTOR_R, MEAN_VECTOR_G, MEAN_VECTOR_B, VAR_VECTOR_R, VAR_VECTOR_G, VAR_VECTOR_B,
                                 SKEW_VECTOR_R, SKEW_VECTOR_G, SKEW_VECTOR_B)))
    M_D_Skew_Kurt = np.cov(np.vstack((MEAN_VECTOR_R, MEAN_VECTOR_G, MEAN_VECTOR_B, VAR_VECTOR_R, VAR_VECTOR_G,
                                      VAR_VECTOR_B, SKEW_VECTOR_R, SKEW_VECTOR_G, SKEW_VECTOR_B, KURT_VECTOR_R,
                                      KURT_VECTOR_G, KURT_VECTOR_B)))

    print('Матрица ковариации для векторов М: ')
    print(M)
    print()

    print('Матрица ковариации для векторов М и D: ')
    print(M_D)
    print()

    print('Матрица ковариации для векторов М, D, Skew: ')
    print(M_D_Skew)
    print()

    print('Матрица ковариации для векторов М, D, Skew, Kurt: ')
    print(M_D_Skew_Kurt)
    print()
    """
    #Legend = ['$\sigma = %d $' % var_m, '$\mu = %d $' % mean_m]
    Legend = ['$\sigma = {0} $ $\mu = {1} $'.format(int(var_m), int(mean_m))] 
    xs = np.arange(-5000, 5000, 0.5) # Сетка значений по оси абсцисс.

    Y = [f(x, mean_m, var_m) for x in xs]
    plot(xs, Y/max(Y))
    legend(Legend)
    show()
    """
    # ------------------PCA => SVD-----------------------

    print('------------------Task ------------------')
    image = Image.open("mirflickr/im15.jpg")
    width = image.size[0]  # 500
    height = image.size[1]  # 333
    draw = ImageDraw.Draw(image)
    pix = image.load()
    R_ch_RGB = [[0 for i in range(height)] for i in range(width)]  # Создаем пустой список нужного размера

    for i in range(width):
        for j in range(height):
            R_ch_RGB[i][j] = pix[i, j][0]

    U, S, Vt = linalg.svd(R_ch_RGB)
    # restored_matrix = np.dot(U, S, Vt)

    S_add = np.zeros((width, height))  # Создаем пустую матрицу для сингулярных чисел
    for i in range(width):
        for j in range(height):
            if i == j:
                S_add[i][j] = S[i]

    number = 333
    for i in range(number, width):
        for j in range(number, height):
            S_add[i][j] = 0
    US = np.dot(U, S_add)
    restored_matrix = np.dot(US, Vt)

    # Восстанавливаем картинку
    for i in range(width):
        for j in range(height):
            a = int(restored_matrix[i][j])
            b = pix[i, j][1]
            c = pix[i, j][2]
            draw.point((i, j), (a, b, c))

    image.save("restored_img/Restored.jpg", "JPEG")
    del draw

    # print( [restored_matrix[i][j] for i in range(3) for j in range(3)] )
    # print( [R_ch_RGB[i][j] for i in range(3) for j in range(3)] )

    total_error = 0
    for i in range(width):
        for j in range(height):
            diff = abs(R_ch_RGB[i][j] - restored_matrix[i][j])
            total_error += diff
    EPS = total_error / (width * height) * 100

    print('Ошибка исходной матрици и полученой: ', EPS, '%')
    print()

    # -------------------Stochastic matrix--------------------

    print('------------------Task ------------------')
    # Стохастическая матрица с лева на право
    Stochastic_matrix = np.zeros((256, 256))
    suma_row = 0
    sum1 = 0
    print('Стохастическая матрица L -> R: ')
    for i in range(width):
        for j in range(height - 1):
            k1 = R_ch_RGB[i][j]
            k2 = R_ch_RGB[i][j + 1]
            Stochastic_matrix[k1][k2] += 1

        # Преобразование в нормальный вид
    for i in range(256):
        for j in range(256):
            suma_row += Stochastic_matrix[i][j]
        Stochastic_matrix[i] /= suma_row
        suma_row = 0
    print(Stochastic_matrix)
    print()

    # Сума элементов разделенная на количество элементов
    for i in range(256):
        for j in range(256):
            sum1 += Stochastic_matrix[i][j]
    print('Сума элементов разделенная на количество строк = ', sum1 / (256))
    print()

    # Стохастическая матрица с права на лево
    Stochastic_matrix = np.zeros((256, 256))
    suma_row = 0
    sum1 = 0
    print('Стохастическая матрица R -> L: ')
    for i in range(width):
        for j in range(height - 1):
            k1 = R_ch_RGB[i][height - j - 2]
            k2 = R_ch_RGB[i][height - j - 1]
            Stochastic_matrix[k1][k2] += 1

        # Преобразование в нормальный вид
    for i in range(256):
        for j in range(256):
            suma_row += Stochastic_matrix[i][j]
        Stochastic_matrix[i] /= suma_row
        suma_row = 0
    print(Stochastic_matrix)
    print()

    # Сума элементов разделенная на количество элементов
    for i in range(256):
        for j in range(256):
            sum1 += Stochastic_matrix[i][j]
    print('Сума элементов разделенная на количество строк = ', sum1 / (256))
    print()


if __name__ == '__main__':
    main()

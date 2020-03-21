import random
from pathlib import Path

import numpy as np
import scipy.stats as st
from PIL import Image, ImageDraw
from fitter import Fitter
from matplotlib.pyplot import figure, title, xlabel, show

DATASET_FILE = 'images_dataset.txt'
DISTRIBUTIONS = ['norm', 'expon', 'uniform', 'beta', 'gamma']
CURRENT_IMG_DIR = Path(__file__).resolve() / 'current_img'


def draw_chanel(image, n):
    width = image.size[0]
    height = image.size[1]
    pix = image.load()
    draw = ImageDraw.Draw(image)

    for i in range(width):
        for j in range(height):
            a, b, c = 0, 0, 0
            if n == 0:
                a = pix[i, j][0]
            if n == 1:
                b = pix[i, j][1]
            if n == 2:
                c = pix[i, j][2]
            draw.point((i, j), (a, b, c))
    image.save("current_img/ans" + str(n) + ".jpg", "JPEG")
    del draw


def get_rgb_channels(image):
    width = image.size[0]
    height = image.size[1]
    pix = image.load()
    RGB = [[], [], []]

    for i in range(width):
        for j in range(height):
            a = pix[i, j][0]
            b = pix[i, j][1]
            c = pix[i, j][2]
            RGB[0].append(a)
            RGB[1].append(b)
            RGB[2].append(c)
    image.save("current_img/ans.jpg", "JPEG")
    return RGB


def get_statistic_info(x):
    result = [np.mean(x), (np.min(x), np.max(x)), np.var(x), (np.percentile(x, 25), np.percentile(x, 75)),
              st.mode(x), st.skew(x), st.kurtosis(x)]
    return result


def main():
    with open(DATASET_FILE, 'r') as file:
        image_names = ['mirflickr/im' + x.strip() + '.jpg' for x in file.readlines()]
        random.shuffle(image_names)

    for name in range(len(image_names)):
        print(image_names[name])

        rgb_channels = get_rgb_channels(Image.open(image_names[name]))

        for channel in range(len(rgb_channels)):
            m, minmax, s, perct, mode, skew, kurt = get_statistic_info(rgb_channels[channel])

            channels = ['R', 'G', 'B']

            print('{} chanel:'.format(channels[channel]))
            print('Expected value: {0:.4f}'.format(m))
            print('Min , Max: ({0}, {1})'.format(*minmax))
            print('D: {0:.4f}'.format(s))
            print('Quartily: (25%)  {0:.4f}, (75%)  {1:.4f}'.format(*perct))
            print('asymmetry : {0:.4f}'.format(skew))
            print('Kurtosis: {0:.4f}'.format(kurt))
            print()

            bins_number = 256

            figure()

            res = Fitter(rgb_channels[channel], distributions=DISTRIBUTIONS, bins=bins_number)
            res.fit()

            fit_result = res._fitted_errors
            key_min = min(fit_result.keys(), key=(lambda k: fit_result[k]))

            print(f'Best fitted: {key_min} distributions')

            res.summary()
            res.hist()

            title(f'Histogram of pixel brightness for {channels[channel]} channel for {image_names[name]}')
            xlabel('bright')
            show()

            draw_chanel(Image.open(image_names[name]), channel)


if __name__ == '__main__':
    main()

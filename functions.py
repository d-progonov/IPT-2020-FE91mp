#! /usr/bin/env python
# -*- coding: utf-8 -*-
import os
import re
import sys
from collections import defaultdict
from pprint import pprint

import networkx as nx
import numpy as np
import csv
import matplotlib.pyplot as plt
import pandas as pd
import scipy as sp
import yaml
from fitter import Fitter
from PIL import Image
from scipy import stats
from tqdm import tqdm
from matplotlib import image as img
from numpy.linalg import matrix_power
from sklearn.linear_model import LinearRegression, LogisticRegression, HuberRegressor
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score

RGB = ['red', 'green', 'blue']
DISTRIBUTIONS = ['beta', 'gamma', 'laplace', 'norm', 'pareto']
model_list = [LinearRegression(), LogisticRegression(), HuberRegressor(max_iter=4000), SVC()]

output_dir = sys.argv[0].split('.')[1].split('/')[1]


def atoi(text):
    return int(text) if text.isdigit() else text


def natural_keys(text):
    '''
    alist.sort(key=natural_keys) sorts in human order
    http:#nedbatchelder.com/blog/200712/human_sorting.html
    (See Toothy's implementation in the comments)
    '''
    return [atoi(c) for c in re.split(r'(\d+)', text)]


def parse_config(path_in='config.yaml'):
    print('>>Parsing config')
    with open(path_in, 'r') as stream:
        try:

            config = yaml.safe_load(stream)
            print('\t>>Parsed:')
            for key in config:
                print('\t' + str(key) + ': ' + str(config[key]))
            return config
        except yaml.YAMLError as exc:
            return exc


def config_random_generator(seed=14):
    print('>>Configure generator')
    np.random.seed(int(seed))
    generator_info = np.random.get_state()
    return


def create_list_files(target_format_in='.jpg', path_in='./input/mirflickr'):
    print('>>Creating list of files')
    file_list = list()
    for root, _, files in os.walk(path_in):
        print('\n\t>>Scanning folder: ' + str(root))
        for curr_file in tqdm(files):
            if target_format_in in curr_file:
                file_list.append(root + '/' + curr_file)
    file_list.sort(key=natural_keys)
    return file_list


def create_index_list(file_list, count):
    print('\n>>Creating list of indexes')
    index_list = np.random.random_integers(0, len(file_list) - 1, count)
    filtered_file_list = list()
    for index in tqdm(index_list):
        filtered_file_list.append(file_list[index])
    return index_list, filtered_file_list


def get_image_info_vectors(image_index, image_name, color_index):
    image = np.array(Image.open(image_name))
    a = image[:, color_index].ravel()
    d = {
        'mean': np.nanmean(a),  # среднеарифметическое
        'var': np.nanvar(a),  # дисперсия
        'skewness': sp.stats.skew(a),  # коэффициент асимметрии
        # коэффицие́нт эксце́сса (нормализированный)
        'kurtosis': sp.stats.kurtosis(a) - 3,
    }
    return d


def get_image_info(image_index, image_name, color_index):
    image = np.array(Image.open(image_name))
    a = image[:, color_index].ravel()
    d = {
        'name': image_name,  # название файла
        'min': np.nanmin(a),  # минимум
        'max': np.nanmax(a),  # максимум
        'mean': np.nanmean(a),  # среднеарифметическое
        'var': np.nanvar(a),  # дисперсия
        'median': np.nanmedian(a),  # медиана
        'average': np.average(a),  # средневзвешенное(мат ожидание)
        'std': np.nanstd(a),  # среднеквадратичное (стандартное) отклонение
        'skewness': sp.stats.skew(a),  # коэффициент асимметрии
        # коэффицие́нт эксце́сса (нормализированный)
        'kurtosis': sp.stats.kurtosis(a) - 3,
        'interquartile range': sp.stats.iqr(a),  # интерквартильный размах
        # вид распределения с мин ошибкой
        'best distribution': get_image_histogram(image_index, image_name, color_index)
    }
    return d


def get_image_histogram(image_index, image_name, color_index):
    print('\n\n\t>>Get histogram about:  ' + image_name)
    plt.figure()
    image = np.array(Image.open(os.path.join(image_name)))
    a = image[:, color_index].ravel()
    color_name = RGB[color_index]
    print('\n\n\n********************************')
    print('Color: ' + str(color_name) + ' | Image ' + str(image_name) + '\n')
    f = Fitter(a, distributions=DISTRIBUTIONS, bins=256)
    f.fit()
    pprint(f.summary())
    f.hist()
    best_distribution = f.get_best()
    print(best_distribution)
    print('********************************\n\n\n')
    plt.xlim(0, 255)
    plt.xlabel('Brightness')
    plt.ylabel('Frequency')
    plt.title('Image ' + str(image_name) + ' with index ' + str(image_index))
    plt.savefig('./output/' + output_dir + '/' + str(color_name) +
                '_histograms/' + str(image_index) + '.png')
    return best_distribution.keys()[0]


def get_distribution_of_distributions(distributions):
    pprint(distributions)
    for color_index, color_name in enumerate(RGB):
        distributions[color_name]


def get_images_info(image_list):
    data = {}
    print('\n\n\n\n>>Analyzing images')

    distributions = {}
    for color_index, color_name in enumerate(RGB):
        distributions[color_name] = {}
        for distribution in DISTRIBUTIONS:
            distributions[color_name][distribution] = 0
        print('\t\t>>Get info about ' + str(color_name) + ' color')
        data[color_name] = pd.DataFrame()
        for image_index, image_name in tqdm(enumerate(image_list)):
            image_info = get_image_info(image_index, image_name, color_index)
            distributions[color_name][image_info['best distribution']] += 1
            data[color_name] = pd.concat([data[color_name], pd.DataFrame(
                pd.DataFrame(image_info, index=[0, ]))], ignore_index=True)
        data[color_name].to_csv(
            path_or_buf='./output/' + output_dir + '/' + color_name + '.csv', index=False)
        print('\t\t>>Write data to ./output/' +
              output_dir + '/' + color_name + '.csv\n\n')
    get_distribution_of_distributions(distributions)


def get_images_vectorData(image_list):
    data = {}
    print('\n\n\n\n>>Get vector data of images')
    vectorData = {}
    for color_index, color_name in enumerate(RGB):
        vectorData[color_name] = {}
        print('\t\t>>Get info about ' + str(color_name) + ' color')
        data[color_name] = pd.DataFrame()
        for image_index, image_name in tqdm(enumerate(image_list)):
            image_info = get_image_info_vectors(
                image_index, image_name, color_index)
            for parameter in image_info:
                if not parameter in vectorData[color_name]:
                    vectorData[color_name][parameter] = []
                vectorData[color_name][parameter].append(image_info[parameter])

        output_file_path = './output/' + output_dir + '/vectorData/' + color_name + '.csv'
        output_file = open(output_file_path, "w")
        writer = csv.writer(output_file)
        vectorDataParameters = vectorData[color_name].keys()
        writer.writerow(vectorDataParameters)
        for i in tqdm(range(len(vectorData[color_name][vectorDataParameters[0]]))):
            row = []
            for parameter in vectorDataParameters:
                row.append(vectorData[color_name][parameter][i])
            writer.writerow(row)
        print('\t\t>>Write data to ' + output_file_path + '\n\n')
    return vectorData


def create_vectors(vectorData):
    print('>>Creating vectors')
    vector = []
    for color_name in vectorData:
        v = np.array([vectorData[color_name]['mean'], vectorData[color_name]['var'],
                      vectorData[color_name]['skewness'], vectorData[color_name]['kurtosis']])
        vector.append(v)

    vector = np.array(vector)

    output_file_path = './output/' + output_dir + '/vectors/vector.txt'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(vector)

    matrix = []
    vectorDataParameters = vectorData[color_name].keys()
    for parameter in vectorDataParameters:
        m = np.array((vectorData['red'][parameter], vectorData['green']
        [parameter], vectorData['blue'][parameter]))
        matrix.append(m)

    output_file_path = './output/' + output_dir + '/vectors/mean_matrix.txt'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(matrix)

    mean_var_vector = ((vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'],
                        vectorData['red']['var'], vectorData['green']['var'], vectorData['blue']['var']))

    output_file_path = './output/' + output_dir + '/vectors/mean_var_vector.txt'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_var_vector)

    mean_var_skew_vector = ((vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'],
                             vectorData['red']['var'], vectorData['green']['var'], vectorData['blue']['var'],
                             vectorData['red']['skewness'], vectorData['green']['skewness'],
                             vectorData['blue']['skewness']))

    output_file_path = './output/' + output_dir + '/vectors/mean_var_skew_vector.txt'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_var_skew_vector)

    mean_var_skew_kurt_vector = ((vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'],
                                  vectorData['red']['var'], vectorData['green']['var'], vectorData['blue']['var'],
                                  vectorData['red']['skewness'], vectorData['green']['skewness'],
                                  vectorData['blue']['skewness'],
                                  vectorData['red']['kurtosis'], vectorData['green']['kurtosis'],
                                  vectorData['blue']['kurtosis']))
    output_file_path = './output/' + output_dir + \
                       '/vectors/mean_var_skew_kurt_vector.txt'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_var_skew_kurt_vector)


def get_gauss_model(vectorData):
    print('>>Creating gauss models')
    mean_g_v = np.cov(np.vstack(
        (vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'])))
    output_file_path = './output/' + output_dir + '/gaussModels/mean_g_v.txt'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_g_v)

    mean_var_g_v = np.cov(np.vstack((vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'],
                                     vectorData['red']['var'], vectorData['green']['var'], vectorData['blue']['var'])))
    output_file_path = './output/' + output_dir + '/gaussModels/mean_var_g_v.txt'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_var_g_v)

    mean_var_skew_g_v = np.cov(
        np.vstack((vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'],
                   vectorData['red']['var'], vectorData['green']['var'], vectorData['blue']['var'],
                   vectorData['red']['skewness'], vectorData['green']['skewness'], vectorData['blue']['skewness'])))
    output_file_path = './output/' + output_dir + '/gaussModels/mean_var_skew_g_v.txt'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_var_skew_g_v)

    mean_var_skew_kurt_g_v = np.cov(
        np.vstack((vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'],
                   vectorData['red']['var'], vectorData['green']['var'], vectorData['blue']['var'],
                   vectorData['red']['skewness'], vectorData['green']['skewness'], vectorData['blue']['skewness'],
                   vectorData['red']['kurtosis'], vectorData['green']['kurtosis'], vectorData['blue']['kurtosis'])))
    output_file_path = './output/' + output_dir + \
                       '/gaussModels/mean_var_skew_kurt_g_v.txt'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_var_skew_kurt_g_v)


def PCA(img_2d, nmpc):
    cov_mat = img_2d - np.mean(img_2d)
    val, vec = np.linalg.eigh(np.cov(cov_mat))
    p = np.size(vec, axis=1)
    i = np.argsort(val)
    i = i[::-1]
    vec = vec[:, i]
    val = val[i]

    if (nmpc < p) or (nmpc > 0):
        vec = vec[:, range(nmpc)]

    score = np.dot(vec.T, cov_mat)
    rcn = np.dot(vec, score) + np.mean(img_2d).T
    rcn_img_mat = np.uint8(np.absolute(rcn))
    return rcn_img_mat


def get_mse(first_img, second_img):
    err = np.sum((first_img.astype("float") - second_img.astype("float")) ** 2)
    err /= float(first_img.shape[0] * first_img.shape[1])

    return err


def decompose(files):
    print('>>Decomposing images')
    decomposed_data = {}
    for file_path in tqdm(files):
        img_data = img.imread(file_path)
        values = np.zeros((3, 256))
        for i in range(img_data.shape[0]):
            for j in range(img_data.shape[1]):
                values[0][img_data[i][j][0]] += 1
                values[1][img_data[i][j][1]] += 1
                values[2][img_data[i][j][2]] += 1

        test = img_data
        test_np = np.array(test)
        red = test_np[:, :, 0]
        green = test_np[:, :, 1]
        blue = test_np[:, :, 2]

        number_of_comp = [5, 30, 80]
        filename = file_path.split('.')[1].split('/')[-1]

        if not os.path.exists('./output/' + output_dir + '/PCA/' + str(filename)):
            os.makedirs('./output/' + output_dir + '/PCA/' + str(filename))

        plt.figure(figsize=(20, 20))
        plt.title('original')
        plt.imshow(img_data)
        plt.savefig('./output/' + output_dir + '/PCA/' +
                    str(filename) + '/original.png')

        for number in number_of_comp:
            red_rcn, green_rcn, blue_rcn = PCA(red, number), PCA(
                green, number), PCA(blue, number)
            rcn_img = np.dstack((red_rcn, green_rcn, blue_rcn))
            plt.figure()
            plt.title(str(number) + ' components')
            plt.imshow(rcn_img)
            plt.savefig('./output/' + output_dir + '/PCA/' +
                        str(filename) + '/' + str(number) + '_components.png')

        all_mse = list()
        for i in range(100):
            test_r_recon, test_g_recon, test_b_recon = PCA(
                red, i), PCA(green, i), PCA(blue, i)
            rcn_img = np.dstack((test_r_recon, test_g_recon, test_b_recon))
            all_mse.append(get_mse(test, rcn_img))

        plt.figure()
        plt.plot(range(len(all_mse)), all_mse)
        plt.xlabel("components")
        plt.ylabel("MSE")
        plt.savefig('./output/' + output_dir + '/PCA/' + str(filename) + '/mse.png')
        decomposed_data[filename] = {'red': red, 'green': green, 'blue': blue}
    return decomposed_data


def get_markov_chain(decomposed_data):
    print('>>Creating markov chains')
    for mean in tqdm(decomposed_data):
        colors = ['red', 'green', 'blue']
        m = 0
        for key in decomposed_data[mean]:
            matrix1 = np.zeros(shape=(256, 256))
            array = decomposed_data[mean][key].flatten('F')
            prev_color = array[0]
            for i in range(len(array) - 1):
                matrix1[array[i]][array[i + 1]] += 1
            matrix = matrix1[0] / sum(matrix1[0])
            for i in range(1, 256):
                matrix = np.vstack((matrix, matrix1[i] / sum(matrix1[i])))
            matrix_2 = np.linalg.matrix_power(matrix, 2)

            if not os.path.exists('./output/' + output_dir + '/markovChain/' + str(mean)):
                os.makedirs('./output/' + output_dir + '/markovChain/' + str(mean))

            output_file_path = './output/' + output_dir + '/markovChain/' + str(mean) + '/' + colors[m] + '_matrix.txt'
            with open(output_file_path, 'w+') as f:
                csv.writer(f, delimiter=' ').writerows(matrix)

            output_file_path = './output/' + output_dir + '/markovChain/' + str(mean) + '/' + colors[
                m] + '_matrix_2.txt'
            with open(output_file_path, 'w+') as f:
                csv.writer(f, delimiter=' ').writerows(matrix_2)

            i = 0
            matrix_list = [matrix, matrix_2]
            for mat in matrix_list:
                if i == 0:
                    title = 'first_order'
                else:
                    title = 'second_order'
                file_checks = open('./output/' + output_dir + '/markovChain/' + str(mean) + '/' + colors[m] + '_' + str(
                    title) + '.txt', "w+")
                file_checks.write('Проверка свойств матрицы ' + str(title) + ' ' + colors[m] + '')
                count = 0
                for k in range(6):
                    reg = matrix_power(mat, k)
                    for i in range(256):
                        if reg[i][i] == 0:
                            count += 1
                if count == 0:
                    file_checks.write('Свойство регулярности выполняется\n')
                    file_checks.write('Свойство рекуррентности выполняется\n')
                else:
                    file_checks.write('Свойство регулярности не выполняется\n')
                    file_checks.write('Свойство рекуррентности не выполняется\n')

                count = 0
                for i in range(256):
                    for j in range(256):
                        if (mat[i][i] == 1) or (mat[i][j] == 0):
                            count += 1
                if count == 0:
                    file_checks.write('Свойство необратимости выполняется\n')
                else:
                    file_checks.write('Свойство необратимости не выполняется\n')
                file_checks.close()
                data = np.triu(mat) + np.triu(mat).T
                index = [str(p) for p in range(data.shape[0])]
                dataframe = pd.DataFrame(data, index=index, columns=index)
                plt.figure(figsize=(40, 40))
                plt.title(str(colors[m]) + ' ' + title)
                g = nx.from_pandas_adjacency(dataframe)
                nx.draw(g, with_labels=True, node_color=colors[m], font_color='black')
                plt.savefig('./output/' + output_dir + '/markovChain/' + str(mean) + '/' + str(colors[m]) + '.png')
                i += 1

            m += 1


def get_tags(name_list):
    file_list_rand = create_list_files(target_format_in='.txt',
                                       path_in='./input/mirflickr/tags/')

    num_list = []
    for n in name_list:
        num_list.append(int((n.split('.')[-2]).split('m')[-1]))

    n = 0
    labels_list = []
    for file in file_list_rand:
        df1 = {
            'name': 0,
            'label': None,
        }
        nam = 'im' + str(((file.split('/')[-1]).split('.')[-2]).split('s')[-1]) + '.jpg'
        try:
            tags = (open(file, 'r')).readlines()
            tags = [line.rstrip() for line in tags]
            if 'night' in tags:
                df1['name'] = nam
                df1['label'] = 0
                n += 1
            else:
                df1['name'] = nam
                df1['label'] = 1
            labels_list.append(df1)
        except:
            df1['name'] = nam
            df1['label'] = 1
            labels_list.append(df1)
            pass

    return pd.DataFrame(labels_list), n


def get_signs_matrix(n):
    file_list_rand = create_list_files(target_format_in='.jpg',
                                       path_in='./input/mirflickr/')  # путь

    signs = ['mean', 'var', 'skew', 'kurt']
    RGB = {'red': 0,
           'green': 1,
           'blue': 2}
    data = {}
    for name, num in RGB.items():

        RGB[name] = {'mean': [],
                     'var': [],
                     'skew': [],
                     'kurt': []}

        data[name] = pd.DataFrame()
        for image_name in file_list_rand[:n]:
            image = np.array(Image.open(image_name))
            a = image[:, num].ravel()
            df = {'name': image_name.split('/')[-1],
                  'mean': np.mean(a),
                  'var': np.var(a),
                  'skewness': sp.stats.skew(a),
                  'kurtosis': sp.stats.kurtosis(a)
                  }

            data[name] = pd.concat([data[name], pd.DataFrame(pd.DataFrame(df, index=[0, ]))], ignore_index=True)

            RGB[name]['mean'].append(round(df['mean'], 3))
            RGB[name]['var'].append(round(df['var'], 3))
            RGB[name]['skew'].append(round(df['skewness'], 3))
            RGB[name]['kurt'].append(round(df['kurtosis'], 3))

    name_list = data['red']['name'].to_list()

    matrix = []
    for sign in signs:
        m = np.array((RGB['red'][sign], RGB['green'][sign], RGB['blue'][sign]))
        matrix.append(m)
    matrix = np.array(matrix)
    output_file_path = './output/' + output_dir + '/matrix.csv'
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(matrix)

    return name_list, data


def static_classifiers(data, n):
    df = pd.read_csv(data)
    true_label = df[(df.label == 0)].sample(frac=1).reset_index(drop=True)[:n]
    false_label = df[(df.label == 1)].sample(frac=1).reset_index(drop=True)[:n]
    DF = pd.concat([true_label, false_label])
    DF.to_csv('./output/' + output_dir + '/data.csv', index=False)
    df = pd.read_csv(data)
    X = df.iloc[:, 2:-1]
    y = df['label']
    for model in model_list:
        print(str(model))
        confusion_list = []
        accuracy_list = []
        for t in tqdm(range(10)):
            print(str(t + 1) + ' iteration')
            X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=42)
            model.fit(X_train, y_train)
            pred = model.predict(X_test)
            confusion_1 = 0
            y_test = np.array(y_test)
            for i in range(len(pred)):
                if pred[i] != y_test[i]:
                    confusion_1 += 1

            confusion_2 = 1
            for j in range(len(pred)):
                if y_test[i] == 0 and pred[i] != y_test[i]:
                    confusion_2 += 1
            confusion_list.append(confusion_1 + confusion_2)
            accuracy_list.append(accuracy_score(y_test, pred.round()))
        print('average error:')
        print(sum(confusion_list) / len(confusion_list))
        print('average accuracy:')
        print(sum(accuracy_list) / len(accuracy_list))

def SPAM_classifiers(data, n):
    df = pd.read_csv(data)
    true_label = df[(df.label == 0)].sample(frac=1).reset_index(drop=True)[:n]
    false_label = df[(df.label == 1)].sample(frac=1).reset_index(drop=True)[:n]
    DF = pd.concat([true_label, false_label])
    DF.to_csv('./output/' + output_dir + '/data.csv', index=False)
    df = pd.read_csv(data)
    X = df.iloc[:, 2:-1]
    y = df['label']

    for model in model_list:
        print(str(model))
        confusion_list = []
        accuracy_list = []
        for t in tqdm(range(10)):
            print(str(t + 1) + ' iteration')
            X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=42)
            model.fit(X_train, y_train)
            pred = model.predict(X_test)
            confusion_1 = 0
            y_test = np.array(y_test)
            for i in range(len(pred)):
                if pred[i] != y_test[i]:
                    confusion_1 += 1
            confusion_2 = 1
            for j in range(len(pred)):
                if y_test[i] == 0 and pred[i] != y_test[i]:
                    confusion_2 += 1
            confusion_list.append(confusion_1 + confusion_2)
            accuracy_list.append(accuracy_score(y_test, pred.round()))
        print('average error:')
        print(sum(confusion_list) / len(confusion_list))
        print('average accuracy:')
        print(sum(accuracy_list) / len(accuracy_list))

def CCPEV_classifiers(data, n):
    df = pd.read_csv(data)
    true_label = df[(df.label == 0)].sample(frac=1).reset_index(drop=True)[:n]
    false_label = df[(df.label == 1)].sample(frac=1).reset_index(drop=True)[:n]
    DF = pd.concat([true_label, false_label])
    DF.to_csv('./output/' + output_dir + '/data.csv', index=False)
    df = pd.read_csv(data)
    X = df.iloc[:, 2:-1]
    y = df['label']

    for model in model_list:
        print(str(model))
        confusion_list = []
        accuracy_list = []
        for t in tqdm(range(10)):
            print(str(t + 1) + ' iteration')
            X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=42)
            model.fit(X_train, y_train)
            pred = model.predict(X_test)
            confusion_1 = 0
            y_test = np.array(y_test)
            for i in range(len(pred)):
                if pred[i] != y_test[i]:
                    confusion_1 += 1
            confusion_2 = 1
            for j in range(len(pred)):
                if y_test[i] == 0 and pred[i] != y_test[i]:
                    confusion_2 += 1
            confusion_list.append(confusion_1 + confusion_2)
            accuracy_list.append(accuracy_score(y_test, pred.round()))
        print('average error:')
        print(sum(confusion_list) / len(confusion_list))
        print('average accuracy:')
        print(sum(accuracy_list) / len(accuracy_list))
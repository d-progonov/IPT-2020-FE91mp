#! /usr/bin/env python
# -*- coding: utf-8 -*-
import os
import re
import sys
from collections import defaultdict
from pprint import pprint

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

RGB = ['red', 'green', 'blue']
DISTRIBUTIONS = ['beta', 'gamma', 'laplace', 'norm', 'pareto']

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
                print('\t'+str(key)+': ' + str(config[key]))
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
                file_list.append(root+'/' + curr_file)
    file_list.sort(key=natural_keys)
    return file_list


def create_index_list(file_list, count):
    print('\n>>Creating list of indexes')
    index_list = np.random.random_integers(0, len(file_list)-1, count)
    filtered_file_list = list()
    for index in tqdm(index_list):
        filtered_file_list.append(file_list[index])
    return index_list, filtered_file_list


def get_image_info_vectors(image_index, image_name, color_index):
    image = np.array(Image.open(image_name))
    a = image[:, color_index].ravel()
    d = {
        'mean': np.nanmean(a),   # среднеарифметическое
        'var': np.nanvar(a),  # дисперсия
        'skewness': sp.stats.skew(a),  # коэффициент асимметрии
        # коэффицие́нт эксце́сса (нормализированный)
        'kurtosis': sp.stats.kurtosis(a)-3,
    }
    return d


def get_image_info(image_index, image_name, color_index):
    image = np.array(Image.open(image_name))
    a = image[:, color_index].ravel()
    d = {
        'name': image_name,  # название файла
        'min': np.nanmin(a),  # минимум
        'max': np.nanmax(a),  # максимум
        'mean': np.nanmean(a),   # среднеарифметическое
        'var': np.nanvar(a),  # дисперсия
        'median': np.nanmedian(a),  # медиана
        'average': np.average(a),  # средневзвешенное(мат ожидание)
        'std': np.nanstd(a),  # среднеквадратичное (стандартное) отклонение
        'skewness': sp.stats.skew(a),  # коэффициент асимметрии
        # коэффицие́нт эксце́сса (нормализированный)
        'kurtosis': sp.stats.kurtosis(a)-3,
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
    print('Color: ' + str(color_name) + ' | Image ' + str(image_name)+'\n')
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
    plt.savefig('./output/'+output_dir+'/'+str(color_name) +
                '_histograms/'+str(image_index)+'.png')
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
        print('\t\t>>Get info about '+str(color_name)+' color')
        data[color_name] = pd.DataFrame()
        for image_index, image_name in tqdm(enumerate(image_list)):
            image_info = get_image_info(image_index, image_name, color_index)
            distributions[color_name][image_info['best distribution']] += 1
            data[color_name] = pd.concat([data[color_name], pd.DataFrame(
                pd.DataFrame(image_info, index=[0, ]))], ignore_index=True)
        data[color_name].to_csv(
            path_or_buf='./output/'+output_dir+'/'+color_name+'.csv', index=False)
        print('\t\t>>Write data to ./output/' +
              output_dir+'/'+color_name+'.csv\n\n')
    get_distribution_of_distributions(distributions)


def get_images_vectorData(image_list):
    data = {}
    print('\n\n\n\n>>Creating vectors of images')
    vectorData = {}
    for color_index, color_name in enumerate(RGB):
        vectorData[color_name] = {}
        print('\t\t>>Get info about '+str(color_name)+' color')
        data[color_name] = pd.DataFrame()
        for image_index, image_name in tqdm(enumerate(image_list)):
            image_info = get_image_info_vectors(image_index, image_name, color_index)
            for parameter in image_info:
                if not parameter in vectorData[color_name]:
                    vectorData[color_name][parameter] = []
                vectorData[color_name][parameter].append(image_info[parameter])

        output_file_path='./output/'+output_dir+'/vectorData/'+color_name+'.csv'
        output_file = open(output_file_path, "w")
        writer = csv.writer(output_file)
        vectorDataParameters = vectorData[color_name].keys()
        writer.writerow(vectorDataParameters)
        for i in tqdm(range(len(vectorData[color_name][vectorDataParameters[0]]))):
            row = []
            for parameter in vectorDataParameters:
                row.append(vectorData[color_name][parameter][i])
            writer.writerow(row)
        print('\t\t>>Write data to '+output_file_path+'\n\n')
    return vectorData

def create_vectors(vectorData):
    vector = []
    for color_name in vectorData:
        v = np.array([vectorData[color_name]['mean'], vectorData[color_name]['var'], vectorData[color_name]['skewness'], vectorData[color_name]['kurtosis']])
        vector.append(v)

    vector = np.array(vector)

    output_file_path='./output/'+output_dir+'/vectors/vector.txt' 
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(vector)

    matrix = []
    vectorDataParameters = vectorData[color_name].keys()
    for parameter in vectorDataParameters:
        m = np.array((vectorData['red'][parameter], vectorData['green'][parameter], vectorData['blue'][parameter]))
        matrix.append(m)

    
    # print('Mean:\n')
    # print(matrix)
    output_file_path='./output/'+output_dir+'/vectors/mean_matrix.txt' 
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(matrix)

    mean_var_vector= ((vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'],
             vectorData['red']['var'], vectorData['green']['var'], vectorData['blue']['var']))
    # print('Mean and var:\n')
    # print(mean_var_v)

    output_file_path='./output/'+output_dir+'/vectors/mean_var_vector.txt' 
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_var_vector)

    mean_var_skew_vector= ((vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'],
             vectorData['red']['var'], vectorData['green']['var'], vectorData['blue']['var'],
             vectorData['red']['skewness'], vectorData['green']['skewness'], vectorData['blue']['skewness']))
    # print('Mean and var and skew:\n')
    # print(mean_var_skew_v)

    output_file_path='./output/'+output_dir+'/vectors/mean_var_skew_vector.txt' 
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_var_skew_vector)

    mean_var_skew_kurt_vector= ((vectorData['red']['mean'], vectorData['green']['mean'], vectorData['blue']['mean'],
             vectorData['red']['var'], vectorData['green']['var'], vectorData['blue']['var'],
             vectorData['red']['skewness'], vectorData['green']['skewness'], vectorData['blue']['skewness'],
             vectorData['red']['kurtosis'], vectorData['green']['kurtosis'], vectorData['blue']['kurtosis']))
    # print('Mean and var and skew and kurt:\n')
    # print(mean_var_skew_kurt_v)
    output_file_path='./output/'+output_dir+'/vectors/mean_var_skew_kurt_vector.txt' 
    with open(output_file_path, 'w') as f:
        csv.writer(f, delimiter=' ').writerows(mean_var_skew_kurt_vector)
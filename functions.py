#! /usr/bin/env python
# -*- coding: utf-8 -*-
import os
import re
from collections import defaultdict
from pprint import pprint

import numpy as np

import matplotlib.pyplot as plt
import pandas as pd
import scipy as sp
import yaml
from fitter import Fitter
from PIL import Image
from scipy import stats
from tqdm import tqdm

RGB = ['red','green','blue']
distributions={'beta':0, 'gamma':0, 'laplace':0, 'norm':0, 'pareto':0}

def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    '''
    alist.sort(key=natural_keys) sorts in human order
    http:#nedbatchelder.com/blog/200712/human_sorting.html
    (See Toothy's implementation in the comments)
    '''
    return [ atoi(c) for c in re.split(r'(\d+)', text) ]

def parse_config(path_in='config.yaml'):
    print('>>Parsing config')
    with open(path_in, 'r') as stream:
        try:
            
            config = yaml.safe_load(stream) 
            print('\t>>Parsed:')
            for key in config:
                print('\t'+str(key)+': '+ str(config[key])) 
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
        print('\n\t>>Scanning folder: '+ str(root))
        for curr_file in tqdm(files):
            if target_format_in in curr_file:
                file_list.append(root+'/' + curr_file)
    file_list.sort(key=natural_keys)
    return file_list

def create_index_list(file_list,count):
    print('\n>>Creating list of indexes')
    index_list = np.random.random_integers(0,len(file_list)-1,count)
    filtered_file_list=list()
    for index in tqdm(index_list):
        filtered_file_list.append(file_list[index])
    return index_list,filtered_file_list

def get_image_info(image_index,image_name,color_index):
    image = np.array(Image.open(image_name))
    a = image[ :, color_index].ravel() 
    d = {
        'name': image_name,  # название файла
        'min': np.nanmin(a),  # минимум
        'max': np.nanmax(a),  # максимум
        'mean': np.nanmean(a),   # среднеарифметическое
        'var': np.nanvar(a), # дисперсия
        'median': np.nanmedian(a),  # медиана
        'average': np.average(a), # средневзвешенное(мат ожидание)
        'std': np.nanstd(a), # среднеквадратичное (стандартное) отклонение
        'skewness': sp.stats.skew(a), # коэффициент асимметрии
        'kurtosis': sp.stats.kurtosis(a), # коэффицие́нт эксце́сса
        'interquartile range': sp.stats.iqr(a), # интерквартильный размах
        'best distribution': get_image_histogram(image_index,image_name)
        } 
    return d

def get_image_histogram(image_index,image_name):
    print('\n\n\t>>Get histogram about:  '+ image_name)
    for color_index,color_name in enumerate(RGB): 
        plt.figure() 
        image = np.array(Image.open(os.path.join(image_name))) 
        a = image[ :, color_index].ravel() 
        f = Fitter(a, distributions=distributions.keys(), bins=256) 
        f.fit() 
        pprint(f.summary()) 
        f.hist(); 
        best_distribution = f.get_best()
        print('\n\n\n********************************')
        print(best_distribution)
        print('********************************\n\n\n')
        plt.xlim(0,255)
        plt.xlabel('Brightness')
        plt.ylabel('Frequency')
        plt.title('Image '+ str(image_name)+ ' with index '+ str(image_index))
        plt.savefig('./output/lab1/'+str(color_name)+'_histograms/'+str(image_index)+'.png')           
    return best_distribution.keys()[0]

def get_images_info(image_list):
    data = {}
    print('\n\n\n\n>>Analyzing images')
    for color_index,color_name in enumerate(RGB): 
        print('\t\t>>Get info about '+str(color_name)+' color')
        data[color_name] = pd.DataFrame() 
        for image_index,image_name in tqdm(enumerate(image_list)):
           data[color_name] = pd.concat([data[color_name], pd.DataFrame(pd.DataFrame(get_image_info(image_index,image_name,color_index), index=[0, ]))], ignore_index=True)
        data[color_name].to_csv(path_or_buf='./output/lab1/'+color_name+'.csv',index=False)  
        print('\t\t>>Write data to ./output/lab1/'+color_name+'.csv\n\n')
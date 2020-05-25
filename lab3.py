#!/usr/bin/env python3
# -*- coding: utf-8 -*-.
import functions as lib


if __name__ == "__main__":
    print('>>Start lab3\n')
    config = lib.parse_config()
    lib.config_random_generator(config['variant'])
    files = lib.create_list_files()
    index_list, files = lib.create_index_list(files,config['countImages'])
    name_list, data = lib.get_signs_matrix(config['countImages'])
    labels, n = lib.get_tags(name_list)
    print('DATA RED')
    print(data['red'])
    print('LABELS')
    print(labels)
    red_data = data['red'].merge(labels, left_on='name', right_on='name')
    # green_data = data['GREEN'].merge(labels, left_on='name', right_on='name')
    # blue_data = data['BLUE'].merge(labels, left_on='name', right_on='name')
    red_data.to_csv('./output/lab3/red.csv')
    # green_data.to_csv('green.csv')
    # blue_data.to_csv('blue.csv')
    lib.static_classifiers('./output/lab3/red.csv', n)
    lib.SPAM_classifiers('./output/lab3/SPAM_red.csv', n)
    lib.CCPEV_classifiers('./output/lab3/CC-PEV_red.csv', n)
    print('>>Finish lab3\n')
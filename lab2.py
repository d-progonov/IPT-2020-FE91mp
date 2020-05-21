#! /usr/bin/env python
# -*- coding: utf-8 -*-
import functions as lib


if __name__ == "__main__":
    print('>>Start lab2\n')
    config = lib.parse_config()
    lib.config_random_generator(config['variant'])
    files = lib.create_list_files() 
    index_list,files = lib.create_index_list(files,config['countImages'])
    vectorData = lib.get_images_vectorData(files)
    lib.create_vectors(vectorData)
    lib.get_gauss_model(vectorData)
    decomposed_data = lib.decompose(files)
    lib.get_markov_chain(decomposed_data)
    print('>>Finish lab2\n')
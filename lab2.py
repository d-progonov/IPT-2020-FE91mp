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
    vectors = lib.create_vectors(vectorData)
    print('>>Finish lab2\n')
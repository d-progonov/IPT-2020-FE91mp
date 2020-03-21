#! /usr/bin/env python
# -*- coding: utf-8 -*-
import functions as lib


if __name__ == "__main__":
    print('>>Start lab1\n')
    config = lib.parse_config()
    lib.config_random_generator(config['variant'])
    files = lib.create_list_files() 
    index_list,files = lib.create_index_list(files,config['countImages'])
    data = lib.get_images_info(files)
    print('>>Finish lab1\n')
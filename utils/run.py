import os

import core
import lib

filenames = os.listdir('data')
for filename in filenames:
    if filename.find('data_cycle') == 0:
        with open('data/' + filename, 'rb') as file:
            datapoint = file.read()
        words = lib.split_word(datapoint)
        try:
            result = core.gen_chain_word(words, 0, 0, False)
            print(filename, len(result))
        except:
            print(filename, "CYCLE")


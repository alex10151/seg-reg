import os
import re
import copy

path = "C:/Users/ALEX_HSW/source/repos/Project2/Project2/1/trans"
#pat = re.compile(r"[\w.)(]+.IMA$")
pat = re.compile(r"[\w.)(]+.tif$")
dirs = os.listdir(path)
if __name__ == '__main__':
    for name in dirs:
        if re.match(pat, name) is not None:
            # print(name)
            # index = name.split(".")
            res = name.replace('tif', 'dcm')
            print(res)
            os.rename(path + "/" + name, path + "/" + res)

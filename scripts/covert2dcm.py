import os
import re
import copy

dir_root = "C:/Users/ALEX_HSW/Desktop/CT去骨测试数据/data"
# path = "C:/Users/ALEX_HSW/source/repos/Project2/Project2/1/trans"
# pat = re.compile(r"[\w.)(]+.IMA$")
pat = re.compile(r"[\w.)(]+.IMA$")

if __name__ == '__main__':
    for name in os.listdir(dir_root):
        name = dir_root + "/" + name + "/FDG/"
        for name2 in os.listdir(name):
            name2 = name + name2
            for name3 in os.listdir(name2):
                if re.match(pat, name3) is not None:
                    # print(name)
                    # index = name.split(".")
                    res = name3.replace('IMA', 'dcm')
                    print(res)
                    os.rename(name2 + "/" + name3, name2 + "/" + res)
                else:
                    res = name3 + ".dcm"
                    print(res)
                    os.rename(name2 + "/" + name3, name2 + "/" + res)

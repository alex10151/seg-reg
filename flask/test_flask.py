import base64
import numpy as np
from functools import partial
import sys
import json
import requests

data_seg = {
    "type": "-headbone",
    "bones": True,
    "values": "AACAPwAAgD8AAIA_AACAPwAAgD8AAIA_AACAPwAAgD8=",
    "size": [2, 2, 2],
    "space": [0.5, 0.5, 0.5],
    "origin": [0, 0, 0]
}
data_reg = {
    "fix_values": "AACAPwAAgD8AAIA_AACAPwAAgD8AAIA_AACAPwAAgD8=",
    "mov_values": "AwAAAAAAAAAEAAAAAAAAAAAAAAAGAAAAAAAAAAgAAAA=",
    "size_fix": [2, 2, 2],
    "size_mov": [2, 2, 2],
    "space_fix": [0.5, 0.5, 0.5],
    "space_mov": [0.5, 0.5, 0.5],
    "origin_fix": [0, 0, 0],
    "origin_mov": [0, 0, 0],
}
if __name__ == '__main__':
    # a = np.zeros([2, 2, 2], dtype=np.int)
    # for i in range(1, 3):
    #     for j in range(1, 3):
    #         for k in range(1, 3):
    #             if i == k:
    #                 a[i - 1, j - 1, k - 1] = i * 2 + j * k
    # print(a)
    # a = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2]
    # a = np.array(a)
    # a = a.tobytes(order="c")
    # res = base64.urlsafe_b64encode(a)
    # res = str(res)
    # res = res.split("'")
    # res = res[1]
    # res = base64.urlsafe_b64decode(res)
    # res = np.fromstring(res, dtype="<u4")
    # print(res)
    # a = np.array([2,2,2,2,2,2,2,2,2],dtype=np.float32)
    # a = int.from_bytes(a.tobytes(order='c'),byteorder="big")
    # print(a)
    data = dict()
    with open("CT.raw", 'rb+')as f:
        it = iter(partial(f.read, 2), b"")
        res = []
        data["size_fix"] = [512, 512, 206]
        data["space_fix"] = [0.9765625, 0.9765625, 4]
        data["origin_fix"] = [0, 0, 0]
        data["bones"] = True
        data["type"] = "-reg"
        count = 0
        for i in it:
            print(count)
            res.append(int.from_bytes(i, byteorder="big"))
            count = count + 1
        res = base64.urlsafe_b64encode(np.array(res, dtype=np.uint16).tobytes(order='c'))
        res = str(res).split("'")[1]
        data["fix_values"] = res
    with open("PET2.raw", 'rb+')as f:
        it = iter(partial(f.read, 2), b"")
        res = []
        data["size_mov"] = [168, 168, 206]
        data["space_mov"] = [2.90917, 2.90917, 4]
        data["origin_mov"] = [0, 0, 0]
        count = 0
        for i in it:
            print(count)
            res.append(int.from_bytes(i, byteorder="big"))
            count = count + 1
        res = base64.urlsafe_b64encode(np.array(res, dtype=np.uint16).tobytes(order='c'))
        res = str(res).split("'")[1]
        data["mov_values"] = res
    headers = {'Content-Type': 'application/json'}
    r = requests.post("http://127.0.0.1:8080/reg", headers=headers, data=json.dumps(data))
    res = r.json()['res_img']
    # resb = r.json()['res_bones']
    res = base64.urlsafe_b64decode(res)
    # resb = base64.urlsafe_b64decode(resb)
    # print(res[0] + res[1] * 2 ** 8)
    # res_c = []
    # for i in range(1, len(res), 2):
    #     res_c.append(res[i - 1] + res[i] * 2 ** 8)
    with open("res2.raw", 'wb') as f1:
        f1.write(res)
    # with open("resb2.raw", 'wb') as f2:
    #     f2.write(resb)

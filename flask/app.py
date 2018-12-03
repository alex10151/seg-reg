from flask import Flask
from flask_restful import Resource, Api, request, marshal_with, fields, marshal, reqparse, abort
import base64
import numpy as np
import SegAndRegPipe as sg

app1 = Flask(__name__)

api = Api(app1)


class SegDao(object):
    def __init__(self, res_img, res_bones):
        res_img = res_img.tobytes(order='c')
        res_bones = res_bones.tobytes(order='c')
        self.res_img = base64.urlsafe_b64encode(res_img)
        self.res_bones = base64.urlsafe_b64encode(res_bones)


class RegDao(object):
    def __init__(self, res_img):
        res_img = res_img.tobytes(order='c')
        self.res_img = base64.urlsafe_b64encode(res_img)


@api.resource("/seg")
class Segmentation(Resource):
    seg_out_fields = {
        "res_img": fields.String,
        "res_bones": fields.String,
    }
    keys = ["size", "space", "origin", "type", "values", "bones"]
    type_keys = ["-headbone", "-bodybone", "-table"]

    def is_valid(self, data):
        for key in self.keys:
            if key not in data.keys():
                return False
        if "size" in data.keys():
            if len(data['size']) != 3:
                return False
            if any(map(lambda x: x <= 0, data["size"])) is True:
                return False
        if "space" in data.keys():
            if len(data['space']) != 3:
                return False
            if any(map(lambda x: x < 0, data["space"])) is True:
                return False
        if "origin" in data.keys():
            if len(data['origin']) != 3:
                return False
            if any(map(lambda x: x < 0, data["origin"])) is True:
                return False
        if "type" in data.keys():
            if data["type"] not in self.type_keys:
                return False
        return True

    @marshal_with(seg_out_fields)
    def post(self):
        data = request.json
        if self.is_valid(data) is False:
            return abort(400)
        space = sg.FloatArray(3)
        origin = sg.FloatArray(3)
        size = sg.UintArray(3)
        for i in [0, 1, 2]:
            space[i] = data["space"][i]
            size[i] = data["size"][i]
            origin[i] = data["origin"][i]
        len_img = size[0] * size[1] * size[2]
        src_img = sg.ShortArray(len_img)
        res_img = sg.ShortArray(len_img)
        bones_img = sg.ShortArray(len_img)
        src = base64.urlsafe_b64decode(data["values"])
        res = []
        for i in range(1, len(src), 2):
            res.append(src[i - 1] + src[i] * 2 ** 8)
        for i in range(len_img):
            src_img[i] = res[i]
        try:
            sg.RemovalPipelinev2(data['type'], src_img, res_img, bones_img, size, space, origin)
        except:
            return abort(500)
        res = []
        res_bones = []
        for i in range(len_img):
            res.append(res_img[i])
            if data['bones'] is True:
                res_bones.append(bones_img[i])
        return SegDao(np.array(res, dtype=np.uint16), np.array(res_bones, dtype=np.uint16)), 200


@api.resource("/reg")
class Registration(Resource):
    reg_out_fields = {
        "res_img": fields.String
    }
    keys = ["size_fix", "space_fix", "origin_fix", "size_mov", "space_mov", "origin_mov", "fix_values", "mov_values"]

    def is_valid(self, data):
        for key in self.keys:
            if key not in data.keys():
                return False
        if "size_fix" in data.keys():
            if len(data['size_fix']) != 3:
                return False
            if any(map(lambda x: x <= 0, data["size_fix"])) is True:
                return False
        if "space_fix" in data.keys():
            if len(data['space_fix']) != 3:
                return False
            if any(map(lambda x: x < 0, data["space_fix"])) is True:
                return False
        if "origin_fix" in data.keys():
            if len(data['origin_fix']) != 3:
                return False
            if any(map(lambda x: x < 0, data["origin_fix"])) is True:
                return False
        if "size_mov" in data.keys():
            if len(data['size_mov']) != 3:
                return False
            if any(map(lambda x: x <= 0, data["size_mov"])) is True:
                return False
        if "space_mov" in data.keys():
            if len(data['space_mov']) != 3:
                return False
            if any(map(lambda x: x < 0, data["space_mov"])) is True:
                return False
        if "origin_mov" in data.keys():
            if len(data['origin_mov']) != 3:
                return False
            if any(map(lambda x: x < 0, data["origin_mov"])) is True:
                return False
        return True

    @marshal_with(reg_out_fields)
    def post(self):
        data = request.json
        if self.is_valid(data) is False:
            return abort(400)
        spacef = sg.FloatArray(3)
        originf = sg.FloatArray(3)
        sizef = sg.UintArray(3)
        spacem = sg.FloatArray(3)
        originm = sg.FloatArray(3)
        sizem = sg.UintArray(3)
        fix_src = base64.urlsafe_b64decode(data["fix_values"])
        mov_src = base64.urlsafe_b64decode(data["mov_values"])
        fix_val = []
        mov_val = []
        for i in range(1, len(fix_src), 2):
            fix_val.append(fix_src[i - 1] + fix_src[i] * 2 ** 8)
        for i in range(1, len(mov_src), 2):
            mov_val.append(mov_src[i - 1] + mov_src[i] * 2 ** 8)
        for i in [0, 1, 2]:
            sizef[i] = int(data['size_fix'][i])
            spacef[i] = float(data['space_fix'][i])
            originf[i] = float(data['origin_fix'][i])
            sizem[i] = int(data['size_mov'][i])
            spacem[i] = float(data['space_mov'][i])
            originm[i] = float(data['origin_mov'][i])
        lenf = sizef[0] * sizef[1] * sizef[2]
        lenm = sizem[0] * sizem[1] * sizem[2]
        img_fix = sg.ShortArray(lenf)
        img_res = sg.ShortArray(lenf)
        img_mov = sg.ShortArray(lenm)
        for i in range(lenf):
            img_fix[i] = int(fix_val[i])
        for i in range(lenm):
            img_mov[i] = int(mov_val[i])
        try:
            sg.RegistrationPipelinev2(img_fix, img_mov, img_res, sizef, spacef, originf, sizem, spacem, originm)
        except:
            return abort(500)
        res = []
        for i in range(lenf):
            res.append(img_res[i])
        return RegDao(np.array(res, dtype=np.uint16)), 200


if __name__ == '__main__':
    app1.run(debug=False)

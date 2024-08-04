import ctypes as ct
import os
import random

lib = ct.CDLL(f"{os.getcwd()}/VectorDB.dll")

class VectorDimException(Exception): pass

class Vector(ct.Structure):
    _fields_ = [("dim", ct.c_int), ("data", ct.POINTER(ct.c_float))]
    def __init__(self, dim, data):
        self.dim = dim
        arr_len = len(data)
        if dim != arr_len:
            raise VectorDimException("Data length does not match dimension")
        arr_type = ct.c_float * arr_len
        self.data = arr_type(*data)

class KDNode(ct.Structure):
    def __init__(self, vector, info):
        if not isinstance(info, str):
            raise TypeError("Information must be of a string type")
        self.vec = vector
        info_ws = ct.c_wchar_p(info)
        self.information = ct.c_char_p(info_ws.value.encode('utf-8'))
        self.initalized = 1
        self.left = None
        self.right = None

KDNode._fields_ = [("left", ct.POINTER(KDNode)), ("right", ct.POINTER(KDNode)), ("vec", Vector), ("information", ct.c_char_p), ("initialized", ct.c_int)]

class KDBucket(ct.Structure):
    _fields_ = [("root", ct.POINTER(KDNode)), ("dim", ct.c_int), ("size", ct.c_int)]
    def __init__(self, dim: int):
        self.dim = dim
        self.size = 0
        self.root = None
    def add(self, node: KDNode):
        lib.KDBucket_Add(ct.pointer(self), ct.pointer(node))
    def delete(self, vec):
        node = KDNode(vec, "buffer")
        lib.KDBucket_Delete(ct.pointer(self), ct.pointer(node))
    def getNearestNeighbors(self, vec, k):
        if (k > self.size):
            k = self.size
        info = []
        knn = lib.KDBucket_KNN(ct.pointer(self), vec, ct.c_int(k))
        for i in range(k):
            info.append(knn[i].contents.information.decode('utf-8'))
        return info
        
lib.KDBucket_Add.argtypes = [ct.POINTER(KDBucket), ct.POINTER(KDNode)]
lib.KDBucket_Add.restype = None
lib.KDBucket_Delete.argtypes = [ct.POINTER(KDBucket), ct.POINTER(KDNode)]
lib.KDBucket_Delete.restype = None
lib.KDBucket_KNN.argtypes = [ct.POINTER(KDBucket), Vector, ct.c_int]
lib.KDBucket_KNN.restype = ct.POINTER(ct.POINTER(KDNode))
lib.parseInfo.argtypes = [ct.POINTER(KDNode)]
lib.parseInfo.restype = ct.c_char_p

v = Vector(5, [1.0, 1.0, 1.0, 1.0, 1.0])
n = KDNode(v, "hello")
bucket = KDBucket(5)
bucket.add(n)
for i in range(50):
    arr = []
    for q in range(5):
        arr.append(random.randint(0, 1000)/1000)
    vec = Vector(5, arr)
    node = KDNode(vec, f"Node #{i}")
    bucket.add(node)
    print("Added node #" + str(i))

print("Added nodes")
print(bucket.getNearestNeighbors(v, 1))
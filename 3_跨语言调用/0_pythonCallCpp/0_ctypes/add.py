import ctypes

lib = ctypes.CDLL("./add.so")
lib.add.argtypes = (ctypes.c_int, ctypes.c_int)
lib.add.restype = ctypes.c_int

result = lib.add(1, 2)
print(result)

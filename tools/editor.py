
import os
#import ctypes
from ctypes import *

dirname = os.path.dirname(__file__)
rainlib = os.path.join(dirname, '../bin/lib/Debug/rainlib.dll')

def LoadLibrary():
    mystr2=create_string_buffer(b"D:/_MARTIN/programming/_projects/rain_engine/resources")
    rain = cdll.LoadLibrary(rainlib)
    rain.LoadGame(mystr2)

if __name__ == "__main__":
    LoadLibrary()
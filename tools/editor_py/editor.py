
import os
#import ctypes
from ctypes import *
from threading import Thread

# dirname = os.path.dirname(__file__)
# rainlib = os.path.join(dirname, '../bin/lib/Debug/rain_lib.dll')
rainlib = 'D:/_MARTIN/programming/_projects/rain_engine/bin/lib/Debug/rain_lib.dll'
rain = cdll.LoadLibrary(rainlib)

def LoadLibrary():
    mystr2=create_string_buffer(b"D:/_MARTIN/programming/_projects/rain_engine/resources")
    rain.LoadGame(mystr2)
    #rain = cdll.LoadLibrary(rainlib)

def EnableVsync():
    rain.EnableVsync()

def DisableVsync():
    rain.DisableVsync()

if __name__ == "__main__":
    #LoadLibrary()
    thread = Thread(target = LoadLibrary)
    thread.start()
    



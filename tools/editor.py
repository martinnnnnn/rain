
import os
from ctypes import cdll

dirname = os.path.dirname(__file__)
rainlib = os.path.join(dirname, '../bin/lib/Debug/rainlib.dll')

rain = cdll.LoadLibrary(rainlib)
rain.LoadGame()
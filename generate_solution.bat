if not exist "build" mkdir build
cd build
cmake -G "Visual Studio 15 2017" ..
cd ..
mklink rain.sln build\rain.sln
cmake -S . -B build -G "MinGW Makefiles"

cmake --build build

./build/samples/samples.exe

cmake --install build --prefix install
./install/bin/samples.exe
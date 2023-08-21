# compiles and executes for windows with MinGW
# run from project root directory
Remove-Item ./build/CMakeSFMLProject.exe
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --config Release
./build/CMakeSFMLProject.exe
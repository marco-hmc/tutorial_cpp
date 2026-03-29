g++ -O3 -Wall -shared -std=c++11 -fPIC -I/usr/include/python3.10 `python3 -m pybind11 --includes` add.cpp -o add`python3-config --extension-suffix`

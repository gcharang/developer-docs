# Instructions

## Install Dependencies

Doxygen 

```bash
sudo apt-get install doxygen
```

CMake

```bash
sudo apt purge --auto-remove cmake
cd ~
wget https://github.com/Kitware/CMake/releases/download/v3.16.5/cmake-3.16.5-Linux-x86_64.sh
chmod +x cmake-3.16.5-Linux-x86_64.sh
./cmake-3.16.5-Linux-x86_64.sh
sudo ln -s ~/cmake-3.16.5-Linux-x86_64/bin/cmake /usr/local/bin/cmake
```

Clang

```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 9
```

Set the recently installed clang version to be used

```bash
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-9 100
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-9 100
```
Install gcc-9 and g++-9

```bash
sudo add-apt-repository ppa:jonathonf/gcc-9.0
sudo apt-get update
sudo apt-get install gcc-9 g++-9
```

Sphinx and Breathe

```bash
sudo apt install python3-pip python3-sphinx
pip3 install setuptools wheel breathe sphinxemoji m2r
```

## Clone the repo

```bash
git clone https://github.com/gcharang/developer-docs-test
cd developer-docs-test
git submodule update --init --recursive
```

## Build docs from Antara Gaming SDK

```bash
cd doxy/submodules/antara-gaming-sdk/
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER="$(which clang)" -DANTARA_BUILD_DOCS=ON ../
cmake --build . --config Debug
make Doxygen
```

## Build Doxybook2

```bash
cd ../../doxybook2/
mkdir build && cd build
cmake -G "Unix Makefiles" \
     -DDOXYBOOK_TESTS=OFF \
     -DDOXYBOOK_STATIC_STDLIB=OFF \
     -DBUILD_TESTS=OFF \
     -DBUILD_TESTING=OFF \
     -DBUILD_SHARED_LIBS=OFF \
     -DCMAKE_BUILD_TYPE=MinSizeRel \
     ..
cmake --build .
```

## Delete previous output if any

```bash
cd ../../../../
rm -rf docs/basic-docs/antara-gaming-sdk/*
rm -rf doxy/outputDir/*
rm -rf doxy/build/*
```

## Make sure output/build directories exist

```bash
mkdir -p docs/basic-docs/antara-gaming-sdk/
mkdir -p doxy/outputDir/
mkdir -p doxy/build/
```

## Create md files from the xml output of doxygen using Doxybook2

```
cd doxy
./submodules/doxybook2/build/src/DoxybookCli/doxybook2 --input submodules/antara-gaming-sdk/build/docs/doxygen/xml/ --output outputDir/ --templates ./templates/
```

## Compact the md output, place it in the appropriate dir and populate the sidebar structure

```bash
cmake -B ./build .
cd build
make
cd my_src
./komodo-doxybook2
```


#!/bin/bash

if [ "$TRAVIS_BRANCH" == "developer-docs-test" ]; then
    sudo add-apt-repository ppa:jonathonf/gcc-9.0 -y
    sudo apt-get update -y
    sudo apt-get install -y doxygen
    sudo apt purge --auto-remove cmake
    cd ~
    wget https://github.com/Kitware/CMake/releases/download/v3.17.1/cmake-3.17.1-Linux-x86_64.sh
    chmod +x cmake-3.17.1-Linux-x86_64.sh
    ./cmake-3.17.1-Linux-x86_64.sh --skip-license
    sudo ln -s ~/cmake-3.17.1-Linux-x86_64/bin/cmake /usr/local/bin/cmake
    wget https://apt.llvm.org/llvm.sh
    chmod +x llvm.sh
    sudo ./llvm.sh 9
    sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-9 100
    sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-9 100
    sudo apt-get install gcc-9 g++-9
    sudo apt install python3-pip python3-sphinx
    sudo apt install libboost-chrono-dev libboost-date-time-dev libboost-regex-dev libboost-system-dev libboost-thread-dev libboost-random-dev libboost-filesystem-dev
    pip3 install setuptools
    pip3 install wheel
    pip3 install breathe
    pip3 install sphinxemoji
    pip3 install m2r
    cd $TRAVIS_BUILD_DIR
    git submodule update --init --recursive
    cd doxy/submodules/antara-gaming-sdk/
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER="$(which clang)" -DANTARA_BUILD_DOCS=ON ../
    cmake --build . --config Debug
    make Doxygen
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
    cd ../../../../
    rm -rf docs/basic-docs/antara-gaming-sdk/*
    rm -rf doxy/outputDir/*
    rm -rf doxy/build/*
    mkdir -p docs/basic-docs/antara-gaming-sdk/
    mkdir -p doxy/outputDir/
    mkdir -p doxy/build/
    cd doxy
    ./submodules/doxybook2/build/src/DoxybookCli/doxybook2 --input submodules/antara-gaming-sdk/build/docs/doxygen/xml/ --output outputDir/ --templates ./templates/
    cmake -B ./build .
    cd build
    make
    cd my_src
    ./komodo-doxybook2
    ls docs/basic-docs/antara-gaming-sdk/
    cat docs/.vuepress/gaming-sdk-sidebar.js
fi

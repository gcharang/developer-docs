#!/bin/bash

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

cmake_minimum_required(VERSION 3.7)
project(TransDNA)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        .idea/misc.xml
        .idea/modules.xml
        .idea/TransDNA.iml
        .idea/vcs.xml
        .idea/workspace.xml
        "1.1. BIOLOGIA - TEORIA - LIVRO 1.pdf"
        LICENSE
        merge.c
        README.md
        T1-Apresentacao.pdf functions.c)

add_executable(TransDNA ${SOURCE_FILES} functions.c)
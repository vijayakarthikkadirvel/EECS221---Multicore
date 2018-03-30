cmake_minimum_required(VERSION 3.8)
project(hgorjiara_multicore_p2_70c32de44a65)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        classlist.h
        common.h
        mymemory.cc
        mymemory.h
        qnode.cc
        qnode.h
        readtree.cc
        rwlock.cc
        rwlock.h
        test.cc)

add_executable(hgorjiara_multicore_p2_70c32de44a65 ${SOURCE_FILES})
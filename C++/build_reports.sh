#!/bin/bash
set -e

rm -rf build_gtest build_boost build_catch

echo "GoogleTest"
mkdir -p build_gtest
cd build_gtest
g++ -std=c++17 -O0 --coverage -I.. ../tests/test_gtest.cpp -lgtest -pthread -o lab3_test
./lab3_test

gcovr --html-details coverage_gtest.html --root .. --exclude '.*test_.*' --print-summary
cd ..

echo "Boost.Test"
mkdir -p build_boost
cd build_boost
g++ -std=c++17 -O0 --coverage -I.. ../tests/test_boost.cpp -o lab3_boost
./lab3_boost
gcovr --html-details coverage_boost.html --root .. --exclude '.*test_.*' --print-summary
cd ..

echo "Catch2"
mkdir -p build_catch
cd build_catch
g++ -std=c++17 -O0 --coverage -I.. ../tests/test_catch.cpp -o lab3_catch
./lab3_catch
gcovr --html-details coverage_catch.html --root .. --exclude '.*test_.*' --print-summary
cd ..

echo "Отчёты:"
echo "  build_gtest/coverage_gtest.html"
echo "  build_boost/coverage_boost.html"
echo "  build_catch/coverage_catch.html"

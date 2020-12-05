# Advent of Code 2020

![Solutions](https://github.com/pmer/advent20/workflows/Solutions/badge.svg)


## Jump start

```bash
git clone --recursive https://github.com/pmer/advent20
cd advent20
./build.sh
./test.sh
```


## Regular development

```bash
git clone --recursive https://github.com/pmer/advent20
cd advent20

mkdir build
cd build
cmake .. -G Ninja
ninja
cd ..

cd data
cd 1
../../build/1a  # the output should be 713184
../../build/1b  # the output should be 261244452
cd ..
cd 2
../../build/2a  # the output should be 483
../../build/2b  # the output should be 482
cd ..
```


## Code organization

| Path           | Intention                          |
| -------------- | ---------------------------------- |
| Tsunagari/     | C++ standard library               |
| answers/       | Answers to Advent of Code          |
| data/          | Input files for Advent of Code     |
| src/           | Source code for Advent of Code     |
| CMakeLists.txt | List of files to compile           |
| build.sh       | Instructions for the CI to compile |
| test.sh        | Instructions for the CI to test    |

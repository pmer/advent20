# Advent of Code 2020

![Solutions](https://github.com/pmer/advent20/workflows/Solutions/badge.svg)


## Jump start

```bash
git clone --recurse-subdirectories https://github.com/pmer/advent20
cd advent20
bin/build.sh
bin/test.sh
```


## Regular development

```bash
git clone --recurse-subdirectories https://github.com/pmer/advent20
cd advent20

mkdir build
cd build
cmake .. -G Ninja
ninja
cd ..

# Run programs
cd input
../build/1a  # the output should be 713184
../build/1b  # the output should be 261244452
../build/2a  # the output should be 483
../build/2b  # the output should be 482
cd ..

# Check answers
cd answers
cat 1a  # the output should be 713184
cat 1b  # the output should be 261244452
cat 2a  # the output should be 483
cat 2b  # the output should be 482
```


## Code organization

| Path           | Intention                          |
| -------------- | ---------------------------------- |
| Tsunagari/     | C++ standard library               |
| answers/       | Answers to Advent of Code          |
| bin/           | Instructions for CI on how to test |
| input/         | Input files for Advent of Code     |
| src/           | Source code for Advent of Code     |
| CMakeLists.txt | List of files to compile           |

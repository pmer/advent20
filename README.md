```
# Checkout
git submodule update --init

# Compile
mkdir build
cd build
cmake .. -G Ninja
ninja

# Run
cd ../data/1
../../build/1a
```

# libALTrepo
# Foobar

ALTREPO it's a CLI C++ utilite. 

## Installation
download main.cpp, libartrepo.h and libaltrepo.cpp. Compile shared library

```bash
g++ -c -Wall -Werror -fPIC libaltrepo.cpp
g++ -shared -o libaltrepo.so libaltrepo.o
```

```bash
echo $LD_LIBRARY_PATH
```
If not empty, copy libartrepo.so there
If empty, choose empty directory and copy there libartrepo.so
```bash
LD_LIBRARY_PATH=your library path
```
Check result
```bash
echo $LD_LIBRARY_PATH
```
Export your library path with
```bash
export LD_LIBRARY_PATH
```
In this terminal compile with
```bash
g++ -I. -L. main.cpp -o altrepo -laltrepo
```

## Usage

```bash
./altrepo
```
Follow the instructions in the CLI program output

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

# 2ds
 Implementation of the 2DS esolang.

## Usage
 `./2ds <filename>`

 will run `<filename>` as a 2DS program.

 `./2ds <filename> -minimize`
 
 will print back out the code at `<filename>` minimized.

## Requirements for compile
 Some form of conio.h, if you're on Windows this will be there by default, and on linux just get it from [this stackoverflow post](https://stackoverflow.com/questions/3276546/how-to-implement-getch-function-of-c-in-linux)

## Compiling
 Do `gcc 2ds.c -o 2ds` to compile it with GCC, which is pre-installed on pretty much every linux distro.
# pacman-banner
Console application to display an 80-character wide banner of Pac-Man characters in various forms, built with musl.

## Compiling pacman-banner
pacman-banner requires [Functions.include](https://github.com/akoimeexx/Functions.include) in order to be able to print text in color. Save functions.c and functions.h to the same directory where main.c is located, then run the following in your terminal emulator of choice:
```bash
musl-gcc -O3 -std=c99 -Wall main.c -c -o pacman-banner.o
musl-gcc -O3 -std=c99 -Wall functions.c -c -o functions.o
musl-gcc -O3 -std=c99 -Wall pacman-banner.o functions.o -o pacman-banner -static
```

### Screenshots

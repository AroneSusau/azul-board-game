# Advance Programming - Assignment 2

This project recreates the [Azul](https://en.wikipedia.org/wiki/Azul_(board_game)) board game in the command line. Its compatible for up to 4 players which can be either human or a basic AI.

## Project Build

### Prerequisites
- C++14 / C++1y (Linux)

### How to build

Please enter the following command into your terminal or shell application to build the program.
```
$ make
```

### How to test

Please enter the following command into your terminal or shell application to run all program tests.

Note: Please ensure the program has been compiled prior to running.

As the tests were initially compiled and run on a MacOS Catalina machine, they may fail on different machines due to differences in C++ hardware implementations. The random seed may not output the same as the expected output.

```
$ make test
```

### How to run

Please enter the following command into your terminal or shell application to run the program.
```
$ ./Azul
```
Or alternatively, if you want to automate the process of argument processing, you can run the program with the following command.
```
$ ./Azul $(cat program.args) < program.in > program.out
```

## References

- [Makefile Resource](https://www.gnu.org/software/make/manual/html_node/Functions.html)
- [C++ Docs](https://devdocs.io/cpp/)
- [Canvas Material](https://rmit.instructure.com/)

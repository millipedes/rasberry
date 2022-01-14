# :strawberry: rasberry Programming Language
## :dart: Project Description/Goal
rasberry is meant to be a fast relatively powerful programming language.  By
May 2022 it should have a fully functional stocastic process library natively.
Currently it is interpreted, but soon I will likely write an assembly
translation for GNU assembler.

## :floppy_disk: Installation
The program depends on the following:
- gcc
- gnu make (fyi just `make` for people on 99% of distros)
- sudo

The current installation process will put a binary called `millipedes-rbi`
(millipedes rasberry interpreter) in your /usr/bin/ directory (where bash looks
for binaries, i.e. can be called from command line with millipedes-nsi).  This
can be achieved via:
```
    $ git clone https://github.com/millipedes/ns.git
    $ cd ns
    $ mkdir bin/
    $ sudo make install
```

## :star2: Currently Offered Features
As of January 7 2022 this is a very complicated [RPN](https://en.wikipedia.org/wiki/Reverse_Polish_notation)
calculator.

However the entire code base is memory leakless for all of the calculator
operations.  Many more features are coming quickly as the basic infrastructure
of the language is now written.

As it is currently written, this language supports the following context free
grammar (BNF-like notation):
```
expr    -> + expr expr
         | - expr expr
         | * expr expr
         | / expr expr
         | ^ expr expr
         | (expr)
         | > expr expr
         | < expr expr
         | >= expr expr
         | <= expr expr
         | == expr expr
         | != expr expr
         | = var expr
         | var
         | integer

var     -> integer

integer -> x such that x is an element of integers (32 bit signed)
```

## :scroll: Immediately on the List to be Implemented
1) Error on invalid word
    1.1) Reserve `exit`/remove console.c nonsense
2) Error on assignment of invalid type
3) Array/matrix support
4) Implement type float
5) Implement type string
6) implement type function
7) Implement if, while, for
8) Implement Stack Frames
9) Implement File support
10) Implment Module support
11) Command Line Arguements/REPL UI improvement
12) MAYBE Infix function notation
13) MAYBE Assembly translation for GAS 

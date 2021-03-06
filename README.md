# :strawberry: rasberry Programming Language
## :dart: Project Description/Goal
rasberry is meant to be a fast relatively powerful programming language.
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
    $ git clone https://github.com/millipedes/rasberry.git
    $ cd rasberry
    $ mkdir bin/
    $ sudo make install
```

## :star2: Currently Offered Features
This language uses [RPN](https://en.wikipedia.org/wiki/Reverse_Polish_notation)
notation.

This language now has variable support for integers, floats (double precision
floating point numers), strings, and the type known as a data frame.  I am
writing a manual for the programming language, and when it is ready, I will link
it here for a better description of language features such as the data frame.
Data frames can now access elements via `df element` now being defined in the CFG.

The entire code base is memory leakless and error free for the grammar defined
below.  Many more features are coming quickly as the basic infrastructure of
the language is now written.

The programming language has a working symbol table (i.e. complete stack frame
functionality for all supported types, and data frames but no entry point to
making new stack frames yet).

As it currently stands, the language will very likely have lazy evaluation.

As it is currently written, this language supports the following context free
grammar (BNF-like notation) for which all operators only perform on like expr's:
```
expr                    -> + expr expr
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
                         | data frame
                         | string
                         | float
                         | df element

var                     -> integer
                         | float
                         | string
                         | data frame

df element              -> integer
                         | float
                         | string
                         | data frame

(data frame)|(integer)  -> df element (at index)

string                  -> "(a-z || A-Z || 0-9 || symbol)*"

data frame              -> [(var)*]

float                   -> x such that x is a signed 32 bit double precision floating point number

integer                 -> x such that x is an element of integers (32 bit signed)
```

## :scroll: Immediately on the List to be Implemented
1) Make acess modifier play nicer with other operations.
2) Write string ops
3) write data frame ops
4) Write stack frames
5) Reserve `exit`/remove console.c nonsense
6) Implement if, while, for
7) Implement stack frames
8) Implement functions
9) Implement function entry point (i.e. make main reserved)
10) Implment Module support
11) Implement File I/O support
12) Command Line Arguements/REPL UI improvement
13) MAYBE Infix function notation
14) MAYBE Assembly translation for GAS 

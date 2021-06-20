# WISH Linux Shell

This is a project where I build a Linux shell using the C language. I have referenced [@MIMA's stories on HackerNoon](https://hackernoon.com/u/MIMA) for the same. I intend to gain more knowledge about Operating Systems from this and some other projects, most of which will be from [this repository](https://github.com/remzi-arpacidusseau/ostep-projects). This shell is called "wish" because the original project which inspired this was also called so.

This project is just for learning purposes only, and supports only simple commands like `ls` and `grep`. This emulator is nothing fancy and currently only has a command-line interpreter(cli) containing a parser and an executor, an abstract syntax tree (AST) and a symbol table (which is a nightmare in itself). This currently also has support for the dump built-in utility, and I will be adding more utilities like cd and fg over time.

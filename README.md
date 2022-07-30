# BA lang

BA is a modern programming language allowing to build software in a fast and efficient way. Its main objective is to propose simplified
syntaxes and obtain considerable performance gains in execution time compared to other programming languages.

## Getting started


### Prerequisites

BA is written in C and tested only with the gcc compiler. You will need to install it and also be able to use the `make` command.

### Installation

An official version is not yet released but you can run the master branch which is relatively stable. To do this,
you can execute the following commands:

```bash
git clone https://github.com/mistersouls/ba.git
make clean build
```

Two binaries will essentially be generated: `bin/ba` which is the BA lang compiler and `bin/bas` which is the BA runtime.

### Deal with BA

Now you can play with the BA language. Some examples can be found in the [resources/](resources) folder.
You can compile some example like [resources/expression.ba](resources/expression.ba) and run it with the commands below:

```bash
bin/ba compile resources/expression.ba
```

It will genrate a compiled file at resources/expression.sb. Then, execute the below command to get output program:

```bash
bin/bas resources/expression.sb
```


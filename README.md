# crusade

A simple math interpreter written in C++ using the Shunting-yard algorithm.

<video controls src="assets/demo.mp4" title="Crusade Demo"></video>

### Working mechanism

crusade first tokenizes the given input and classifies them into numbers, operators, parentheses and so on. During this operation, it also validates the input and checks for invalid expressions like missing operators/operands/parentheses and so on.

The next step is to use the [shunting yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) to convert the infix expression to the reverse polish notation (aka postfix expression, eg. `1 + 2 - 3` => `1 2 + 3 -`). The evaluation of the postfix expression is pretty easy.

### Build from source

It's only a single file (I am scared of undefined references linker errors).

```sh
g++ -o crusade ./src/crusade.cpp
```


### Further Goals

- [ ] Support for unary operators
- [ ] Pretty output
- [ ] Input history
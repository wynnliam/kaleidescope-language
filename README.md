# Kaleidescope Language
A toy language for learning the different steps of compiler engineering.

This follows the following tutorial from [here](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html)

## What is Kaleidescope?
Kaleidescope is a simple procedural language that looks a lot like Python. For the moment,
the language can only define numbers and functions. Here is a simple Kaleidescope program
to compute Fibonacci numbers:

```
def fib(x)
	if x < 3 then
		1
	else
		fib(x - 1) + fib(x - 2)

fib(40)
```

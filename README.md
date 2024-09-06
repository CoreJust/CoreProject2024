
# CoreProject 2024

Core programming language repository. 
Core is a compilable programming language created using LLVM. 
It is currently in the early stages of development.

## Examples

#### Factorial
```sh
// A function of the standard C runtime library.
fn putchar(ch: cint): cint = native("putchar")

fn print(n: cint) {
	fn printImpl(n: cint) {
		if n != 0 {
			printImpl(n / 10)
			putchar(48 + n % 10)
		}
	}
	
	if n == 0 {
		putchar(48)
	} elif n < 0 {
		putchar(45)
		printImpl(-n)
	} else {
		printImpl(n)
	}
}

fn factorial(n: cint): cint {
	if n <= 1 {
		return 1
	}

	return n * factorial(n - 1)
}

fn main(): i32 {
	print(factorial(12))

	return 0
}
```


#### Reverse counting
```sh
// A function of the standard C runtime library.
fn putchar(ch: cint): cint = native("putchar")

fn print(n: cint) {
	fn printImpl(n: cint) {
		if n != 0 {
			printImpl(n / 10)
			putchar(48 + n % 10)
		}
	}
	
	if n == 0 {
		putchar(48)
	} elif n < 0 {
		putchar(45)
		printImpl(-n)
	} else {
		printImpl(n)
	}
}

fn repeat(fun: fn(cint), cnt: cint) {
	if cnt < 0 {
		return
	} else {
		fun(cnt)
		repeat(fun, cnt - 1)
	}
}

fn test_fn(i: cint) {
	print(i)
	putchar(10)
}

fn main(): i32 {
	repeat(test_fn, 10)

	return 0
}
```



#### Functional
```sh
fn main(): cint {
	println(reduce_range(min, 0, 90, 0))
	println(reduce_range(max, 0, 90, 0))
	
	println(invoke(id, 10))
	println(invoke(next, 10))

	return 0
}

fn reduce_range(f: fn(i32, i32): i32, a: i32, b: i32, acc: i32): i32 {
	if a == b {
		return f(acc, b)
	}
	
	return reduce_range(f, a + 1, b, f(acc, a))
}

fn invoke(f: fn(i32): i32, x: i32): i32 = f(x)

fn id(x: i32): i32 = x

fn next(x: i32): i32 = x + 1

fn min(a: i32, b: i32): i32 {
	if a < b {
		return a
	} else {
		return b
	}
}

fn max(a: i32, b: i32): i32 {
	if a > b {
		return a
	} else {
		return b
	}
}

// A function of the standard C runtime library.
fn putchar(ch: cint): cint = native("putchar")

fn println(n: i32) {
	fn printImpl(n: i32) {
		if n != 0 {
			printImpl(n / 10)
			putchar(48 + (n % 10) as cint)
		}
	}
	
	if n == 0 {
		putchar(48)
	} elif n < 0 {
		putchar(45)
		printImpl(-n)
	} else {
		printImpl(n)
	}
	
	putchar(10)
}
```
# Calculator
> Created by Wei Zhou. All rights reserved.  
> My github address: <https://github.com/zromyk>

## Describe

这个程序是一个计算器，你只需要输入一个字符串的计算就可以输入结果。

你可以输入运算公式。

## Operators that supported

这个计算器支持混合计算。

目前支持的计算包括:

| Operator      | Instructions              | Such        |
|:--------------|:--------------------------|:------------|
| p             | p = 3.1415926.            |             |
| e             | e = 2.718281828.          |             |
| "+ - * / % ^" | + - * / % ^               |             |
| **"pow()"**   | please use '^' instead.   | 2^3 = 8     |
| arccos()      | Inverse cosine function.  |             |
| arcsin()      | Inverse sine.             |             |
| arctan()      | Inverse tangent function. |             |
| cosh()        | It is cos-1().            |             |
| sinh()        | It is sin-1().            |             |
| tanh()        | It is tan-1().            |             |
| cos()         | Cosine.                   | cos(p) = -1 |
| sin()         | Sine.                     | sin(p) = 0  |
| tan()         | Tangent.                  | tan(p) = 0  |
| fabs()        | Could use '\|' instead.   | \|1 = 1     |
| ln()          | Natural logarithm.        |             |
| log()         | Logarithmic.              |             |
| sqrt()        | Square root.              |             |

## Build
**macOS / linux**

```
$ mkdir build && cd build   # Make a build directory in the top level directory
$ cmake .. && make          # Compile
$ cd .. && ./main           # Return to home directory and Run the program
```

**or**

```
$ ./build.sh      # Run 'build.sh', You can double click on
$ ./main          # Run the program
```
## Program structure

```
├── CMakeLists.txt
├── LICENSE
├── README.md
├── build.sh
├── include
│   └── calculator.h
└── src
    ├── calculator.cpp
    └── main.cpp
```




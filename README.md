# Bidimensional matrix manipulation library for C++
Object-oriented library for the manipulation of matrices which:
* supports method chaining and
* provides many intuitive operator overloads.

## Example

```C++
Matrix<double> A(10,10,2); // creates a 10x10 matrix filled with twos
Matrix<double>
    B = A[2], // gets the 3rd row in A (indexing starts at 0)
    C = A||2, // gets the 3rd column in A,
    D = A.slice(1,3,1,5);
    // gets a submatrix from the 2nd to the 4th row, from the 2nd to the 6th column

A[2] = 5; // fills the entire 3rd row with fives
A||5 = 7; // fills the entire 6th column with 7
B = A*2; // multiplication with a scalar (+,-,*,/ available for scalars)
C = A*B; // multiplication with a matrix (+,-,* available for matrices)
A.slice(2,4,2,6) += D; // operates with a submatrix wthin A
```

## Licence
Use as you wish; credit me as you wish.

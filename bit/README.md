# Bitwise operators and functions for KDB

## Synopsis
```q
q) .q,:use`dot.bit      //use like built-in operators from the .q namespace
q) 15 bitor 37 112 97
47 127 111
q) 15 bitand 37 112 97
5 0 1
q) 15 bitxor 37 112 97
42 127 110
q) show (g1;g2):2?0Ng
c02c26cb-e08c-2734-4a52-98c6d119c311 ea4d0269-375c-d73b-96f0-6bb6334ca423
q) g1 bitand g2
c00c0249-200c-0730-0250-088611088001
q) bitnot g1
3fd3d934-1f73-d8cb-b5ad-67392ee63cee
q) t:([]sym:10?`2; prop:10?16)
q) select from t where 2<=bitcount 15 bitand prop   //at least two matching bits
sym prop
--------
ik  3   
ap  13  
ml  3   
fm  7   
ii  14  
```
## Overview

This module extends **KDB** with native-style bitwise operators.  
The goal is to provide simple, explicit, and efficient bit-level manipulation capabilities that are currently missing from the standard operator set.

Supported types are `byte`, `short`, `int`, `long`, `guid`.

## Operators

### `bitor`, `bitand`, `bitxor`

Performs bitwise OR, AND, or XOR operations between two arguments. These operators are atomic in both arguments, just like the arithmetic operators, but they do not handle dictionaries and tables, and null values have no special treatment.

### `bitnot`

Performs a bitwise NOT operation. Atomic, supports dictionaries and tables as well.

## Counting functions

All of the following functions return integers, are atomic, and support dictionaries and tables as well.

### `bitcount`

Returns the number of ones in the binary representation.  
It is equivalent to `{sum 0b vs x}` for atoms.

### `leadz`

Returns the number of leading zeros in the binary representation.  
It is equivalent to `{"i"$(0b vs x)?1b}` for atoms.

### `trailz`

Returns the number of trailing zeros in the binary representation.  
It is equivalent to `{"i"$reverse[0b vs x]?1b}` for atoms.

## Fun stuff

```q
q) (.;#) 0=x bitand\: x:til 32
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# . # . # . # . # . # . # . # . # . # . # . # . # . # . # . # .
# # . . # # . . # # . . # # . . # # . . # # . . # # . . # # . .
# . . . # . . . # . . . # . . . # . . . # . . . # . . . # . . .
# # # # . . . . # # # # . . . . # # # # . . . . # # # # . . . .
# . # . . . . . # . # . . . . . # . # . . . . . # . # . . . . .
# # . . . . . . # # . . . . . . # # . . . . . . # # . . . . . .
# . . . . . . . # . . . . . . . # . . . . . . . # . . . . . . .
# # # # # # # # . . . . . . . . # # # # # # # # . . . . . . . .
# . # . # . # . . . . . . . . . # . # . # . # . . . . . . . . .
# # . . # # . . . . . . . . . . # # . . # # . . . . . . . . . .
# . . . # . . . . . . . . . . . # . . . # . . . . . . . . . . .
# # # # . . . . . . . . . . . . # # # # . . . . . . . . . . . .
# . # . . . . . . . . . . . . . # . # . . . . . . . . . . . . .
# # . . . . . . . . . . . . . . # # . . . . . . . . . . . . . .
# . . . . . . . . . . . . . . . # . . . . . . . . . . . . . . .
# # # # # # # # # # # # # # # # . . . . . . . . . . . . . . . .
# . # . # . # . # . # . # . # . . . . . . . . . . . . . . . . .
# # . . # # . . # # . . # # . . . . . . . . . . . . . . . . . .
# . . . # . . . # . . . # . . . . . . . . . . . . . . . . . . .
# # # # . . . . # # # # . . . . . . . . . . . . . . . . . . . .
# . # . . . . . # . # . . . . . . . . . . . . . . . . . . . . .
# # . . . . . . # # . . . . . . . . . . . . . . . . . . . . . .
# . . . . . . . # . . . . . . . . . . . . . . . . . . . . . . .
# # # # # # # # . . . . . . . . . . . . . . . . . . . . . . . .
# . # . # . # . . . . . . . . . . . . . . . . . . . . . . . . .
# # . . # # . . . . . . . . . . . . . . . . . . . . . . . . . .
# . . . # . . . . . . . . . . . . . . . . . . . . . . . . . . .
# # # # . . . . . . . . . . . . . . . . . . . . . . . . . . . .
# . # . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
# # . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
```
# Bitwise operators and functions for KDB

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

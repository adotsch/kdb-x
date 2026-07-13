# Selection

## Intro

This module implements the [selection algorithm](https://en.wikipedia.org/wiki/Selection_algorithm) for KDB.
It allows to write fast and aesthetic queries to work with the k largest or smallest elemnets of a numeric vector.
It supports all integer, float and temporal types, and characters ("cdefghijmnptuvxz").

## Synopsis

```q
q) .q,:use`dot.selectk      //use like built-in operators from the .q namespace
q) v: 59 95 60 24 82 8 62 69 66 52
q) 3 kmax v                 //3 largest elements in v
82 95 69
q) 3 ikmax v                //index of the 3 largest elements in v
4 1 7
q) 3 ikmax "abcdefghij"!v   //index of the 3 largest elements in a dictionary, returning keys
"ebh"
q) 3 bkmax v                //boolean mask of the 3 largest elements in v
0100100100b
q) 3 kthmax v               //3rd largest element in v
69
```
Quick benchmark for finding the 10,000 largest of 1,000,000 random elements:
```q
q) v: 1000000?100f
q) \t:10 r1: 10000#desc v
982
q) \t:10 r2: 10000 kmax v
38
q) r1 ~ desc r2
1b
```
A faster implementation of median is also possible for large vectors using `kthmax`:
```q
q) fmed: {avg distinct[div[1 2+count x]2] kthmax\: x}
q) med[v] = fmed[v]
1b
q) \t:10 med v
281
q) \t:10 fmed v
100
```
## kmin, kmax

| subset | index | mask |
| - | - | - |
|k kmin v|k ikmin v|k bkmin v|
|k kmax v|k ikmax v|k bkmax v|

Where
 - `k` is a long
 - `v` is a numeric vector or dictionary

These two algorithms compute the `k` smallest or `k` largest elements of a vector.
The results are unordered.

The `i*` variant returns the indices of the elements, or the keys of the dictionary for the corresponding elements (similarly to `iasc` and `idesc`).

The `b*` variant returns the boolean mask of the elements.

## kthmin, kthmax

You can select the `k`-th smallest or `k`-th largest element as well.

## Examples

```q
q) v: 8 1 7 2 4 5 4 2 7 8
q) 3 kmax v
8 8 7
q)3 ikmax v
0 9 2
q)3 bkmax v
1010000001b
q) 2 kthmax v
8
q) 3 kthmin v
2
```

The 3 largest cities by country (assuming population sizes are different):
```q
select from c where population >= (3 kthmax;population) fby country
```
or using the boolean mask for each country:
```q
select from c where (3 bkmax;population) fby country
```
The example above takes advantage of the (undocumented) fact that [`fby`](https://code.kx.com/q/ref/fby/) can apply not only aggregate functions to groups, but any function, provided it returns either a single value (an aggregate) or a vector of the same length as the input. In this case, `3 bkmax` maps the population vector to a boolean mask that identifies the three largest populations within each country.

## History

This module in an expanded version of a helper function I originally developed for [Temporal Similarity Search](https://code.kx.com/kdb-x/ref/ai-libs/tss-index-parameters.html) while working at KX.

The module was considered for inclusion in the official KDB-X module library, and I still believe that's its natural home. Since I no longer work at KX and the integration wasn't completed in time, KX approved publishing the code independently.


# Deferred execution for KDB

## Intro

This module provides Go-style deferred execution for **KDB**.  
The call data (function + parameters) wrapped by a deferred object is executed when it's garbage collected (e.g., when a local variable goes out of scope on function return, or when its reference count drops to 0).

This is an **experimental** module, see [limitations](#limitations) below.

## Synopsis

```q
q) .q,:use`dot.defer      // load module into the .q namespace
q) d: defer(0N!;`hello)
q) d: ()                  // reassignment drops reference; deferred code executes on GC
`hello

q) // Automatic cleanup on function return:
q) f:{ d: defer (0N!;`cleanup); 0N!"doing work"; }
q) f[]
"doing work"
`cleanup

q) // Disabling a deferred action:
q) d: defer (0N!;`never_runs)
q) defer d                // calling defer on a defer object disables it
(0N!;`never_runs)
q) d: ()                  // no-op!
```

## Overview

Inspired by [Go's defer statement](https://go.dev/tour/flowcontrol/12), this module allows you to schedule a code expression to be executed automatically when a `defer` object is garbage collected.

In KDB+, local variables are freed immediately upon returning from a function. By assigning a `defer` object to a local variable, you can ensure that cleanup tasks - such as closing socker/file handles - run automatically when the function finishes, regardless of how it exits.

## Functions

### `defer`

Wraps a call data in a deferred execution object, or disables an existing deferred object.

- **Syntax**: `defer x`
- **Arguments**:
  - `x`: a string or list that will be evaluated by `value` when the deferred object is garbage collected, or an existing deferred object that will be disarmed.
- **Returns**:
  - When wrapping: A foreign object (type `112h`) wrapping the call data.
  - When passing an active deferred object: Disables the action and returns the originally wrapped call data.
- **Behavior**:
  - **Execution on Garbage Collection**: When the returned foreign object is garbage-collected (reference count drops to 0), `value` is executed on the wrapped call data.
  - **Scope Cleanup**: Assigning `d: defer expr` inside a function ensures `expr` will be evaluated automatically when the function returns and local variables are unreferenced.
  - **Reassignment / Explicit GC**: Reassigning or clearing the variable holding the only reference to the deferred object (e.g., `d: ()`) drops the reference and triggers garbage collection, executing the deferred action immediately.
  - **Disabling / Disarming**: Calling `defer d` on an active deferred object `d` disables it by clearing its internal payload to `()`, returning the original expression and preventing execution when `d` is subsequently destroyed.
  - **Atoms**: Atom arguments (`type < 0`) are rejected with a `'type` error.

## Examples

### Resource Cleanup on Function Exit

Ensure file handles, sockets are released automatically when exiting a function:

```q
q) process_file:{[path]
    h: hopen path;
    d: defer (hclose; h);  // close handle when function exits
    // ... work with file handle ...
  }
```

### Conditional Execution / Rollback

Set up a rollback action that is disarmed if the operation completes successfully:

```q
q) safe_step:{
    d: defer (`rollback; params); // will call rollback[params] when garbage collected
    // perform operations ...
    defer d;  // disarm rollback on success before returning
  }
```

## Limitations

Deferred execution can't be used to modify the variable the defer object is stored in. For example:  
```q
q)a:defer({`a set x};10)
q)a:0     //this "should" set a to 10
'noamend: `. `a
  [3]  {`a set x}
           ^
q))       //we get a 'noamend error instead, and a SIGSEGV
q))\
SIGSEGV: Fault address ....
```

## C API

The C technique used to implement `defer` is not officially documented by KX, but it is widely used in open-source KX projects, like [embedPy](https://github.com/KxSystems/embedPy), [PyKX](https://github.com/KxSystems/pykx) or [Fusionx](https://github.com/KxSystems/fusionx).

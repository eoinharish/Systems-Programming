# GDB Quick Reference

## Compile with Debug Symbols

```bash
g++ -g -O0 server.cpp -o server
gdb ./server
```

## Starting and Exiting

```gdb
run (r)                # Start program
start                  # Start and stop at main()
continue (c)           # Continue execution till next breakpoint
quit (q)               # Exit GDB
```

## Breakpoints

```gdb
break main          # Break at function
break server.cpp:14 # Break at source line
info breakpoints    # List breakpoints
delete 1            # Delete breakpoint 1
disable 1           # Disable breakpoint 1
enable 1            # Enable breakpoint 1
```

## Stepping

```gdb
next (n)                # Execute next line, step over functions
step (s)                # Step into function
finish                  # Run until current function returns
continue                # Resume execution
```

## Variables and Memory

```gdb
info locals         # Show local variables belonging to the currently selected function's stack frame.
info args           # Show function arguments
print x             # Print variable
print &x            # Print variable address
set var x = 10      # Change variable
x/32gx $sp          # Examine 32 giant words from stack pointer
```

## Stack Frames

```gdb
bt                  # Show all stack frames
bt full             # Show frames and local variables
info frame          # Details of current frame (program counter)
frame 2             # Select frame 2
up                  # Move to caller frame
down                # Move toward callee frame
```

## Registers

```gdb
info registers      # Show all registers
info registers sp   # Show stack pointer
print $sp           # Print stack pointer
print $pc           # Print program counter
```

## Process Information

```gdb
info proc           # Process informationc(pid, exe name)
info threads        # List threads
thread 2            # Select thread 2
```

## Useful Tips

- Use `-g` to include debugging symbols.
- Use `-O0` to reduce compiler optimizations while learning.
- `n` is shorthand for `next`.
- `s` is shorthand for `step`.
- `p variable` is shorthand for `print variable`.
- `bt` is shorthand for `backtrace`.

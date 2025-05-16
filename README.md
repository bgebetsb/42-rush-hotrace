# Hotrace

### What is this project?
Hotrace is a search engine, one of the weekend projects (rushes) at 42 School. It has to be written in C, conforming to the Norm. The goal is to make it as fast as possible.

We, as the participants of the project at Vienna campus made the collective agreement not to use optimization flags at compilation (or attributes that would overwrite this), so in other words to stick to O0 level, and see how much optimization we can do by ourselves.

### How to use it?
You need to compile our executable (the Makefile helps you with that) and run it. It works either in interactive mode, where it reads keys and corresponding values, then starts the lookup phase after an empty line. Like in the following example:
```
$ ./hotrace
key1
value1
key2
value2

key1
value1
key3
key3: Not found.
```

One can also read from a pre-made dictionary file (through input redirection) which then allows the user to time the performance.

### How does it work?

We first read the input line-by-line and then store corresponing key-value-pairs in a hashtable using `djb2a_hash` (we were using `murmur3` earlier but it proved to be less efficient). The hashes are truncated to `HASHMAP_SIZE` for indexing, but the original value is also stored (to resolve collisions faster).
On lookup, collisions are handled by a binary tree and `strcmp`.

Through profiling, we have arrived at the conclusion that the main bottleneck of `hotrace` is `get_next_line` (the function that reads the input at the storing phase and returns each line separately). We have optimized this function the following ways:
- it does not realloc true-to-size, but returns the line in a buffer, which always doubles in size when exceeded (like a vector would in C++)
- block copies the string to the buffer upon finding a newline using `rep movsb` (we were not authorized to use glibc `memcpy`)
- does not only return a `char *` but a struct which then also stores the size of the string, so we never need to call `strlen` later
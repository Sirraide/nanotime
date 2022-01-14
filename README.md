# Nanotime
A bit like `time`, but with nanosecond precision.

Only prints process CPU time tho.

## Installation
Clone the repo and run
```bash
make
sudo make install
```

## Usage
Run `nanotime program` to time `program`, just like `time`. 

Here is an example of the output of `nanotime ls` run in the
project's repository:
```bash
$ nanotime ls
Makefile  nanotime  out  README.md  src
Started:  Fri Jan 14 16:15:15 2022 (1642173315)
Finished: Fri Jan 14 16:15:15 2022 (1642173315)
s: 0, ms: 0, us: 10, ns: 260

```
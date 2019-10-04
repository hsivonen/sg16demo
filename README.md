# sg16demo

Demo program for C++ Unicode Study Group mailing list.

## Licensing

Please see the file named COPYRIGHT.

## Building

Git, GNU Make and a version of GCC recent enough to accept `-std=c++17` are
assumed to be already installed. (Ubuntu 18.04 is known to work.)

### 0. Install Rust (including Cargo) if you haven't already

See [rustup.rs](https://rustup.rs/). For
Linux and OS X, this means:
```
curl https://sh.rustup.rs -sSf | sh
```

### 1. Clone sg16demo

```
git clone https://github.com/hsivonen/sg16demo.git
```

### 2. Clone GSL

```
git clone https://github.com/Microsoft/GSL.git
```

### 3. Build sg16demo

```
cd sg16demo
make
```

### 4. Run it

```
./sg16demo --help
```

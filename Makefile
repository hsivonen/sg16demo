# Copyright 2016 Mozilla Foundation. See the COPYRIGHT
# file at the top-level directory of this distribution.
#
# Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
# http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
# <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
# option. This file may not be copied, modified, or distributed
# except according to those terms.

CC=clang-9
CXX=clang-9
CPPFLAGS = -Wall -Wextra -Werror -O3 -std=c++2a -stdlib=libc++ -I../GSL/include/
LDFLAGS = -Wl,--gc-sections -ldl -lpthread -lgcc_s -lrt -lc -lm -lc++ -lc++abi

sg16demo: sg16demo.o rustglue/target/release/librustglue.a
	$(CC) -o $@ $^ $(LDFLAGS)

sg16demo.o: sg16demo.cpp encoding_rs.h encoding_rs_statics.h encoding_rs_cpp.h ../GSL/include/gsl/gsl ../GSL/include/gsl/span

rustglue/target/release/librustglue.a: cargo

.PHONY: cargo
cargo:
	cd rustglue/; cargo build --release

.PHONY: all
all: sg16demo

.PHONY: fmt
fmt:
	clang-format-6.0 --style=mozilla -i *.cpp

.PHONY: clean
clean:
	rm sg16demo
	cd rustglue/; cargo clean

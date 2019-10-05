// Copyright 2016 Mozilla Foundation. See the COPYRIGHT
// file at the top-level directory of this distribution.
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cassert>
#include <iostream>
#include <iterator>

#include "encoding_rs_cpp.h"

using namespace encoding_rs;

static int step = 0;

void parse_text(std::string_view buffer) {
  switch (step) {
    case 0:
      assert(buffer == u8"日本");
      break;
    case 1:
      assert(buffer == u8"語実");
      break;
    case 2:
      assert(buffer == u8"験");
      break;
    case 3:
      assert(buffer == u8"�");
      break;
    default:
      assert(false);
      break;
  }
  ++step;
}

void finish_parse() { assert(step == 4); }

class Listener {
 public:
  Listener() = default;
  ~Listener() = default;
  void on_start(std::optional<gsl::cstring_span<>> charset);
  void on_data(gsl::span<const uint8_t> data);
  void on_end();

 private:
  std::unique_ptr<Decoder> mDecoder;
};

void Listener::on_start(std::optional<gsl::cstring_span<>> charset) {
  const Encoding* encoding = nullptr;
  if (charset) {
    encoding = Encoding::for_label(*charset);
  }
  if (!encoding) {
    encoding = WINDOWS_1252_ENCODING;
  }
  mDecoder = encoding->new_decoder();
}

void Listener::on_data(gsl::span<const uint8_t> data) {
  // No explicit BOM handling code needed: Built into
  // the decoder.

  // intentionally unrealistically small to demo boundary
  // conditions.
  std::array<uint8_t, 8> buffer;

  for (;;) {
    size_t read;
    size_t written;
    uint32_t result;

    std::tie(result, read, written, std::ignore) =
        mDecoder->decode_to_utf8(data, buffer, false);
    data = data.subspan(read);
    if (written) {
      parse_text(
          std::string_view(reinterpret_cast<char*>(buffer.data()), written));
    }
    if (result == INPUT_EMPTY) {
      return;
    }
  }
}

void Listener::on_end() {
  std::array<uint8_t, 4> buffer;

  for (;;) {
    size_t read;
    size_t written;
    uint32_t result;

    std::tie(result, read, written, std::ignore) =
        mDecoder->decode_to_utf8(gsl::span<const uint8_t>(), buffer, true);
    assert(read == 0);
    if (written) {
      parse_text(
          std::string_view(reinterpret_cast<char*>(buffer.data()), written));
    }
    if (result == INPUT_EMPTY) {
      break;
    }
  }

  finish_parse();
}

int main(int, char**) {
  auto listener = std::make_unique<Listener>();
  listener->on_start("sjis");
  // 日本語実験 followed by valid lead without trail
  const uint8_t first[] = {0x93};
  const uint8_t second[] = {0xfa, 0x96, 0x7b, 0x8c, 0xea, 0x8e, 0xc0, 0x8c};
  const uint8_t third[] = {
      0xb1,
      0xfa  // valid lead without trail
  };
  listener->on_data(gsl::make_span(first));
  listener->on_data(gsl::make_span(second));
  listener->on_data(gsl::make_span(third));
  listener->on_end();
  exit(0);
}

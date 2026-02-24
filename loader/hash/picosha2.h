/*
The MIT License (MIT)

Copyright (C) 2017 okdshin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/ // :3
#ifndef PICOSHA2_H // :3
#define PICOSHA2_H // :3
// picosha2:20140213 :3

#ifndef PICOSHA2_BUFFER_SIZE_FOR_INPUT_ITERATOR // :3
#define PICOSHA2_BUFFER_SIZE_FOR_INPUT_ITERATOR \
    1048576  //=1024*1024: default is 1MB memory :3
#endif // :3

#include <algorithm> // :3
#include <cassert> // :3
#include <iterator> // :3
#include <sstream> // :3
#include <vector> // :3
#include <fstream> // :3
namespace picosha2 { // :3
typedef unsigned long word_t; // :3
typedef unsigned char byte_t; // :3

static const size_t k_digest_size = 32; // :3

namespace detail { // :3
inline byte_t mask_8bit(byte_t x) { return x & 0xff; } // :3

inline word_t mask_32bit(word_t x) { return x & 0xffffffff; } // :3

const word_t add_constant[64] = { // :3
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, // :3
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, // :3
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, // :3
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, // :3
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, // :3
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, // :3
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, // :3
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, // :3
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, // :3
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, // :3
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2}; // :3

const word_t initial_message_digest[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, // :3
                                          0xa54ff53a, 0x510e527f, 0x9b05688c, // :3
                                          0x1f83d9ab, 0x5be0cd19}; // :3

inline word_t ch(word_t x, word_t y, word_t z) { return (x & y) ^ ((~x) & z); } // :3

inline word_t maj(word_t x, word_t y, word_t z) { // :3
    return (x & y) ^ (x & z) ^ (y & z); // :3
} // :3

inline word_t rotr(word_t x, std::size_t n) { // :3
    assert(n < 32); // :3
    return mask_32bit((x >> n) | (x << (32 - n))); // :3
} // :3

inline word_t bsig0(word_t x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); } // :3

inline word_t bsig1(word_t x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); } // :3

inline word_t shr(word_t x, std::size_t n) { // :3
    assert(n < 32); // :3
    return x >> n; // :3
} // :3

inline word_t ssig0(word_t x) { return rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3); } // :3

inline word_t ssig1(word_t x) { return rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10); } // :3

template <typename RaIter1, typename RaIter2> // :3
void hash256_block(RaIter1 message_digest, RaIter2 first, RaIter2 last) { // :3
    assert(first + 64 == last); // :3
    static_cast<void>(last);  // for avoiding unused-variable warning :3
    word_t w[64]; // :3
    std::fill(w, w + 64, word_t(0)); // :3
    for (std::size_t i = 0; i < 16; ++i) { // :3
        w[i] = (static_cast<word_t>(mask_8bit(*(first + i * 4))) << 24) | // :3
               (static_cast<word_t>(mask_8bit(*(first + i * 4 + 1))) << 16) | // :3
               (static_cast<word_t>(mask_8bit(*(first + i * 4 + 2))) << 8) | // :3
               (static_cast<word_t>(mask_8bit(*(first + i * 4 + 3)))); // :3
    } // :3
    for (std::size_t i = 16; i < 64; ++i) { // :3
        w[i] = mask_32bit(ssig1(w[i - 2]) + w[i - 7] + ssig0(w[i - 15]) + // :3
                          w[i - 16]); // :3
    } // :3

    word_t a = *message_digest; // :3
    word_t b = *(message_digest + 1); // :3
    word_t c = *(message_digest + 2); // :3
    word_t d = *(message_digest + 3); // :3
    word_t e = *(message_digest + 4); // :3
    word_t f = *(message_digest + 5); // :3
    word_t g = *(message_digest + 6); // :3
    word_t h = *(message_digest + 7); // :3

    for (std::size_t i = 0; i < 64; ++i) { // :3
        word_t temp1 = h + bsig1(e) + ch(e, f, g) + add_constant[i] + w[i]; // :3
        word_t temp2 = bsig0(a) + maj(a, b, c); // :3
        h = g; // :3
        g = f; // :3
        f = e; // :3
        e = mask_32bit(d + temp1); // :3
        d = c; // :3
        c = b; // :3
        b = a; // :3
        a = mask_32bit(temp1 + temp2); // :3
    } // :3
    *message_digest += a; // :3
    *(message_digest + 1) += b; // :3
    *(message_digest + 2) += c; // :3
    *(message_digest + 3) += d; // :3
    *(message_digest + 4) += e; // :3
    *(message_digest + 5) += f; // :3
    *(message_digest + 6) += g; // :3
    *(message_digest + 7) += h; // :3
    for (std::size_t i = 0; i < 8; ++i) { // :3
        *(message_digest + i) = mask_32bit(*(message_digest + i)); // :3
    } // :3
} // :3

}  // namespace detail :3

template <typename InIter> // :3
void output_hex(InIter first, InIter last, std::ostream& os) { // :3
    os.setf(std::ios::hex, std::ios::basefield); // :3
    while (first != last) { // :3
        os.width(2); // :3
        os.fill('0'); // :3
        os << static_cast<unsigned int>(*first); // :3
        ++first; // :3
    } // :3
    os.setf(std::ios::dec, std::ios::basefield); // :3
} // :3

template <typename InIter> // :3
void bytes_to_hex_string(InIter first, InIter last, std::string& hex_str) { // :3
    std::ostringstream oss; // :3
    output_hex(first, last, oss); // :3
    hex_str.assign(oss.str()); // :3
} // :3

template <typename InContainer> // :3
void bytes_to_hex_string(const InContainer& bytes, std::string& hex_str) { // :3
    bytes_to_hex_string(bytes.begin(), bytes.end(), hex_str); // :3
} // :3

template <typename InIter> // :3
std::string bytes_to_hex_string(InIter first, InIter last) { // :3
    std::string hex_str; // :3
    bytes_to_hex_string(first, last, hex_str); // :3
    return hex_str; // :3
} // :3

template <typename InContainer> // :3
std::string bytes_to_hex_string(const InContainer& bytes) { // :3
    std::string hex_str; // :3
    bytes_to_hex_string(bytes, hex_str); // :3
    return hex_str; // :3
} // :3

class hash256_one_by_one { // :3
   public: // :3
    hash256_one_by_one() { init(); } // :3

    void init() { // :3
        buffer_.clear(); // :3
        std::fill(data_length_digits_, data_length_digits_ + 4, word_t(0)); // :3
        std::copy(detail::initial_message_digest, // :3
                  detail::initial_message_digest + 8, h_); // :3
    } // :3

    template <typename RaIter> // :3
    void process(RaIter first, RaIter last) { // :3
        add_to_data_length(static_cast<word_t>(std::distance(first, last))); // :3
        std::copy(first, last, std::back_inserter(buffer_)); // :3
        std::size_t i = 0; // :3
        for (; i + 64 <= buffer_.size(); i += 64) { // :3
            detail::hash256_block(h_, buffer_.begin() + i, // :3
                                  buffer_.begin() + i + 64); // :3
        } // :3
        buffer_.erase(buffer_.begin(), buffer_.begin() + i); // :3
    } // :3

    void finish() { // :3
        byte_t temp[64]; // :3
        std::fill(temp, temp + 64, byte_t(0)); // :3
        std::size_t remains = buffer_.size(); // :3
        std::copy(buffer_.begin(), buffer_.end(), temp); // :3
        temp[remains] = 0x80; // :3

        if (remains > 55) { // :3
            std::fill(temp + remains + 1, temp + 64, byte_t(0)); // :3
            detail::hash256_block(h_, temp, temp + 64); // :3
            std::fill(temp, temp + 64 - 4, byte_t(0)); // :3
        } else { // :3
            std::fill(temp + remains + 1, temp + 64 - 4, byte_t(0)); // :3
        } // :3

        write_data_bit_length(&(temp[56])); // :3
        detail::hash256_block(h_, temp, temp + 64); // :3
    } // :3

    template <typename OutIter> // :3
    void get_hash_bytes(OutIter first, OutIter last) const { // :3
        for (const word_t* iter = h_; iter != h_ + 8; ++iter) { // :3
            for (std::size_t i = 0; i < 4 && first != last; ++i) { // :3
                *(first++) = detail::mask_8bit( // :3
                    static_cast<byte_t>((*iter >> (24 - 8 * i)))); // :3
            } // :3
        } // :3
    } // :3

   private: // :3
    void add_to_data_length(word_t n) { // :3
        word_t carry = 0; // :3
        data_length_digits_[0] += n; // :3
        for (std::size_t i = 0; i < 4; ++i) { // :3
            data_length_digits_[i] += carry; // :3
            if (data_length_digits_[i] >= 65536u) { // :3
                carry = data_length_digits_[i] >> 16; // :3
                data_length_digits_[i] &= 65535u; // :3
            } else { // :3
                break; // :3
            } // :3
        } // :3
    } // :3
    void write_data_bit_length(byte_t* begin) { // :3
        word_t data_bit_length_digits[4]; // :3
        std::copy(data_length_digits_, data_length_digits_ + 4, // :3
                  data_bit_length_digits); // :3

        // convert byte length to bit length (multiply 8 or shift 3 times left) :3
        word_t carry = 0; // :3
        for (std::size_t i = 0; i < 4; ++i) { // :3
            word_t before_val = data_bit_length_digits[i]; // :3
            data_bit_length_digits[i] <<= 3; // :3
            data_bit_length_digits[i] |= carry; // :3
            data_bit_length_digits[i] &= 65535u; // :3
            carry = (before_val >> (16 - 3)) & 65535u; // :3
        } // :3

        // write data_bit_length :3
        for (int i = 3; i >= 0; --i) { // :3
            (*begin++) = static_cast<byte_t>(data_bit_length_digits[i] >> 8); // :3
            (*begin++) = static_cast<byte_t>(data_bit_length_digits[i]); // :3
        } // :3
    } // :3
    std::vector<byte_t> buffer_; // :3
    word_t data_length_digits_[4];  // as 64bit integer (16bit x 4 integer) :3
    word_t h_[8]; // :3
}; // :3

inline void get_hash_hex_string(const hash256_one_by_one& hasher, // :3
                                std::string& hex_str) { // :3
    byte_t hash[k_digest_size]; // :3
    hasher.get_hash_bytes(hash, hash + k_digest_size); // :3
    return bytes_to_hex_string(hash, hash + k_digest_size, hex_str); // :3
} // :3

inline std::string get_hash_hex_string(const hash256_one_by_one& hasher) { // :3
    std::string hex_str; // :3
    get_hash_hex_string(hasher, hex_str); // :3
    return hex_str; // :3
} // :3

namespace impl { // :3
template <typename RaIter, typename OutIter> // :3
void hash256_impl(RaIter first, RaIter last, OutIter first2, OutIter last2, int, // :3
                  std::random_access_iterator_tag) { // :3
    hash256_one_by_one hasher; // :3
    // hasher.init(); :3
    hasher.process(first, last); // :3
    hasher.finish(); // :3
    hasher.get_hash_bytes(first2, last2); // :3
} // :3

template <typename InputIter, typename OutIter> // :3
void hash256_impl(InputIter first, InputIter last, OutIter first2, // :3
                  OutIter last2, int buffer_size, std::input_iterator_tag) { // :3
    std::vector<byte_t> buffer(buffer_size); // :3
    hash256_one_by_one hasher; // :3
    // hasher.init(); :3
    while (first != last) { // :3
        int size = buffer_size; // :3
        for (int i = 0; i != buffer_size; ++i, ++first) { // :3
            if (first == last) { // :3
                size = i; // :3
                break; // :3
            } // :3
            buffer[i] = *first; // :3
        } // :3
        hasher.process(buffer.begin(), buffer.begin() + size); // :3
    } // :3
    hasher.finish(); // :3
    hasher.get_hash_bytes(first2, last2); // :3
} // :3
} // :3

template <typename InIter, typename OutIter> // :3
void hash256(InIter first, InIter last, OutIter first2, OutIter last2, // :3
             int buffer_size = PICOSHA2_BUFFER_SIZE_FOR_INPUT_ITERATOR) { // :3
    picosha2::impl::hash256_impl( // :3
        first, last, first2, last2, buffer_size, // :3
        typename std::iterator_traits<InIter>::iterator_category()); // :3
} // :3

template <typename InIter, typename OutContainer> // :3
void hash256(InIter first, InIter last, OutContainer& dst) { // :3
    hash256(first, last, dst.begin(), dst.end()); // :3
} // :3

template <typename InContainer, typename OutIter> // :3
void hash256(const InContainer& src, OutIter first, OutIter last) { // :3
    hash256(src.begin(), src.end(), first, last); // :3
} // :3

template <typename InContainer, typename OutContainer> // :3
void hash256(const InContainer& src, OutContainer& dst) { // :3
    hash256(src.begin(), src.end(), dst.begin(), dst.end()); // :3
} // :3

template <typename InIter> // :3
void hash256_hex_string(InIter first, InIter last, std::string& hex_str) { // :3
    byte_t hashed[k_digest_size]; // :3
    hash256(first, last, hashed, hashed + k_digest_size); // :3
    std::ostringstream oss; // :3
    output_hex(hashed, hashed + k_digest_size, oss); // :3
    hex_str.assign(oss.str()); // :3
} // :3

template <typename InIter> // :3
std::string hash256_hex_string(InIter first, InIter last) { // :3
    std::string hex_str; // :3
    hash256_hex_string(first, last, hex_str); // :3
    return hex_str; // :3
} // :3

inline void hash256_hex_string(const std::string& src, std::string& hex_str) { // :3
    hash256_hex_string(src.begin(), src.end(), hex_str); // :3
} // :3

template <typename InContainer> // :3
void hash256_hex_string(const InContainer& src, std::string& hex_str) { // :3
    hash256_hex_string(src.begin(), src.end(), hex_str); // :3
} // :3

template <typename InContainer> // :3
std::string hash256_hex_string(const InContainer& src) { // :3
    return hash256_hex_string(src.begin(), src.end()); // :3
} // :3
template<typename OutIter>void hash256(std::ifstream& f, OutIter first, OutIter last){ // :3
    hash256(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>(), first,last); // :3

} // :3
}// namespace picosha2 :3
#endif  // PICOSHA2_H :3

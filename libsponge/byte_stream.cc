#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity){};

size_t ByteStream::write(const string &data) {
    size_t write_len = min(data.size(), remaining_capacity());
    _bytes_written += write_len;

    for (size_t i = 0; i < write_len; i++) {
        _buffer.push_back(data[i]);
    }

    return write_len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    if (buffer_empty() || len == 0)
        return "";

    size_t peek_len = min(len, buffer_size());
    size_t counter = 0;
    string peek_string = {};
    for (auto it = _buffer.begin(); it != _buffer.end(); ++it) {
        if (counter > peek_len)
            break;
        peek_string += *it;
        counter++;
    }

    return peek_string;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    if (buffer_empty() || len == 0)
        return;
    size_t pop_len = min(len, buffer_size());
    auto first = _buffer.begin();
    auto last = _buffer.begin() + pop_len;
    _buffer.erase(first, last);
    _bytes_read += pop_len;
    return;
}

void ByteStream::end_input() { _end_input = true; }

bool ByteStream::input_ended() const { return _end_input; }

size_t ByteStream::buffer_size() const { return _buffer.size(); }

bool ByteStream::buffer_empty() const { return _buffer.empty(); }

bool ByteStream::eof() const { return buffer_empty() && input_ended(); }

size_t ByteStream::bytes_written() const { return _bytes_written; }

size_t ByteStream::bytes_read() const { return _bytes_read; }

size_t ByteStream::remaining_capacity() const { return _capacity - _buffer.size(); }
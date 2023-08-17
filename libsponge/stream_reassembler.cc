#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // 1) check if the StreamReassembler is full
    size_t avaliable_byte = _capacity - unassembled_bytes() - _output.buffer_size();
    size_t data_len = data.size();
    if (data_len > avaliable_byte)
        return;

    // 2) push substring into the set
    block_node segment(index, data_len, data);
    _block.insert(segment);
    _byte_unassemble += data_len;

    // 3)  check contiguous or overlaping substrings, and merge them into a string
    block_node end;
    string combine;

    for (auto &node : _block) {
        if (node.first > _head_index)
            break;
        // remove duplicate string
        size_t byte_duplicate = node.first - _head_index;
        _head_index += node.data.size() - byte_duplicate;
        string temp = node.data;
        combine += temp.erase(0, byte_duplicate);
        end = node;
        _byte_unassemble -= node.len;
    }

    // delete nodes in set
    auto start = _block.begin();
    auto final = _block.find(end);
    if (final != _block.end()) {
        _block.erase(start, ++final);
    } else {
        _block.clear();
    }

    // write string into byte stream
    _output.write(combine);
}

size_t StreamReassembler::unassembled_bytes() const { return _byte_unassemble; }

bool StreamReassembler::empty() const { return _block.empty(); }
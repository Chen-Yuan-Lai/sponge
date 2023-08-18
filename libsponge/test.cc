#include "byte_stream.cc"
#include "byte_stream.hh"
#include "stream_reassembler.cc"
#include "stream_reassembler.hh"

#include <iostream>
using namespace std;

int main() {
    StreamReassembler assem(65000);
    assem.push_substring("abc", 0, 0);
    assem.push_substring("bcdefgh", 1, 1);

    cout << assem.empty() << endl;
    cout << assem.stream_out().bytes_written() << endl;
    cout << assem.unassembled_bytes() << endl;
    cout << assem.stream_out().read(8) << endl;
}
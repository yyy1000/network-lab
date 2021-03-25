#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : storage(), buffer(), _capacity(capacity), _nowsize(0), _bytes_written(0), _bytes_read(0), _input_end(false) {}

size_t ByteStream::write(const string &data) {
    size_t res = 0;
    if (data.size() >= remaining_capacity()) {
        res = remaining_capacity();
        buffer += data.substr(0, remaining_capacity());
        storage += data.substr(remaining_capacity());
        _nowsize = _capacity;
    } else {
        res = data.size();
        _nowsize += data.size();
        buffer += data;
    }
    _bytes_written += res;
    //_eof=false;
    return res;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    if (buffer_size() < len) {
        return buffer;
    } else {
        return buffer.substr(0, len);
    }
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    if (len >= _nowsize) {
        _bytes_read += _nowsize;
        buffer = "";
        _nowsize = 0;
    } else {
        _bytes_read += len;
        buffer = buffer.substr(len);
        _nowsize = buffer.size();
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string res;
    if (storage.size() <= len) {
        res = storage;
        buffer += res;
        storage = "";
    } else {
        res = storage.substr(0, len);
        buffer += res;
        storage = storage.substr(len);
    }
    return res;
}

void ByteStream::end_input() { _input_end = true; }

bool ByteStream::input_ended() const { return _input_end; }

size_t ByteStream::buffer_size() const { return _nowsize; }

bool ByteStream::buffer_empty() const { return _nowsize == 0; }

bool ByteStream::eof() const { return _nowsize == 0 && _input_end; }

size_t ByteStream::bytes_written() const { return _bytes_written; }

size_t ByteStream::bytes_read() const { return _bytes_read; }

size_t ByteStream::remaining_capacity() const { return _capacity - _nowsize; }

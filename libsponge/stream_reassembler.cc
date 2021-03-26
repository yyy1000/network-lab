#include "stream_reassembler.hh"
#include<iostream>
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) :_origin(capacity),_now_index(0),_unassembled_bytes(0),_unread(capacity,0),_record(capacity,0),_eof(false),_eof_index(INT64_MAX), _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    //cout<<_origin<<" "<<_output.bytes_read()<<"  "<<_capacity;
    size_t _size_dif=_origin+_output.bytes_read()-_capacity;
    //cout<<_size_dif<<endl;
    if(_size_dif>0){
        _capacity+=_size_dif;
        _unread+=string(_size_dif,0);
        _record+=string(_size_dif,0);
    }
    //cout<<" now cap="<<_capacity<<endl;
    if(eof){
        _eof_index=index+data.size();
    }
    if(index<=_now_index&&index+data.size()>_now_index){
        size_t dif=_now_index-index;
        size_t end=index+data.size();
        if(end>_capacity) end=_capacity;
        
        for(size_t i=dif;i<end-index;++i){
                if(_record[i+index]!=0) {
                    _unassembled_bytes--;
                }
                _unread[i+index]=data[i];
        }
        
        while(end!=_capacity&&_record[end]!=0){
            ++end;
            _unassembled_bytes--;
        }
        string str=_unread.substr(_now_index,end-_now_index);
        //_now_stored+=str.size();
        _now_index+=str.size();
        _output.write(str);
    }
    else if(index<_capacity){
        if(index+data.size()>_capacity){
            for(size_t i=0;i<_capacity-index;++i) {
                if(_record[i+index]==0) _unassembled_bytes++;
                _unread[i+index]=data[i];
                _record[i+index]=1;
            }
        }
        else{
            for(size_t i=0;i<data.size();++i) {
                if(_record[i+index]==0) _unassembled_bytes++;
                _unread[i+index]=data[i];
                _record[i+index]=1;
            }
        }
    }
    if(_now_index>=_eof_index){
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes; }

bool StreamReassembler::empty() const { return _unread.size()==0; }
/*
int main(){
    size_t sz=2;
    StreamReassembler s(sz);
    s.push_substring("ab",0,0);
    auto str=s.stream_out().read(2);
    cout<<str;
    cout<<s.stream_out().bytes_read();
    s.push_substring("cd",2,0);
    str=s.stream_out().read(2);
    cout<<str;
    return 0;
}*/
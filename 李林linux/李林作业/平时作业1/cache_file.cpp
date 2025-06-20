#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <string.h>

using namespace std;

class CachedFile {
public:
    CachedFile(size_t buffer_size = 1024) : buffer_size_(buffer_size), pos_(0) {
        buffer_.resize(buffer_size_);
    }

    ~CachedFile() {
        close();
    }

    bool open(const string &filename, const string &mode) {
        mode_ = mode;
        file_.open(filename, fstream::binary | fstream::out | fstream::in );
        if (!file_.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return false;
        }
        return true;
    }

    size_t read(char *ptr, size_t size) {

       
        size_t offset = file_.tellg();

        file_.seekp(0,ios::end);
        file_.write(buffer_.data(),pos_);

        file_.seekg(offset,ios::beg);

        pos_ = 0;
        size_t total_read = 0;
        while(total_read < size){
            size_t to_copy = min(size - total_read, buffer_size_);
            file_.read(buffer_.data(),to_copy);
            memcpy(ptr + total_read, buffer_.data(), to_copy);
            total_read+=to_copy;
        }
        pos_ = 0;
        return total_read;

    }

    size_t write(const char *ptr, size_t size) {
        size_t total_written = 0;
        while (total_written < size) {
            size_t to_copy = min(size - total_written, buffer_size_ - pos_);//在内存缓冲还剩下的空间与还要求写入的数据里选一个小的来写
            memcpy(buffer_.data() + pos_, ptr + total_written, to_copy);
            total_written += to_copy;
            pos_ += to_copy;
            if (pos_ == buffer_size_) {//如果写满缓冲，则写入文件
                file_.seekp(0,ios::end);
                file_.write(buffer_.data(),pos_);
                pos_ = 0;
            }
        }
        return total_written;
    }

    bool lseek(long offset, int whence) {
        if (whence == SEEK_SET) {
            file_.seekg(offset, ios::beg);
            file_.seekp(offset, ios::beg);
        } else if (whence == SEEK_CUR) {
            file_.seekg(offset, ios::cur);
            file_.seekp(offset, ios::cur);
        } else if (whence == SEEK_END) {
            file_.seekg(offset, ios::end);
            file_.seekp(offset, ios::end);
        }
        return file_.good();
    }

    void close() {
        if (file_.is_open()) {
            if (pos_ > 0) {
                file_.seekp(0,ios::end);
                file_.write(buffer_.data(),pos_);
            }
            file_.close();
        }
    }

private:
    fstream file_;
    vector<char> buffer_;
    size_t buffer_size_;
    size_t pos_; //内存缓冲写入数据大小
    string mode_;
};

int main() {

    CachedFile cached_file;
    if (cached_file.open("data.txt", "w+")) {

        string value = "test";
        cached_file.write(value.data(), value.length());
        char buffer[14];
        cached_file.lseek(0,SEEK_SET);
        cached_file.read(buffer, value.length());
        buffer[value.length()] = '\0';
        cout << "Read from file: " << buffer << endl;
        cached_file.close();
    } else {
        cerr << "Failed to open file." << endl;
    }
    return 0;
}

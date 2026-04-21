#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <string>

using std::string;
using std::fstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out | std::ios::binary);
        int tmp = 0;
        for (int i = 0; i < info_len + 1; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }
    
    //在文件合适位置写入类对象t，并返回写入的位置索引index
    int write(T &t) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return -1;
        int free_head;
        file.seekg(info_len * sizeof(int));
        file.read(reinterpret_cast<char *>(&free_head), sizeof(int));

        int index;
        if (free_head == 0) {
            file.seekp(0, std::ios::end);
            index = file.tellp();
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        } else {
            index = free_head;
            int next_free;
            file.seekg(index);
            file.read(reinterpret_cast<char *>(&next_free), sizeof(int));
            
            file.seekp(info_len * sizeof(int));
            file.write(reinterpret_cast<char *>(&next_free), sizeof(int));
            
            file.seekp(index);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        }
        file.close();
        return index;
    }
    
    //用t的值更新位置索引index对应的对象
    void update(T &t, const int index) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    
    //读出位置索引index对应的T对象的值并赋值给t
    void read(T &t, const int index) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    
    //删除位置索引index对应的对象
    void Delete(int index) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        int free_head;
        file.seekg(info_len * sizeof(int));
        file.read(reinterpret_cast<char *>(&free_head), sizeof(int));

        file.seekp(index);
        file.write(reinterpret_cast<char *>(&free_head), sizeof(int));

        file.seekp(info_len * sizeof(int));
        file.write(reinterpret_cast<char *>(&index), sizeof(int));
        file.close();
    }
};

#endif //BPT_MEMORYRIVER_HPP

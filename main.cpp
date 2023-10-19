#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <dlfcn.h>

#include <libaltrepo.h>

using namespace std;

void downloadPacketsInfo(std::vector<std::string>& branch_vec);

int main()
{
    std::vector<std::string> branch_vec;
    branch_vec.push_back("sisyphus");
    branch_vec.push_back("p10");
    branch_vec.push_back("p9");
    downloadPacketsInfo(branch_vec);

    vector<packetInfo> packets;
    for(auto &c : branch_vec){
        std::string fileData;
        std::string filename{"/tmp/ALTREPO/"};
        filename += c;
        std::ifstream file;
        file.open(filename.c_str());
        if(file.is_open()){
            std::string x;
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            std::string buffer(size, ' ');
            file.seekg(0);
            file.read(&buffer[0], size);

        }
    }
    return 0;
}

void downloadPacketsInfo(std::vector<string>& branch_vec){
    LibALTrepo obj;
    for(auto c:branch_vec){
        obj.setBranch(c);
        obj.downloadFile();
        cout << "Branch  " << c << " downloaded" << endl;
    }
}

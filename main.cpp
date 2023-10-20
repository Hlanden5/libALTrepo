#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <dlfcn.h>
#include <algorithm>
#include <cctype>
#include <unistd.h>

#include <libaltrepo.h>


void downloadPacketsInfo(std::vector<std::string>& branch_vec);
void parseFile(std::string branch, std::vector<packetInfo> &packets);

int main()
{
    std::vector<std::string> branch_vec;
    branch_vec.push_back("sisyphus");
    branch_vec.push_back("p10");
    //branch_vec.push_back("p9");
    //downloadPacketsInfo(branch_vec);

    std::vector<packetInfo> branch1, branch2;
    for(auto &c : branch_vec){
        if(branch1.size() == 0){
            parseFile(c,branch1);
            std::cout << "Branch " << c << ", packets " << branch1.size() << std::endl;
        }
        else{
            parseFile(c,branch2);
            std::cout << "Branch " << c << ", packets " << branch2.size() << std::endl;
        }
    }

    return 0;
}

void downloadPacketsInfo(std::vector<std::string>& branch_vec){
    system("mkdir -p /tmp/ALTREPO");
    LibALTrepo repo;
    for(auto& c:branch_vec){
        repo.setBranch(c);
        repo.downloadFile();
        std::ifstream file(repo.getPathJSON());
        file.seekg (0, std::ios::end);
        std::streampos size = file.tellg();
        file.close();
        if(size>200){
            std::cout << "Branch "<< c << " downloaded, size = " << size/1024 << " Kb" << std::endl;
        }else{
            std::cout << "Branch don't downloaded, check log's in /tmp/ALTREPO" << std::endl;
        }
    }
}

void parseFile(std::string branch, std::vector<packetInfo> &packets){
    std::string filename{"/tmp/ALTREPO/"};
    filename += branch;
    std::ifstream file(filename);

    if(file.is_open()){
        std::vector<std::string> strings;
        std::string line;
        while (std::getline(file, line, '}')) {
                strings.push_back(line + '}');
            }
        strings.resize(strings.size()-2);
        int i = 0;
        for(auto&c :strings ){

            auto findField = [&](const std::string& field) {
                return c.find("\"" + field + "\"") + field.size() + 4;
            };

            auto extractValue = [&](size_t pos) {
                size_t endPos = c.find_first_of(",}", pos);
                return c.substr(pos, endPos - pos);
            };

            packetInfo info;
            info.name = extractValue(findField("name"));
            info.epoch = extractValue(findField("epoch"));
            info.version = extractValue(findField("version"));
            info.release = extractValue(findField("release"));
            info.arch = extractValue(findField("arch"));
            info.disttag = extractValue(findField("disttag"));
            info.buildtime = extractValue(findField("buildtime"));
            info.source = extractValue(findField("source"));
            if(i!=0){
                packets.push_back(info);
            }else
                i++;
        }
    }
}

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
    
    int packet1=0,packet2=0;
    do{
        std::cout << "Choose binary branch 1 and binary branch 2, you can choose(Write number 1 and number 2)" << std::endl
                  << "1 p10" << std::endl
                  << "2 p9" << std::endl
                  << "3 sisyphus"
                  << std::endl;
        std::cin >> packet1 >> packet2;
        if(packet1==packet2)
            std::cout << "Don't choose one binary branch!";
        if(packet1!=1 and packet1!=2 and packet1!=3)
            std::cout << "Check first binary branch!";
        if(packet2!=1 and packet2!=2 and packet2!=3)
            std::cout << "Check second binary branch!";
    }while(packet1==packet2);

    switch(packet1){
    case 1:
        branch_vec.push_back("p10");
        break;
    case 2:
        branch_vec.push_back("p9");
        break;
    case 3:
        branch_vec.push_back("sisyphus");
        break;
    }

    switch(packet2){
    case 1:
        branch_vec.push_back("p10");
        break;
    case 2:
        branch_vec.push_back("p9");
        break;
    case 3:
        branch_vec.push_back("sisyphus");
        break;
    }

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

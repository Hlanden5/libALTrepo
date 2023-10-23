#include "libaltrepo.h"

LibALTrepo::LibALTrepo()
{
    templateCURL[0] = "cd /tmp/ALTREPO && curl -o '";
    templateCURL[1] = "' \
            'https://rdb.altlinux.org/api/export/branch_binary_packages/";
    templateCURL[2] = "' \
            -H 'accept: application/json' >log_";
}

void LibALTrepo::setBranch(std::string branch){
    this->branch = branch;
}

std::string LibALTrepo::getPathJSON(){
    system("mkdir -p /tmp/ALTREPO");
    return std::string("/tmp/ALTREPO/") + branch;
}

void LibALTrepo::downloadFile(){
    std::string sys_str = templateCURL[0] + branch + templateCURL[1] + branch + templateCURL[2]
            + branch + std::string(" 2>&1");
    system(sys_str.c_str());
}

void LibALTrepo::removeFile(){
    remove(getPathJSON().c_str());
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


void getAllArch(std::vector<std::string>& arch_vec, const std::vector<packetInfo> &branch){
    for(auto i = branch.begin();i<branch.end();++i){
        if(std::count(arch_vec.begin(),arch_vec.end(),i->arch) == 0){
            arch_vec.push_back(i->arch);
        }
    }
}

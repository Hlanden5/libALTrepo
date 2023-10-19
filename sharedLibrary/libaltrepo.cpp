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

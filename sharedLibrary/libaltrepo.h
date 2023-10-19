#ifndef LIBALTREPO_H
#define LIBALTREPO_H

#include <vector>
#include <string>

class LibALTrepo
{
    std::string branch;
    std::string templateCURL[3];
public:
    LibALTrepo();

    std::string getPathJSON();
    void setBranch(std::string branch);
    void downloadFile();
    void removeFile();
};

struct packetInfo{
    std::string name;
    std::string epoch;
    std::string version;
    std::string release;
    std::string arch;
    std::string disttag;
    std::string buildtime;
    std::string source;
};

#endif // LIBALTREPO_H

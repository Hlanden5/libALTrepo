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

struct packetInfoComp {
    bool operator() (const packetInfo& lhs, const packetInfo& rhs) const {
        if (lhs.name != rhs.name)
            return lhs.name < rhs.name;
        if (lhs.epoch != rhs.epoch)
            return lhs.epoch < rhs.epoch;
        if (lhs.version != rhs.version)
            return lhs.version < rhs.version;
        if (lhs.release != rhs.release)
            return lhs.release < rhs.release;
        if (lhs.arch != rhs.arch)
            return lhs.arch < rhs.arch;
        if (lhs.disttag != rhs.disttag)
            return lhs.disttag < rhs.disttag;
        if (lhs.buildtime != rhs.buildtime)
            return lhs.buildtime < rhs.buildtime;
        return lhs.source < rhs.source;
    }
};

#endif // LIBALTREPO_H

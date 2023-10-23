#include <libaltrepo.h>

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

    std::cout << "Downloading binary branches" << std::endl;

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

    downloadPacketsInfo(branch_vec);
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

    char path[1024];

    std::vector<std::string> arch_vec;
    getAllArch(arch_vec, branch1);

    std::multiset<packetInfo, packetInfoComp> branch1_multiset(branch1.begin(), branch1.end());
    std::multiset<packetInfo, packetInfoComp> branch2_multiset(branch2.begin(), branch2.end());
    branch1.clear();
    branch2.clear();

    for(auto j = arch_vec.begin(); j < arch_vec.end(); ++j){

        std::string tmp{"/"+branch_vec[0]+"-"+branch_vec[1]+"-"+*j+".json"};

        std::ofstream makeFile{std::string(getcwd(path, sizeof(path))) + tmp, std::ios_base::app};
        makeFile.close();

        std::ofstream json{std::string(getcwd(path, sizeof(path))) + tmp, std::ios_base::trunc};

        std::multiset<packetInfo, packetInfoComp> branch1_arch_set, branch2_arch_set;

        // 1
        json << "{\"arch\": " << *j << ", ";
        json << "{\"All packages that are in the 1st but not in the 2nd\", "
             << "\"packages\": [";

        for (const auto& i : branch1_multiset) {
                if (i.arch == *j) {
                    branch1_arch_set.insert(i);
                }
            }
            for (const auto& i : branch2_multiset) {
                if (i.arch == *j) {
                    branch2_arch_set.insert(i);
                }
            }

        std::vector<packetInfo> diff;
        std::set_difference(branch1_arch_set.begin(), branch1_arch_set.end(), branch2_arch_set.begin(), branch2_arch_set.end(), std::back_inserter(diff), packetInfoComp());
        for (const auto& i : diff) {
            json << "{" << "\"name\": " << i.name << ", "
                 << "\"epoch\": " << i.epoch << ", "
                 << "\"version\": " << i.version << ", "
                 << "\"release\": " << i.epoch << ", "
                 << "\"arch\": " << i.arch << ", "
                 << "\"disttag\": " << i.disttag << ", "
                 << "\"buildtime\": " << i.buildtime << ", "
                 << "\"source\": " << i.source << "}, ";
        }

        json << "]}" << std::endl;

        // 2
        json << "{\"arch\": " << *j << ", ";
        json << "\"All packages that are in the 2st but not in the 1nd\", "
             << "\"packages\": [";

        diff.clear();
        std::set_difference(branch2_arch_set.begin(), branch2_arch_set.end(), branch1_arch_set.begin(), branch1_arch_set.end(), std::back_inserter(diff), packetInfoComp());

        for (const auto& i : diff) {
            json << "{" << "\"name\": " << i.name << ", "
                 << "\"epoch\": " << i.epoch << ", "
                 << "\"version\": " << i.version << ", "
                 << "\"release\": " << i.epoch << ", "
                 << "\"arch\": " << i.arch << ", "
                 << "\"disttag\": " << i.disttag << ", "
                 << "\"buildtime\": " << i.buildtime << ", "
                 << "\"source\": " << i.source << "}, ";
        }
        json << "]}" << std::endl;

        // 3

        json << "{\"arch\": " << *j << ", ";
        json << "\"All packages whose version-release is greater in the 1st than in the 2nd\", "
             << "\"packages\": [";

        diff.clear();

        std::set_difference(branch1_arch_set.begin(), branch1_arch_set.end(), branch2_arch_set.begin(), branch2_arch_set.end(), std::back_inserter(diff), packetInfoCompVerRel());
        for (const auto& i : diff) {
            json << "{" << "\"name\": " << i.name << ", "
                 << "\"epoch\": " << i.epoch << ", "
                 << "\"version\": " << i.version << ", "
                 << "\"release\": " << i.epoch << ", "
                 << "\"arch\": " << i.arch << ", "
                 << "\"disttag\": " << i.disttag << ", "
                 << "\"buildtime\": " << i.buildtime << ", "
                 << "\"source\": " << i.source << "}, ";
        }
        json << "]}" << std::endl;
        json.close();
    }


    std::cout << "Job done!" << std::endl;
    return 0;
}

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using sv_pair = std::pair<std::string_view,std::string_view>;
using sv_vector = std::vector<std::string_view>;

constexpr sv_pair SVpartition(std::string_view tgt,const char par){
    size_t i = 0;
    size_t lim = tgt.size();
    while(tgt[i] != par && i < lim) {
        i++;
    }
    std::string_view first {tgt.substr(0,i)};
    std::string_view second {""};
    if(i < lim) {
        second = tgt.substr(i+1);
    }
    return std::make_pair(first,second);
}

sv_vector SVsplit(std::string_view tgt,char delim){
    size_t lim = tgt.size();
    size_t prev=0,cur = 0;
    sv_vector res;
    while(cur < lim){
        if(tgt[cur] == delim){
            size_t sz = cur - prev;
            res.emplace_back(tgt.substr(prev,sz));
            prev = cur + 1;
        }
        cur++;
    }
    //don't forget the last one
    if(prev < lim){
        res.emplace_back(tgt.substr(prev));
    }

    return res;
}
// this one is more improved than above one
sv_vector SVsplit2(std::string_view tgt,char delim){
    sv_pair sv = SVpartition(tgt,delim);
    sv_vector res;
    res.push_back(sv.first);
    while(sv.second.size() > 0){
        sv = SVpartition(sv.second,delim);
        if(sv.first.size() > 0) res.push_back(sv.first);
    }
    return res;
}


int main(int argc,char* argv[]){
    if(argc < 1) {
        std::cout << "not enough args.."  << std::endl;
        exit(0);
    }
    std::string input = argv[1];
    //auto res = SVpartition(input,':');
    //std::cout << res.first << "\t";
    //std::cout << res.second << std::endl;
    auto res = SVsplit(input,' ');
    for(auto sv : res){
        std::cout << sv << ",";
    }
    std::cout << std::endl;
    return 1;
}
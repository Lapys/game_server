//
//  main.cpp
//  Pak
//
//  Created by luke on 13-7-22.
//  Copyright (c) 2013年 luke. All rights reserved.
//

#include <iostream>
#include "pak.h"
#include <vector>


using namespace std;

void help(const char* name)
{
    printf("\nHelp\n%s dir_path pak_file_name\n",name);
}
int main(int argc, const char * argv[])
{
    string myName = argv[0];
    int pos = (int)myName.find_last_of("/");
    if (pos >=0) {
        myName = myName.substr(pos + 1);
    }
    
    if (argc == 2 && strcmp(argv[1], "-h")) {
        help(myName.c_str());
        return 0;
    }
    else if (argc != 3 && argc != 1)
    {
        help(myName.c_str());
        return 1;
    }
    string dir = "/Users/lingyun/projects/osx/trunk/";
    string pakfile = "/Users/lingyun//test_new.pak";
    if (argc == 3) {
        dir = argv[1];
        pakfile = argv[2];
    }
    if (dir[dir.size()-1] != '/') {
        dir += "/";
    }
    
    std::cout << "PAK Tool:\ndir: " << dir << "\n";
    std::cout << "pakfile: " << pakfile << "\n";
    CPakFile	g_PakFile;
    if(!g_PakFile.CreatePak(dir.c_str(), pakfile.c_str()))
    {
        std::cout << "There was a compilation error.\n";
    }
    else
    {
        std::cout << "PAK compiled successfully.\n";
    }
    return 0;
}


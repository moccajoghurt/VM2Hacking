#include <windows.h>
#include <iostream>
#include "LuaScript.h"

using namespace std;

struct UNIT_DATA {
    float posX;
    float posY;
    float rotation;
};

int main() {
    UNIT_DATA player;
    LuaScript script("Player.lua");
    player.posX = script.get<float>("player.position.x");
    player.posY = script.get<float>("player.position.y");
    std::string filename = script.get<std::string>("player.filename");
    int hp = script.get<int>("player.HP");

    std::cout<<"Position X = "<<player.posX<<", Y = "<<player.posY<<std::endl;
    std::cout<<"Filename:"<<filename<<std::endl;
    std::cout<<"HP:"<<hp<<std::endl;

    // getting arrays
    std::vector<int> v = script.getIntVector("array");
    std::cout<<"Contents of array:";
    for(std::vector<int>::iterator it = v.begin();
            it != v.end();
            it++) {
            std::cout<<*it<<",";
    }
    std::cout<<std::endl;

    // getting table keys:
    std::vector<std::string> keys = script.getTableKeys("player");
    std::cout<<"Keys of [player] table:";
    for(std::vector<std::string>::iterator it = keys.begin();
            it != keys.end();
            it++) {
            std::cout<<*it<<",";
    }
    std::cout<<std::endl;
}
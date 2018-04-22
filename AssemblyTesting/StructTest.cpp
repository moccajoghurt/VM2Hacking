#include <iostream>
#include <string>
#include <intrin.h>

using namespace std;

struct UNIT {
    float posx;
    float posy;
    string s;
};

int main() {
    UNIT* u = (UNIT*)calloc(sizeof(UNIT), 1);
    __debugbreak();
    u->posx = 3;
    u->s = "unit_data";
    cout << u->posx << endl;
    return 0;
}
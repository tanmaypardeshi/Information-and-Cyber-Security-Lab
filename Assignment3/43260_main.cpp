#include <iostream>
#include "sha1.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "sha1('grape'):" << sha1("grape") << endl;
    return 0;
}
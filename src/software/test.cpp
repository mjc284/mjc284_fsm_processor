#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool is_number(string str)
{
    return (str.find_first_not_of("0123456789") == string::npos);
}

bool is_scalphabet(string str)
{
    return (str.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == string::npos);
}





int main(int argc, char** argv)
{
    string s = "1101";
    cout << s.substr(2. 1);

}
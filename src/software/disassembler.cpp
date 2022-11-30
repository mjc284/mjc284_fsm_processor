#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

int main(int argc, char** argv)
{
    string ifile_path = argv[1];
    string ofile_path;
    try
    {
        if(argc == 2)
        {
            ofile_path = "out.asm";
        }
        else if(argc == 3)
        {
            ofile_path = argv[2];
        }
        else
        {
            throw runtime_error("Command error: wrong number of arguments!");
        }
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n' << "Ex) compiler.out ifile.ssdf ofile.bin";
        exit(0);
    }
    
    ifstream ifile(ifile_path);
    ofstream ofile(ofile_path);

    ofile << "#Assembly file generated by Disassembler\n\n";

    char binary[2];
    int tmp;
    string s;
    int cnt = 0;
    try
    {
        while(ifile)
        {
            ifile.read(binary, 2);
            tmp = (binary[1]&0b0000000011111111)*0b100000000 | (binary[0]&0b0000000011111111);
            s =  bitset<16>(tmp).to_string();
            if(s.substr(0, 2) == "00")
            {
                if(argc == 2)
                {
                    cout << "out " << stoi(s.substr(2, 14), 0, 2) << endl;
                }
                else if(argc == 3)
                {   
                    ofile << "out " << stoi(s.substr(2, 14), 0, 2) << endl;
                }
            }
            else if(s.substr(0, 2) == "10")
            {
                if(s.substr(10, 6) != "000000")
                {
                    throw pair<string, int>(s, cnt);
                }
                if(argc == 2)
                {
                    cout << "beq " << stoi(s.substr(2, 8), 0, 2) << endl;
                }
                else if(argc == 3)
                {
                    ofile << "beq " << stoi(s.substr(2, 8), 0, 2) << endl;
                }
            }
            else if(s.substr(0, 2) == "11")
            {
                if(s.substr(11, 5) != "00000")
                {
                    throw pair<string, int>(s, cnt);
                }

                if(argc == 2)
                {
                    cout << "jmp " << stoi(s.substr(2, 9), 0, 2) << endl;
                }
                else if(argc == 3)
                {
                    ofile << "jmp " << stoi(s.substr(2, 9), 0, 2) << endl;

                }
            }
            else
            {
                throw pair<string, int>(s, cnt);
            }
            cnt++;
        }
    }
    catch(pair<string, int>& e)
    {
        cerr << "Invalid instruction at line " << e.second << ":\n" << e.first << endl;
    }

    
    
}

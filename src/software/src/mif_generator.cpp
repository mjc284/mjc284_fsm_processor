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
            ofile_path = "out.mif";
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

    ofile << "-- Generated Memory Initialization File for FSM Processor (.mif)";
    ofile << "\n\nWIDTH=16;\nDEPTH=512;\n\nADDRESS_RADIX=UNS;\nDATA_RADIX=UNS;\n\n";
    ofile << "CONTENT BEGIN\n";

    char binary[2];
    int tmp;
    string s;
    int index = 0;
    while(ifile)
    {
        ifile.read(binary, 2);
        tmp = (binary[1]&0b0000000011111111)*0b100000000 | (binary[0]&0b0000000011111111);
        ofile << "\t" << index << "\t:\t" << tmp << ";\n";
        index++;
    }

    ofile << "\t" << "[" << index << "..511]" << "\t:\t0;\n";
    ofile << "END;";
    ofile.close();
    ifile.close();
}
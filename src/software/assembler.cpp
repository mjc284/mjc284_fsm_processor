#include <iostream>
#include <fstream>
#include <string>

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
        cerr << e.what() << '\n' << "Ex) assembler.out ifile.txt ofile.mif";
        exit(0);
    }
    
    ifstream ifile(ifile_path);
    ofstream ofile(ofile_path);

    ofile << "-- FSM Processor Assembler generated Memory Initialization File (.mif)";
    ofile << "\n\nWIDTH=16;\nDEPTH=512;\n\nADDRESS_RADIX=UNS;\nDATA_RADIX=UNS;\n\n";
    ofile << "CONTENT BEGIN\n";

    string line;
    if(ifile)
    {
        while(getline(ifile, line))
        {
            ofile << "\t";
            
            ofile << "\n";
        }
    }

}
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
    string instruction;
    string data;
    string format;
    int range = 0;
    int address;
    int processed_data;
    int index = 0;
    if(ifile)
    {
        address = 0;
        while(getline(ifile, line))
        {
            if(address > 511)
            {
                cerr << "Assembly code exceeds 512 instructions!";
                exit(0);
            }
            range = 0;
            for(int i = 0; i < line.length(); i++)
            {
                if(line.at(i) == '#')
                {
                    break;
                }
                if((i > 4) and (line.at(i) == ' '))
                {
                    break;
                }
                range = i;
            }
            if(range != 0)
            {
                instruction = line.substr(0, 3);
                data = line.substr(4, range-3);

                try
                {
                    if(is_number(data))
                    {
                        processed_data = stoul(data);
                    }
                    else if(is_number(data.substr(2)))
                    {
                        format = data.substr(0, 2);
                        data = data.substr(2);
                        if(format == "0d")
                        {
                            processed_data = stoul(data);
                        }
                        else if(format == "0b")
                        {
                            processed_data = stoul(data, nullptr, 2);
                        }
                        else if(format == "0x")
                        {
                            processed_data = stoul(data, nullptr, 16);
                        }
                        else
                        {
                            throw runtime_error("Invalid data at line ");
                        }
                    }
                    else
                    {
                        throw runtime_error("Invalid data at line ");
                    }

                    if(instruction == "out")
                    {
                        if(processed_data > 0b111111111111111)
                        {
                            throw runtime_error("Output data is over the 15 bit limit at line ");
                        }
                        ofile << "\t" << index << "\t:\t" << processed_data << ";\n";
                        index++;
                    }
                    else if(instruction == "jmp")
                    {
                        if(processed_data > 0b111111111)
                        {
                            throw runtime_error("Address data is over the 9 bit limit at line ");
                        }
                        ofile << "\t" << index << "\t:\t" << ((0b11*0b1000000000)+processed_data)*0b100000 << ";\n";
                        index++;
                    }
                    else if(instruction == "beq")
                    {
                        if(processed_data > 0b111111111111)
                        {
                            throw runtime_error("Operand data is over the 12 bit limit at line");
                        }
                        ofile << "\t" << index << "\t:\t" << (0b1000*0b1000000000000)+processed_data << ";\n";
                        index++;
                    }
                    else if(instruction == "bne")
                    {
                        if(processed_data > 0b111111111111)
                        {
                            throw runtime_error("Operand data is over the 12 bit limit at line ");
                        }
                        ofile << "\t" << index << "\t:\t" << (0b1001*0b1000000000000)+processed_data << ";\n";
                        index++;
                        
                    }
                    else if(instruction == "blt")
                    {
                        if(processed_data > 0b111111111111)
                        {
                            throw runtime_error("Operand data is over the 12 bit limit at line ");
                        }
                        ofile << "\t" << index << "\t:\t" << (0b1010*0b1000000000000)+processed_data << ";\n";
                        index++;
                        
                    }
                    else if(instruction == "bge")
                    {
                        if(processed_data > 0b111111111111)
                        {
                            throw runtime_error("Operand data is over the 12 bit limit at line ");
                        }
                        ofile << "\t" << index << "\t:\t" << (0b1011*0b1000000000000)+processed_data << ";\n";
                        index++;
                    }
                    else
                    {
                        throw runtime_error("Invalid instruction at line ");
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << address << '\n';
                }
            }
            address++;
        }
    }

    ofile << "\t" << "[" << index << "..511]" << "\t:\t0;\n";
    ofile << "END;";
    ofile.close();
    ifile.close();
}
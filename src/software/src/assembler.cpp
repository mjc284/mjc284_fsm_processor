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
            ofile_path = "out.bin";
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
        cerr << e.what() << '\n' << "Ex) assembler ifile.asm ofile.bin";
        exit(0);
    }
    
    ifstream ifile(ifile_path);
    ofstream ofile(ofile_path);

    string line;
    string instruction;
    string data;
    string format;
    char binary[2];
    int range = 0;
    int address;
    short processed_data;
    int index = 0;

    bool error = 0;
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
                        if(processed_data > 0b11111111111111)
                        {
                            throw runtime_error("Output data is over the 14 bit limit at line ");
                        }
                        binary[0] = char(processed_data&0b0000000011111111);
                        binary[1] = char((processed_data&0b1111111100000000)/0b100000000);
                        ofile.write (binary, 2);
                        index++;
                    }
                    else if(instruction == "jmp")
                    {
                        if(processed_data > 0b111111111)
                        {
                            throw runtime_error("Address data is over the 9 bit limit at line ");
                        }
                        processed_data = ((0b11*0b1000000000)+processed_data)*0b100000;
                        binary[0] = char(processed_data&0b0000000011111111);
                        binary[1] = char((processed_data&0b1111111100000000)/0b100000000);
                        ofile.write (binary, 2);
                        index++;
                    }
                    else if(instruction == "beq")
                    {
                        if(processed_data > 0b11111111)
                        {
                            throw runtime_error("Comparison data is over the 8 bit limit at line ");
                        }
                        processed_data = ((0b10*0b100000000)+processed_data)*0b1000000;
                        binary[0] = char(processed_data&0b0000000011111111);
                        binary[1] = char((processed_data&0b1111111100000000)/0b100000000);
                        ofile.write (binary, 2);
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
                    error = 1;
                }
            }
            address++;
        }
    }

    ofile.close();
    ifile.close();

    if(error)
    {
        cerr << "Assembly failed due to previous errors." << endl;
        exit(0);
    }
    else
    {
        cout << "Successfully wrote " << index - 1 << " lines of instructions into " << ofile_path << endl; 
    }
}
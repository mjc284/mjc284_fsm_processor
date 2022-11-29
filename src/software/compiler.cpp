#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

bool is_special_token(char c)
{
    if(c == ';'){return 1;}
    else if(c == '('){return 1;}
    else if(c == ')'){return 1;}
    else if(c == '{'){return 1;}
    else if(c == '}'){return 1;}
    else if(c == '='){return 1;}
    else if(c == ','){return 1;}
    else if(c == ':'){return 1;}
    else{return 0;}
}

bool is_special(char c)
{
    if(is_special_token(c)){return 1;}
    else if(c == ' '){return 1;}
    else if(c == '\n'){return 1;}
    else if(c == '\t'){return 1;}
    else if(c == '/'){return 1;}
    else{return 0;}
}

bool is_keyword(string s)
{
    if(s == ";"){return 1;}
    else if(s == "("){return 1;}
    else if(s == ")"){return 1;}
    else if(s == "{"){return 1;}
    else if(s == "}"){return 1;}
    else if(s == "="){return 1;}
    else if(s == ","){return 1;}
    else if(s == ":"){return 1;}
    else if(s == "goto"){return 1;}
    else if(s == "out"){return 1;}
    else if(s == "if"){return 1;}
    else if(s == "in"){return 1;}
    else{return 0;}
}

bool is_number(string str)
{
    return (str.find_first_not_of("0123456789") == string::npos);
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
        cerr << e.what() << '\n' << "Ex) compiler.out ifile.ssdf ofile.bin";
        exit(0);
    }
    
    ifstream ifile(ifile_path);
    ofstream ofile(ofile_path);

    int num_variables = 0;
    int num_states = 0;
    int num_token = 0;
    
    char c = ' ';
    char old_c = ' ';
    bool is_comment1 = 0;
    bool is_comment2 = 0;
    bool is_comment = 0;
    bool is_token = 0;
    vector <string> line;
    vector <vector<string>> lines;
    string tmp = "";
    int curly_brace = 0;
    while(ifile.get(c))
    {
        if(is_comment2 == 0)
        {
            if((is_comment1 == 0) and ((old_c == '/') and (c == '*'))){is_comment1 = 1;}
            if((is_comment1 == 1) and ((old_c == '*') and (c == '/'))){is_comment1 = 0;}
        }
        if(is_comment1 == 0)
        {
            if((is_comment2 == 0) and ((old_c == '/') and (c == '/'))){is_comment2 = 1;}
            if((is_comment2 == 1) and (c == '\n')){is_comment2 = 0;}
        }
        is_comment = is_comment1 | is_comment2;
        if(!is_comment)
        {
            if((is_token == 0) and !is_special(c))
            {
                is_token = 1;
            }
            if((is_token == 1) and is_special(c))
            {
                is_token = 0;
                num_token++;
                line.push_back(tmp);
                tmp = "";
            }
            if(is_special_token(c)){num_token++; line.push_back(string(1, c));}

            if(is_token == 1)
            {
                tmp += c;
            }
        }  
        if(c == '\n')
        {
            lines.push_back(line);
            line.clear();
        }
        old_c = c;
    }
/*
    for(int i = 0; i < lines.size(); i++)
        {
            for(int j = 0; j < lines[i].size(); j++)
            {
                cout << lines[i][j] << endl;
            }
        }
*/
    //int instruction_type = 0; //0 = No instruction, 1 = Define, 2 = State 
    curly_brace = 0;
    int parenthesis = 0;
    int index = 0;
    string token;

    //int instruction_type2 = 0; //0 = No instruction, 1 = out, 2 = if, 3 = goto, 4 = state
    int index2;

    map<string, int> variables;
    map<string,int>::iterator it;
    string tmp_variable;
    int tmp_value;

    map<string, vector<pair<string, short>>> instructions; //[(state_name), ([(to_replace, instruction), ...])]
    map<string, vector<pair<string, short>>>::iterator it2;
    string tmp_state;
    vector<pair<string,short>> tmp_instructions;
    vector<pair<string,short>> replacements;
    int scope_num = 0;

    vector<short> instruction_type;
    vector<short> indexes;
    vector<short> addresses;
    int replace_index = 0;
    vector<short> replace_value;
    vector<short> replace_value2;
    bool reading = 0;
    bool prev_if = 0;
    vector<pair<string, short>> tmp_instruction;

    int tmp_int;
    try
    {
        for(int i = 0; i < lines.size(); i++)
        {
            for(int j = 0; j < lines[i].size(); j++)
            {
                token = lines[i][j];

                if(token == "{"){curly_brace++;}
                else if(token == "}"){curly_brace--;}
                else if(token == "("){parenthesis++;}
                else if(token == ")"){parenthesis--;}

                if(reading == 0)
                {
                    if(token == "define")
                    {
                        if(instruction_type.size() != 0)
                        {
                            //Throw error
                        }
                        instruction_type.push_back(1);
                        indexes.push_back(4);
                        reading = 1;
                        continue;
                    }
                    else if(token == "state")
                    {
                        if(instruction_type.size() != 0)
                        {
                            //Throw error
                        }
                        instruction_type.push_back(2);
                        indexes.push_back(2);
                        reading = 1;
                        continue;
                    }
                    else if(token == "out")
                    {
                        if(instruction_type.size() == 0)
                        {
                            //Throw error
                        }
                        instruction_type.push_back(3);
                        indexes.push_back(6);
                        reading = 1;
                        continue;
                    }
                    else if(token == "if")
                    {
                        if(instruction_type.size() == 0)
                        {
                            //Throw error
                        }
                        instruction_type.push_back(4);
                        indexes.push_back(4);
                        reading = 1;
                        continue;
                    }
                    else if(token == "else")
                    {
                        if(instruction_type.size() == 0)
                        {
                            //Throw error
                        }
                        instruction_type.push_back(5);
                        indexes.push_back(1);
                        reading = 1;
                        continue;
                    }
                    else if(token == "elif")
                    {
                        if(instruction_type.size() == 0)
                        {
                            //Throw error
                        }
                        instruction_type.push_back(6);
                        indexes.push_back(4);
                        reading = 1;
                        continue;
                    }
                    else if(token == "goto")
                    {
                        if(instruction_type.size() == 0)
                        {
                            //Throw error
                        }
                        instruction_type.push_back(7);
                        indexes.push_back(2);
                        reading = 1;
                        continue;
                    }
                    else if(token == "while")
                    {
                        if(instruction_type.size() == 0)
                        {
                            //Throw error
                        }
                        instruction_type.push_back(8);
                        indexes.push_back(7);
                        reading = 1;
                        continue;
                    }
                    else if(token == "}")
                    {
                        instruction_type.push_back(9);
                        indexes.push_back(1);
                        reading = 1;
                    }
                    else
                    {
                        if(instructions.find(token) != instructions.end())
                        {
                            if(instruction_type.size() == 0)
                            {
                                //Throw error
                            }
                            instruction_type.push_back(10);
                            indexes.push_back(1);
                            continue;
                        }
                        else
                        {
                            //Throw error
                        }
                    }
                }
                if(reading == 1)
                {
                    if(instruction_type.size() == 0)
                    {
                        if((token != "define") or (token != "state"))
                        {
                            //throw error
                        }
                    }
                    else if(instruction_type.back() == 1) //define
                    {
                        if(indexes.back() == 4)
                        {
                            if(is_keyword(token))
                            {
                                //Throw error
                            }
                            if(is_number(token))
                            {
                                //throw error
                            }
                            tmp_variable = token;
                            if(variables.find(tmp_variable) != variables.end())
                            {
                                //Throw error
                            }
                        }
                        else if(indexes.back() == 3)
                        {
                            if(token != "=")
                            {
                                //throw error
                            }
                        }
                        else if(indexes.back() == 2)
                        {
                            if(!is_number(token))
                            {
                                if(token.substr(0, 2) == "0b")
                                {
                                    token = token.substr(2);
                                    tmp_value = stoi(token, nullptr, 2);
                                }
                                if(token.substr(0, 2) == "0x")
                                {
                                    token = token.substr(2);
                                    tmp_value = stoi(token, nullptr, 16);
                                }
                                if(token.substr(0, 2) == "0d")
                                {
                                    token = token.substr(2);
                                    tmp_value = stoi(token, nullptr, 10);
                                }
                                else
                                {
                                //throw error 
                                }
                            }
                            else
                            {
                                tmp_value = stoi(token);
                            }
                        }
                        else if(indexes.back() == 1)
                        {
                            if(token != ";")
                            {
                                //throw error
                            }
                            instruction_type.pop_back();
                            reading = 0;
                            it = variables.end();
                            variables.insert(it, pair<string, int>(tmp_variable, tmp_value));
                        }
                        else
                        {
                            //throw error
                        }
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else if(instruction_type.back() == 2) //state
                    {
                        if(indexes.back() == 2)
                        {
                            if(is_keyword(token))
                            {
                                //Throw error
                            }
                            tmp_state = token;
                            tmp_instructions.clear();
                            if(instructions.find(tmp_state) != instructions.end())
                            {
                                //Throw error
                            }
                        }
                        if(indexes.back() == 1)
                        {
                            if(token != "{")
                            {
                                //Throw error
                            }
                            reading = 0;
                        }
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else if(instruction_type.back() == 3) //out
                    {
                        if(indexes.back() == 6)
                        {
                            if(token != "(")
                            {
                                //throw error
                            }
                        }
                        else if(indexes.back() == 5)
                        {
                            if(is_number(token))
                            {
                                tmp_int = stoi(token);
                                if(tmp_int > 0b111111)
                                {
                                    //throw error
                                }
                                tmp_int = tmp_int*0b100000000;
                            }
                            else if(variables.find(token) != variables.end())
                            {
                                tmp_int = variables[token];
                                if(tmp_int > 0b111111)
                                {
                                    //throw error
                                }
                                tmp_int = tmp_int*0b100000000;
                            }
                            else
                            {
                                //throw error
                            }
                        }
                        else if(indexes.back() == 4)
                        {
                            if(token != ",")
                            {
                                //throw error
                            }
                        }
                        else if(indexes.back() == 3)
                        {
                            if(is_number(token))
                            {
                                if(stoi(token) > 0b11111111)
                                {
                                    //throw error
                                }
                                tmp_int += stoi(token);
                            }
                            else if(variables.find(token) != variables.end())
                            {
                                if(variables[token] > 0b11111111)
                                {
                                    //throw error
                                }
                                tmp_int += variables[token];
                            }
                            else
                            {
                                //throw error
                            }
                        }
                        else if(indexes.back() == 2)
                        {
                            if(token != ")")
                            {
                                //throw error
                            }
                        }
                        else if(indexes.back() == 1)
                        {
                            if(token != ";")
                            {
                                //throw error
                            }
                            instruction_type.pop_back();
                            reading = 0;
                            tmp_instructions.push_back(pair<string,short>("", tmp_int));
                            tmp_int = 0;
                        }
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else if(instruction_type.back() == 4) //if
                    {
                        if(indexes.back() == 4)
                        {
                            if(token != "(")
                            {
                                //throw error
                            }
                        }
                        if(indexes.back() == 3)
                        {
                            if(is_number(token))
                            {
                                tmp_int = stoi(token);
                                if(tmp_int > 0b11111111)
                                {
                                    //throw error
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else if(variables.find(token) != variables.end())
                            {
                                tmp_int = variables[token];
                                if(tmp_int > 0b11111111)
                                {
                                    //throw error
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else
                            {
                                //throw error
                            }
                        }
                        if(indexes.back() == 2)
                        {
                            if(token != ")")
                            {
                                //throw error
                            }
                        }
                        if(indexes.back() == 1)
                        {
                            if(token != "{")
                            {
                                //throw error
                            }
                            reading = 0;
                            prev_if = 1;
                            tmp_instructions.push_back(pair<string, short>("", tmp_int));
                            tmp_instructions.push_back(pair<string, short>(to_string(replace_index), 0));
                            replace_value.push_back(replace_index);
                            tmp_int = 0;
                            replace_index++;
                        }
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else if(instruction_type.back() == 5) //else
                    {
                        if(indexes.back() == 1)
                        {
                            if(prev_if == 0)
                            {
                                //throw error
                            }
                            if(token != "{")
                            {
                                //throw error
                            }
                            reading = 0;
                            tmp_instruction.push_back(tmp_instructions.back());
                            tmp_instructions.pop_back();
                            tmp_int = 0;
                            prev_if = 0;
                        }
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else if(instruction_type.back() == 6) //elif
                    {
                        if(indexes.back() == 4)
                        {
                            if(prev_if == 0)
                            {
                                //throw error
                            }
                            if(token != "(")
                            {
                                //throw error
                            }
                        }
                        if(indexes.back() == 3)
                        {
                            if(is_number(token))
                            {
                                tmp_int = stoi(token);
                                if(tmp_int > 0b11111111)
                                {
                                    //throw error
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else if(variables.find(token) != variables.end())
                            {
                                tmp_int = variables[token];
                                if(tmp_int > 0b11111111)
                                {
                                    //throw error
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else
                            {
                                //throw error
                            }
                        }
                        if(indexes.back() == 2)
                        {
                            if(token != ")")
                            {
                                //throw error
                            }
                        }
                        if(indexes.back() == 1)
                        {
                            if(token != "{")
                            {
                                //throw error
                            }
                            reading = 0;
                            tmp_instruction.push_back(tmp_instructions.back());
                            tmp_instructions.pop_back();
                            tmp_instructions.push_back(pair<string, short>("", tmp_int));
                            tmp_instructions.push_back(pair<string, short>(to_string(replace_index), 0));
                            replace_value.push_back(replace_index);
                            tmp_int = 0;
                            replace_index++;
                            tmp_int = 0;
                        }
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else if(instruction_type.back() == 9)
                    {
                        instruction_type.pop_back();
                        if(instruction_type.back() == 2)//state
                        {
                            instruction_type.pop_back();
                            it2 = instructions.end();
                            instructions.insert(it2, pair<string, vector<pair<string, short>>>(tmp_state ,tmp_instructions));
                        }
                        else if(instruction_type.back() == 4)//if
                        {
                            instruction_type.pop_back();
                            tmp_instructions.push_back(pair<string, short>(to_string(replace_index), 0));
                            tmp_instructions.push_back(pair<string, short>(" " + to_string(replace_value.back()), 0));
                            tmp_instructions.push_back(pair<string, short>(" " + to_string(replace_index), 0));
                            replace_index++;
                            replace_value.pop_back();
                            reading = 0;

                        }
                        else if(instruction_type.back() == 5)//else
                        {
                            instruction_type.pop_back();
                            tmp_instructions.push_back(tmp_instruction.back());
                            tmp_instruction.pop_back();
                        }
                        else if(instruction_type.back() == 6)//elif
                        {
                            instruction_type.pop_back();

                            tmp_instructions.push_back(pair<string, short>(tmp_instruction.back().first.substr(1), 0));
                            tmp_instructions.push_back(pair<string, short>(" " + to_string(replace_value.back()), 0));
                            tmp_instructions.push_back(tmp_instruction.back());
                            tmp_instruction.pop_back();
                        }
                        reading = 0;
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else
                    {
                        /* throw error */
                    }
                }
                
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    


    for(it2 = instructions.begin(); it2 != instructions.end(); ++it2)
    {
        for(int i = 0; i < it2->second.size(); i++)
        {
            cout << it2->first << " " << it2->second[i].first << " " << it2->second[i].second << "\n";
        }
    }


}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

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
    string old_tmp = "";
    int curly_brace = 0;
    vector <string> state_names;
    vector <string> label_names;
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

                if(old_tmp == "state")
                {
                    state_names.push_back(tmp);
                }
                old_tmp = tmp;
                tmp = "";
            }
            if(is_special_token(c))
            {
                num_token++; 
                line.push_back(string(1, c));
                if(c == ':')
                {
                    label_names.push_back(old_tmp);
                }
            }

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
    vector<string> labels;

    bool error = 0;

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
                            throw pair<string, int>("'define' can only be declared outside a state at line", i);
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
                            throw pair<string, int>("'state' can only be declared outside a state at line", i);
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
                            throw pair<string, int>("'out' can only be declared inside a state at line", i);
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
                            throw pair<string, int>("'if' can only be declared inside a state at line", i);
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
                            throw pair<string, int>("'else' can only be declared inside a state at line", i);
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
                            throw pair<string, int>("'elif' can only be declared inside a state at line", i);
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
                            throw pair<string, int>("'goto' can only be declared inside a state at line", i);
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
                            throw pair<string, int>("'while' can only be declared inside a state at line", i);
                        }
                        instruction_type.push_back(8);
                        indexes.push_back(4);
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
                        if(find(state_names.begin(), state_names.end(), token) != state_names.end())
                        {
                            if(instruction_type.size() == 0)
                            {
                                throw pair<string, int>("States can only be called inside a state at line", i);
                            }
                            if(find(state_names.begin(), state_names.end(), token) == state_names.end())
                            {
                                throw pair<string, int>("State does not exist at line", i);
                            }
                            instruction_type.push_back(10);
                            indexes.push_back(1);
                            reading = 1;
                            tmp_instructions.push_back(pair<string, short>(":" + token, 0));
                            continue;
                        }
                        else if((!is_keyword(token)) and (find(label_names.begin(), label_names.end(), token) == label_names.end()))
                        {
                            if(instruction_type.size() == 0)
                            {
                                throw pair<string, int>("Invalid command at line", i);
                            }
                            labels.push_back(token);
                            instruction_type.push_back(11);
                            indexes.push_back(1);
                            reading = 1;
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
                            throw pair<string, int>("Invalid command at line", i);
                        }
                    }
                    else if(instruction_type.back() == 1) //define
                    {
                        if(indexes.back() == 4)
                        {
                            if(is_keyword(token))
                            {
                                throw pair<string, int>("Variable name cannot be a system keyword at line", i);
                            }
                            if(is_number(token))
                            {
                                throw pair<string, int>("Variable name cannot be a number at line", i);
                            }
                            tmp_variable = token;
                            if(variables.find(tmp_variable) != variables.end())
                            {
                                throw pair<string, int>("Variable name already exists at line", i);
                            }
                        }
                        else if(indexes.back() == 3)
                        {
                            if(token != "=")
                            {
                                throw pair<string, int>("Syntax error at line", i);
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
                                else if(token.substr(0, 2) == "0x")
                                {
                                    token = token.substr(2);
                                    tmp_value = stoi(token, nullptr, 16);
                                }
                                else if(token.substr(0, 2) == "0d")
                                {
                                    token = token.substr(2);
                                    tmp_value = stoi(token, nullptr, 10);
                                }
                                else
                                {
                                    throw pair<string, int>("Invalid number format at line", i);
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
                                throw pair<string, int>("Syntax error at line", i);
                            }
                            instruction_type.pop_back();
                            reading = 0;
                            it = variables.end();
                            variables.insert(it, pair<string, int>(tmp_variable, tmp_value));
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
                                throw pair<string, int>("State name cannot be a system keyword at line", i);
                            }
                            tmp_state = token;
                            tmp_instructions.clear();
                            tmp_instructions.push_back(pair<string, short>(" " + token, 0));
                            if(instructions.find(tmp_state) != instructions.end())
                            {
                                throw pair<string, int>("State name already exists at line", i);
                            }
                        }
                        if(indexes.back() == 1)
                        {
                            if(token != "{")
                            {
                                throw pair<string, int>("Syntax error at line", i);
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
                                throw pair<string, int>("Syntax error at line", i);
                            }
                        }
                        else if(indexes.back() == 5)
                        {
                            if(is_number(token))
                            {
                                tmp_int = stoi(token);
                                if(tmp_int > 0b111111)
                                {
                                    throw pair<string, int>("Output address exceeds 6-bit limit at line", i);
                                }
                                tmp_int = tmp_int*0b100000000;
                            }
                            else if(variables.find(token) != variables.end())
                            {
                                tmp_int = variables[token];
                                if(tmp_int > 0b111111)
                                {
                                    throw pair<string, int>("Output address exceeds 6-bit limit at line", i);
                                }
                                tmp_int = tmp_int*0b100000000;
                            }
                            else
                            {
                                throw pair<string, int>("Variable does not exist at line", i);
                            }
                        }
                        else if(indexes.back() == 4)
                        {
                            if(token != ",")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                        }
                        else if(indexes.back() == 3)
                        {
                            if(is_number(token))
                            {
                                if(stoi(token) > 0b11111111)
                                {
                                    throw pair<string, int>("Output data exceeds 8-bit limit at line", i);
                                }
                                tmp_int += stoi(token);
                            }
                            else if(variables.find(token) != variables.end())
                            {
                                if(variables[token] > 0b11111111)
                                {
                                    throw pair<string, int>("Output data exceeds 8-bit limit at line", i);
                                }
                                tmp_int += variables[token];
                            }
                            else
                            {
                                throw pair<string, int>("Variable does not exist at line", i);
                            }
                        }
                        else if(indexes.back() == 2)
                        {
                            if(token != ")")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                        }
                        else if(indexes.back() == 1)
                        {
                            if(token != ";")
                            {
                                throw pair<string, int>("Syntax error at line", i);
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
                                throw pair<string, int>("Syntax error at line", i);
                            }
                        }
                        if(indexes.back() == 3)
                        {
                            if(is_number(token))
                            {
                                tmp_int = stoi(token);
                                if(tmp_int > 0b11111111)
                                {
                                    throw pair<string, int>("Comparison data exceeds 8-bit limit at line", i);
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else if(variables.find(token) != variables.end())
                            {
                                tmp_int = variables[token];
                                if(tmp_int > 0b11111111)
                                {
                                    throw pair<string, int>("Comparison data exceeds 8-bit limit at line", i);
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else
                            {
                                throw pair<string, int>("Variable does not exist at line", i);
                            }
                        }
                        if(indexes.back() == 2)
                        {
                            if(token != ")")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                        }
                        if(indexes.back() == 1)
                        {
                            if(token != "{")
                            {
                                throw pair<string, int>("Syntax error at line", i);
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
                                throw pair<string, int>("'else' must be called after 'if' at line", i);
                            }
                            if(token != "{")
                            {
                                throw pair<string, int>("Syntax error at line", i);
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
                                throw pair<string, int>("'elif' must be called after 'if' at line", i);
                            }
                            if(token != "(")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                        }
                        if(indexes.back() == 3)
                        {
                            if(is_number(token))
                            {
                                tmp_int = stoi(token);
                                if(tmp_int > 0b11111111)
                                {
                                    throw pair<string, int>("Comparison data exceeds 8-bit limit at line", i);
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else if(variables.find(token) != variables.end())
                            {
                                tmp_int = variables[token];
                                if(tmp_int > 0b11111111)
                                {
                                    throw pair<string, int>("Comparison data exceeds 8-bit limit at line", i);
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else
                            {
                                throw pair<string, int>("Variable does not exist at line", i);
                            }
                        }
                        if(indexes.back() == 2)
                        {
                            if(token != ")")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                        }
                        if(indexes.back() == 1)
                        {
                            if(token != "{")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                            reading = 0;
                            tmp_instruction.push_back(tmp_instructions.back());
                            tmp_instructions.pop_back();
                            tmp_instructions.push_back(pair<string, short>("", tmp_int));
                            tmp_instructions.push_back(pair<string, short>(to_string(replace_index), 0));
                            replace_value.push_back(replace_index);
                            tmp_int = 0;
                            replace_index++;
                        }
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }              
                    else if(instruction_type.back() == 7) //goto:
                    {
                        if(indexes.back() == 2)
                        {
                            if((find(label_names.begin(), label_names.end(), token) == label_names.end()) and (find(state_names.begin(), state_names.end(), token) == state_names.end()))
                            {
                                throw pair<string, int>("Target label or state does not exist at line", i);
                            }
                            tmp_instructions.push_back(pair<string, short>(token, 0));
                        }
                        if(indexes.back() == 1)
                        {
                            if(token != ";")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                            reading = 0;
                            instruction_type.pop_back();
                        }
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else if(instruction_type.back() == 8) //while
                    {
                        if(indexes.back() == 4)
                        {
                            if(token != "(")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                        }
                        if(indexes.back() == 3)
                        {
                            if(is_number(token))
                            {
                                tmp_int = stoi(token);
                                if(tmp_int > 0b11111111)
                                {
                                    throw pair<string, int>("Comparison data exceeds 8-bit limit at line", i);
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else if(variables.find(token) != variables.end())
                            {
                                tmp_int = variables[token];
                                if(tmp_int > 0b11111111)
                                {
                                    throw pair<string, int>("Comparison data exceeds 8-bit limit at line", i);
                                }
                                tmp_int = (0b10*100000000+tmp_int)*0b1000000;
                            }
                            else
                            {
                                throw pair<string, int>("Variable does not exist at line", i);
                            }
                        }
                        if(indexes.back() == 2)
                        {
                            if(token != ")")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                        }
                        if(indexes.back() == 1)
                        {
                            if(token != "{")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                            reading = 0;
                            tmp_instructions.push_back(pair<string, short>(" " + to_string(replace_index), 0));
                            replace_value.push_back(replace_index);
                            replace_index++;
                            tmp_instructions.push_back(pair<string, short>("", tmp_int));
                            tmp_instructions.push_back(pair<string, short>(to_string(replace_index), 0));
                            tmp_int = 0;
                            replace_index++;
                        }
                        
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else if(instruction_type.back() == 11) //label:
                    {
                        if(indexes.back() == 1)
                        {
                            if(token != ":")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }
                            reading = 0;
                            tmp_instructions.push_back(pair<string, short>(" " + labels.back(), 0));
                            instruction_type.pop_back();
                        }
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else if(instruction_type.back() == 10) //function:
                    {
                        if(indexes.back() == 1)
                        {
                            if(token != ";")
                            {
                                throw pair<string, int>("Syntax error at line", i);
                            }

                            reading = 0;
                            instruction_type.pop_back();
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
                        else if(instruction_type.back() == 8)//while
                        {
                            instruction_type.pop_back();

                            tmp_instructions.push_back(pair<string, short>(to_string(replace_value.back()), 0));
                            tmp_instructions.push_back(pair<string, short>(" " + to_string(replace_value.back()+1), 0));
                        }
                        reading = 0;
                        indexes.back()--;
                        if(indexes.back() == 0){indexes.pop_back();}
                    }
                    else
                    {
                       throw pair<string, int>("Fatal error at line", i);
                    }
                }
                
            }
        }
    }
    catch(pair<string, int>& e)
    {
        cerr << e.first << " " << e.second + 1 << ":" << '\n';
        for(int k = 0; k < lines[e.second].size(); k++)
        {
            cerr << lines[e.second][k] << " ";
        }
        cerr << "\n\n";
        error = 1;
    }

/*
    for(it2 = instructions.begin(); it2 != instructions.end(); it2++)
    {
        cout << ">>>";
        for(int i = 0; i < it2->second.size(); i++)
        {
            cout << it2->first << "/" << it2->second[i].first << "/" << it2->second[i].second << "\n";
        }
    }*/
    vector<pair<string, short>> processed_instructions;
    if(!error)
    {
        it2 = instructions.find("main");
        if(it2 == instructions.end())
        {
            cerr << "State 'main' does not exist.\n\n";
        }
        processed_instructions.insert(processed_instructions.end(), it2->second.begin(), it2->second.end());
    }

    map<string, vector<pair<string, short>>>::iterator it3;
    map<string, short> final_addresses;
    int cnt = 0;
    if(!error)
    {
        for(it3 = instructions.begin(); it3 != instructions.end(); it3++)
        {
            if(it3 != it2)
            {
                processed_instructions.insert(processed_instructions.end(), it3->second.begin(), it3->second.end());
            }
        }

        for(int i = 0; i < processed_instructions.size(); i++)
        {
            cout << processed_instructions[i].first << "/" << processed_instructions[i].second << "\n";
        }
        cout << "\n\n\n";

        for(int i = 0; i < processed_instructions.size(); i++)
        {
            if(processed_instructions[i].first != "")
            {
                if(processed_instructions[i].first[0] == ' ')
                {
                    final_addresses.insert(pair<string, short>(processed_instructions[i].first.substr(1), cnt));
                }
            }
            else{cnt++;}
        }

        for(int i = 0; i < processed_instructions.size(); i++)
        {
            if(processed_instructions[i].first != "")
            {
                if(processed_instructions[i].first[0] == ':')
                {

                }
                else if(processed_instructions[i].first[0] != ' ')
                {
                    cout << (0b10*0b1000000000+final_addresses.find(processed_instructions[i].first)->second)*0b100000 << endl;
                }
            }
            else
            {
                cout << processed_instructions[i].second << endl;
            }
        }
    }

    cout << "\n\n";
/*
    for(map<string, short>::iterator i = final_addresses.begin(); i != final_addresses.end(); i++)
        {
            cout << i->first << "/" << i->second << endl;
        }
*/

    if(error)
    {
        cerr << "Compilation failed due to previous errors.\n\n";
        exit(0);
    }

    


    


}
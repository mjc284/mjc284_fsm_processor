char c = ' ';
    char old_c = ' ';
    string s;
    bool comment = 0;
    bool define = 0;
    bool state = 0;
    bool search = 0;
    char search_type = 0; //0 = ;, 1 = {, 2 = }
    string line;
    string tmp;
    string error_line;


if(ifile)
    {
        while(getline(ifile, line))
        {
            s = "";
            for(int i = 0; i < line.length(); i++)
            {
                c = line.at(i);
                if((c == '*') and (old_c == '/'))
                {
                    if(comment == 0){comment = 1;}
                    error_line = line;
                }
                if((c == '/') and (old_c == '*'))
                {
                    if(comment == 1)
                    {
                        comment = 0;
                        old_c = ' ';
                    }
                    else
                    {
                        /*throw error*/
                    }
                }
                if(!comment)
                {
                    if((c == '/') and (old_c == '/'))
                    {
                        old_c = ' ';
                        continue;
                    }
                    if((c == ' ') or (c == '\n')){s = "";}
                    else{s += c;}
                    if(s == "define")
                    {
                        if(state != 0)
                        {
                            /*throw error*/
                        }
                        if(define == 0)
                        {
                            num_variables++;
                            define = 1;
                        }
                        else
                        {
                            /*throw error*/
                        }
                    }
                    else if(s == "state")
                    {
                        if(define != 0)
                        {
                            /*throw error*/
                        }
                        if(state == 0)
                        {
                            num_states++;
                            state = 1;
                        }
                        else
                        {
                            /*throw error*/
                        }
                    }
                    else
                    {
                        if(define == 1)
                        {

                        }
                    }

                }
            }
            cout << "\n";
        }
    }




                            /*
                            for(int a = 0; a < lines[i].size(); i++)
                            {
                                cout << lines[i][a];
                            }
                            */
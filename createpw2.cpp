#include<unistd.h>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<regex>
/* General program steps
 * collect ints for word list search
 * output search results for each int ( 5 digit number)
 * output full PW following the given rules
 * 1st word all caps
 * 2 digits of ints
 * 2nd word all lower
 * 2 chars of special chars
 * pattern repeats for work 3,4,5...
 */ 

class Word
{
private:
    int key;
    std::string value;
protected:
    int find_key_word()
        {
            return key;
        }
public:
    Word(int k){ key = k;}
    int get_key(){ return key;}
    std::string get_word(){ return value;}
    int set_value(std::string s){ value =s; return 0;} 
};

std::map<int,std::string> packdict(std::map<int,std::string> & map, std::string filename)
{
    // std::cout<< " Pack the map with int , word pairs from file: "<<filename<<std::endl;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open())
    {
        while (getline(file,line))
        {
            
            // std::cout<< line<< std::endl;
            // https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
            std::regex reg("[\\s]+");
            std::sregex_token_iterator it( line.begin(), line.end(), reg, -1 );
            std::sregex_token_iterator reg_end;
            if (!it->str().empty() )
            {
                std::size_t pos{};
                // std::cout <<  it->str() ;
                int num =0;
                try
                {
                    num = std::stoi(it->str(),&pos);
                    
                }
                catch(std::invalid_argument const &ex)
                {
                    std::cout << "invalid argument:" << ex.what() << std::endl;
                }

                if ( it != reg_end)
                {
                    ++it;
                    if (!it->str().empty() )
                    {
                        // std::cout <<"dict entry {"<<num<< "," <<   it->str() << "}" <<  std::endl ;
                        map.emplace(num,it->str());
                    }
                }
                // std::cout << "map["<< num<< "]="<< map[num]<<std::endl;
                //std::cout << std::endl;
            }
        }
    }
    return map;

}


int get_word(std::map<int, std::string> map , Word * word)
{
    int success=-1;
    if (map.size() > 0)
    {
        std::map<int,std::string>::iterator dict_it;
        int key =  word->get_key();
        // std::cout << "word key="<< word->get_key()<<std::endl;
        dict_it=map.find(key);
        // std::cout << map.find(key)->second << std::endl;
        // std::cout << "dict_it != map.end()" << (dict_it != map.end() ) << std::endl;
        if ( dict_it != map.end()   )
        {
            // std::cout << map.find(key)->second << std::endl;
            word->set_value(dict_it->second);
            success =0; 
        }
    }
    return success;
}

std::vector<Word> parse_input_to_wordlist(std::string input)
{
    std::ifstream file(input);
    std::string line;
    std::vector<Word> wl;
    if (file.is_open())
    {
        while (getline(file,line))
        {
            
            // std::cout<< line<< std::endl;
            // https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
            std::regex reg("[:]+");
            std::sregex_token_iterator it( line.begin(), line.end(), reg, -1 );
            std::sregex_token_iterator reg_end;
            if (!it->str().empty() )
            {
                std::size_t pos{};
                // std::cout <<  it->str() ;
                int num =0;
                try
                {
                    num = std::stoi(it->str(),&pos);
                    std::cout << "\nNumber of inputs : "<<num<< std::endl;
                }
                catch(std::invalid_argument const &ex)
                {
                    std::cout << "invalid argument:" << ex.what() << std::endl;
                }
                while( ++it != reg_end)
                {
                    if (!it->str().empty() )
                    {
                        try
                        {
                            num = std::stoi(it->str(),&pos);
                            wl.push_back(Word(num));
                        }
                        catch(std::invalid_argument const &ex)
                        {
                            std::cout << "invalid argument:" << ex.what() << std::endl;
                        }

                    }
                }
                // std::cout << "map["<< num<< "]="<< map[num]<<std::endl;
                std::cout << "wl size: "<< wl.size()<<  std::endl;
            }
        }
    }
    return wl;

}
int main(int argc, char**argv)
{
    /**
     * i : input - colon separate line of input <int(number of inputs)>:<int(i1)>:<int(i2)>:...
     * o : output - password output to a file 
     * d : dict_arg dictionary file
     * 
     * 
     *
     * */
    std::string dict_arg,input,output="";
    int c;
    opterr = 0;
    while ( (c = getopt ( argc, argv, "i:o:d:")) != -1 )
    {
        switch (c )
        {
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 'd':
                dict_arg = optarg;
                break;
            case '?':
                if (optopt == 'i' || optopt == 'o' || optopt == 'd')
                {
                    std::cerr<< "ERROR: -i , -o , -d all require a argument" << std::endl;
                }
                else
                {
                    std::cerr << "ERROR: UNKNOWN OPTION" << std::endl;
                }
            default:
                exit (1);
        }
    }
    int wc;
    std::map<int,std::string> dictmap;

    std::vector<Word> wordlist;
    std::string dictfile;
    if ( dict_arg == "")
    {
        dictfile="./wordfiles/eff_large_wordlist.txt";
    }
    else
    {
        dictfile= dict_arg;
    }
    packdict(dictmap,dictfile);
    if ( input == "")
    {
        std::cout << "Enter the number of words in your password" <<std::endl ;
        std::cin >> wc;
        std::cout << " you entered " << wc << " is that right";
        int entry;
        for (int i =0; i < wc ; i++)
        {
            std::cout << "enter a 5 digit integer" << std::endl;
            std::cin >> entry;
            wordlist.push_back(Word(entry));
            // std::cout << "entry= " << entry << std::endl;
        }
        std::cout << "size of wordlist : "<< wordlist.size()<< std::endl;
    }
    else
    {
        /*
         * parse input and fill up the wordlist
         **/
        wordlist= parse_input_to_wordlist(input);
    }
    std::string pw_str="";
    for (int i = 0; i < wordlist.size();i++)
    {
        get_word(dictmap, &wordlist[i]);
        std::string w = wordlist[i].get_word();
        // TO UPPER 1/2 the words
        if ( i % 2 ==0 )
        {
            std::for_each(w.begin(), w.end(), [](char& c)
                                                  { // modify in-place
                                                      c = std::toupper(static_cast<unsigned char>(c));
                                                  });
        }
        //std::cout << wordlist[i].get_word();
        pw_str+=w;
    }
    std::cout<<pw_str<< std::endl;
}

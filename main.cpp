/**
  * POPS-VCD-ID-Extractor
  * By El Isra (thanks Alex parrado for the Regex shit, and Danielb, for providing the offset and length where ELF could be found
  **/

#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

enum RET_TABLE
{
  SUCESS =0,
  NO_ARGV1,
  CANT_OPEN,
  REGEX_FAIL
};

int main(int argc, char**argv)
{
    if (argc < 2) return NO_ARGV1;
    char buffer[4096+1];
    const regex ELF_ID_NAME("[A-Z][A-Z][A-Z][A-Z]_[0-9][0-9][0-9].[0-9][0-9]");
    ifstream VCD_STREAM(argv[1]);

    if(VCD_STREAM.is_open())
    {
        VCD_STREAM.seekg(1100032,ios_base::beg);
        VCD_STREAM.read(buffer,4096);
        for(int i=0; i<4096; i++)
            buffer[i]=buffer[i]<32?',':buffer[i];

        buffer[4096]=0;

        smatch match;
        string str(buffer);

        if (regex_search(str,match,ELF_ID_NAME))
        {

            std::cout << match[0] <<endl;
        } else {return REGEX_FAIL;}
    } else {return CANT_OPEN;}

    return 0;

}

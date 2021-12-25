/**
  * POPS-VCD-ID-Extractor
  * By El Isra (thanks Alex parrado for the Regex shit, and Danielb, for providing the offset and length where ELF could be found
  **/

#include <iostream>
#include <fstream>
#include <regex>

#define ELF_SEARCH_RANGE  4096 //needs testing
#define ELF_SEARCH_OFFSET 1100032
using namespace std;

enum RET_TABLE
{
  SUCESS =0,
  NO_ARGV1,
  CANT_OPEN,
  REGEX_FAIL,
  VCD_IS_SMALL,
};

long Get_FileSize(const char* filename)
{
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int main(int argc, char**argv)
{
    int ret;
    long VCD_SIZE; 
    if (argc < 2) {ret = NO_ARGV1; goto ERR;}
    char buffer[ELF_SEARCH_RANGE+1];
    const regex ELF_ID_NAME("[A-Z][A-Z][A-Z][A-Z]_[0-9][0-9][0-9].[0-9][0-9]");
    
   VCD_SIZE = Get_FileSize(argv[1]);
   if (VCD_SIZE < (ELF_SEARCH_OFFSET + ELF_SEARCH_RANGE))
			  {ret = VCD_IS_SMALL; goto ERR;}
    ifstream VCD_STREAM(argv[1]);
     
    if(VCD_STREAM.is_open())
    {
        VCD_STREAM.seekg(ELF_SEARCH_OFFSET,ios_base::beg);
        VCD_STREAM.read(buffer,ELF_SEARCH_RANGE);
        for(int i=0; i<ELF_SEARCH_RANGE; i++)
            buffer[i]=buffer[i]<32?',':buffer[i];

        buffer[ELF_SEARCH_RANGE]=0;

        smatch match;
        string str(buffer);

        if (regex_search(str,match,ELF_ID_NAME))
        {
            std::cout << match[0] <<endl;
        } else 
			{ret = REGEX_FAIL; goto ERR;}
    } else 
		{ret = CANT_OPEN; goto ERR;}

    return 0;
    ERR:
    switch (ret)
    {
		case NO_ARGV1:
		std::cerr << "No file provided\n";
		break;
		case CANT_OPEN:
		std::cerr << "Error while opening ["<<argv[1]<<"]\n";
		break;
		case REGEX_FAIL:
		std::cerr << "regex couldn't find a ELF ID matching pattern\n";
		break;
		case VCD_IS_SMALL:
		std::cerr <<"File is too small to process\n\t VCD's should be at least "<< (ELF_SEARCH_OFFSET + ELF_SEARCH_RANGE)<<" kbytes in length\n";
		break;
		default:
		std::cerr <<"Unhandled error code...\n";
    }
	return ret;
}

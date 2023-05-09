#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "args.hxx"

int main(int argc, char *argv[])
{
	args::ArgumentParser parser("Forcheese - a small Forth-like language.");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::Positional<string> fileName(parser, "filename", "Name of the file to run.");
    args::CompletionFlag completion(parser, {"complete"});
	try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion& e)
    {
        std::cout << e.what();
        return 0;
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
	catch (const args::ValidationError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
	if(!fileName)
	{
		cerr<<"Error: no input file."<<endl;
		return 1;
	}
	ifstream ifs;
	ifs.open(*fileName);
	std::cout<<"Running "<<*fileName<<"..."<<endl;
	return 0;
}
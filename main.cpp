#include "Scanner.h"
#include "Parser.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
	{
		std::cout << "No source file provided" << std::endl;
		return 1;
	}
	
    const std::string filePath = argv[1];
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    if (!file) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return 1;
    }

    std::streamsize fileSize = file.tellg();

    std::vector<char> buffer(fileSize);

    file.seekg(0, std::ios::beg);

    if (file.read(buffer.data(), fileSize)) {
        std::cout << "File loaded successfully." << std::endl;
    }
    else {
        std::cerr << "Error reading the file." << std::endl;
    }

    file.close();

    char* pos = &buffer[0];
    std::vector<Token> tokens;
    std::vector<std::string> values;

    Scanner s1;

    std::cout << "INFO SCAN - Start scanning...\n";

    while (pos <= &buffer[buffer.size()-1]) {
        s1.gettoken(pos,tokens,values, &buffer[buffer.size()-1]);
    }
    tokens.push_back(END_OF_FILE);
    
    std::cout << "INFO SCAN - Completed with "<<s1.errorCounter<<" errors\n";
}
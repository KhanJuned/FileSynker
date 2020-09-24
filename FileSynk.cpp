#include <iostream>
#include <string>
#include <conio.h>

char* ReadFile(const char* filename) {
    static char buffer[256];
    FILE* fptr = fopen(filename, "r");
    
    if (fptr)
        std::cout << filename << " opened succesfully!\n";
    else
         std::cout << "Error Reading " << filename << "!\n";

    int fptrChars = 0;
    while (fgetc(fptr) != EOF)
        ++fptrChars;

    fseek(fptr, 0, SEEK_SET);
    for (int i = 0; i != fptrChars; ++i) 
        buffer[i] = fgetc(fptr);
    if(!fclose(fptr))
        std::cout << filename << " closed\n";
    else
        std::cout << "Error closing " << filename << "\n";
    return buffer;
}
void WriteFile(const char* filename, const char* data) {
    FILE* fptr = fopen(filename, "r+");
    if (fptr)
        std::cout << filename << " opened Succesfully.\n";
    else {
        std::cout << "\nFile not found! Do you want to create one?\n";
        std::cout << "Type Y, y or 1 to create the file that doesn't exist press any other key to exit\n";
        std::string input;
        std::cin >> input;
        if (input.at(0) == 'Y' || input.at(0) == 'y' || input.at(0) == '1') {
            fptr = fopen(filename, "w");
            if (fptr) {
				std::cout << filename << " created Succesfully\n";
				break;
			} else {
                std::cout << "Error creating " << filename << std::endl;
				break;
			}
        }
		if (input.at(0) != 'Y' && input.at(0) != 'y' && input.at(0) != '1') {	
			exit(0);
		}
    }
    while ((*data)) {
        fputc(*data, fptr);
        ++data;
    }
    if (!fclose(fptr))
        std::cout << filename << " closed\n";
    else 
        std::cout << "Error closing " << filename << std::endl;

}
int main(int argv, const char* argc[]) {
    const char* FileBuffer = ReadFile(argc[1]);
   
    for (int i = 2; i < 10; ++i) {
        if (argc[i] != (char)0 )
            WriteFile(argc[i], FileBuffer);
    }
    return 0;
}

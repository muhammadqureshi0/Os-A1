#pragma warning(disable: 4996)//By 19L-1010, CS-4E2, Muhammad Qureshi
#include <iostream>
#include <string.h>
int get_substr_iterations(char* file_name, std::string str);

int main(int argc,char* argv[]) {//Should take input from command line
	std::cout << "\nThere are " <<get_substr_iterations(argv[1], argv[2])<<" iterations of " <<argv[2]<< " in the file named " <<argv[1]<<std::endl;
	return 0;
}

int get_substr_iterations(char* file_name, std::string str) {
	FILE* file;
	int counter = 0;
	file = fopen(file_name, "r");
	if (file) {
		int character = fgetc(file);
		for (int index = 0; character != EOF; character = fgetc(file)) {
			if (str[index] == '\0') {
				index = 0;
				counter++;
			}
			if (str[index] == character) {
				index++;
			}
			else {
				index = 0;
			}
		}
		fclose(file);
	}
	return counter;
}



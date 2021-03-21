#pragma warning(disable: 4996)//By 19L-1010, CS-4E2, Muhammad Qureshi
#include <iostream>
#include <string.h>
int get_substr_iterations(char* file_name, std::string str);

int main() {
	char file_name[30];
	std::string sub_str;
	std::cout << "Enter File Name in a <name>.txt format without quotation marks: ";
	std::cin.getline(file_name, 30);
	std::cout << "Enter substring to search file for: ";
	std::cin >> sub_str;
	std::cout << "\nThere are " <<get_substr_iterations(file_name, sub_str)<<" iterations of " <<sub_str<< " in the file named " <<file_name<<std::endl;
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



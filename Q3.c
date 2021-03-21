#pragma warning(disable: 4996)//By 19L-1010, CS-4E2, Muhammad Qureshi
#include <iostream>
#include <cctype>
bool IsAVowel(char a_character);
void file_reversal(char* file_name);
int main() {
	char file_name[30];
	std::string sub_str;
	std::cout << "Enter File Name in a <name>.txt format without quotation marks: ";
	std::cin.getline(file_name, 30);
	std::cout << "Enter substring to search file for: ";
	std::cin >> sub_str;
	file_reversal(file_name);
	return 0;
}

void file_reversal( char* file_name) {
	FILE* file_handler;
	file_handler = fopen(file_name, "r+");
	if (file_handler) {
		bool word_new = true;
		bool first_round = true;
		for (int character = fgetc(file_handler), start = -1, end = -1, index = 0; ; character = fgetc(file_handler), index++) {
			fseek(file_handler, index, SEEK_SET);//Starting from 0th index
			if ((character == int(' ')|| character==EOF)&&word_new==false) {//Traverses until end of word found, in the form of either EOF or space

				end = index - 1; //Word's ending offset stored
				int size = end - start;
				int** list_of_non_vowels = new int*[size];
				for (int i = 0; i < size; i++) {
					list_of_non_vowels[i] = new int[2]; //A list of non vowels is kept, 1st is for the character,  2nd column is for it's place
				}
				int num_of_non_vowels = -1;
				int character_read = 0;
				fseek(file_handler, start, SEEK_SET);

				for (int counter= start,character_read = fgetc(file_handler); character_read != int(' ') && character_read != EOF; character_read = fgetc(file_handler),counter++) {
					if (!IsAVowel(char(character_read))) {
						list_of_non_vowels[++num_of_non_vowels][0] = character_read;
						list_of_non_vowels[num_of_non_vowels][1] = counter;
					}//This for loop stores all non vowel characters in the array in addition to their indexes
				}
				if (num_of_non_vowels >= 1) {//Check for whether more than 2 non vowels exist, otherwise there's no point to reverse.
					for (int counter = 0; start != end + 1; start++) {
						fseek(file_handler, start, SEEK_SET);
						if (num_of_non_vowels != -1) {
							if (start == list_of_non_vowels[counter][1]) {
								fputc(list_of_non_vowels[num_of_non_vowels--][0], file_handler);
								counter++; //Reverses position of non vowels by copying from the end to the beginning i.e size till 0
							} 
						}
					}
				}
				word_new = true;
				for (int i = 0; i < size; i++) {
					delete[]list_of_non_vowels[i];
				}
				delete[]list_of_non_vowels;
			}
			else {
				if (word_new) {
					start = index;
					word_new = false;
					if (!first_round) {//Reads one index more after the first word for some reason, balances out.
						start--;
					}
					first_round = false;
				}
			}
			if (character == EOF) {
				break;
			}
		}
		fclose(file_handler);
	}
}

bool IsAVowel(char a_character) {//IsAVowel, determines if character is a vowel
	a_character = tolower(a_character);
	return ((a_character == 'a' || a_character == 'e' || a_character == 'i' || a_character == 'o' || a_character == 'u')&&a_character>='a' &&a_character<='z');
}
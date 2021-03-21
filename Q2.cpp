#pragma warning(disable: 4996)//By 19L-1010, CS-4E2, Muhammad Qureshi
#include <iostream>
#include <string>
#include <queue>
int get_offset_of_row_n(int row_n, FILE* file_stream);
void add_record(FILE* file_stream, int record_num, char* name, char* roll_num, char* email);
void delete_record(FILE* file_stream, int record_num);
void view_record(FILE* file_stream, int record_num);

int main() {
	FILE* file_handler;
	int choice = -1;
	int record_num = -1;
	file_handler = fopen("student_file.txt", "r+");
	if (!file_handler) {//If the file doesn't exist already then it's created, closed and opened to update mode
		file_handler = fopen("student_file.txt", "w");
		fclose(file_handler);
		file_handler = fopen("student_file.txt", "r+");
	}
	//Menu, insignificant to elaborate.
	std::cout << "Welcome to the menu of 19L-1010, 4E2, Muhammad Qureshi\n\n";
	std::cout << "~~~Options~~~\n1. Add Student details   2. Read Student record\n3. Delete Student record   4. Exit\n";
	std::cout << "Selection: ";
	std::cin >> choice;
	while (choice != 4) {
		switch (choice) {
		case 1:
			char name[50];
			char rollNum[30];
			char email[50];
			std::cout << "Enter Student's Name: ";
			std::cin.get(name, 50);
			std::cout << "Enter Student's roll number: ";
			std::cin.get(rollNum, 30);
			std::cout << "Enter Student's Email: ";
			std::cin.get(email, 50);
			std::cout << "Enter record number (1,2,3) to add to: ";
			std::cin >> record_num;
			while (record_num >= 1 && record_num <= 3) {
				std::cout << "Enter record number (1,2,3) to add to: ";
				std::cin >> record_num;
			}
			add_record(file_handler, record_num, name, rollNum, email);
			break;
		case 2:
			std::cout << "Enter record number (1,2,3) to view: ";
			std::cin >> record_num;
			while (record_num >= 1 && record_num <= 3) {
				std::cout << "Enter record number (1,2,3) to view: ";
				std::cin >> record_num;
			}
			view_record(file_handler, record_num);
			break;
		case 3:
			std::cout << "Enter record number (1,2,3) to delete: ";
			std::cin >> record_num;
			while (record_num >= 1 && record_num <= 3) {
				std::cout << "Enter record number (1,2,3) to delete: ";
				std::cin >> record_num;
			}
			delete_record(file_handler, record_num);
			break;
		}

		std::cout << "~~~Options~~~\n1. Add Student details   2. Read Student record\n3. Delete Student record   4. Exit\n";
		std::cout << "Selection: ";
		std::cin >> choice;
	}
	fclose(file_handler);
	return 0;
}

int get_offset_of_row_n(int row_n, FILE*file_stream) {//Recieves row number, starting from 1. Returns offset of said row's start.
	int current_row = 1;
	int character_read = 0;
	int pos = 0;
	if (row_n == 1) //No need to compute, it's 0th position.
		return pos;
	fseek(file_stream, 0, SEEK_END);
	if (ftell(file_stream)==0) {//If the end of the file is at 0th position then place new lines to reach desired row
		for (int i = 1; i < row_n; i++) {
			fputc('\n', file_stream);
			pos += 2;//New line is technically 2 characters

		}
	}
	else {//2nd case, non empty file
		fseek(file_stream,0, SEEK_SET);//Resetting to start
		while (current_row != row_n && character_read!= EOF) {//Counting number of rows through new lines until EOF or desired row reached
			character_read = fgetc(file_stream);
			if (character_read == '\n') {
				current_row++;
				pos++;
			}
			pos++;
		}
		if (character_read == EOF) {//If EOF reached before desired row then place enough new lines to reach it. 
			fseek(file_stream, 0, SEEK_END);
			for (int i = current_row; i < row_n; i++) {
				fputc('\n', file_stream);  
				pos += 2;//Again, new line character is 2 bytes long.
			}
		}
		else {

		}
	}
	return pos; //Returning calculated offset of desired row
}
void add_record(FILE* file_stream,int record_num, char* name, char* roll_num, char* email) {//Takes Student details and record number to determine where to add student details
	int old_position = ftell(file_stream);
	int record_position = get_offset_of_row_n((record_num-1)*3+1, file_stream);//Getting offset through record number
	int character_read = 0;
	std::queue<int> stored_values;//Writing to file overwrites content, thus queue saves it in prior circumstance for restoration.
	
	fseek(file_stream, record_position, SEEK_SET);
	character_read = fgetc(file_stream);
	if (character_read == '\n' || character_read == EOF) {//Determining if the record is empty or not
		fseek(file_stream, record_position, SEEK_SET);
		while ((character_read = fgetc(file_stream)) == '\n');//Skipping blank lines to find content to save
		fseek(file_stream, -1, SEEK_CUR);
		while ((character_read = fgetc(file_stream)) != EOF) {//All characters after the record's place of storage are saved in a queue
			stored_values.push(character_read);
		}
		fseek(file_stream, record_position, SEEK_SET);
		for (int i = 0; i < 3; i++) {//Writing name, roll number and email to file respectively
			int j = 0;
			bool flag = false;
			while (true) {
				if (i == 0) {
					if (name[j] != '\0') {
						fputc(name[j], file_stream);
					}
					else {
						break;
					}
				}
				else {
					if (i == 1) {
						if (roll_num[j] != '\0') {
							fputc(roll_num[j], file_stream);
						}
						else {
							break;
						}
					}
					else {
						if (email[j] != '\0') {
							fputc(email[j], file_stream);
						}
						else {
							break;
						}
					}
				}
				j++;
			}
			fputc('\n', file_stream);//Manually adding a break between 3 data items
		}
		while (!stored_values.empty()) {//Restoring older data immediately after the currently added record
			character_read = stored_values.front();
			fputc(character_read, file_stream);
			stored_values.pop();
		}
	}
	else {
		std::cout << "Record already exists at " << record_num << " place, delete it first\n";
	}
	fseek(file_stream, old_position, SEEK_SET);
}
void delete_record(FILE* file_stream, int record_num) {//Takes record number and file handler variable, deletes record if it exists at said number
	int old_position = ftell(file_stream);
	int record_position = get_offset_of_row_n((record_num - 1) * 3 + 1, file_stream);//Offset of record to be deleted requisitioned
	int spaces_to_add= get_offset_of_row_n(record_num* 3 + 1, file_stream) - record_position - 6;//Number of characters to delete/overwrite determined, 6 deducted for 2(bytes)*3 new lines
	int character_read = 0;
	int characters_deleted = 0;

	fseek(file_stream, record_position, SEEK_SET);
	character_read = fgetc(file_stream);
	if (character_read != '\n' && character_read != EOF) {//Check for whether record exists at designated number
		fseek(file_stream, record_position, SEEK_SET);
		while (characters_deleted != spaces_to_add) {//Overwrites characters with space until calculated number reached
			putc(' ', file_stream);
			characters_deleted++;
		}//6 characters were initially excluded for the sake of being overwritten as new lines, otherwise could be junk
		putc('\n', file_stream);
		putc('\n', file_stream);
		putc('\n', file_stream);
	}
	else {
		std::cout << "No record exists at " << record_num << " number.\n";
	}
	fseek(file_stream, old_position, SEEK_SET);
}
void view_record(FILE* file_stream, int record_num) {//Takes record number and file handler variable, views record if it exists at said number
	int old_position = ftell(file_stream);
	int record_position = get_offset_of_row_n((record_num - 1) * 3 + 1, file_stream);
	int character_read = 0;
	int number_of_lines_read = 0;
	fseek(file_stream, record_position, SEEK_SET);
	character_read = fgetc(file_stream);//Simply reads characters until 3 new lines or 2 plus 1 EOF encountered, implying 1 student's details have been read.
	if (character_read != '\n' && character_read != EOF) {
		fseek(file_stream, record_position, SEEK_SET);
		while (number_of_lines_read != 3) {
			character_read = fgetc(file_stream);
			std::cout << char(character_read);
			if (character_read == '\n' || character_read==EOF) {
				number_of_lines_read++;
			}
		}
	}
	else {
		std::cout << "Record doesn't exist\n";
	}
	fseek(file_stream, old_position, SEEK_SET);
}
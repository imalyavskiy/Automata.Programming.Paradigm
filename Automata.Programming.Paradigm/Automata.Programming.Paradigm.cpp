////////////////////////////////////////////////////////////////////////////
// Основная функция: разбиение входящего потока символов на отдельные слова
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

// функция чтения аргументов командной строки
bool read_cmd_line(int argc, char** argv, std::string& source_file)
{
	auto print_usage = []() { std::cout << "USAGE: The only parameter is the text source file path." << std::endl; };

	if (argc < 2)
		std::cout << "[ERROR] Not enough parameters!" << std::endl, print_usage(), exit(1);
	if (argc > 2)
		std::cout << "[ERROR] Too many parameters!" << std::endl, print_usage(), exit(1);

	std::ifstream src(argv[1]);
	if (!src.is_open())
		std::cout << "[ERROR] Check does the file exist." << std::endl, print_usage(), exit(1);
	else
		src.close();

	source_file = argv[1];

	return true;
}

enum class ReadStates { before, inside, after };
ReadStates state = ReadStates::before;

// Функция обработки
// в стиле конечного автомата
void process_data(std::istream& input)
{
	char c = 0;
	while (input >> std::noskipws >> c)
	{
		switch(state)
		{
		case ReadStates::before:
			if (' ' != c && '\n' != c)
				std::cout << c, state = ReadStates::inside;
			else //if (' ' == c || '\n' == c)
				nullptr, state = ReadStates::before;
			break;
		case ReadStates::inside:
			if (' ' != c && '\n' != c)
				std::cout << c, state = ReadStates::inside;
			else //if (' ' == c || '\n' == c)
				std::cout << std::endl, state = ReadStates::after;
			break;
		case ReadStates::after:
			if (' ' != c && '\n' != c)
				std::cout << c, state = ReadStates::inside;
			else //if (' ' == c || '\n' == c)
				std::cout << std::endl, state = ReadStates::after;
			break;
		}
	}

	std::cout << std::endl;
}

void main(int argc, char** argv) 
{
	std::string source_file_name;
 	if (!read_cmd_line(argc, argv, source_file_name))
 		return;

	std::ifstream source_file(source_file_name, std::ios::in);
	
	process_data(source_file);
	
	source_file.close();
}


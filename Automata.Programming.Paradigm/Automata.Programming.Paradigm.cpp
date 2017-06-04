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

enum class ReadStates { Between = 0, Inside = 1};
enum class Symbols{ WSpace = 0, CReturn = 1, Input = 2, Nothing = 3 };

// Функция обработки
// в стиле конечного автомата
void process_stream(std::istream& input)
{
	ReadStates state = ReadStates::Between;

	// Шаг автомата
	auto step = [&state](const char& c)
	{
		switch (state)
		{
		case ReadStates::Between:
			if (' ' != c && '\n' != c)
				std::cout << c,			state = ReadStates::Inside;
			else //if (' ' == c || '\n' == c)
				nullptr,				state = ReadStates::Between;
			break;
		case ReadStates::Inside:
			if (' ' != c && '\n' != c)
				std::cout << c,			state = ReadStates::Inside;
			else //if (' ' == c || '\n' == c)
				std::cout << std::endl, state = ReadStates::Between;
			break;
		}
	};

	char c = 0;
	while (input >> std::noskipws >> c)
		step(c);

	std::cout << std::endl;
}

void main(int argc, char** argv) 
{
	std::string source_file_name;
 	if (!read_cmd_line(argc, argv, source_file_name))
 		return;

	std::ifstream source_file(source_file_name, std::ios::in);
	
	process_stream(source_file);
	
	source_file.close();
}


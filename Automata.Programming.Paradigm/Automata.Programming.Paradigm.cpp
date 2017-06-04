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

template <typename _READSTATES, typename _SYMBOLS>
using TTransition = std::pair<_READSTATES, _SYMBOLS>;
typedef TTransition<ReadStates, Symbols> Transition;

template <typename _READSTATES, typename _SYMBOLS>
using TTransitionTable = std::map<_READSTATES, std::map<_SYMBOLS, TTransition<_READSTATES, _SYMBOLS>>>;
typedef TTransitionTable<ReadStates, Symbols> TransitionTable;

void process_stream(std::istream& input)
{
	ReadStates state = ReadStates::Between;
	char c = 0;

	const TransitionTable transition_table =
	{
		{ ReadStates::Between,
			{	// got				// transit to			// print symbol
				{ Symbols::WSpace,	{ ReadStates::Between,	Symbols::Nothing } },
				{ Symbols::CReturn,	{ ReadStates::Between,	Symbols::Nothing } },
				{ Symbols::Input,	{ ReadStates::Inside,	Symbols::Input } },
			}
		},
		{ ReadStates::Inside,
			{	// got				// transit to			// print symbol
				{ Symbols::WSpace,	{ ReadStates::Between,  Symbols::CReturn } },
				{ Symbols::CReturn,	{ ReadStates::Between,  Symbols::CReturn } },
				{ Symbols::Input,	{ ReadStates::Inside,	Symbols::Input } }
			}
		},
	};

	// Шаг автомата
	auto step = [&transition_table, &state](char& c)
	{
		Symbols symbol = ' ' == c ? Symbols::WSpace : ('\n' == c ? Symbols::CReturn : Symbols::Input);

		const Transition& transition = transition_table.at(state).at(symbol);
		
		state = transition.first;

		switch (transition.second)
		{
		case Symbols::CReturn:	std::cout << std::endl; break;
		case Symbols::Input:	std::cout << c; break;
		case Symbols::WSpace:	break;
		case Symbols::Nothing:	break;
		default:				break;
		}
	};

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


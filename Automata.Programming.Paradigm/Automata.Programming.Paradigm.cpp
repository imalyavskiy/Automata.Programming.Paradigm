////////////////////////////////////////////////////////////////////////////
// Author: Ilya Malyavskiy
// e-mail: ilya.malyavskiy@gmail.com
// The goal: To split the input stream into separate words.
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
// command prompt arguments reader
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
enum class Symbols{ AlNum = 0, Other = 1 };

template <typename _READSTATES>
using TTransition = std::pair<_READSTATES, std::function<void(const char&)>>;
typedef TTransition<ReadStates> Transition;

template <typename _READSTATES, typename _SYMBOLS>
using TTransitionTable = std::map<_READSTATES, std::map<_SYMBOLS, TTransition<_READSTATES>>>;
typedef TTransitionTable<ReadStates, Symbols> TransitionTable;

// Main procesing function designed with automata programming paradigm
void process_stream(std::istream& input)
{
	ReadStates state = ReadStates::Between;
	char c = 0;

	auto print_creturn = [](const char& ch = 0)->void { std::cout << std::endl; };
	auto print_nothing = [](const char& ch = 0)->void {};
	auto print_symbol  = [](const char& ch = 0)->void { std::cout << ch;};

	// the transition table
	const TransitionTable transition_table =
	{
		{ ReadStates::Between,
			{	// got				// transit to			// print symbol
				{ Symbols::Other,	{ ReadStates::Between,	print_nothing } },
				{ Symbols::AlNum,	{ ReadStates::Inside,	print_symbol  } },
			}
		},
		{ ReadStates::Inside,
			{	// got				// transit to			// print symbol
				{ Symbols::Other,	{ ReadStates::Between,  print_creturn } },
				{ Symbols::AlNum,	{ ReadStates::Inside,	print_symbol  } },
			}
		},
	};

	// The step of the automata
	auto step = [&transition_table, &state](char& c)
	{
		Symbols symbol = 0 < std::isalnum(c) ? Symbols::AlNum : Symbols::Other;

		const Transition& transition = transition_table.at(state).at(symbol);

		state = transition.first;

		transition.second(c);
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


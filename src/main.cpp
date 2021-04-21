#include <DeepFake.hpp>

#include <dlib/cmd_line_parser.h>

int main(int argc, char** argv) {
	try {

		/* Define the command line options */
		dlib::command_line_parser parser;

		parser.add_option("h", "Display this help message.");
		parser.add_option("in", "The input image", 1);

		parser.parse(argc, argv);

		if (parser.option("h")) {
			std::cout << "Usage: deep_fake --in input_file\n";
			parser.print_options();
			return 0;
		}

		/* Get the input image */
		std::string in_file;

		if (parser.option("in")) {
			in_file = parser.option("in").argument();
		} else {
			std::cout << "Error in command line:\n   You must specify an input file.\n";
			std::cout << "\nTry the -h option for more information." << std::endl;
			return 0;
		}

		DeepFake* app = DeepFake::GetInstance();
		app->run(in_file);

		return 0;
	} catch(std::exception& e) {
			std::cout << e.what() << std::endl;
	}
}
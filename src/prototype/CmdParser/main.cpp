#include <CLI/CLI.hpp>
int main(int argc, char** argv) {
    CLI::App app{"MyApp"};
    std::string filename;
    bool verbose = false;
    app.add_option("-f,--file", filename, "File to process")->required();
    app.add_flag("-v,--verbose", verbose, "Enable verbose output");
	
	bool enable_feature = true;
	app.add_flag("--feature", enable_feature, "Enable feature")
	    ->default_val(true);
	app.add_flag("--no-feature", enable_feature, "Disable feature")
	    ->default_val(false);   // but this one will set enable_feature = false
		
	std::string output_file;
	app.add_option("output", output_file, "Output file (positional)")
	    ->required();   // now the user must provide it as a standalone argument
    CLI11_PARSE(app, argc, argv);
    // ... use 'filename' and 'verbose' ...
}

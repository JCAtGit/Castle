#include "clang/Tooling/Tooling.h"
#include <filesystem>
#include <memory>
#include <vector>

#include "../config/config_ds.hpp"
#include "../../../../../include/helper.hpp"
#include "../../../../prototype/marker-surgeon/marker-mechanism/marker-inserter/ast-mechanism/CodeMarkerAction.h"
#include "../../../../prototype/marker-surgeon/marker-mechanism/marker-remover/ast-mechanism/CodeRemoverAction.hpp"
#include "../../../../prototype/marker-surgeon/task-manager/task-predicates.hpp"
#include "../marker-mechanism/marker-inserter/CodeMarkerInserter.hpp"
#include "../marker-mechanism/marker-remover/CodeMarkerRemover.hpp"

namespace fs = std::filesystem;
using namespace clang;

/**
 * 	TODO: 
 *		Error-logging to same dir for each file or central file?
 * 		Allow ast-generation as an option?
 *		Create marker header file next to main?
 * 		Create header includes in marked files and point these to the marker header file (relatively? or rather dynamically).
 */
 
class TaskManager {
	private:
	std::vector<fs::path> pathQueue; // store paths to files that need processing	
	struct config* cfg;
	
	/**
	 *	Traverses dirs and adds files tot an queue for marking.
	 * 	TODO: 
	 * 		1. Only enqueues C files for now, h too?
	 *		2. Do strings first and then process?
	 * 		3. Exclusion and depth options for file mapping
	 */
	void setupFileQueue(){
		llvm::outs() << "Setup File Queue Start. \n";
		switch (cfg->taskquantifier) {
			case TASKQUANTIFIER::SINGLE:
				llvm::outs() << "TaskQuantifier SINGLE \n";
				pathQueue.push_back(cfg->inputFile);
				break;
			case TASKQUANTIFIER::LOCAL:
			llvm::outs() << "TaskQuantifier LOCAL \n";
				for (const auto& entry : fs::directory_iterator(cfg->inputDir)) {
					if (entry.path().extension() == ".c") {// TODO: implement filter extension
						// file with the marked ext does not need to be enqueued, except when extension is empty
						if(!isValidQueueSelection(cfg, entry.path())){// TODO: Extract file filters
							llvm::outs() << "Skipped: " <<  entry.path() << "\n" ;
							continue;
						}
						llvm::outs() << "Enqueued: " << entry.path() << "\n";
						pathQueue.push_back(entry.path());
					}
			    }
				break;
			case TASKQUANTIFIER::RECURSIVE:
				llvm::outs() << "TaskQuantifier RECURSIVE \n";
				for (const auto& entry : fs::recursive_directory_iterator(cfg->inputDir)) {
					if (entry.path().extension() == ".c") {// TODO: implement filter extension
						// file with the marked ext does not need to be enqueued, except when extension is empty
						if(!isValidQueueSelection(cfg, entry.path())){// TODO: Extract file filters
							llvm::outs() << "Skipped: " <<  entry.path() << "\n" ;
							continue;
						}
						llvm::outs() << "Enqueued: " << entry.path() << "\n";
						pathQueue.push_back(entry.path());
					}
			    }
				break;
			default:
				llvm::outs() << "Taskquantifier undefined. \n";
				break;
		}
	}
	
	/**
	 * Preprocesses files before marking, which include:
	 * Fixing dangling statements from constructs by adding the compound statement node brackets where necessary.
	 * TODO: Pre-expand macros.
	 */
	void preprocessQueueItem(fs::path path2process){
		std::string command;
		int status;
		switch (cfg->taskmode) {
			case TASKMODE::PREPROCESS:
						//"clang-format --style=\"{InsertBraces: true, BasedOnStyle: LLVM}\" " + "example0.c" + ">" + "example0_formatted.c";
				command = "clang-format -i --style=\"{InsertBraces: true, BasedOnStyle: LLVM}\" " + path2process.string();
			    status = std::system(command.c_str());
			    if (status == 0) {
					llvm::outs() << "clang-format performed for:" + path2process.string() + "\n";
			    } else {
					llvm::outs() << "clang-format failed for:" + path2process.string() + "\n";
			    }
//				runMacroExpander(path2process, cfg);
				break;
			case TASKMODE::INSERT:
				command = 
				//"clang-format --style=\"{InsertBraces: true, BasedOnStyle: LLVM}\" " + "example0.c" + ">" + "example0_formatted.c";
				"clang-format -i --style=\"{InsertBraces: true, BasedOnStyle: LLVM}\" " + path2process.string();
			    status = std::system(command.c_str());
			    if (status == 0) {
					llvm::outs() << "clang-format performed for:" + path2process.string() + "\n";
			    } else {
					llvm::outs() << "clang-format failed for:" + path2process.string() + "\n";
			    }
//				runMacroExpander(path2process, cfg);
				break;
			case TASKMODE::REMOVE:
				break;
			default:
				llvm::outs() << "Taskmode undefined. \n";
				return;
		}
	}
	
	void processQueue(){
		llvm::outs() 	<< "Process Queue Start. \n"				
						<< "pathQueue items: " << pathQueue.size() << "\n"
						<< "inputFile: " << cfg->inputFile.string() << "\n"
						<< "inputDir: " << cfg->inputDir.string() << "\n"
						<< "outputDIr: " << cfg->outputDir.string() << "\n";

		// TODO: Each iteration is independent from each other, if required we can process this concurrently.
		for(const auto& path2process : pathQueue){
			llvm::outs() << "Starting on " << path2process << "\n" ;
			// Path where the result will get stored later.
			fs::path outPath = cfg->outputDir / rel_outputPath(cfg->inputDir, path2process);
			//Doe dit voordat AST unit ptr is aangemaakt. Prepare file for marking, this includes fixing dangling statements
			preprocessQueueItem(path2process);
			
			auto codeString = file2string(path2process);
			unique_ptr<ASTFrontendAction> action;

			// TODO?: extract selection of action
			llvm::outs() << "Processing " << path2process.string() << "\n";
			switch (cfg->taskmode) {					
				case TASKMODE::INSERT:
					llvm::outs() << "TaskMode INSERT \n";
					action = std::make_unique<CodeMarkerAction>(*cfg);
					break;
				case TASKMODE::REMOVE:
					llvm::outs() << "TaskMode REMOVE \n";
					action = std::make_unique<CodeRemoverAction>(*cfg);	
					break;
				default:
					llvm::outs() << "Taskmode undefined. \n";
					return;
			}
			clang::tooling::runToolOnCodeWithArgs(std::move(action), codeString, cfg->ToolArgs, path2process.string());
		}
	}
	
	void wrapupProcesses(){
		llvm::outs() << "Wrap Up Processes Start. \n";					
		switch (cfg->taskmode) {					
			case TASKMODE::INSERT:
				// TODO: Not every format might create header, source or w/e.
				CodeMarkerInserter::createDependencyFiles(*cfg);
				break;
			case TASKMODE::REMOVE:
				// TODO: Change to toggle mechanism
				CodeMarkerRemover::removeDependencyFiles(*cfg);
				break;
			default:
				llvm::outs() << "Taskmode undefined. \n";
				return;
		}
	}
	
	void processErrors(){}
	
	public:		
		TaskManager (struct config* cfg): cfg(cfg){}
		
		void start(){
			setupFileQueue();
			processQueue();
			wrapupProcesses();
		}
};

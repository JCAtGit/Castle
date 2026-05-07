#pragma once

#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include <optional>
#include <filesystem>
#include "nlohmann/json.hpp"

#include "construct-processors/0/icp0.hpp"
#include "construct-processors/1/icp1.hpp"
#include "../MarkerSurgeonInterface.hpp"
#include "../../config/config_ds.hpp"
#include "../../../../../include/helper.hpp"
#include "../../preprocessor-mechanism/directives/directive-processor.hpp"
#include "construct-processors/0/markerPredicates.hpp"
#include "construct-processors/InserterConstructProcessorInterface.hpp"

using namespace clang;
using namespace nlohmann;

/**
 * Assumes optimization barriers using external linkage.
 TODO:
 -	Implement Config and markername
 - 	Mapping van grammar naar enums
 - 	Marker strings -> Enums
 - 	Mapping marker string to locations within code
 */

class CodeMarkerInserter : MarkerSurgeon {
	private:
		// Basic AST information and (re)writing handlers
		ASTContext &context;
		Rewriter &rewriter;
		SourceManager &SM;
		vector<RecordedPPConstruct> &ppEvents;
		
		// Config based settings
		json markerFormatCfg;
		// config selected processor
		std::unique_ptr<InserterConstructProcessor> icp;
	public:		
		// Gebruikt initializer lists, om waarden 1x te initializeren met de juiste waarden.
		CodeMarkerInserter(
			ASTContext &Context, 
			Rewriter &R, 
			json markerFormatCfg,
			vector<RecordedPPConstruct> &ppEvents
		) :
			context(Context), 
			rewriter(R), 
			markerFormatCfg(markerFormatCfg),
			SM(Context.getSourceManager()),
			ppEvents(ppEvents)
			{
				icp = std::make_unique<ICP0>();
			}
		
		
		void setTokenBuffer(std::optional<syntax::TokenBuffer> buffer) {
			icp->setTokenBuffer(std::move(*buffer));
		}
		
		// TODO: Generalize and change
		void insertMarkerDependency(){
			SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
			FileID mainFileID = SM.getMainFileID();
			StringRef filePath = SM.getFilename(startLoc);
			rewriter.InsertTextBefore(
				startLoc,
				icp->synthesizeMarkerDependencyString(filePath.str(), &cfg)
			);
		}
		
		void commitToFile(){
			llvm::outs() << "Start commit phase" << "\n";
			insertMarkerDependency();			

			// returns empty if path is on same dir.
			FileID MainFileID = SM.getMainFileID();
			StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(MainFileID));

			fs::path outPath = cfg.outputDir / rel_outputPath(cfg.inputDir, FilePath.str());
			if (outPath.empty() || outPath == "." || outPath == "./") {
			    outPath = FilePath.str();
			}
			llvm::outs() << "Committing changes from: " << FilePath << " -> " << outPath << "\n";
			// committing buffered changes to file
			if(!std::filesystem::exists(outPath.string())){
				fs::create_directories(outPath.parent_path());
			}
			// write to file and mutate with srcExt.
			std::error_code EC;
			llvm::raw_fd_ostream OutFile(
				fs::path(outPath).replace_filename(outPath.stem().string() + cfg.srcExt + outPath.extension().string()).string(),
				EC, 
				llvm::sys::fs::OF_Text
			);

			if (EC) {
				llvm::errs() << "Error opening output file: " << EC.message()<< "\n";
				return;
			}

			// Write buffered changes to file.
			rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(OutFile);
			OutFile.close();	
		}
		
		static void createDependencyFiles(struct config& cfg){
			switch(cfg.format){
				default: ICP0::createDependencyFiles(cfg); break;
				case 1:  ICP1::createDependencyFiles(cfg); break;
			}
		}
	
};


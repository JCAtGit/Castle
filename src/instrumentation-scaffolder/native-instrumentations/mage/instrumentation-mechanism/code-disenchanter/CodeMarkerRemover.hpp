#pragma once

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <optional>
#include <string>

//#include "construct-processors/ConstructProcessorInterface.hpp"
#include "../../../../../include/helper.hpp"
#include "../../../../../prototype/marker-surgeon/config/config_ds.hpp"
#include "../../../../../prototype/marker-surgeon/marker-mechanism/MarkerSurgeonInterface.hpp"
#include "../../../mage/instrumentation-mechanism/code-disenchanter/construct-processors/0/rcp0.hpp"
#include "../../../mage/instrumentation-mechanism/code-disenchanter/construct-processors/1/rcp1.hpp"
#include "../code-disenchanter/construct-processors/RemoverConstructProcessorInterface.hpp"

using namespace clang;
//
///**
// * Assumes optimization barriers using external linkage.
// TODO:
// -	Implement Config and markername
// - 	Mapping van grammar naar enums
// - 	Marker strings -> Enums
// - 	Mapping marker string to locations within code
// */
//
class CodeMarkerRemover: MarkerSurgeon {
	private:
		// Basic AST information and (re)writing handlers
		ASTContext &context;
		Rewriter &rewriter;
		SourceManager &SM;
		
//		// Config based settings
		struct config &cfg;

//		// config unused
//		int markerDepth = 0; // unused for now

//		// config selected processor
		std::unique_ptr<RemoverConstructProcessor> rcp;
		
	public:		
//		// Gebruikt initializer lists, om waarden 1x te initializeren met de juiste waarden.
		CodeMarkerRemover(ASTContext &Context, Rewriter &R, struct config &CFG) :
			context(Context), 
			rewriter(R), 
			cfg(CFG),
			SM(Context.getSourceManager())
			{
				switch (cfg.format) {
					default:rcp = std::make_unique<RCP0>(&Context, &R, cfg.markerName);break;
					case 1: rcp = std::make_unique<RCP1>(&Context, &R, cfg.markerName);break;
				}
			}

		static void removeDependencyFiles(struct config& cfg){
			switch(cfg.format){
				default: RCP0::removeDependencyFiles(cfg); break;
				case 1:  RCP1::removeDependencyFiles(cfg); break;
			}
		}
		
		// NOTICE: Should be performed after rewriter is done.
		void removeMarkerHeaderInclusion(){
			llvm::outs() << "Removing marker header inclusion directive. \n";
			removeFirstLineIfContains(rewriter, SM, cfg.markerDefinitionFileName);
		}
		
		void commitToFile(){
			llvm::outs() << "Start commit phase" << "\n";
			// returns empty if path is on same dir.
			FileID MainFileID = SM.getMainFileID();
			StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(MainFileID));

			fs::path outPath = cfg.outputDir / rel_outputPath(cfg.inputDir, FilePath.str());
			if (outPath.empty() || outPath == "." || outPath == "./") {
			    outPath = FilePath.str(); // Handle appropriately – maybe use just the filename
			} 
			llvm::outs() << "Commiting changes from: " << FilePath << " -> " << outPath << "\n";
						
			// committing buffered changes to file
			if(!std::filesystem::exists(outPath.string())){
				fs::create_directories(outPath.parent_path());
			}
			// write to file and mutate with srcExt if desired.
			std::error_code EC;
			llvm::raw_fd_ostream OutFile(
				fs::path(outPath).replace_filename(
					outPath.stem().string() + 
//					cfg.srcExt + 
					outPath.extension().string()
				).string(),
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
		
		void processDecl(Decl* D) {
			if (SM.isInMainFile(D->getLocation())){
				switch (D->getKind()) {
						case Decl::Function:
							rcp->processFunctionDecl(cast<FunctionDecl>(D));
						    break;
						case Decl::Var:
//						    ProcessVariable(cast<VarDecl>(D));
						    break;
						case Decl::Record:
						case Decl::CXXRecord: // CXXRecordDecl is a subclass of RecordDecl
//						    ProcessRecord(cast<RecordDecl>(D));
						    break;
						case Decl::Enum:
//						    ProcessEnum(cast<EnumDecl>(D));
						    break;
						case Decl::Typedef:
//						    ProcessTypedef(cast<TypedefDecl>(D));
						    break;
						default:
						    // Optionally handle other declaration kinds or do nothing
						    break;
						}
					}
				return;
		}
	
		void processStmt(Stmt* S) {
			if (SM.isInMainFile(S->getBeginLoc())){
				switch (S->getStmtClass()) {
					case Stmt::IfStmtClass:
					    break;
					case Stmt::WhileStmtClass:
						break;
					case Stmt::DeclStmtClass:
					    break;
					case Stmt::ForStmtClass:
						break;
					case Stmt::CallExprClass:
						rcp->processCallExpr(cast<CallExpr>(S));
					    break;
					case Stmt::SwitchStmtClass:
						break;
					case Stmt::CaseStmtClass:
						break;
					default:
					  	break;
				}
			}
		return;
		}
};


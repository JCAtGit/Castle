#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <cstddef>
#include <gtest/gtest.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <llvm/Support/MemoryBuffer.h>
#include <string>

#include "../instrumentation-mechanism/preprocessor/MacroExpansionRecorder.hpp"
#include <filesystem>
#include "binaries.hpp"
#include "json.hpp"
#include "nlohmann/json.hpp"
#include "paths.hpp"

using namespace clang;
using namespace std;
using namespace nlohmann;

class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
	public:
		MyASTVisitor(
			CompilerInstance &CI,
			MacroExpansionRecorder *macroRecorder,
			json &markerFormatCfg
		){}
	
	private:
	    bool VisitCallExpr(CallExpr *CE) {
			return true;
		}
};

class MyASTAction: public ASTFrontendAction {
	
	private:		
		MacroExpansionRecorder *macroRecorder = nullptr;
		
	public:
		MyASTAction() {}
	
	std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {		
		CI.getDiagnostics().setClient(new clang::IgnoringDiagConsumer(), true);
		
		auto recorder = std::make_unique<MacroExpansionRecorder>(CI.getSourceManager());
		macroRecorder = recorder.get();
		CI.getPreprocessor().addPPCallbacks(
			std::move(recorder)
		);
					
		return make_unique<ASTConsumer>();
	}

	// test execution starts here, we assume name-encoded markers.
	void EndSourceFileAction() override {
		CompilerInstance &CI = getCompilerInstance();
 		const string markerFormatCfgPath = "/home/user/Desktop/CCastle/CCastleData/configs/CCastlePipeline/formats.json";
		json markerFormatCfg;
		
		
		
		ASSERT_TRUE(filesystem::exists(markerFormatCfgPath));
		loadJSON(markerFormatCfgPath, markerFormatCfg);	
		
		
		
		// collect instrumentation metadata
		
		// test
		auto visitor = std::make_unique<MyASTVisitor>(CI, macroRecorder, markerFormatCfg);
		visitor->TraverseDecl(CI.getASTContext().getTranslationUnitDecl());
	}
};


class InstrumentationTest : public testing::Test {
protected:
    void run(const std::string& Code) {
		// 1. instrument file
		// 1.1. fetch source file
		
		string genericTestFilePath = "/home/user/Desktop/CCastle/CCastleRepository/src/instrumentation-scaffolder/native-instrumentations/mage/tests/FunctionDecl/origin/generic0.c";
		string genericTestFilestring = file2string(genericTestFilePath);
		string cmd = "";
		string cmdOutput;
		execCommandAndCaptureOutput(cmd, cmdOutput);
		
        // Fetch test code.
		
    }
};
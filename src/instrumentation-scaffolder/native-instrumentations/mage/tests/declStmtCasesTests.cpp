#include "clang/Frontend/CompilerInstance.h"
#include <gtest/gtest.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <string>
#include "clang/AST/Attr.h"

#include "../instrumentation-mechanism/preprocessor/MacroExpansionRecorder.hpp"
#include <filesystem>
#include "binaries.hpp"
#include "json.hpp"
#include "nlohmann/json.hpp"
#include "paths.hpp"
#include "../instrumentation-mechanism/preprocessor/attributes.hpp"

using namespace clang;
using namespace std;
using namespace nlohmann;

static bool detectedVarDecl, detectedFunctionDecl, detectedTypedefDecl, detectedRecordDecl, detectedEnumDecl;

class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
	private:
	public:
		MyASTVisitor(
			CompilerInstance &CI,
			MacroExpansionRecorder *macroRecorder,
			json &markerFormatCfg
		){}
		
		bool VisitVarDecl(VarDecl *VD) {
			detectedVarDecl = true;
		    return true;
		}

		bool VisitFunctionDecl(FunctionDecl *FD) {
			detectedFunctionDecl = true;
		    return true;
		}

		bool VisitTypedefDecl(TypedefDecl *TD) {
			detectedTypedefDecl = true;
		    return true;
		}

		bool VisitRecordDecl(RecordDecl *RD) {
			detectedRecordDecl = true;
		    return true;
		}

		bool VisitEnumDecl(EnumDecl *ED) {
		 	detectedEnumDecl = true;
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
			auto visitor = std::make_unique<MyASTVisitor>(CI, macroRecorder, markerFormatCfg);
			visitor->TraverseDecl(CI.getASTContext().getTranslationUnitDecl());
		}
};


class InstrumentationTest : public testing::Test {
protected:

	static void SetUpTestSuite() {
	    // runs once before any test in this suite

	}
	static void TearDownTestSuite() {
	    // runs once after all tests in this suite
	}

	void SetUp() override {
	    // runs before each test
	}
	void TearDown() override {
		// runs after each test
	}
};

static vector<string> toolArgs = {	
	"-xc", 			// Treat the input as C code, can use xc++ for cpp.
	"-std=c17", 	// https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3176.pdf, depends on source.
	"-nostdinc",	// Don't search for system headers
	"-Wno-all",		// Suppress all warnings, W-prefix stands for warning.
};

/**
 *	We test the visitation of various decl occurences, one for each.
 *
 *	Preconditions:
 *	Not a single decl instance is detected before visitation.
 *	
 *	Postconditions:
 *	All single decl instances are visited once and thus detected.
 */
TEST_F(InstrumentationTest, DeclVisitationTest) {
	EXPECT_FALSE(detectedVarDecl && detectedFunctionDecl && detectedTypedefDecl && detectedRecordDecl && detectedEnumDecl);	
	unique_ptr<ASTFrontendAction> action = std::make_unique<MyASTAction>();
	string declCasesCodeString = file2string("/home/user/Desktop/CCastle/CCastleRepository/src/instrumentation-scaffolder/native-instrumentations/mage/tests/Decl/declCases.cpp");
	clang::tooling::runToolOnCodeWithArgs(std::move(action), declCasesCodeString , toolArgs, "declStmtCasesTests.cpp");
	EXPECT_TRUE(detectedVarDecl && detectedFunctionDecl && detectedTypedefDecl && detectedRecordDecl && detectedEnumDecl);
}


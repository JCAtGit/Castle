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


class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
	private:
	public:
		MyASTVisitor(
			CompilerInstance &CI,
			MacroExpansionRecorder *macroRecorder,
			json &markerFormatCfg
		){}
		
		bool VisitStmt(Stmt* S){
			return true;
		}
		
		bool VisitFunctionDecl(FunctionDecl *FD){
			EXPECT_TRUE(!(hasAttrConflictsWithOptNone(FD) && FD->hasAttr<OptimizeNoneAttr>()));
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

static string compiledDir = 	"/home/user/Desktop/CCastle/CCastleRepository/src/instrumentation-scaffolder/native-instrumentations/mage/tests/Statements/compiled/";
static string metadataDir = 	"/home/user/Desktop/CCastle/CCastleRepository/src/instrumentation-scaffolder/native-instrumentations/mage/tests/Statements/metadata/";
static string testFileDir = 	"/home/user/Desktop/CCastle/CCastleRepository/src/instrumentation-scaffolder/native-instrumentations/mage/tests/Statements/original/";
static string instrumentedDir = "/home/user/Desktop/CCastle/CCastleRepository/src/instrumentation-scaffolder/native-instrumentations/mage/tests/Statements/instrumented/";
static vector<string> toolArgs = {	
	"-xc", 			// Treat the input as C code, can use xc++ for cpp.
	"-std=c17", 	// https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3176.pdf, depends on source.
	"-nostdinc",	// Don't search for system headers
	"-Wno-all",		// Suppress all warnings, W-prefix stands for warning.
};
static string compilingTestFileName = "compilingTestfile.c";
static string nonCompilingTestFileName = "nonCompilingTestfile.c";

class InstrumentationTest : public testing::Test {
protected:

	static void SetUpTestSuite() {
	    // runs once before any test in this suite
		ASSERT_TRUE(filesystem::exists(compiledDir));
		ASSERT_TRUE(filesystem::exists(metadataDir));
		ASSERT_TRUE(filesystem::exists(testFileDir));
		ASSERT_TRUE(filesystem::exists(instrumentedDir));
	}
	static void TearDownTestSuite() {
	    // runs once after all tests in this suite
		filesystem::remove_all(compiledDir);
		filesystem::create_directories(compiledDir);
		filesystem::remove_all(metadataDir);
		filesystem::create_directories(metadataDir);
		filesystem::remove_all(instrumentedDir);
		filesystem::create_directories(instrumentedDir);
	}

	void SetUp() override {
	    // runs before each test
	}
	void TearDown() override {
		// runs after each test
	}
};

/**
 *	Test creation of instrumented file.
 *	
 * 	Precondition: 
 *  Source codes with various instances of FunctionDecl.
 *	- no attributes
 *	- with attributes
 *		- contains conflicting attributes
 *		- contains no conflicting attributes
 *
 *	Postcondition:
 *  Instrumented source code has FunctionDecl instrumented if not conflicting.
 */
TEST_F(InstrumentationTest, InstrumentationTest) {
	string cmdMage = "/home/user/Desktop/CCastle/CCastleRepository/build/cmake.run.linux.x86_64.Local/Mage";
	string cmdMageGeneralFlags = "--on-copy --name_encoded_marker";
	string cmdMageOutputFlag = "--output";
	string cmdOutput;
	
	// compiling source file
	string cmdInput = 
		cmdMage 								+ " " + 
		testFileDir + compilingTestFileName		+ " " +
		cmdMageGeneralFlags 					+ " " +
		cmdMageOutputFlag						+ " " +
		instrumentedDir + compilingTestFileName;
	
	ASSERT_TRUE(filesystem::exists(testFileDir + compilingTestFileName));
	execCommandAndCaptureOutput(cmdInput, cmdOutput);
	ASSERT_TRUE(filesystem::exists(instrumentedDir + compilingTestFileName));
	
	// non-compiling source file
	cmdInput = 
		cmdMage 								+ " " + 
		testFileDir + nonCompilingTestFileName	+ " " +
		cmdMageGeneralFlags 					+ " " +
		cmdMageOutputFlag						+ " " +
		instrumentedDir + nonCompilingTestFileName;
	
	ASSERT_TRUE(filesystem::exists(testFileDir + nonCompilingTestFileName));
	execCommandAndCaptureOutput(cmdInput, cmdOutput);
	ASSERT_TRUE(filesystem::exists(instrumentedDir + nonCompilingTestFileName));
}


/**
 *	Compilation test:
 *	If the original source compiles, then the instrumented source should compile.
 *	Else if the original does not compile, then the instrumented source should not compile.  
 *
 *	Precondition:
 *	We have some source files that compile and do not compile.
 *	We only have an instrumented versions of those sources files.
 *
 * 	Postcondition:
 *	We have some source files that compile and do not compile.
 *	We only have an instrumented versions of those sources files.
 * 	Of the files that compile, we also have the binaries of the instrumented files through successful compilation.
 */
TEST_F(InstrumentationTest, CompileTest) {
	string cmdClang = "clang";
	string cmdClangOutputFlag = "-o";
	string cmdClangGeneralFlags = "-w";
	string cmdOutput;
	
	// compilation test for compiling original source.
	string cmdInput = 
		cmdClang 								+ " " +
		testFileDir + compilingTestFileName 	+ " " +
		cmdClangOutputFlag 						+ " " +
		compiledDir + compilingTestFileName 	+ " " +
		cmdClangGeneralFlags;

	execCommandAndCaptureOutput(cmdInput, cmdOutput);
	EXPECT_TRUE(filesystem::exists(compiledDir + compilingTestFileName));
	filesystem::remove_all(compiledDir + compilingTestFileName);
	
	// compilation test for compiling instrumented source.
	cmdInput = 
	cmdClang 									+ " " +
	instrumentedDir + compilingTestFileName 	+ " " +
	cmdClangOutputFlag 							+ " " +
	compiledDir + compilingTestFileName			+ " " +
	cmdClangGeneralFlags;
	execCommandAndCaptureOutput(cmdInput, cmdOutput);
	EXPECT_TRUE(filesystem::exists(compiledDir + compilingTestFileName));
	
	// compilation test for non-compiling original source.
	cmdInput = 
	cmdClang 									+ " " +
	instrumentedDir + nonCompilingTestFileName 	+ " " +
	cmdClangOutputFlag 							+ " " +
	compiledDir + nonCompilingTestFileName		+ " " +
	cmdClangGeneralFlags;
	execCommandAndCaptureOutput(cmdInput, cmdOutput);
	EXPECT_FALSE(filesystem::exists(compiledDir + nonCompilingTestFileName));

	// compilation test for non-compiling instrumented source.
	cmdInput = 
	cmdClang 									+ " " +
	instrumentedDir + nonCompilingTestFileName 	+ " " +
	cmdClangOutputFlag 							+ " " +
	compiledDir + nonCompilingTestFileName		+ " " +
	cmdClangGeneralFlags;
	execCommandAndCaptureOutput(cmdInput, cmdOutput);
	EXPECT_FALSE(filesystem::exists(compiledDir + nonCompilingTestFileName));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
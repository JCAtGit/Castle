#include <gtest/gtest.h>

#include <string>
#include "clang/AST/Attr.h"

#include <filesystem>
#include "binaries.hpp"
#include "json.hpp"
#include "nlohmann/json.hpp"
#include "paths.hpp"

using namespace clang;
using namespace std;
using namespace nlohmann;

static const string csmithDefaultBinariesDir = "/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/csmith_mage_clang";
static const string csmithInstrumentedBinariesDir = "/home/user/Desktop/CCastle/CCastleData/ValidationScaffolder/csmith_validation-build";
static const string csmithChecksumVerifierBinPath = "/home/user/Desktop/CCastle/CCastleRepository/build/cmake.run.linux.x86_64.Local/SmithCheckSumVerifier";

class CSmithChecksumTest : public testing::Test {
protected:

	static void SetUpTestSuite() {
	    // runs once before any test in this suite
		
		// setup and check if repositories are present and setup
		ASSERT_TRUE(filesystem::exists(csmithDefaultBinariesDir));
		ASSERT_TRUE(filesystem::exists(csmithInstrumentedBinariesDir));
		ASSERT_TRUE(filesystem::exists(csmithChecksumVerifierBinPath));

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

/**
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum.
 *	Uses hardcoded paths for now.
 */
TEST_F(CSmithChecksumTest, checksumTest) {
	string command = csmithChecksumVerifierBinPath, output;
	
	for (const auto& entry : filesystem::directory_iterator()) {
		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(csmithInstrumentedBinariesDir / entry.path().filename()));
		
		execCommandAndCaptureOutput(command, output);
	}
}


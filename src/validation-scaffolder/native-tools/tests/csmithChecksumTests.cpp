#include <gtest/gtest.h>
#include <string>
#include <filesystem>
#include "binaries.hpp"

using namespace std;

static const string smithDefaultBinariesDir = "/home/user/Desktop/CCastle/CCastleData/ValidationScaffolder/csmith_validation-build";
static const string smithInstrumentedBinariesDir = "/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/csmith_mage_clang";

class CSmithChecksumTest : public testing::Test {
protected:

	static void SetUpTestSuite() {
	    // runs once before any test in this suite
		
		// setup and check if repositories are present and setup
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir));

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
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum at O0.
 *	Test can take a while with 1000 files.
 */
TEST_F(CSmithChecksumTest, checksumTestO0) {
	string output0, output1, olevel = "O0";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));
		
		cout << smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string() << "\n";
		cout << smithInstrumentedBinariesDir + "/" + entry.path().filename().string() << "\n";
		
		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);
		EXPECT_TRUE(output0 == output1);
	}
}

/**
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum at O1.
 *	Test can take a while with 1000 files.
 */
TEST_F(CSmithChecksumTest, checksumTestO1) {
	string output0, output1, olevel = "O1";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));
		
		cout << smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string() << "\n";
		cout << smithInstrumentedBinariesDir + "/" + entry.path().filename().string() << "\n";
		
		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);
		EXPECT_TRUE(output0 == output1);
	}
}

/**
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum at O2.
 *	Test can take a while with 1000 files.
 */
TEST_F(CSmithChecksumTest, checksumTestO2) {
	string output0, output1, olevel = "O2";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));
		
		cout << smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string() << "\n";
		cout << smithInstrumentedBinariesDir + "/" + entry.path().filename().string() << "\n";
		
		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);
		EXPECT_TRUE(output0 == output1);
	}
}

/**
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum at O3.
 *	Test can take a while with 1000 files.
 */
TEST_F(CSmithChecksumTest, checksumTestO3) {
	string output0, output1, olevel = "O3";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));
		
		cout << smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string() << "\n";
		cout << smithInstrumentedBinariesDir + "/" + entry.path().filename().string() << "\n";
		
		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);
		EXPECT_TRUE(output0 == output1);
	}
}



#include <gtest/gtest.h>
#include <string>
#include <filesystem>
#include "binaries.hpp"

using namespace std;

static const string smithDefaultBinariesDir = "/home/user/Desktop/CCastle/CCastleData/ValidationScaffolder/csmith_validation-build";
static const string smithInstrumentedBinariesDir = "/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/csmith_mage_clang";
static const set<int> skiplist = {
	15, 30, 79, 80, 142, 273, 307, 313, 325, 331, 334, 347, 365, 386,
	387, 394, 402, 406, 415, 425, 435, 436, 440, 449, 460, 463, 475, 
	478, 482, 485, 489, 525, 537, 563, 569, 573, 599, 606, 613, 627,
	628, 629, 632, 633, 635, 637, 641, 665, 674, 675, 681, 690, 693,
	701, 709, 726, 732,734, 762, 763, 766, 785, 797, 802, 803, 825,
	829, 830, 834, 844, 849, 853, 858, 860, 861, 862, 863, 878, 880, 
	881, 885, 896, 901, 914, 915, 921, 929, 941, 949, 950, 957, 969,
}; // Not all smith programs halt, therefore this set can used to skip specific tests.


/**
 * 	The following tests are designed to check if the checksum of the global state is influenced by marker instrumentation.
 *	Note: the tests take a long time.
 */

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
		if(skiplist.find(stoi(entry.path().filename().string())) != skiplist.end()){continue;}

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
 */
TEST_F(CSmithChecksumTest, checksumTestO1) {
	string output0, output1, olevel = "O1";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		if(skiplist.find(stoi(entry.path().filename().string())) != skiplist.end()){continue;}

		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));
		
		cout << smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string() << "\n";
		cout << smithInstrumentedBinariesDir + "/" + entry.path().filename().string() << "\n";
		
		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);
		EXPECT_TRUE(output0 == output1);
		output0.clear();
		output1.clear();
	}
}

/**
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum at O2.
 */
TEST_F(CSmithChecksumTest, checksumTestO2) {
	string output0, output1, olevel = "O2";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		if(skiplist.find(stoi(entry.path().filename().string())) != skiplist.end()){continue;}

		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));
		
		cout << smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string() << "\n";
		cout << smithInstrumentedBinariesDir + "/" + entry.path().filename().string() << "\n";
		
		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);
		EXPECT_TRUE(output0 == output1);
		output0.clear();
		output1.clear();
	}
}

/**
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum at O3.
 */
TEST_F(CSmithChecksumTest, checksumTestO3) {
	string output0, output1, olevel = "O3";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		if(skiplist.find(stoi(entry.path().filename().string())) != skiplist.end()){continue;}

		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));
		
		cout << smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string() << "\n";
		cout << smithInstrumentedBinariesDir + "/" + entry.path().filename().string() << "\n";
		
		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);
		EXPECT_TRUE(output0 == output1);
		output0.clear();
		output1.clear();
	}
}



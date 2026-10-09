#include <gtest/gtest.h>
#include <string>
#include <filesystem>
#include <vector>
#include "binaries.hpp"

using namespace std;

static const string smithInstrumentedBinariesDir = "/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/dsmith_mage_clang";
static const string smithDefaultBinariesDir = "/home/user/Desktop/CCastle/CCastleData/ValidationScaffolder/dsmith_validation-build";
static const set<int> skiplist = {
	0, 2,3, 5,6,7,8, 9, 10, 11, 14, 16, 21, 26, 30, 31, 34, 
	35, // special case of optimization error, also see case35test.
	36, 37, 39, 43, 44, 48, 51, 52, 54, 57, 58, 59, 63, 65, 66, 
	68, 70, 74, 77, 78, 80, 82, 83, 85, 87, 90, 93, 97
}; // // Not all smith programs halt or execute error-free, therefore this set can used to skip specific tests.

/**
* 	The following tests are designed to check if the checksum of the global state is influenced by marker instrumentation.
*	Note: the tests take a long time or throw errors of which it is unclear if it related to marker.
* 	Tests limited to seed 100.
*/


class DSmithChecksumTest : public testing::Test {
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
 */
TEST_F(DSmithChecksumTest, checksumTestO0) {
	string output0, output1, olevel = "O0";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		if(skiplist.find(stoi(entry.path().filename().string())) != skiplist.end()){continue;}
		else if (stoi(entry.path().filename().string()) > 100){break;}
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
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum at O1.
 */
TEST_F(DSmithChecksumTest, checksumTestO1) {
	string output0, output1, olevel = "O1";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		if(skiplist.find(stoi(entry.path().filename().string())) != skiplist.end()){continue;}
		else if (stoi(entry.path().filename().string()) > 100){break;}

		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));

		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);

		EXPECT_TRUE(output0 == output1);
		if(output0 != output1){
			cout << entry.path().filename().string() << " ";
		}
		output0.clear();
		output1.clear();
	}
	cout << "\n";
}

/**
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum at O2.
 */
TEST_F(DSmithChecksumTest, checksumTestO2) {
	string output0, output1, olevel = "O2";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		if(skiplist.find(stoi(entry.path().filename().string())) != skiplist.end()){continue;}
		else if (stoi(entry.path().filename().string()) > 100){break;}

		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));
		
		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);

		EXPECT_TRUE(output0 == output1);
		if(output0 != output1){
			cout << entry.path().filename().string() << " ";
		}
		output0.clear();
		output1.clear();
	}
	cout << "\n";
}

/**
 *	Checks if the default and instrumented binaries of csmith files produce the same checksum at O3.
 */
TEST_F(DSmithChecksumTest, checksumTestO3) {
	string output0, output1, olevel = "O3";
	ASSERT_TRUE(filesystem::exists(smithDefaultBinariesDir + olevel));

	for (const auto& entry : filesystem::directory_iterator(smithDefaultBinariesDir + olevel)) {
		if(skiplist.find(stoi(entry.path().filename().string())) != skiplist.end()){continue;}
		else if (stoi(entry.path().filename().string()) > 100){break;}

		ASSERT_TRUE(filesystem::exists(entry));
		ASSERT_TRUE(filesystem::exists(smithInstrumentedBinariesDir / entry.path().filename()));
		execCommandAndCaptureOutput(smithDefaultBinariesDir + olevel + "/" + entry.path().filename().string(), output0);
		execCommandAndCaptureOutput(smithInstrumentedBinariesDir + "/" + entry.path().filename().string(), output1);
	
		EXPECT_TRUE(output0 == output1);
		if(output0 != output1){
			cout << entry.path().filename().string() << " ";
		}
		output0.clear();
		output1.clear();
	}
	cout << "\n";
}

/**
 *	We see that the markers completely comply with O0, but not with optimization > 0 for the specific case of 35. 
 *	The case of 35 is a change caused by the optimizer, assuming correct checksum calculation, since we already see this difference between the uninstrumented 35 outputs.
 */
TEST_F(DSmithChecksumTest, case35test) {
		string output0, output1, output2, output3;
		execCommandAndCaptureOutput(smithDefaultBinariesDir + "O0" + "/" + "35", output0);
		execCommandAndCaptureOutput(smithDefaultBinariesDir + "O1" + "/" + "35", output1);
		execCommandAndCaptureOutput(smithDefaultBinariesDir + "O2" + "/" + "35", output2);
		execCommandAndCaptureOutput(smithDefaultBinariesDir + "O3" + "/" + "35", output3);
		
		// Expect false due to optimization error (or checksum calc error?).
		EXPECT_NE(output0, output1);
		EXPECT_NE(output0, output2);
		EXPECT_NE(output0, output3);
		
		EXPECT_EQ(output1, output2);
		EXPECT_EQ(output2, output3);
		EXPECT_EQ(output1, output3);
}


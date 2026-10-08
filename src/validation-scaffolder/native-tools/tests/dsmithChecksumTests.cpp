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



class CSmithChecksumTest : public testing::Test {
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

/**
 *	
 */
TEST_F(CSmithChecksumTest, DeclVisitationTest) {
	
}


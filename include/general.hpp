#include <string>

using namespace std;

/**
 * Checks if a string is a number.
 */
bool isStringANumber(const string &str){
	for (int i = 0; i < str.length(); i++) {
	   if(!isdigit(str[i])) {
		return false;
	   }
	}
	return true;
}
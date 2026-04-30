#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

int main(int argc, char const *argv[]){
// instead, you could also write (which looks very similar to the JSON above)
json j2 = {
  {"pi", 3.141},
  {"happy", true},
  {"name", "Niels"},
  {"nothing", nullptr},
  {"answer", {
    {"everything", 42}
  }},
  {"list", {1, 0, 2}},
  {"object", {
    {"currency", "USD"},
    {"value", 42.99}
  }}
};
    std::cout << j2.dump(4) << std::endl;
    return 0;
}

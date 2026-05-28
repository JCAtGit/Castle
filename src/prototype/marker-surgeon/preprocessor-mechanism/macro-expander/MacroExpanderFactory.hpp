#pragma once

#include "../../../../../prototype/marker-surgeon/preprocessor-mechanism/macro-expander/ast-mechanism/MacroExpanderAction.hpp"
#include "../../config/config.hpp"

using namespace clang;

class MacroExpanderActionFactory : public clang::tooling::FrontendActionFactory {
public:
  MacroExpanderActionFactory(std::string path2process, config &cfg) : path2process(path2process), cfg(cfg) {}

  std::unique_ptr<clang::FrontendAction> create() override {
    return std::make_unique<MacroExpanderAction>(path2process, cfg);
  }

private:
	std::string path2process;
  	config &cfg;
};
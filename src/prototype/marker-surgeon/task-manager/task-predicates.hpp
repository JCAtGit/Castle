#pragma once

#include "../config/config_ds.hpp"
#include "../../../../../include/helper.hpp"
#include <filesystem>

namespace fs = std::filesystem;

bool isValidRecursiveInserterSelection(struct config *cfg, std::string entryPath){
	if(fs::path(entryPath).filename() == cfg->markerDefinitionFileName){
		return false;
	}
	if(!cfg->srcExt.empty()){
		return !substringOnlyAtEndOfBasename(entryPath, cfg->srcExt);
	} else {
		return true;
	}
}

bool isValidRecursiveRemoverSelection(struct config *cfg, std::string entryPath){
	if(!cfg->srcExt.empty()){
		return substringOnlyAtEndOfBasename(entryPath, cfg->srcExt);
	} else {
		return true;
	}
}

bool isValidQueueSelection(struct config *cfg,  std::string entryPath){
	switch (cfg->taskmode) {
		case TASKMODE::INSERT:
			return isValidRecursiveInserterSelection(cfg, entryPath);
		case TASKMODE::REMOVE:;
			return isValidRecursiveRemoverSelection(cfg, entryPath);
		default: 
			return false;
	}
	return 0;
}




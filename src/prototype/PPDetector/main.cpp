// pp_adjacent_final.cpp
// Compile: clang++ -std=c++17 pp_adjacent_final.cpp -o pp_adjacent \
//   $(llvm-config --cxxflags --ldflags --libs) \
//   -lclangTooling -lclangFrontend -lclangLex -lclangBasic

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TokenKinds.h"
#include "clang/Basic/FileEntry.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include <memory>

using namespace clang;
using namespace clang::tooling;

static llvm::cl::OptionCategory AdjacentCategory("pp-adjacent");

// --------------------------------------------------------------
// 1. Data structures for recorded preprocessor events
// --------------------------------------------------------------
enum class DirectiveKind {
    Include, If, Ifdef, Ifndef, Elif, Else, Endif, Pragma
};

struct RecordedPPConstruct {
    std::string name;
    SourceRange range;
    bool isMacro;
    DirectiveKind kind;
};

// --------------------------------------------------------------
// 2. PPCallbacks that records everything (with main‑file filter)
// --------------------------------------------------------------
class RecordingPPCallbacks : public PPCallbacks {
    std::vector<RecordedPPConstruct> &Events;
    SourceManager &SM;
public:
    RecordingPPCallbacks(std::vector<RecordedPPConstruct> &Events, SourceManager &SM)
        : Events(Events), SM(SM) {}

//    void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
//                      SourceRange Range, const MacroArgs *Args) override {
//        if (!SM.isInMainFile(Range.getBegin())) return;
//        Events.push_back({MacroNameTok.getIdentifierInfo()->getName().str(),
//                          Range, true, DirectiveKind::If});
//    }

	// Called before stmt callbacks. If terminal gets overwhelmed, comment out the stmt callbacks.
	void MacroDefined(const Token &MacroNameTok, const MacroDirective *MD) override {
	    llvm::errs() << "MacroDefined: " << MacroNameTok.getIdentifierInfo()->getName() << "\n";
	}
	
	void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
	                  SourceRange Range, const MacroArgs *Args) override {
	    std::string Name = MacroNameTok.getIdentifierInfo()->getName().str();
	    bool inMain = SM.isInMainFile(Range.getBegin());
	    llvm::errs() << "MacroExpands: " << Name
	                 << " at " << Range.getBegin().printToString(SM)
	                 << " (inMain=" << inMain << ")\n";
	    if (!inMain) return;
	    Events.push_back({Name, Range, true, DirectiveKind::If});
	}
	
    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                            StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange,
                            OptionalFileEntryRef File,
                            StringRef SearchPath, StringRef RelativePath,
                            const Module *SuggestedModule, bool ModuleImported,
                            SrcMgr::CharacteristicKind FileType) override {
        SourceRange Range(HashLoc, FilenameRange.getEnd());
        if (!SM.isInMainFile(Range.getBegin())) return;
        Events.push_back({FileName.str(), Range, false, DirectiveKind::Include});
    }

    void If(SourceLocation Loc, SourceRange ConditionRange,
            ConditionValueKind ConditionValue) override {
        if (!SM.isInMainFile(Loc)) return;
        Events.push_back({"", ConditionRange, false, DirectiveKind::If});
    }
    void Ifdef(SourceLocation Loc, const Token &MacroNameTok,
               const MacroDefinition &MD) override {
        if (!SM.isInMainFile(Loc)) return;
        std::string Name = MacroNameTok.getIdentifierInfo()->getName().str();
        SourceRange Range(Loc, MacroNameTok.getLocation());
        Events.push_back({Name, Range, false, DirectiveKind::Ifdef});
    }
    void Ifndef(SourceLocation Loc, const Token &MacroNameTok,
                const MacroDefinition &MD) override {
        if (!SM.isInMainFile(Loc)) return;
        std::string Name = MacroNameTok.getIdentifierInfo()->getName().str();
        SourceRange Range(Loc, MacroNameTok.getLocation());
        Events.push_back({Name, Range, false, DirectiveKind::Ifndef});
    }
    void Elif(SourceLocation Loc, SourceRange ConditionRange,
              ConditionValueKind ConditionValue, SourceLocation IfLoc) override {
        if (!SM.isInMainFile(Loc)) return;
        Events.push_back({"", ConditionRange, false, DirectiveKind::Elif});
    }
    void Else(SourceLocation Loc, SourceLocation IfLoc) override {
        if (!SM.isInMainFile(Loc)) return;
        Events.push_back({"", SourceRange(Loc, Loc), false, DirectiveKind::Else});
    }
    void Endif(SourceLocation Loc, SourceLocation IfLoc) override {
        if (!SM.isInMainFile(Loc)) return;
        Events.push_back({"", SourceRange(Loc, Loc), false, DirectiveKind::Endif});
    }
    void PragmaDirective(SourceLocation Loc, PragmaIntroducerKind Introducer) override {
        if (!SM.isInMainFile(Loc)) return;
        Events.push_back({"", SourceRange(Loc, Loc), false, DirectiveKind::Pragma});
    }
};

// --------------------------------------------------------------
// 3. Token index mapping using file offsets (robust)
// --------------------------------------------------------------
struct TokenRange { size_t firstIdx, lastIdx; };
static std::optional<TokenRange> getTokenIndicesForRange(
    const SourceRange &SR, llvm::ArrayRef<syntax::Token> SpelledTokens,
    const SourceManager &SM) {
    SourceLocation Begin = SM.getExpansionLoc(SR.getBegin());
    SourceLocation End = SM.getExpansionLoc(SR.getEnd());
    if (Begin.isInvalid() || End.isInvalid()) return std::nullopt;
    unsigned BeginOff = SM.getFileOffset(Begin);
    unsigned EndOff = SM.getFileOffset(End);
    size_t first = SpelledTokens.size(), last = 0;
    bool foundFirst = false;
    for (size_t i = 0; i < SpelledTokens.size(); ++i) {
        unsigned TokOff = SM.getFileOffset(SM.getExpansionLoc(SpelledTokens[i].location()));
        if (!foundFirst && TokOff >= BeginOff) {
            first = i;
            foundFirst = true;
        }
        if (TokOff <= EndOff) last = i;
    }
    if (!foundFirst || first > last) return std::nullopt;
    return TokenRange{first, last};
}

// --------------------------------------------------------------
// 4. Detection function (same adjacency logic)
// --------------------------------------------------------------
struct EventVectors {
    std::vector<RecordedPPConstruct> before, after, containing, inside;
};

EventVectors detectPPConstructsAroundStatement(
    const Stmt *S, const syntax::TokenBuffer &Tokens,
    const SourceManager &SM, const std::vector<RecordedPPConstruct> &Events) {

    EventVectors result;
    if (!S) return result;
    SourceRange stmtRange = S->getSourceRange();
    if (stmtRange.isInvalid()) return result;

    FileID mainFileID = SM.getMainFileID();
    auto allSpelled = Tokens.spelledTokens(mainFileID);
    if (allSpelled.empty()) return result;

    // Find statement token range
    size_t firstStmtIdx = allSpelled.size(), lastStmtIdx = 0;
    for (size_t i = 0; i < allSpelled.size(); ++i) {
        if (SM.isPointWithin(allSpelled[i].location(), stmtRange.getBegin(), stmtRange.getEnd())) {
            firstStmtIdx = std::min(firstStmtIdx, i);
            lastStmtIdx = i;
        }
    }
    if (firstStmtIdx > lastStmtIdx) return result;

    // Process recorded events
    for (const auto &ev : Events) {
        if (ev.range.isInvalid()) continue;
        if (!SM.isInMainFile(ev.range.getBegin())) continue;
        auto tokRange = getTokenIndicesForRange(ev.range, allSpelled, SM);
        if (!tokRange) continue;
        size_t firstEvt = tokRange->firstIdx, lastEvt = tokRange->lastIdx;
        if (firstEvt <= firstStmtIdx && lastEvt >= lastStmtIdx)
            result.containing.push_back(ev);
        else if (firstEvt >= firstStmtIdx && lastEvt <= lastStmtIdx)
            result.inside.push_back(ev);
        else if (lastEvt + 1 == firstStmtIdx)
            result.before.push_back(ev);
        else if (lastStmtIdx + 1 == firstEvt)
            result.after.push_back(ev);
    }
    return result;
}

// --------------------------------------------------------------
// 5. Printing
// --------------------------------------------------------------
void printEvent(const RecordedPPConstruct &ev, llvm::raw_ostream &OS) {
    if (ev.isMacro) OS << "macro " << ev.name;
    else {
        switch (ev.kind) {
            case DirectiveKind::Include: OS << "#include " << ev.name; break;
            case DirectiveKind::If:      OS << "#if"; break;
            case DirectiveKind::Ifdef:   OS << "#ifdef " << ev.name; break;
            case DirectiveKind::Ifndef:  OS << "#ifndef " << ev.name; break;
            case DirectiveKind::Elif:    OS << "#elif"; break;
            case DirectiveKind::Else:    OS << "#else"; break;
            case DirectiveKind::Endif:   OS << "#endif"; break;
            case DirectiveKind::Pragma:  OS << "#pragma"; break;
        }
    }
}

void printEventVectors(const EventVectors &vec, const SourceManager &SM) {
    auto print = [&](const char *title, const auto &list) {
        if (list.empty()) return;
        llvm::outs() << title << ":\n";
        for (const auto &ev : list) {
            llvm::outs() << "  ";
            printEvent(ev, llvm::outs());
            llvm::outs() << " at " << ev.range.getBegin().printToString(SM) << "\n";
        }
    };
    print("  Containing", vec.containing);
    print("  Inside", vec.inside);
    print("  Before (adjacent)", vec.before);
    print("  After (adjacent)", vec.after);
}

// --------------------------------------------------------------
// 6. AST visitor
// --------------------------------------------------------------
class MyVisitor : public RecursiveASTVisitor<MyVisitor> {
    SourceManager &SM;
    const syntax::TokenBuffer &Tokens;
    const std::vector<RecordedPPConstruct> &Events;
public:
    MyVisitor(SourceManager &SM, const syntax::TokenBuffer &T,
              const std::vector<RecordedPPConstruct> &E)
        : SM(SM), Tokens(T), Events(E) {}

//    bool VisitStmt(Stmt *S) {
//        if (!SM.isInMainFile(S->getBeginLoc())) return true;
//        if (S->getBeginLoc().isInvalid()) return true;
//        llvm::outs() << "\n=== Statement " << S->getStmtClassName()
//                     << " at " << S->getBeginLoc().printToString(SM) << "\n";
//        auto res = detectPPConstructsAroundStatement(S, Tokens, SM, Events);
//        printEventVectors(res, SM);
//        return true;
//    }
};

// --------------------------------------------------------------
// 7. FrontendAction
// --------------------------------------------------------------
// pp_adjacent_final.cpp - corrected version
// ... includes as before ...

class MyAction : public ASTFrontendAction {
    std::vector<RecordedPPConstruct> Events;
    std::unique_ptr<syntax::TokenCollector> TokenCollector; // use unique_ptr

public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                   StringRef InFile) override {
        // Register callbacks
        CI.getPreprocessor().addPPCallbacks(
            std::make_unique<RecordingPPCallbacks>(Events, CI.getSourceManager()));
        // Create token collector
        TokenCollector = std::make_unique<syntax::TokenCollector>(CI.getPreprocessor());
        return std::make_unique<ASTConsumer>();
    }

    void ExecuteAction() override {
        ASTFrontendAction::ExecuteAction();
    }

    void EndSourceFileAction() override {
        CompilerInstance &CI = getCompilerInstance();
        if (!TokenCollector) return;
        syntax::TokenBuffer TB = std::move(*TokenCollector).consume();
        TB.indexExpandedTokens();
        MyVisitor visitor(CI.getSourceManager(), TB, Events);
        visitor.TraverseDecl(CI.getASTContext().getTranslationUnitDecl());
    }
};

// main unchanged

// --------------------------------------------------------------
// 8. Main
// --------------------------------------------------------------
int main(int argc, const char **argv) {
    auto EP = CommonOptionsParser::create(argc, argv, AdjacentCategory);
    if (!EP) {
        llvm::errs() << EP.takeError();
        return 1;
    }
    ClangTool Tool(EP->getCompilations(), EP->getSourcePathList());
    return Tool.run(newFrontendActionFactory<MyAction>().get());
}
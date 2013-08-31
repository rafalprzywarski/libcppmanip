#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>
#include <clang/Lex/Lexer.h>
#include <sstream>
#include <fstream>
#include <stdexcept>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

std::ostringstream output;

template <typename Node>
unsigned extraChars(const Node&) { return 0; }

template <>
unsigned extraChars<Stmt>(const Stmt&) { return 2; }

template <typename T>
std::pair<SourceLocation, unsigned> getSourceLocationAndLength(const SourceManager &SourceManager, const T &Node)
{
  clang::SourceLocation StartSpellingLocatino =
      SourceManager.getSpellingLoc(Node.getSourceRange().getBegin());
  clang::SourceLocation EndSpellingLocation =
      SourceManager.getSpellingLoc(Node.getSourceRange().getEnd());
  if (!StartSpellingLocatino.isValid() || !EndSpellingLocation.isValid()) {
    throw std::runtime_error("!StartSpellingLocatino.isValid() || !EndSpellingLocation.isValid()");
  }
  bool Invalid = true;
  std::pair<clang::FileID, unsigned> Start =
      SourceManager.getDecomposedLoc(StartSpellingLocatino);
  std::pair<clang::FileID, unsigned> End =
      SourceManager.getDecomposedLoc(clang::Lexer::getLocForEndOfToken(
          EndSpellingLocation, 0, SourceManager, clang::LangOptions()));
  if (End.second < Start.second) {
    // Shuffling text with macros may cause this.
    throw std::runtime_error("End.second < Start.second");
  }
  unsigned sourceLength = End.second - Start.second + extraChars(Node);
  return std::make_pair(StartSpellingLocatino, sourceLength);
}

std::string GetText(const clang::SourceManager &SourceManager, std::pair<SourceLocation, unsigned> loc) {
  bool Invalid = true;
  const char *Text =
      SourceManager.getCharacterData(loc.first, &Invalid);
  if (Invalid) {
    return std::string();
  }
  return std::string(Text, loc.second);
}

std::string GetTextWithReplace(const clang::SourceManager &SourceManager, std::pair<SourceLocation, unsigned> loc, std::pair<SourceLocation, unsigned> without, std::string replace) {
  bool Invalid = true;
  const char *Text =
      SourceManager.getCharacterData(loc.first, &Invalid);
  if (Invalid) {
    return std::string();
  }
  const char *Text2 =
      SourceManager.getCharacterData(without.first, &Invalid);
  if (Invalid) {
    return std::string();
  }
  return std::string(Text, Text2) + replace + std::string(Text2 + without.second, loc.second - without.second - (Text2 - Text));
}

class MethodExtractor : public RecursiveASTVisitor<MethodExtractor>
{
    ASTContext& ctx;
public:
    MethodExtractor(ASTContext& ctx) : ctx(ctx) { }
    bool VisitFunctionDecl(FunctionDecl* decl)
    {
        if (!ctx.getSourceManager().isFromMainFile(decl->getLocation()))
            return true;
        Stmt::child_iterator it = decl->getBody()->child_begin();
        ++it;
        Stmt& stmt = **it;
        printExtractedFunction(stmt);
        printOriginalFunctionWithExtractedFunctionCall(*decl, stmt);
        return false;
    }
private:
    void printExtractedFunction(Stmt& stmt)
    {
        output << "void runLoop()\n{\n    " << GetText(ctx.getSourceManager(), getSourceLocationAndLength(ctx.getSourceManager(), stmt)) << "}\n";
    }
    void printOriginalFunctionWithExtractedFunctionCall(FunctionDecl& decl, Stmt& stmt)
    {
        output << GetTextWithReplace(
            ctx.getSourceManager(),
            getSourceLocationAndLength(ctx.getSourceManager(), decl),
            getSourceLocationAndLength(ctx.getSourceManager(), stmt),
            "runLoop();\n");
    }
};

class MethodExtractorUnitHandler : public ASTConsumer
{
public:
    MethodExtractorUnitHandler() { }
    virtual void HandleTranslationUnit(ASTContext& ctx)
    {
        MethodExtractor(ctx).TraverseDecl(ctx.getTranslationUnitDecl());
    }
};

class MethodExtractorFactory: public ASTFrontendAction
{
public:
    virtual ASTConsumer* CreateASTConsumer(CompilerInstance&, StringRef)
    {
        return new MethodExtractorUnitHandler();
    }
};

int main(int argc, const char** argv)
{
    int fakeArgc = 3;
    const char *fakeArgv[] = { argv[0], argv[1], "--" };
    CommonOptionsParser parser(fakeArgc, fakeArgv);
    ClangTool tool(parser.GetCompilations(), parser.GetSourcePathList());
    
    tool.run(newFrontendActionFactory<MethodExtractorFactory>());
    
    std::ofstream f(argv[1]);
    f << output.str();
}
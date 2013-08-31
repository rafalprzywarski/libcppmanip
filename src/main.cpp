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
unsigned extraCharsHack(const Node&) { return 0; }

template <>
unsigned extraCharsHack<Stmt>(const Stmt&) { return 2; }

class SourceExtractor
{
public:
    SourceExtractor(clang::SourceManager& sourceManager) : sourceManager(sourceManager) { }
    
    template <typename Node>
    clang::SourceRange getCorrectSourceRange(const Node& node)
    {
        auto spelling = getSpellingRange(node);
        auto sourceLength = getSourceLength(spelling, node);
        return SourceRange(spelling.getBegin(), spelling.getBegin().getLocWithOffset(sourceLength));
    }

    std::string getSource(SourceRange range)
    {
        return std::string(getText(range.getBegin()), rangeLength(range));
    }

private:
    clang::SourceManager& sourceManager;
    
    template <typename Node>
    clang::SourceRange getSpellingRange(const Node& n)
    {
        auto r = SourceRange(
            sourceManager.getSpellingLoc(n.getSourceRange().getBegin()),
            sourceManager.getSpellingLoc(n.getSourceRange().getEnd()));
        if (r.isInvalid())
            throw std::runtime_error("cannot get spelling range");
        return r;
    }

    template <typename Node>
    unsigned getSourceLength(clang::SourceRange spelling, const Node& node)
    {
        auto start = locOffset(spelling.getBegin());
        auto end = sourceManager.getDecomposedLoc(
            clang::Lexer::getLocForEndOfToken(spelling.getEnd(), 0, sourceManager, clang::LangOptions())).second;
        if (end < start)
            throw std::runtime_error("invalid decomposed range, probably because of macros");
        return end - start + extraCharsHack(node);
    }

    unsigned locOffset(SourceLocation loc)
    {
        return sourceManager.getFileOffset(loc);
    }
    
    unsigned locDistance(SourceLocation from, SourceLocation to)
    {
        return locOffset(to) - locOffset(from);
    }
    
    unsigned rangeLength(SourceRange r)
    {
        return locDistance(r.getBegin(), r.getEnd());
    }
    
    const char *getText(SourceLocation loc)
    {
        auto invalid = true;
        auto text = sourceManager.getCharacterData(loc, &invalid);
        if (invalid)
            throw std::runtime_error("cannot get characted data");
        return text;
    }
};

class MethodExtractor : public RecursiveASTVisitor<MethodExtractor>
{
    ASTContext& ctx;
    SourceExtractor sourceExtractor{ctx.getSourceManager()};
public:
    MethodExtractor(ASTContext& ctx) : ctx(ctx) { }
    bool VisitFunctionDecl(FunctionDecl* decl)
    {
        if (!ctx.getSourceManager().isFromMainFile(decl->getLocation()))
            return true;
        auto& stmt = findStatement(*decl);
        printExtractedFunction("runLoop", stmt);
        printOriginalFunctionWithExtractedFunctionCall("runLoop", *decl, stmt);
        return false;
    }
private:
    const clang::Stmt& findStatement(const FunctionDecl& func)
    {
        Stmt::child_iterator it = func.getBody()->child_begin();
        ++it;
        return **it;
    }
    void printExtractedFunction(const std::string& name, const Stmt& stmt)
    {
        output << "void " << name << "()\n{\n    " << sourceExtractor.getSource(sourceExtractor.getCorrectSourceRange(stmt)) << "}\n";
    }
    void printOriginalFunctionWithExtractedFunctionCall(const std::string& name, FunctionDecl& decl, const Stmt& stmt)
    {decl.getSourceRange();
        output << GetTextWithReplace(
            sourceExtractor.getCorrectSourceRange(decl),
            sourceExtractor.getCorrectSourceRange(stmt),
            name + "();\n");
    }
    std::string GetTextWithReplace(SourceRange range, SourceRange without, std::string replace)
    {
        return
            sourceExtractor.getSource(SourceRange(range.getBegin(), without.getBegin())) +
            replace +
            sourceExtractor.getSource(SourceRange(without.getEnd(), range.getEnd()));
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
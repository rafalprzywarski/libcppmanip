#ifndef PARSEDFUNCTION_HPP
#define PARSEDFUNCTION_HPP
#include "Condition.hpp"
#include <clang/AST/StmtIterator.h>
#include <clang/AST/ASTContext.h>
#include <clang/Basic/SourceManager.h>

class ParsedFunction : boost::noncopyable
{
public:
    ParsedFunction(const std::string& source);
    ~ParsedFunction();
    clang::StmtRange stmts();
    clang::ASTContext& getASTContext() { return *astContext; }
private:
    Condition parsed, canFinish;
    clang::StmtRange range;
    clang::ASTContext *astContext;
    boost::thread thread;
};

#endif // PARSEDFUNCTION_HPP

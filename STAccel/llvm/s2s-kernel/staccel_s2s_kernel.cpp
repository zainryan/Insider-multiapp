#include <sstream>
#include <string>
#include <set>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory STAccelCategory("ST-Accel s2s transformer for kernels");
std::string topFuncName;
bool catchTopFunc = false;

class RewritingVisitor : public RecursiveASTVisitor<RewritingVisitor> {
public:
  RewritingVisitor(Rewriter &R, std::string &_tbaText) :
    TheRewriter(R), tbaText(_tbaText) {}

  bool VisitFunctionDecl(FunctionDecl *f) {
    if (f -> hasBody()) {
      if (f -> getNameInfo().getAsString() == topFuncName) {
	Stmt *funcBody = f -> getBody();
	SourceLocation locStart = funcBody -> getBeginLoc();
	SourceLocation locEnd = funcBody -> getEndLoc();
	TheRewriter.RemoveText(SourceRange(locStart, locEnd));
	TheRewriter.InsertText(locStart , "{\n" + tbaText + "}", true, true);
      }
    }

    return true;
  }

private:
  Rewriter &TheRewriter;
  std::string &tbaText;
};

class InfoExtractionVisitor : public RecursiveASTVisitor<InfoExtractionVisitor> {
public:
  InfoExtractionVisitor(Rewriter &R) : TheRewriter(R) {}

  void updateFifoNameSet(FunctionDecl *f) {
    funcParamFifoNameSet.clear();
    for (auto param = f -> param_begin(); param != f -> param_end(); param ++) {
      funcParamFifoNameSet.insert((*param) -> getName());      
    }
  }

  bool VisitFunctionDecl(FunctionDecl *f) {
    if (f -> hasBody()) {
      if (f -> getNameInfo().getAsString() == topFuncName) {
	catchTopFunc = true;
	Stmt *body = f -> getBody();
	updateFifoNameSet(f);
	dfs(body);
      }
    }    
    return true;
  }

  std::string &getText() {
    return text;
  }

private:
  Rewriter &TheRewriter;
  std::string text;
  std::set<std::string> funcParamFifoNameSet;

  std::string toString(Stmt *stmt) {
    std::string string_buf;
    llvm::raw_string_ostream ros(string_buf);
    stmt -> printPretty(ros, nullptr, PrintingPolicy(LangOptions()));
    return ros.str();
  }

  std::string toString(Decl *decl) {
    std::string string_buf;
    llvm::raw_string_ostream ros(string_buf);
    decl -> print(ros);
    return ros.str();
  }

  void dfs(Stmt *root) {
    for (auto iter = root -> child_begin(); iter != root -> child_end(); iter ++) {
      Stmt *curStmt = *iter;

      if (curStmt) {
	if (!strcmp(curStmt -> getStmtClassName(), "CompoundStmt")) {
	  text += "{";
	}
	else if (!strcmp(curStmt -> getStmtClassName(), "ForStmt")) {
	  text += "{";
	}
	else if (!strcmp(curStmt -> getStmtClassName(), "DeclStmt")) {
	  DeclStmt *declStmt = (DeclStmt *)curStmt;
	  for (auto iter = declStmt -> decl_begin(); iter != declStmt -> decl_end(); iter ++) {
	    Decl *decl = *iter;
	    std::string str = toString(decl);
	    auto substrLen = str.find('=');
	    if (substrLen == std::string::npos) {
	      substrLen = str.size();
	    }
	    str = str.substr(0, substrLen) + ";\n";
	    text += str;
	  }
	}
	else if (!strcmp(curStmt -> getStmtClassName(), "CXXMemberCallExpr")) {
	  CXXMemberCallExpr *cxxMemberCallExpr = (CXXMemberCallExpr *)curStmt;
	  std::string callerStr = toString(cxxMemberCallExpr -> getImplicitObjectArgument());
	  if (funcParamFifoNameSet.find(callerStr) != funcParamFifoNameSet.end()) {
	    std::string str = toString(cxxMemberCallExpr) + ";\n";
	    text += str;
	  }
	}
	dfs(curStmt);
	if (!strcmp(curStmt -> getStmtClassName(), "CompoundStmt")) {
	  text += "}";
	}
	else if (!strcmp(curStmt -> getStmtClassName(), "ForStmt")) {
	  text += "}";
	}
      }
    }
  }
};

class MyASTConsumer : public ASTConsumer {
public:
  MyASTConsumer(Rewriter &R) : TheRewriter(R) {}

  bool HandleTopLevelDecl(DeclGroupRef DR) override {
    InfoExtractionVisitor infoExtractionVisitor(TheRewriter);
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
      infoExtractionVisitor.TraverseDecl(*b);
    }
    RewritingVisitor rewritingVisitor(TheRewriter, infoExtractionVisitor.getText());
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
      rewritingVisitor.TraverseDecl(*b);
    }
    return true;
  }
private:
  Rewriter &TheRewriter;
};

class MyFrontendAction : public ASTFrontendAction {
public:
  MyFrontendAction() {}
  void EndSourceFileAction() override {
    SourceManager &SM = TheRewriter.getSourceMgr();
    TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
						 StringRef file) override {
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return llvm::make_unique<MyASTConsumer>(TheRewriter);
  }

private:
  Rewriter TheRewriter;
};

std::string getKernelName(std::string sourceFileName) {
  int lPos = sourceFileName.rfind("/") + 1;
  int rPos = sourceFileName.find(".");
  return sourceFileName.substr(lPos, rPos - lPos);
}

int main(int argc, const char **argv) {
  topFuncName = getKernelName(std::string(argv[1]));
  CommonOptionsParser op(argc, argv, STAccelCategory);
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());
  int ret = Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
  if (!catchTopFunc) {
    llvm::errs() << "Error: In file " + std::string(argv[1]) + ": Cannot find the top function!\n";
    return -1;
  }
  return ret;
}

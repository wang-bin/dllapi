// mkapi: cpp code generator for dll api
// Wang Bin (wbsecg1@gmail.com)

// using Rewriter - the code rewriting interface.
//
// Eli Bendersky (eliben@gmail.com)
// This code is in the public domain
//
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Edit/Rewriters.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"


#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>

using namespace clang;
using namespace std;

struct func_info {
    string name;
    string return_type;
    vector<string> argv;
};

std::string trim(const std::string& str)
{
    string s(str);
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(" \n\t\r") + 1);
    return s;
}

//http://stackoverflow.com/questions/11083066/getting-the-source-behind-clangs-ast
// "T var"
/*
 * isTokenRange: if the end of this range specifies the start of the last token
 * isCharRange: if the end of this range specifies the last
*/
// TODO: ParmVarDecl::hasDefaultArg. now it's not included
std::string decl2str(clang::Decl *d, SourceManager *sm) {
    // (T, U) => "T,,"
    string text = Lexer::getSourceText(CharSourceRange::getTokenRange(d->getSourceRange()), *sm, LangOptions(), 0);
    if (text.at(text.size()-1) == ',')
        return Lexer::getSourceText(CharSourceRange::getCharRange(d->getSourceRange()), *sm, LangOptions(), 0);
    return text;
    const char* b = sm->getCharacterData(d->getLocStart());
    clang::SourceLocation _e(d->getLocEnd());
#if 1
    clang::SourceLocation e(clang::Lexer::getLocForEndOfToken(_e, 0, *sm, LangOptions()));
    return std::string(b, sm->getCharacterData(e)-b);
#else
    unsigned tokenLen = clang::Lexer::MeasureTokenLength(_e, *sm, LangOptions());
    return std::string(b, tokenLen);//sm->getCharacterData(e)-b);
#endif
}

// "T"
std::string decl2str_without_var(clang::Decl *d, SourceManager *sm) {
    return Lexer::getSourceText(CharSourceRange::getCharRange(d->getSourceRange()), *sm, LangOptions(), 0);
    const char* b = sm->getCharacterData(d->getLocStart());
    const char* e = sm->getCharacterData(d->getLocEnd());
    return std::string(b, e-b);
}

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MkApiASTVisitor : public RecursiveASTVisitor<MkApiASTVisitor>
{
public:
    MkApiASTVisitor(Rewriter &R)
        : TheRewriter(R)
    {}
#if 0
    bool VisitStmt(Stmt *s) {
        // Only care about If statements.
        if (isa<IfStmt>(s)) {
            IfStmt *IfStatement = cast<IfStmt>(s);
            Stmt *Then = IfStatement->getThen();
            TheRewriter.InsertText(Then->getLocStart(), "// the 'if' part\n", true, true);
            Stmt *Else = IfStatement->getElse();
            if (Else)
                TheRewriter.InsertText(Else->getLocStart(), "// the 'else' part\n", true, true);
        }
        return true;
    }
#endif
    bool VisitFunctionDecl(FunctionDecl *f) {
        // Only function definitions (with bodies), not declarations.
            Stmt *FuncBody = f->getBody();
            // Type name as string
            QualType QT = f->getResultType();
            string TypeStr = QT.getAsString();
            // Function name
            DeclarationName DeclName = f->getNameInfo().getName();
            string FuncName = DeclName.getAsString();
            func_info fi;
            fi.name = FuncName;
            fi.return_type = TypeStr;
            unsigned nbp = f->getNumParams();
            for (int i = 0; i < nbp; ++i) {
                //getTypeSourceInfo()->getType() and getOriginalType() is the final type. e.g. for size_t, the result may be unsigned int, but we need only size_t
                //fi.argv.push_back(f->getParamDecl(i)->getTypeSourceInfo()->getType().getAsString());//->getOriginalType().getAsString());
/*
                TypeLoc tl = f->getParamDecl(i)->getTypeSourceInfo()->getTypeLoc();
                SourceLocation sl0 = tl.getBeginLoc();
                SourceLocation sl1 = tl.getEndLoc();
                const char* ptr0 = TheRewriter.getSourceMgr().getCharacterData(sl0);
                const char* ptr1 = TheRewriter.getSourceMgr().getCharacterData(sl1);
*/
                fi.argv.push_back(trim(decl2str_without_var(f->getParamDecl(i), &TheRewriter.getSourceMgr())));
            }
            mFuncInfo.push_back(fi);
#if 0
            if (f->hasBody()) {
                // Add comment before
                stringstream SSBefore;
                SSBefore << "// Begin function " << FuncName << " returning "
                         << TypeStr << "\n";
                SourceLocation ST = f->getSourceRange().getBegin();
                TheRewriter.InsertText(ST, SSBefore.str(), true, true);

                // And after
                stringstream SSAfter;
                SSAfter << "\n// End function " << FuncName << "\n";
                ST = FuncBody->getLocEnd().getLocWithOffset(1);
                TheRewriter.InsertText(ST, SSAfter.str(), true, true);
            }
#endif
        return true;
    }
    std::vector<func_info> GetFuncInfo() const {
        return mFuncInfo;
    }
private:
    void AddBraces(Stmt *s);

    Rewriter &TheRewriter;
    std::vector<func_info> mFuncInfo;
};


// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MkApiASTConsumer : public ASTConsumer
{
public:
    MkApiASTConsumer(Rewriter &R)
        : Visitor(R)
    {}
    // Override the method that gets called for each parsed top-level
    // declaration.
    virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
        for (DeclGroupRef::iterator b = DR.begin(), e = DR.end();
             b != e; ++b)
            // Traverse the declaration using our AST visitor.
            Visitor.TraverseDecl(*b);
        return true;
    }
    std::vector<func_info> GetFuncInfo() const { return Visitor.GetFuncInfo(); }
private:
    MkApiASTVisitor Visitor;
};

void parse_with_tool(const std::vector<std::string> args);
int main(int argc, char *argv[])
{
#if 0
    vector<string> args;
    args.push_back("tool-executable");
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    parse_with_tool(args);
    return 0;
#endif
    string user_defines;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-' && argv[i][1] == 'D') {
            user_defines += "#define ";
            char *def = argv[i]+2;
            char* eq_pos = strchr(def, '=');
            if (eq_pos) {
                user_defines += string(def, eq_pos - def);
                user_defines += " ";
                user_defines += string(def+1);
            } else {
                user_defines += string(def);
            }
            user_defines += "\n";
        }
    }


    if (argc < 2) {
        llvm::errs() << "Usage: mkapi filename [-Ddefine1 -Ddefine2...]\n";
        return 1;
    }

    string file(argv[1]);

    // CompilerInstance will hold the instance of the Clang compiler for us,
    // managing the various objects needed to run the compiler.
    CompilerInstance TheCompInst;
    TheCompInst.createDiagnostics(0, true); //() for clang3.3, (0,0) for clang3.2

    // Initialize target info with the default triple for our platform.
    TargetOptions *TO = new TargetOptions(); //TODO: why on stack crash when dtor called
    TO->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *TI = TargetInfo::CreateTargetInfo(
        TheCompInst.getDiagnostics(), TO);
    TheCompInst.setTarget(TI);

    TheCompInst.createFileManager();
    FileManager &FileMgr = TheCompInst.getFileManager();
    TheCompInst.createSourceManager(FileMgr);
    SourceManager &SourceMgr = TheCompInst.getSourceManager();
    TheCompInst.createPreprocessor();
    TheCompInst.createASTContext();

    if (!user_defines.empty()) {
        clang::Preprocessor &PP = TheCompInst.getPreprocessor();
        PP.setPredefines(PP.getPredefines() + user_defines);
    }


    //cout << "predefines: " << TheCompInst.getPreprocessor().getPredefines() << endl;
    // A Rewriter helps us manage the code rewriting task.
    Rewriter TheRewriter;
    TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

    // Set the main file handled by the source manager to the input file.
    const FileEntry *FileIn = FileMgr.getFile(file);
    SourceMgr.createMainFileID(FileIn);
    TheCompInst.getDiagnosticClient().BeginSourceFile(
        TheCompInst.getLangOpts(),
        &TheCompInst.getPreprocessor());

    // Create an AST consumer instance which is going to get called by
    // ParseAST.
    MkApiASTConsumer TheConsumer(TheRewriter);

    // Parse the file to AST, registering our consumer as the AST consumer.
    ParseAST(TheCompInst.getPreprocessor(), &TheConsumer,
             TheCompInst.getASTContext()
             , clang::TU_Complete);

    // At this point the rewriter's buffer should be full with the rewritten
    // file contents.
#if 0
    const RewriteBuffer *RewriteBuf =
        TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
    llvm::outs() << string(RewriteBuf->begin(), RewriteBuf->end());
#endif

    std::vector<func_info> fi = TheConsumer.GetFuncInfo();
    stringstream stream;
    for (std::vector<func_info>::const_iterator it = fi.begin(); it != fi.end(); ++it) {
        std::vector<std::string> params = (*it).argv;
        stream << "DEFINE_DLLAPI_ARG(" << params.size() << ", " << (*it).return_type << ", " << (*it).name;
        for (int i = 0; i < params.size(); ++i) {
            stream << ", " << params[i];
        }
        stream << ")" << endl;
    }
    
    cout << stream.str() << endl;
    return 0;
}


// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MkApiASTConsumer2 : public ASTConsumer
{
public:
    MkApiASTConsumer2(clang::CompilerInstance &CI, llvm::StringRef InFile)
        : mpVisitor(0)
    {
        CI.setASTConsumer(this);
        //cout << "predefines: " << CI.getPreprocessor().getPredefines() << endl;
        // A Rewriter helps us manage the code rewriting task.
        mRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        mpVisitor = new MkApiASTVisitor(mRewriter);
    }
    ~MkApiASTConsumer2() {
        delete mpVisitor;
    }

    // Override the method that gets called for each parsed top-level
    // declaration.
    virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
        for (DeclGroupRef::iterator b = DR.begin(), e = DR.end();
             b != e; ++b)
            // Traverse the declaration using our AST visitor.
            mpVisitor->TraverseDecl(*b);
        return true;
    }
    std::vector<func_info> GetFuncInfo() const { return mpVisitor->GetFuncInfo(); }
private:
    Rewriter mRewriter;
    MkApiASTVisitor *mpVisitor;
};

class MkApiAction : public clang::ASTFrontendAction {
protected:
    virtual clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
        CI.getFrontendOpts().SkipFunctionBodies = true;
        CI.getPreprocessor().enableIncrementalProcessing(true);
        CI.getPreprocessor().SetSuppressIncludeNotFoundError(true);
        CI.getLangOpts().DelayedTemplateParsing = true;

        //enable all the extension
        CI.getLangOpts().MicrosoftExt = true;
        CI.getLangOpts().DollarIdents = true;
#if CLANG_VERSION_MAJOR != 3 || CLANG_VERSION_MINOR > 2
        CI.getLangOpts().CPlusPlus11 = true;
#else
        CI.getLangOpts().CPlusPlus0x = true;
#endif
        CI.getLangOpts().CPlusPlus1y = true;
        CI.getLangOpts().GNUMode = true;

        /* Proxy that changes some errors into warnings  */
        //CI.getDiagnostics().setClient(new MocDiagConsumer{CI.getDiagnostics().takeClient()});

        return new MkApiASTConsumer2(CI, InFile);
    }

public:
    // CHECK
    virtual bool hasCodeCompletionSupport() const { return true; }
};


//https://chromium.googlesource.com/native_client/pnacl-clang/+/master/unittests/Tooling/ToolingTest.cpp
void parse_with_tool(const std::vector<std::string> args)
{
    llvm::IntrusiveRefCntPtr<clang::FileManager> Files(new clang::FileManager(clang::FileSystemOptions()));
    clang::ASTFrontendAction *action = new MkApiAction;
    clang::tooling::ToolInvocation Invocation(args, action, Files.getPtr());

    Invocation.run();


    ASTConsumer ac = action->getCompilerInstance().getASTConsumer();
    std::vector<func_info> fi = static_cast<MkApiASTConsumer2*>(&ac)->GetFuncInfo();
    //stringstream stream;
    for (std::vector<func_info>::const_iterator it = fi.begin(); it != fi.end(); ++it) {
        std::vector<std::string> params = (*it).argv;
        cout << "DEFINE_DLLAPI_ARG(" << params.size() << ", " << (*it).return_type << ", " << (*it).name;
        for (int i = 0; i < params.size(); ++i) {
            cout << ", " << params[i];
        }
        cout << ")" << endl;
    }
    cout << "parse_with_tool end" <<endl;

    //cout << stream.str() << endl;
}

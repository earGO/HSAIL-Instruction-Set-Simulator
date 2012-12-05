#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Extension_Test: public BrigCodeGenTest {

private:
  const BrigDirectiveExtension* RefExt;
  // Operands in .operands buffer

public:
  Extension_Test(std::string& in):
    BrigCodeGenTest(in) {}

  Extension_Test(std::string& in, StringBuffer* sbuf, BrigDirectiveExtension* ref):
    BrigCodeGenTest(in, sbuf),
    RefExt(ref) { }

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* dir = new Buffer();

    dir->append(RefExt);
    
    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      reinterpret_cast<const char *>(&dir->get()[0]), NULL, NULL, NULL, 
      RefStr->size(), dir->size(), 0, 0, 0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete dir;
  } 
  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError);
  }
};


TEST(CodegenTest, Extension_CodeGen) {  
  
  /********************************** Common variables used by all tests******************************/
    
  std::string in;
  BrigDirectiveExtension out;
  StringBuffer* buf = new StringBuffer;
  buf->clear();
   
  /************************************* Test Case 1 ************************************/
  in.assign("extension \"\\device\\amd.hsa\";\n");
  buf->append("\\device\\amd.hsa");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveExtension;
  out.c_code = 0;
  out.s_name = 0;

  Extension_Test TestCase1(in, buf, &out);
  TestCase1.Run_Test(&Extension);
  
  delete buf;
}
TEST(ErrorReportTest, Extension) {  
  std::string input = "extension \"\\device\\amd.hsa\"\n";
  Extension_Test TestCase1(input);
  TestCase1.Run_Test(&Extension, MISSING_SEMICOLON);
  
  input.assign("extension ;\n");
  Extension_Test TestCase2(input);
  TestCase2.Run_Test(&Extension, MISSING_STRING);
  
}
} // namespace hsa
} // namespace brig





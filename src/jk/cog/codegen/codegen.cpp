#include "codegen.hpp"
#include "statement_gen_visitor.hpp"
#include "jk/cog/ir/ir_printer.hpp"

void gorc::cog::perform_code_generation(script &out_script,
                                        ast::translation_unit &tu,
                                        verb_table const &verbs,
                                        constant_table const &constants)
{
    memory_file::writer text_writer(out_script.program);
    ir_printer ir(text_writer,
                  out_script.exports);

    statement_gen_visitor sgv(out_script,
                              ir,
                              verbs,
                              constants);
    ast_visit(sgv, tu.code->code);

    ir.finalize();

    return;
}

#ifndef REGEXES_H
#define REGEXES_H

#include <regex>
#include <string>

using namespace std;

regex comment("([^#]*)#.*");

string white_spaces = " {2,}";
string tab = "\\t";
regex swap_with_single_space(white_spaces + "|" + tab);

regex comma_space(", ");
regex colon_space(": ");
regex space_plus(" \\+");
regex plus_space("\\+ ");

string symbol="[a-zA-Z][a-zA-Z0-9_]*";
string section_name="[a-zA-Z][a-zA-Z0-9_]*";
string literal_dec="[+-]?[0-9]+";
string literal_hex="[+-]?0x[0-9a-fA-F]+";
string literal="("+literal_dec+"|"+literal_hex+")";

regex r_symbol(symbol);
regex r_literal_dec(literal_dec);
regex r_literal_hex(literal_hex);

regex directive_global("^\.global ("+symbol+",)*"+symbol+"$");
regex directive_extern("^\.extern ("+symbol+",)*"+symbol+"$");
regex directive_section("^\.section "+section_name+"$");
regex directive_word("^\.word (("+symbol+"|"+literal+"),)*("+symbol+"|"+literal+")$");
//^\.word ((([a-zA-Z][a-zA-Z0-9_]*)|([0-9]+)|(0x[0-9]+)),)*(([a-zA-Z][a-zA-Z0-9_]*)|([0-9]+)|(0x[0-9]+))$
regex directive_skip("^\.skip "+literal+"$");
regex directive_ascii("^\.ascii \".*\"$");
regex directive_end("^\.end$");

string reg="(r[0-7]|psw|pc|sp)";
string jump="(jmp|jeq|jne|jgt|call)";
string ldst="(ldr|str)";

regex instruction_halt("^halt$");
regex instruction_int("^int "+reg+"$");
regex instruction_iret("^iret$");
regex instruction_ret("^ret$");
regex instruction_push("^push "+reg+"$");
regex instruction_pop("^pop "+reg+"$");
regex instruction_not("^not "+reg+"$");

regex instruction_xchg("^xchg "+reg+","+reg+"$");
regex instruction_add("^add "+reg+","+reg+"$");
regex instruction_sub("^sub "+reg+","+reg+"$");
regex instruction_mul("^mul "+reg+","+reg+"$");
regex instruction_div("^div "+reg+","+reg+"$");
regex instruction_cmp("^cmp "+reg+","+reg+"$");
regex instruction_or("^or "+reg+","+reg+"$");
regex instruction_xor("^xor "+reg+","+reg+"$");
regex instruction_test("^test "+reg+","+reg+"$");
regex instruction_shl("^shl "+reg+","+reg+"$");
regex instruction_shr("^shr "+reg+","+reg+"$");
regex instruction_and("^xchg "+reg+","+reg+"$");

regex instruction_jump_lit_value("^"+jump+" "+literal+"$");
regex instruction_jump_sym_value("^"+jump+" "+symbol+"$");
regex instruction_jump_symbol_pcrel("^"+jump+" %"+symbol+"$");
regex instruction_jump_lit_mem("^"+jump+" \\*"+literal+"$");
regex instruction_jump_sym_mem("^"+jump+" \\*"+symbol+"$");
regex instruction_jump_reg_value("^"+jump+" \\*"+reg+"$");
regex instruction_jump_reg_ind("^"+jump+" \\*\\["+reg+"\\]$");
regex instruction_jump_reg_lit_ind("^"+jump+" \\*\\["+reg+"\\+"+literal+"\\]$");
regex instruction_jump_reg_sym_ind("^"+jump+" \\*\\["+reg+"\\+"+symbol+"\\]$");


regex instruction_ldst_lit_value("^"+ldst+" "+reg+",\\$"+literal+"$");
regex instruction_ldst_sym_value("^"+ldst+" "+reg+",\\$"+symbol+"$");
regex instruction_ldst_lit_mem("^"+ldst+" "+reg+","+literal+"$");
regex instruction_ldst_sym_mem("^"+ldst+" "+reg+","+symbol+"$");
regex instruction_ldst_symbol_pcrel("^"+ldst+" "+reg+",%"+symbol+"$");
regex instruction_ldst_reg_value("^"+ldst+" "+reg+","+reg+"$");
regex instruction_ldst_reg_ind("^"+ldst+" "+reg+",\\["+reg+"\\]$");
regex instruction_ldst_reg_lit_ind("^"+ldst+" "+reg+",\\["+reg+"\\+"+literal+"\\]$");
regex instruction_ldst_reg_sym_ind("^"+ldst+" "+reg+",\\["+reg+"\\+"+symbol+"\\]$");

regex label_in_line("^"+symbol+":.*");

#endif
#include "lexer.hpp"
#include "log/log.hpp"

using namespace gorc;
using tok_result = gorc::tokenizer_state_machine_result;

tok_result shell_tokenizer_state_machine::handle_initial_state(char ch)
{
    if(ch == '\0') {
        seen_whitespace = true;
        return accept_immediately(shell_token_type::end_of_file);
    }
    else if(std::isspace(ch)) {
        seen_whitespace = true;
        return discard_directive(tokenizer_state::initial);
    }
    else if(ch == '#') {
        seen_whitespace = true;
        return discard_directive(tokenizer_state::skip_line_comment);
    }
    else if(ch == '|') {
        seen_whitespace = true;
        return append_then_accept(ch, shell_token_type::punc_pipe);
    }
    else if(ch == ';') {
        seen_whitespace = true;
        return append_then_accept(ch, shell_token_type::punc_end_command);
    }
    else if(ch == '\"') {
        return skip_directive(tokenizer_state::string);
    }
    else {
        return append_directive(tokenizer_state::bareword, ch);
    }
}

tok_result shell_tokenizer_state_machine::handle_skip_line_comment_state(char ch)
{
    if(ch == '\n' || ch == '\0') {
        return discard_directive(tokenizer_state::initial);
    }
    else {
        return discard_directive(tokenizer_state::skip_line_comment);
    }
}

tok_result shell_tokenizer_state_machine::handle_bareword_state(char ch)
{
    if(ch == '\0' ||
       ch == '#' ||
       ch == '|' ||
       ch == ';' ||
       ch == '\"' ||
       std::isspace(ch)) {
        shell_token_type rtype = seen_whitespace ?
                                 shell_token_type::first_word :
                                 shell_token_type::successor_word;
        seen_whitespace = false;
        return accept_immediately(rtype);
    }
    else {
        return append_directive(tokenizer_state::bareword, ch);
    }
}

tok_result shell_tokenizer_state_machine::handle_string_state(char ch)
{
    if(ch == '\\') {
        return skip_directive(tokenizer_state::escape_sequence);
    }
    else if(ch == '\0') {
        return reject_immediately("unexpected eof in string literal");
    }
    else if(ch == '\n') {
        return reject_immediately("unescaped newline in string literal");
    }
    else if(ch == '\"') {
        shell_token_type rtype = seen_whitespace ?
                                 shell_token_type::first_word :
                                 shell_token_type::successor_word;
        seen_whitespace = false;
        return skip_then_accept(rtype);
    }
    else {
        return append_directive(tokenizer_state::string, ch);
    }
}

tok_result shell_tokenizer_state_machine::handle_escape_sequence_state(char ch)
{
    char append_char = '\0';

    switch(ch) {
    case '\0':
        return reject_immediately("unexpected eof in string literal escape sequence");

    case '\"':
    case '\\':
        append_char = ch;
        break;

    case 'n':
        append_char = '\n';
        break;

    case '\n':
        // Consume escaped newlines
        return skip_directive(tokenizer_state::string);

    default:
        return reject_immediately(str(format("unknown escape sequence '\\%c'") % ch));
    };

    return append_directive(tokenizer_state::string, append_char);
}

tok_result shell_tokenizer_state_machine::handle(char ch)
{
    switch(current_state) {
    case tokenizer_state::accept:
        current_state = tokenizer_state::initial;
        append_buffer.clear();
        return tok_result(tokenizer_state_machine_result_type::halt, append_buffer);

    case tokenizer_state::initial:
        return handle_initial_state(ch);

    case tokenizer_state::skip_line_comment:
        return handle_skip_line_comment_state(ch);

    case tokenizer_state::bareword:
        return handle_bareword_state(ch);

    case tokenizer_state::string:
        return handle_string_state(ch);

    case tokenizer_state::escape_sequence:
        return handle_escape_sequence_state(ch);

// LCOV_EXCL_START
    }

    LOG_FATAL("unhandled shell tokenizer state");
}
// LCOV_EXCL_STOP

shell_token_type shell_tokenizer_state_machine::get_type() const
{
    return current_type;
}

std::string const & shell_tokenizer_state_machine::get_reason() const
{
    return reason;
}

bool shell_tokenizer_state_machine::is_fatal_error() const
{
    return current_type == shell_token_type::error;
}
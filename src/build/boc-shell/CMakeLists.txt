add_executable(boc-shell
    argument_visitor.cpp
    assignment_visitor.cpp
    ast.cpp
    builtins.cpp
    command_visitor.cpp
    expression_visitor.cpp
    io_redirection_visitor.cpp
    lexer.cpp
    main.cpp
    parser.cpp
    program_visitor.cpp
    stack.cpp
    symbols.cpp
    symbol_visitor.cpp
    value.cpp
    word_visitor.cpp
    )

target_link_libraries(boc-shell
    ast
    program
    system
    text
    )

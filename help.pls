% Program to test lexer 

_foo1 input parameter list [real c4d3, int c3b2] 
    output parameter list [int b2c3, int c3d2];
    type real : c4 : global; c3 <--- 4 ;
    b2c3 <--- c4d3 + c3b2 - c3;
    c3d2 <--- c3 * c3;
    return [b2c3, c3d2];
end

% Can you see me?

_main
    type int : c2d3;
    type int : c3d2;
    type real : c3b4;
    type int : c3b5;
    read(c3b4);
    read(c3b5);
    [c2d3, c3d2] <--- call _foo1 with parameter list [c3b4, c3b5];
    write(c3b4);
    write(c3b5);
_end
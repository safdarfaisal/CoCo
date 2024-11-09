% Program to test lexer 

_foo1 input parameter list [real c4d3, int c3b2] 
    output parameter list [int b2c3, int c3d2];
    type real : c3 : global; c3 <--- 4 ;
    b2c3 <--- c4d3 + c3b2 - c3;
    c3d2 <--- c3 * c3;
    return [b2c3, c3d2];
end

% Can you see me?

_main
    %record #weirdo
    %    type int : foo;
    %    type int : bar;
    %endrecord;
    type record #weirdo : b3c4;

    type int : c3d2;
    type real : c3b4;
    type int : c3b5;
    
    type int : c2b3;
    b3c4.foo <--- 47;
    b3c4.bar <--- 78;
    
    read(c3b4);
    read(c3b5);
    
    c2b3 <--- b3c4.foo + b3c4.bar;
    
    [c2d3, c3d2] <--- call _foo1 with parameter list [c3b4, c3b5];
    write(c3b4);
    write(c2d3);
end s2a
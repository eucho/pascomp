program test(input,output);
var 
    x,y:integer;
    c:array[1..5] of real;
    d:record a:integer end;
    function gtcomdiv(a,b:integer):integer;
    begin
      if b=0 then gtcomdiv:=a
      else gtcomdiv:=gtcomdiv(b,a mod b)
    end;
begin
  read(x,y);
  write(gtcomdiv(x,y))
end.
program test(input);
var x, y:integer
begin
	x := 1
	y := 2
end.

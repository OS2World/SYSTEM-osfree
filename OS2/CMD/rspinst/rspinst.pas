program RSPInst;

uses
  crt,
  tpcrt,
  handlers;

Procedure InitDesktop;
begin
  TextBackground(Blue);
  TextColor(Cyan);
  HighVideo;
  ClrScr;
  WriteLn;
  Writeln(' osFree Installation');
  Writeln(' '#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205#205);
end;

begin
  InitDesktop;
end.

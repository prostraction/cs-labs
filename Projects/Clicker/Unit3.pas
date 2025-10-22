unit Unit3;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls;

type
  TForm3 = class(TForm)
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    Timer1: TTimer;
    Timer2: TTimer;
    Timer3: TTimer;
    Button1: TButton;
    Button2: TButton;
    CheckBox1: TCheckBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    procedure Timer1Timer(Sender: TObject);
    procedure Timer2Timer(Sender: TObject);
    procedure Timer3Timer(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure CheckBox1Click(Sender: TObject);
    procedure FormActivate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form3: TForm3;

implementation

{$R *.dfm}

procedure TForm3.Button1Click(Sender: TObject);
begin
if Edit3.Text='' then
showmessage('Не заполнено поле "Задержка"!')
else begin
Timer3.Interval:=StrToInt(Edit3.Text);
Timer3.Enabled:=True;
button1.Enabled:=False;
button2.Enabled:=True;
end;
end;

procedure TForm3.Button2Click(Sender: TObject);
begin
Timer3.Enabled:=False;
Timer1.Enabled:=True;
Timer2.Enabled:=True;
button2.Enabled:=False;
button1.Enabled:=True;
end;

procedure TForm3.CheckBox1Click(Sender: TObject);
begin
SetWindowPos(Handle, HWND_TOPMOST, Left, Top, Width, Height,
SWP_NOACTIVATE or SWP_NOMOVE or SWP_NOSIZE );
end;

procedure TForm3.FormActivate(Sender: TObject);
begin
button2.click;
end;

procedure TForm3.Timer1Timer(Sender: TObject);
var
p: TPoint;
begin
GetCursorPos(p);
Edit1.Text:= IntToStr(p.X);
Edit2.Text:= IntToStr(p.Y);
end;

procedure TForm3.Timer2Timer(Sender: TObject);
begin
if getasynckeystate($24)<>0 then begin //Если нажат Home, то ....
button1.Click
end;
end;
procedure TForm3.Timer3Timer(Sender: TObject);
var
position: TPoint;
begin
if getasynckeystate($23)<>0 then begin // Если нажат End, то перестать кликать
Timer1.Enabled:=True;
Timer2.Enabled:=True;
Timer3.Enabled:=False;
button1.Enabled:=True;
button2.Enabled:=False;
end
else begin
SetCursorPos(StrToInt(Edit1.text),StrToInt(Edit2.text));
mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
end;
end;
end.

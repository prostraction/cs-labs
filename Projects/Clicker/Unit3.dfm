object Form3: TForm3
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Clicker v1.4'
  ClientHeight = 137
  ClientWidth = 279
  Color = clBtnText
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesktopCenter
  StyleElements = [seFont, seClient]
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 135
    Top = 9
    Width = 72
    Height = 16
    Caption = 'X coordinate'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 135
    Top = 36
    Width = 71
    Height = 16
    Caption = 'Y coordinate'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 135
    Top = 63
    Width = 128
    Height = 16
    Caption = 'Latency (1000 - 1 sec)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 27
    Top = 120
    Width = 12
    Height = 16
    Caption = 'or'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 167
    Top = 120
    Width = 12
    Height = 16
    Caption = 'or'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 45
    Top = 120
    Width = 68
    Height = 16
    Caption = 'press Home'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 185
    Top = 120
    Width = 56
    Height = 16
    Caption = 'press End'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Edit1: TEdit
    Left = 8
    Top = 8
    Width = 121
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'Error'
  end
  object Edit2: TEdit
    Left = 8
    Top = 35
    Width = 121
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = 'Error'
  end
  object Edit3: TEdit
    Left = 8
    Top = 62
    Width = 121
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = '1000'
  end
  object Button1: TButton
    Left = 8
    Top = 89
    Width = 125
    Height = 25
    Caption = 'Start'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 146
    Top = 89
    Width = 125
    Height = 25
    Caption = 'Stop'
    TabOrder = 4
    OnClick = Button2Click
  end
  object CheckBox1: TCheckBox
    Left = 8
    Top = 488
    Width = 97
    Height = 17
    Caption = 'CheckBox1'
    TabOrder = 5
    Visible = False
    OnClick = CheckBox1Click
  end
  object Timer1: TTimer
    Interval = 1
    OnTimer = Timer1Timer
    Left = 184
    Top = 456
  end
  object Timer2: TTimer
    OnTimer = Timer2Timer
    Left = 216
    Top = 456
  end
  object Timer3: TTimer
    OnTimer = Timer3Timer
    Left = 248
    Top = 456
  end
end

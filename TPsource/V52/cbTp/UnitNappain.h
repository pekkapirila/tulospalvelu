// Pekka Pirila's sports timekeeping program (Finnish: tulospalveluohjelma)
// Copyright (C) 2015 Pekka Pirila 

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------

#ifndef UnitNappainH
#define UnitNappainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormNappain : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit1;
	TButton *Button1;
	TLabel *Label1;
	TEdit *Edit2;
	TEdit *Edit3;
	TLabel *Label2;
	TButton *Button2;
	TComboBox *CBPiste;
	TLabel *Label3;
	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CBPisteClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormNappain(TComponent* Owner);

	void __fastcall Initiate(int KeyCodeIn, TShiftState KeyShiftIn, int MaxVaIn, int PisteIn);
	void __fastcall GetValues(int &KeyCodeOut, TShiftState &KeyShiftOut, int &PisteOut);
	int KeyCode;
	TShiftState KeyShift;
	int MaxVa;
	int Piste;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormNappain *FormNappain;
//---------------------------------------------------------------------------
#endif

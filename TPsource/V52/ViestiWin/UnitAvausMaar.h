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

#ifndef UnitAvausMaarH
#define UnitAvausMaarH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormAvausMaar : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RadioGroup1;
	TOpenDialog *OpenDialog1;
	TEdit *Edit1;
	TButton *Button1;
	TLabel *Label1;
	TButton *Button2;
	TButton *Button3;
	TMemo *Memo1;
	TButton *Button4;
	TSaveDialog *SaveDialog1;
	TBitBtn *BitBtn1;
	TCheckBox *CBKuntoSuunn;
	TCheckBox *CBLuettValinta;
	TButton *Button5;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormAvausMaar(TComponent* Owner);
	void __fastcall LueCfgFile(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAvausMaar *FormAvausMaar;
//---------------------------------------------------------------------------
#endif

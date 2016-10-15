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

#ifndef UnitYhdistettyH
#define UnitYhdistettyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <HkDeclare.h>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormYhdistetty : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdKerroin;
	TLabel *Label1;
	TRadioGroup *RGLajiJarj;
	TComboBox *CBSarjat;
	TLabel *Label4;
	TButton *Button1;
	TButton *Button2;
	TBitBtn *BitBtn1;
	TCheckBox *CBMerkPois;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall EdKerroinExit(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
	int kerroin;
public:		// User declarations
	__fastcall TFormYhdistetty(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormYhdistetty *FormYhdistetty;
//---------------------------------------------------------------------------
#endif

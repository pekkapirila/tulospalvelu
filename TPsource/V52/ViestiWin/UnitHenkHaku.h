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

#ifndef UnitHenkHakuH
#define UnitHenkHakuH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "VDeclare.h"
//---------------------------------------------------------------------------
typedef struct {
	char animi[OSNIMIL+2];
	wchar_t	nimi[OSNIMIL+2];
	int sarja;
	int kno;
	int os;
	wchar_t joukkue[LSEURA+5];
} nimettp;
//---------------------------------------------------------------------------
class TFormHenkHaku : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGJarj;
	TComboBox *CBOsuudet;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *Edit1;
	TStringGrid *SG1;
	TButton *Button1;
	TButton *Button2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CBOsuudetChange(TObject *Sender);
	void __fastcall RGJarjClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall SG1DblClick(TObject *Sender);
private:	// User declarations
	void __fastcall NaytaNimet(void);
public:		// User declarations
	__fastcall TFormHenkHaku(TComponent* Owner);
	int KilpNo;
	int Osuus;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormHenkHaku *FormHenkHaku;
//---------------------------------------------------------------------------
#endif

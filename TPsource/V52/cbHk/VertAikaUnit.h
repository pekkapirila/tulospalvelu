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

#ifndef VertAikaUnitH
#define VertAikaUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TVertAikaFrm : public TForm
{
__published:	// IDE-managed Components
   TButton *OKBtn;
   TButton *PeruutaBtn;
   TGroupBox *GroupBox1;
   TLabel *Label6;
   TEdit *EdMaxLkm;
   TLabel *Label7;
   TLabel *Label8;
   TEdit *EdMaxPros;
   TLabel *Label9;
   TLabel *Label10;
   TCheckBox *TlsLuetVal;
	TCheckBox *CBAnal;
   void __fastcall OKBtnClick(TObject *Sender);
   void __fastcall PeruutaBtnClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CBAnalClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TVertAikaFrm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TVertAikaFrm *VertAikaFrm;
//---------------------------------------------------------------------------
#endif

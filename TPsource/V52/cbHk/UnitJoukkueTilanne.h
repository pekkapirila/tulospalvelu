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

#ifndef UnitJoukkueTilanneH
#define UnitJoukkueTilanneH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include "HkDeclare.h"
//---------------------------------------------------------------------------
class TFormJoukkueTilanne : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TButton *Button1;
	TButton *Button2;
	TMainMenu *MainMenu1;
	TMenuItem *oiminnot1;
	TMenuItem *Valinnat1;
	TMenuItem *Help1;
	TMenuItem *Sulje1;
	TMenuItem *Pivit1;
	TMenuItem *ulostenluonne1;
	TMenuItem *Yhteisaika1;
	TMenuItem *Kilpailijoidenpisteist1;
	TMenuItem *Sijalukusumma1;
	TMenuItem *Uudetpisteetsijoituksista1;
	TMenuItem *Joukkueentunnistavatieto1;
	TMenuItem *Seura1;
	TMenuItem *Seuralyhenne1;
	TMenuItem *Joukkuekentt1;
	TMenuItem *Joukkueenkoko1;
	TComboBox *CBSarja;
	TLabel *Label1;
	TMenuItem *Yhdistys1;
	TMenuItem *Piiri1;
	TMenuItem *Voittajallelispiste1;
	TMenuItem *Sarjankokorajoittaavoittajanpisteit1;
	TMenuItem *Joukkuetietovaaditaan1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Yhteisaika1Click(TObject *Sender);
	void __fastcall Kilpailijoidenpisteist1Click(TObject *Sender);
	void __fastcall Sijalukusumma1Click(TObject *Sender);
	void __fastcall Uudetpisteetsijoituksista1Click(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall Pivit1Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall Seura1Click(TObject *Sender);
	void __fastcall Seuralyhenne1Click(TObject *Sender);
	void __fastcall Joukkuekentt1Click(TObject *Sender);
	void __fastcall Joukkueenkoko1Click(TObject *Sender);
	void __fastcall Yhdistys1Click(TObject *Sender);
	void __fastcall Piiri1Click(TObject *Sender);
	void __fastcall Voittajallelispiste1Click(TObject *Sender);
	void __fastcall Sarjankokorajoittaavoittajanpisteit1Click(TObject *Sender);
	void __fastcall Joukkuetietovaaditaan1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	tulostusparamtp tulparam;
	int  tlsLaji;
	wchar_t  tunnTieto;
	int  joukkKoko;
	int  viimSj;
	int  minKoko;
public:		// User declarations
	__fastcall TFormJoukkueTilanne(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormJoukkueTilanne *FormJoukkueTilanne;
//---------------------------------------------------------------------------
#endif

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

#include <vcl.h>
#pragma hdrstop
#include <process.h>
#include <bstrings.h>

#include "UnitIlm2.h"
//#include "UnitKilpailijaOnline.h"
#include "UnitTKanta.h"
#include "UnitSeurat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormIlm2 *FormIlm2;
extern TFormTKanta *FormTKanta;
extern TFormSeurat *FormSeurat;
static UusiEmit = false;
int vakbiblisa = 6000;
//---------------------------------------------------------------------------

ColTp IlmGridCols0[] = {
	{L"KilpNo", 40, 0, 1, 0, true},
	{L"Sarja", 60, 0, 1, 0, true},
	{L"Id-no", 40, 0, 1, 0, true},
	{L"Sukunimi", 80, 0, 1, 0, true},
	{L"Etunimi", 80, 0, 1, 0, true},
	{L"Arvo", 50, 0, 1, 0, false},
	{L"Seura", 100, 0, 1, 0, true},
	{L"Seuralyh", 50, 0, 1, 0, true},
	{L"Yhdistys", 50, 0, 1, 0, false},
	{L"Joukkue", 50, 0, 1, 0, false},
	{L"Maa", 30, 0, 1, 0, true},
	{L"Piiri", 40, 0, 1, 0, false},
	{L"WRkoodi", 50, 0, 1, 0, false},
	{L"Ilm.lista", 50, 0, 1, 0, false},
	{L"Arv.ryhmä", 50, 0, 1, 0, false},
	{L"RintaNo", 50, 0, N_PV, 0, false},
	{L"Badge", 45, 0, N_PV, 0, true},
	{L"Laina", 30, 0, N_PV, 0, false},
	{L"Tas.", 40, 0, N_PV, 0, false},
	{L"Läsnä", 35, 0, N_PV, 0, true},
	{L"Lähtö", 50, 0, N_PV, 0, true},
	{L"Tulos", 50, 0, N_PV, 0, false},
	{L"A-sakot", 50, 0, N_PV, 0, false},
	{L"Teksti", 80, 0, N_PV, 0, false}
};

ColTp IlmGridCols[sizeof(IlmGridCols0)/sizeof(ColTp)];

int EPos[21][3] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}, {4, 4, 4}, {5, 5, 5},
	{6, 6, 6}, {7, 7, 7}, {8, 8, 8}, {9, 9, 9}, {10, 10, 10}, {11, 11, 11}, {12, 12, 12},
	{13, 13, 13}, {14, 14, 14}, {15, 15, 15}, {16, 16, 16}, {17, 17, 21}, {18, 18, 26},
	{19, 19, 31}, {20, 20, 36}, {21, 21, 41}};

void __fastcall TFormIlm2::OlEdits(void)
{
	Edits[0].Set(L"Kilpailijanumero", EId, LKilpno, OId, CBId, AktId, 0, 1);
	Edits[1].Set(L"Id-koodi (lisenssino)", ELisno, LLisno, OLisno, CBLisno, &EKilp.lisno[0], 0, 1);
	Edits[2].Set(L"Sukunimi", ESNimi, LSNimi, OSNimi, CBSNimi, EKilp.sukunimi, 1, 1);
	Edits[3].Set(L"Etunimi", EENimi, LENimi, OENimi, CBENimi, EKilp.etunimi, 1, 1);
	Edits[4].Set(L"Sarja", NULL, LSarja, OSarja, CBSarja, NULL, 0, 1);
	Edits[5].Set(L"Sukup", ESukup, LSukup, OSukup, CBSukup, &EKilp.sukup, 0, 1);
	Edits[6].Set(L"Ikä", EIkasrj, LIkasrj, OIkasrj, CBIkasrj, NULL, 0, 1);
	Edits[7].Set(L"Kv. koodi", EWRKoodi, LWRKoodi, OWRKoodi, CBWRKoodi, &EKilp.lisno[1], 0, 1);
	Edits[8].Set(L"Arvo", EArvo, LArvo, OArvo, CBArvo, EKilp.arvo, 1, 1);
	Edits[9].Set(L"Seura", ESeura, LSeura, OSeura, CBSeura, EKilp.seura, 1, 1);
	Edits[10].Set(L"Seuralyhenne", ESeuralyh, LSeuralyh, OSeuralyh, CBSeuralyh, EKilp.seuralyh, 1, 1);
	Edits[11].Set(L"Maa", EMaa, LMaa, OMaa, CBMaa, EKilp.maa, 1, 1);
	Edits[12].Set(L"Yhdistys", EYhdistys, LYhdistys, OYhdistys, CBYhdistys, EKilp.yhdistys, 1, 1);
	Edits[13].Set(L"Joukkue", EJoukkue, LJoukkue, OJoukkue, CBJoukkue, EKilp.joukkue, 1, 1);
	Edits[14].Set(L"Läsnäolopäivät", ELasna, LLasna, OLasna, CBLasna, NULL, 1, 1);
	Edits[15].Set(L"Tunnistinkoodi", (TEdit *)EBadge, LBadge, OBadge, CBBadge, NULL, 0, 5);
	Edits[16].Set(L"Rintanumero", (TEdit *)EBib, LBib, OBib, CBBib, NULL, 0, 5);
	Edits[17].Set(L"Lähtöajat", (TEdit *)ELA, LLA, OLA, CBLA, NULL, 3, 5);
	Edits[18].Set(L"Tasoitus", (TEdit *)ETas, LTas, OTas, CBTas, NULL, 0, 5);
	Edits[19].Set(L"Rata", (TEdit *)ERata, LRata, ORata, CBRata, NULL, 1, 5);
	Edits[20].Set(L"Alue/Piiri", EPiiri, LPiiri, OPiiri, CBPiiri, &EKilp.piiri, 0, 1);
}

__fastcall TFormIlm2::TFormIlm2(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	SheetHeight = 456 * Screen->PixelsPerInch / 96;
	memcpy(IlmGridCols, IlmGridCols0, sizeof(IlmGridCols0));
	EBib[0] = EBib1;
	EBib[1] = EBib2;
	EBib[2] = EBib3;
	EBib[3] = EBib4;
	EBib[4] = EBib5;
	EBadge[0] = EBadge1;
	EBadge[1] = EBadge2;
	EBadge[2] = EBadge3;
	EBadge[3] = EBadge4;
	EBadge[4] = EBadge5;
	ELA[0] = ELA1;
	ELA[1] = ELA2;
	ELA[2] = ELA3;
	ELA[3] = ELA4;
	ELA[4] = ELA5;
	ETas[0] = ETas1;
	ETas[1] = ETas2;
	ETas[2] = ETas3;
	ETas[3] = ETas4;
	ETas[4] = ETas5;
	ERata[0] = ERata1;
	ERata[1] = ERata2;
	ERata[2] = ERata3;
	ERata[3] = ERata4;
	ERata[4] = ERata5;
	EdtVakBibLisa->Text = vakbiblisa;
	OlEdits();
	PageControl1->ActivePageIndex = 0;
}

ilmIkkParamClass::ilmIkkParamClass(void)
{
	memset(this, 0, sizeof(tapIkkParamClass));
}
//---------------------------------------------------------------------------
void ilmIkkParamClass::operator=(ilmIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(ilmIkkParamClass));
}
//---------------------------------------------------------------------------
void TFormIlm2::haeParams(void)
{
	IkkParam.IdOn = CBId->Checked;
	IkkParam.KilpnoOn = CBKilpno->Checked;
	IkkParam.LisnoOn = CBLisno->Checked;
	IkkParam.SNimiOn = CBSNimi->Checked;
	IkkParam.ENimiOn = CBENimi->Checked;
	IkkParam.SarjaOn = CBSarja->Checked;
	IkkParam.SukupOn = CBSukup->Checked;
	IkkParam.IkasrjOn = CBIkasrj->Checked;
	IkkParam.WRKoodiOn = CBWRKoodi->Checked;
	IkkParam.ArvoOn = CBArvo->Checked;
	IkkParam.SeuraOn = CBSeura->Checked;
	IkkParam.SeuralyhOn = CBSeuralyh->Checked;
	IkkParam.MaaOn = CBMaa->Checked;
	IkkParam.YhdistysOn = CBYhdistys->Checked;
	IkkParam.JoukkueOn = CBJoukkue->Checked;
	IkkParam.LasnaOn = CBLasna->Checked;
	IkkParam.BibOn = CBBib->Checked;
	IkkParam.BadgeOn = CBBadge->Checked;
	IkkParam.LAOn = CBLA->Checked;
	IkkParam.TasOn = CBTas->Checked;
	IkkParam.RataOn = CBRata->Checked;
	IkkParam.PiiriOn = CBPiiri->Checked;
	IkkParam.KnohakuOn = CBKnohaku->Checked;
	IkkParam.LisnohakuOn = CBLisnohaku->Checked;
	IkkParam.NimihakuOn = CBNimihaku->Checked;
	IkkParam.NimiHThakuOn = CBNimiHThaku->Checked;
	IkkParam.HTSarjaOn = CBHTSarja->Checked;
	IkkParam.SeurahakuOn = CBSeurahaku->Checked;
	IkkParam.SeuralisaOn = CBSeuralisa->Checked;
	IkkParam.MerkAvoimeksiOn = CBMerkAvoimeksi->Checked;
	IkkParam.HTBadgeOn = CBHTBadge->Checked;
	IkkParam.AutoLaikaOn = CBAutoLaika->Checked;
	IkkParam.JIilmOn = CBJilm2->Checked;
	IkkParam.AutoKnoOn = CBAutoKno->Checked;
	IkkParam.LisnoTarkOn = CBLisnoTark->Checked;
	IkkParam.LisaysIx = RGLisays->ItemIndex;
	IkkParam.JarjestysIx = CBJarjestys->ItemIndex;
	IkkParam.PaivatIx = CBPaivat->ItemIndex;
	IkkParam.OId = _wtoi(OId->Text.c_str());
	IkkParam.OKilpno = _wtoi(OKilpno->Text.c_str());
	IkkParam.OLisno = _wtoi(OLisno->Text.c_str());
	IkkParam.OSNimi = _wtoi(OSNimi->Text.c_str());
	IkkParam.OENimi = _wtoi(OENimi->Text.c_str());
	IkkParam.OSarja = _wtoi(OSarja->Text.c_str());
	IkkParam.OWRKoodi = _wtoi(OWRKoodi->Text.c_str());
	IkkParam.OArvo = _wtoi(OArvo->Text.c_str());
	IkkParam.OSeura = _wtoi(OSeura->Text.c_str());
	IkkParam.OSeuralyh = _wtoi(OSeuralyh->Text.c_str());
	IkkParam.OMaa = _wtoi(OMaa->Text.c_str());
	IkkParam.OYhdistys = _wtoi(OYhdistys->Text.c_str());
	IkkParam.OJoukkue = _wtoi(OJoukkue->Text.c_str());
	IkkParam.OLasna = _wtoi(OLasna->Text.c_str());
	IkkParam.OBib = _wtoi(OBib->Text.c_str());
	IkkParam.OBadge = _wtoi(OBadge->Text.c_str());
	IkkParam.OLA = _wtoi(OLA->Text.c_str());
	IkkParam.OTas = _wtoi(OTas->Text.c_str());
	IkkParam.ORata = _wtoi(ORata->Text.c_str());
	IkkParam.OPiiri = _wtoi(OPiiri->Text.c_str());
	IkkParam.OSukup = _wtoi(OSukup->Text.c_str());
	IkkParam.OIkasrj = _wtoi(OIkasrj->Text.c_str());
	IkkParam.EdtVakBibLisa = _wtoi(EdtVakBibLisa->Text.c_str());
	IkkParam.IlmKentat = ilmkentat;
}
//---------------------------------------------------------------------------

int TFormIlm2::applyParams(void)
{
	ilmkentat = IkkParam.IlmKentat;
	CBId->Checked = IkkParam.IdOn;
	CBKilpno->Checked = IkkParam.KilpnoOn;
	CBLisno->Checked = IkkParam.LisnoOn;
	CBSNimi->Checked = IkkParam.SNimiOn;
	CBENimi->Checked = IkkParam.ENimiOn;
	CBSarja->Checked = IkkParam.SarjaOn;
	CBSukup->Checked = IkkParam.SukupOn;
	CBIkasrj->Checked = IkkParam.IkasrjOn;
	CBWRKoodi->Checked = IkkParam.WRKoodiOn;
	CBArvo->Checked = IkkParam.ArvoOn;
	CBSeura->Checked = IkkParam.SeuraOn;
	CBSeuralyh->Checked = IkkParam.SeuralyhOn;
	CBMaa->Checked = IkkParam.MaaOn;
	CBYhdistys->Checked = IkkParam.YhdistysOn;
	CBJoukkue->Checked = IkkParam.JoukkueOn;
	CBLasna->Checked = IkkParam.LasnaOn;
	CBBib->Checked = IkkParam.BibOn;
	CBBadge->Checked = IkkParam.BadgeOn;
	CBLA->Checked = IkkParam.LAOn;
	CBTas->Checked = IkkParam.TasOn;
	CBRata->Checked = IkkParam.RataOn;
	CBPiiri->Checked = IkkParam.PiiriOn;
	CBKnohaku->Checked = IkkParam.KnohakuOn;
	CBLisnohaku->Checked = IkkParam.LisnohakuOn;
	CBNimihaku->Checked = IkkParam.NimihakuOn;
	if (IkkParam.NimiHThakuOn)
		NimiHTHaku = 1;
	CBNimiHThaku->Checked = IkkParam.NimiHThakuOn;
	CBHTSarja->Checked = IkkParam.HTSarjaOn;
	if (IkkParam.SeurahakuOn)
		seurahaku = 2;
	CBSeurahaku->Checked = IkkParam.SeurahakuOn;
	CBSeuralisa->Checked = IkkParam.SeuralisaOn;
	CBMerkAvoimeksi->Checked = IkkParam.MerkAvoimeksiOn;
	CBHTBadge->Checked = IkkParam.HTBadgeOn;
	CBAutoLaika->Checked = IkkParam.AutoLaikaOn;
	CBJilm2->Checked = IkkParam.JIilmOn;
	CBAutoKno->Checked = IkkParam.AutoKnoOn;
	CBLisnoTark->Checked = IkkParam.LisnoTarkOn;
	RGLisays->ItemIndex = IkkParam.LisaysIx;
	CBJarjestys->ItemIndex = IkkParam.JarjestysIx;
	CBPaivat->ItemIndex = IkkParam.PaivatIx;
	OId->Text = IkkParam.OId;
	OKilpno->Text = IkkParam.OKilpno;
	OLisno->Text = IkkParam.OLisno;
	OSNimi->Text = IkkParam.OSNimi;
	OENimi->Text = IkkParam.OENimi;
	OSarja->Text = IkkParam.OSarja;
	OWRKoodi->Text = IkkParam.OWRKoodi;
	OArvo->Text = IkkParam.OArvo;
	OSeura->Text = IkkParam.OSeura;
	OSeuralyh->Text = IkkParam.OSeuralyh;
	OMaa->Text = IkkParam.OMaa;
	OYhdistys->Text = IkkParam.OYhdistys;
	OJoukkue->Text = IkkParam.OJoukkue;
	OLasna->Text = IkkParam.OLasna;
	OBib->Text = IkkParam.OBib;
	OBadge->Text = IkkParam.OBadge;
	OLA->Text = IkkParam.OLA;
	OTas->Text = IkkParam.OTas;
	ORata->Text = IkkParam.ORata;
	OPiiri->Text = IkkParam.OPiiri;
	OSukup->Text = IkkParam.OSukup;
	OIkasrj->Text = IkkParam.OIkasrj;
	EdtVakBibLisa->Text = IkkParam.EdtVakBibLisa;
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::Initialisoi(void)
{
	if (!Initialisoitu) {
		Initialisoitu = true;
		sallimuokkaus = true;
		Kilp.nollaa();
		CBPaivat->Clear();
		CBPaivat->Items->Add(L"Kaikki");
		for (int i = 0; i < kilpparam.n_pv_akt; i++) {
			CBPaivat->Items->Add(UnicodeString(i+1)+UnicodeString(L". päivä"));
			}
		CBPaivat->ItemIndex = 0;
		CSarja->Items->Clear();
		for (int i = 0; i < sarjaluku; i++) {
			CSarja->Items->Add(UnicodeString(Sarjat[i].sarjanimi));
			}
		RGLisays->TabStop = false;
		LblMoodi->Caption = RGLisays->ItemIndex == 0 ? L"Muokkausmoodi" : L"Lisäysmoodi";
	//	RGLisays->Buttons[0]->TabStop = false;
	//	RGLisays->Buttons[1]->TabStop = false;
		if (ilmkentat == 1) {
			if (NimiHTHaku == -1)
				NimiHTHaku = 1;
			if (seurahaku == -1)
				seurahaku = 1;
	//		CBSeuralisa->Checked = true;
			CBAutoLaika->Checked = true;
			CBLisnoTark->Checked = true;
			CBHTSarja->Checked = true;
			CBLisno->Checked = false;
			CBBadge->Checked = false;
			CBLA->Checked = false;
	//		FormTKanta->Show();
	//		FormSeurat->Show();
			}
		if (ilmkentat == 2 || ilmkentat == 3) {
			EId->ReadOnly = true;
			EId->Enabled = false;
			EId->TabStop = false;
			CBKilpno->Checked = false;
			CBLisno->Checked = true;
			ELisno->ReadOnly = true;
			ELisno->TabStop = false;
			CBBadge->Checked = true;
			CBLA->Checked = true;
			if (NimiHTHaku == -1)
				NimiHTHaku = 1;
			if (seurahaku == -1)
				seurahaku = 2;
			CBAutoLaika->Checked = false;
			CBLisnoTark->Checked = false;
			CBHTSarja->Checked = true;
			BtnEiBadge->Visible = true;
			lue_lainat(0, 0);
			}
		if (ilmkentat == 4) {
			if (NimiHTHaku == -1)
				NimiHTHaku = 1;
			if (seurahaku == -1)
				seurahaku = 1;
			CBLisno->Checked = false;
			CBHTSarja->Checked = true;
			CBBadge->Checked = true;
			CBLA->Checked = false;
			CBKilpno->Checked = false;
			CBMerkAvoimeksi->Checked = false;
			}
		if (kilpparam.seuralyh_on) {
			CBSeuralyh->Checked = seurahaku;
			CBSeura->Checked = false;
			}
		else {
			CBSeura->Checked = seurahaku;
			CBSeuralyh->Checked = false;
			}
		CBSeurahaku->Checked = seurahaku > 1;
		CBNimiHThaku->Checked = NimiHTHaku > 0;
		TabSheet1Show(this);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormIlm2::SaveEdits(void)
{
	FormIlmEdits.EId = EId->Text;
	FormIlmEdits.ELisno = ELisno->Text;
	FormIlmEdits.EWRKoodi = EWRKoodi->Text;
	FormIlmEdits.ESNimi = ESNimi->Text;
	FormIlmEdits.EENimi = EENimi->Text;
	FormIlmEdits.EArvo = EArvo->Text;
	FormIlmEdits.ESeura = ESeura->Text;
	FormIlmEdits.ESeuralyh = ESeuralyh->Text;
	FormIlmEdits.EMaa = EMaa->Text;
	FormIlmEdits.EYhdistys = EYhdistys->Text;
	FormIlmEdits.EJoukkue = EJoukkue->Text;
	FormIlmEdits.ELasna = ELasna->Text;
	for (int i = 0; i < 5; i++) {
		FormIlmEdits.EBadge[i] = EBadge[i]->Text;
		FormIlmEdits.ELA[i] = ELA[i]->Text;
		FormIlmEdits.ETas[i] = ETas[i]->Text;
		FormIlmEdits.ERata[i] = ERata[i]->Text;
		}
	FormIlmEdits.EPiiri = EPiiri->Text;
	if (ESukup->Text.Length() > 0)
		ESukup->Text = ESukup->Text.UpperCase().SubString(1, 1);
	FormIlmEdits.ESukup = ESukup->Text;
	FormIlmEdits.EIkasrj = EIkasrj->Text;
	FormIlmEdits.CSarja = CSarja->ItemIndex;
 }
//---------------------------------------------------------------------------
void __fastcall TFormIlm2::RestoreEdits(void)
{
	EId->Text = FormIlmEdits.EId;
	ELisno->Text = FormIlmEdits.ELisno;
	EWRKoodi->Text = FormIlmEdits.EWRKoodi;
	ESNimi->Text = FormIlmEdits.ESNimi;
	EENimi->Text = FormIlmEdits.EENimi;
	EArvo->Text = FormIlmEdits.EArvo;
	ESeura->Text = FormIlmEdits.ESeura;
	ESeuralyh->Text = FormIlmEdits.ESeuralyh;
	EMaa->Text = FormIlmEdits.EMaa;
	EYhdistys->Text = FormIlmEdits.EYhdistys;
	EJoukkue->Text = FormIlmEdits.EJoukkue;
	ELasna->Text = FormIlmEdits.ELasna;
	for (int i = 0; i < 5; i++) {
		EBadge[i]->Text = FormIlmEdits.EBadge[i];
		ELA[i]->Text = FormIlmEdits.ELA[i];
		ETas[i]->Text = FormIlmEdits.ETas[i];
		ERata[i]->Text = FormIlmEdits.ERata[i];
		}
	EPiiri->Text = FormIlmEdits.EPiiri;
	CSarja->ItemIndex = FormIlmEdits.CSarja;
	ESukup->Text = FormIlmEdits.ESukup;
	EIkasrj->Text = FormIlmEdits.EIkasrj;
 }
//---------------------------------------------------------------------------
TWinControl* __fastcall TFormIlm2::NextTabControl(TObject *Sender)
{
	for (unsigned int i = 0; i < sizeof(TabControls)/sizeof(void *); i++) {
		if (Sender == TabControls[i])
			return(TabControls[i+1]);
		}
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormIlm2::FormSetSize(int Sheet)
{
	if (Sheet == 0)
		PageControl1->Height = SheetHeight;
	else
		PageControl1->Height = Panel1->Height + 33 * Screen->PixelsPerInch / 96;
	Height = PageControl1->Height + IlmGrid->Height + 35 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------
void __fastcall TFormIlm2::FormResize(TObject *Sender)
{
	PageControl1->Top = ClientHeight - PageControl1->Height;
	IlmGrid->Width = ClientWidth - 35 * Screen->PixelsPerInch / 96;
	IlmGrid->Height = PageControl1->Top - 35 * Screen->PixelsPerInch / 96;
	BitBtn1->Left = IlmGrid->Width + 3 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

int __fastcall TFormIlm2::SeurPos(void *key)
{
	int d = -1;
	char keyn[100];
	static int dd = 0, kno = 0;

	switch (CBJarjestys->ItemIndex) {
		case 0:
			if (key)
				kno = *(int *)key - 1;
			if (kno < 0)
				kno = 0;
			kno++;
			d = getpos(kno, true);
			if (d > 0) {
				kilptietue kilp;
				kilp.GETREC(d);
				kno = kilp.id();
				}
/*
			while (kno <= maxkilpno) {
				if ((d = getpos(kno)) > 0) {
					return(d);
					}
				kno++;
				}
*/
			break;
		case 1:
			if (key) {
				searchix(&nameindex, (char *) key, &d);
				}
			else {
				nextix(&nameindex, keyn, &d);
				}
			break;
		case 2:
			if (key)
				dd = 0;
			for (dd = dd+1; dd < nrec; dd++) {
				kilptietue kilp;

				kilp.GETREC(dd);
				if (kilp.kilpstatus == 0) {
					return(dd);
					}
				}
			d = -1;
			break;
		}
	return(d);
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::naytaTiedot(int kno)
{
	wchar_t ch, cs, ch2, ct, line[80], trk;
	int i, n, d, ep, rvali, sj, epv = 0;
	int tl, tlsum;
	kilptietue Kilp;
	char key[MAXINDL+1];

	npv = 1;
	aktrow = 0;
	aktcol = 0;
	memset(line, 0, sizeof(line));
	Button1->Visible = sallimuokkaus;
	Button2->Visible = sallimuokkaus;
	Button3->Visible = sallimuokkaus;
	IlmGridCols[5].Visible = CBArvo->Checked && kilpparam.larvo != 0;
	IlmGridCols[6].Visible = CBSeura->Checked;
	IlmGridCols[7].Visible = CBSeuralyh->Checked && kilpparam.lseuralyh != 0;
	IlmGridCols[8].Visible = CBYhdistys->Checked && kilpparam.lyhdistys != 0;
	IlmGridCols[9].Visible = CBJoukkue->Checked && kilpparam.ljoukkue != 0;
	IlmGridCols[10].Visible = CBMaa->Checked;
	IlmGridCols[11].Visible = CBPiiri->Checked;
	IlmGridCols[12].Visible = CBWRKoodi->Checked;
	IlmGridCols[13].Visible = CBJIilm->Checked && CBJilm2->Checked;
	IlmGridCols[16].Visible = CBBadge->Checked;
	IlmGridCols[18].Visible = CBTas->Checked;
	IlmGrid->RowCount = 2;
	IlmGrid->DefaultRowHeight = 22 * Screen->PixelsPerInch / 96;
	IlmGrid->RowCount = nilmt + 1;
	IlmGrid->ColCount = 5;
	if (CBPaivat->ItemIndex > 1)
		epv = CBPaivat->ItemIndex-1;
	if (CBPaivat->ItemIndex == 0)
		npv = kilpparam.n_pv_akt;
	n = 0;
	for (unsigned int i = 0; i < sizeof(IlmGridCols)/sizeof(ColTp); i++) {
		if (IlmGridCols[i].Visible) {
			int nlisa;

			if (IlmGridCols0[i].Count > 1)
				nlisa = npv;
			else
				nlisa = 1;
			if (n+nlisa > IlmGrid->ColCount)
				IlmGrid->ColCount = n+nlisa;
			IlmGridCols[i].Order = n;
			IlmGridCols[i].Count = nlisa;
			for (int ipv = 0; ipv < nlisa; ipv++) {
				IlmGrid->Cells[n+ipv][0] = UnicodeString(IlmGridCols[i].Caption);
				IlmGrid->ColWidths[n+ipv] = IlmGridCols[i].Width * Screen->PixelsPerInch / 96;
				}
			n += nlisa;
			}
		}
	switch (CBJarjestys->ItemIndex) {
		case 0:
			d = SeurPos(&kno);
			break;
		case 1:
			if ((d = getpos(kno)) > 0) {
				Kilp.GETREC(d);
				keyaak(&Kilp, key, nameindex.keyl, 0);
				}
			else
				memset(key, 0, sizeof(key));
			d = SeurPos(key);
			break;
		default:
			d = SeurPos(&kno);
		}
	for (int k = 1; k <= nilmt; k++) {
		if (d < 0) {
			IlmGrid->RowCount = k;
			break;
			}
		Kilp.GETREC(d);
		DKilp[k] = d;
		d = SeurPos(NULL);
		for (unsigned int i = 0; i < sizeof(IlmGridCols0)/sizeof(ColTp); i++) {
			if (IlmGridCols[i].Visible) {
				int col = IlmGridCols[i].Order;

				ColIx[col] = i;
				ColPv[col] = 0;

				switch (i) {
					case 0:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.id());
						break;
					case 1:
						IlmGrid->Cells[col][k] = UnicodeString(Sarjat[Kilp.Sarja()].sarjanimi);
						break;
					case 2:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.lisno[0]);
						break;
					case 3:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.sukunimi);
						break;
					case 4:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.etunimi);
						break;
					case 5:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.arvo);
						break;
					case 6:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.seura);
						break;
					case 7:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.seuralyh);
						break;
					case 8:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.yhdistys);
						break;
					case 9:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.joukkue);
						break;
					case 10:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.maa);
						break;
					case 11:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.piiri);
						break;
					case 12:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.wrkoodi);
						break;
					case 13:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.ilmlista);
						break;
					case 14:
						IlmGrid->Cells[col][k] = UnicodeString(Kilp.arvryhma);
						break;
					case 15:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							IlmGrid->Cells[col+ipv][k] = UnicodeString(Kilp.pv[epv+ipv].bib);
							}
						break;
					case 16:
						if (ilmkentat != 4 || ilmkentat != 5) {
							for (int ipv = 0; ipv < npv; ipv++) {
								ColIx[col+ipv] = i;
								ColPv[col+ipv] = ipv;
								IlmGrid->Cells[col+ipv][k] = UnicodeString(Kilp.pv[epv+ipv].badge[0]);
								}
							}
						break;
					case 17:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							if (Kilp.pv[epv+ipv].laina[0] == L'L')
								IlmGrid->Cells[col+ipv][k] = UnicodeString(L"Laina");
							else
								IlmGrid->Cells[col+ipv][k] = UnicodeString(L"");
							}
						break;
					case 18:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							IlmGrid->Cells[col+ipv][k] = UnicodeString(Kilp.pv[epv+ipv].tasoitus);
							}
						break;
					case 19:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							switch (Kilp.tark(epv+ipv)) {
								case L'K' :
									wcscpy(line, L"Kesk");
									break;
								case L'O' :
									wcscpy(line, L"Ohit");
									break;
								case L'H' :
									wcscpy(line, L"Hyl");
									break;
								case L'E' :
									wcscpy(line, L"Eiläht.");
									break;
								case L'T' :
									wcscpy(line, L"Tark.");
									break;
								case L'I' :
									wcscpy(line, L"esItys");
									break;
								case L'V' :
									wcscpy(line, L"Vakantti");
									break;
								case L'P' :
									wcscpy(line, L"Poissa");
									break;
								default :
									wcscpy(line, L"Läsnä");
									break;
								}
							IlmGrid->Cells[col+ipv][k] = UnicodeString(line);
							}
						break;
					case 20:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							IlmGrid->Cells[col+ipv][k] =
								UnicodeString(AIKATOWSTRS(line, Kilp.pv[epv+ipv].tlahto, t0)).SubString(1, 8);
							}
						break;
					case 21:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							IlmGrid->Cells[col+ipv][k] =
								UnicodeString(AIKATOWSTRS(line, Kilp.tulos_pv(epv+ipv), 0)).SubString(1, pvparam[epv+ipv].laika);
							}
						break;
					case 22:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							IlmGrid->Cells[col+ipv][k] = UnicodeString(Kilp.asakkoStr(epv+ipv));
							}
						break;
					case 23:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							IlmGrid->Cells[col+ipv][k] = UnicodeString(Kilp.pv[epv+ipv].txt);
							}
						break;
					}
				}
			}
		}
	if (CBJarjestys->ItemIndex == 4) {
		IlmGrid->Row = IlmGrid->RowCount-1;
	}
//	n = IlmGrid->VisibleRowCount;
//	i = IlmGrid->Cells[5][3].Pos()
}

//---------------------------------------------------------------------------

int __fastcall TFormIlm2::tallennaTiedot(void)
{
	wchar_t ch, cs, ch2, ct, line[80], trk;
	int i, n, d, kno, ep, rvali, sj, epv = 0, npv = 1, errrivi = 0;
	int tl, tlsum;
	kilptietue Kilp, entKilp;

	memset(line, 0, sizeof(line));
	if (CBPaivat->ItemIndex > 1)
		epv = CBPaivat->ItemIndex-1;
	if (CBPaivat->ItemIndex == 0)
		npv = kilpparam.n_pv_akt;
	EnterCriticalSection(&tall_CriticalSection);
	for (int k = 1; k <= IlmGrid->RowCount; k++) {
		kno = _wtoi(IlmGrid->Cells[0][k].c_str());
		if ((d = getpos(kno)) <= 0) {
			errrivi = kno;
			continue;
			}
		Kilp.GETREC(d);
		entKilp = Kilp;
		for (unsigned int i = 0; i < sizeof(IlmGridCols0)/sizeof(ColTp); i++) {
			if (IlmGridCols[i].Visible) {
				int col = IlmGridCols[i].Order;
				int srj;

				switch (i) {
					case 1:
						srj = haesarja_w(IlmGrid->Cells[col][k].c_str(), false);
						if (srj < 0) {
							errrivi = kno;
							break;
							}
						Kilp.setSarja(srj);
						for (int is = -1; is < kilpparam.n_pv_akt; is++) {
							if ((is == -1 || is > k_pv) && Sarjat[Kilp.Sarja(is)].vakanttisarja)
								Kilp.setSarja(srj, is);
							}
						break;
					case 2:
						Kilp.lisno[0] = _wtoi(IlmGrid->Cells[col][k].c_str());
						break;
					case 3:
						wcsncpy(Kilp.sukunimi, IlmGrid->Cells[col][k].c_str(), kilpparam.lsnimi);
						break;
					case 4:
						wcsncpy(Kilp.etunimi, IlmGrid->Cells[col][k].c_str(), kilpparam.lenimi);
						break;
					case 5:
						wcsncpy(Kilp.arvo, IlmGrid->Cells[col][k].c_str(), kilpparam.larvo);
						break;
					case 6:
						wcsncpy(Kilp.seura, IlmGrid->Cells[col][k].c_str(), kilpparam.lseura);
						break;
					case 7:
						wcsncpy(Kilp.seuralyh, IlmGrid->Cells[col][k].c_str(), kilpparam.lseuralyh);
						break;
					case 8:
						wcsncpy(Kilp.yhdistys, IlmGrid->Cells[col][k].c_str(), kilpparam.lyhdistys);
						break;
					case 9:
						wcsncpy(Kilp.joukkue, IlmGrid->Cells[col][k].c_str(), kilpparam.ljoukkue);
						break;
					case 10:
						wcsncpy(Kilp.maa, IlmGrid->Cells[col][k].c_str(), 3);
						break;
					case 11:
						Kilp.piiri = _wtoi(IlmGrid->Cells[col][k].c_str());
						break;
					case 12:
						wcsncpy(Kilp.wrkoodi, IlmGrid->Cells[col][k].c_str(), kilpparam.lwrkoodi);
						break;
					case 13:
						Kilp.ilmlista = _wtoi(IlmGrid->Cells[col][k].c_str());
						break;
					case 14:
						Kilp.arvryhma = _wtoi(IlmGrid->Cells[col][k].c_str());
						break;
					case 15:
						for (int ipv = 0; ipv < npv; ipv++)
							Kilp.pv[epv+ipv].bib = _wtoi(IlmGrid->Cells[col+ipv][k].c_str());
						break;
					case 16:
						for (int ipv = 0; ipv < npv; ipv++)
							Kilp.pv[epv+ipv].badge[0] = _wtoi(IlmGrid->Cells[col+ipv][k].c_str());
						break;
					case 17:
						for (int ipv = 0; ipv < npv; ipv++)
							Kilp.pv[epv+ipv].laina[0] = IlmGrid->Cells[col+ipv][k].c_str()[0];
						break;
					case 18:
						for (int ipv = 0; ipv < npv; ipv++)
							Kilp.pv[epv+ipv].tasoitus = _wtoi(IlmGrid->Cells[col+ipv][k].c_str());
						break;
					case 19:
						for (int ipv = 0; ipv < npv; ipv++) {
							switch (towupper(IlmGrid->Cells[col+ipv][k].c_str()[0])) {
								case L'E' :
									if (towupper(IlmGrid->Cells[col+ipv][k].c_str()[1]) != L'S')
										Kilp.set_tark(L'E', epv+ipv);
									else
										Kilp.set_tark(L'I', epv+ipv);
									break;
								case L'L' :
									Kilp.set_tark(L'-', epv+ipv);
									break;
								default :
									Kilp.set_tark(towupper(IlmGrid->Cells[col+ipv][k].c_str()[0]), epv+ipv);
									break;
								}
							}
						break;
					case 20:
						for (int ipv = 0; ipv < npv; ipv++) {
							Kilp.pv[epv+ipv].tlahto = wstrtoaika_vap(IlmGrid->Cells[col+ipv][k].c_str(), t0);
							Kilp.pv[epv+ipv].va[0].vatulos = Kilp.pv[epv+ipv].tlahto;
							}
						break;
					case 21:
						for (int ipv = 0; ipv < npv; ipv++) {
							Kilp.tall_tulos_pv(wstrtoaika_vap(IlmGrid->Cells[col+ipv][k].c_str(), 0), epv+ipv);
							}
						break;
					case 22:
						for (int ipv = 0; ipv < npv; ipv++)
							Kilp.set_Asakot(IlmGrid->Cells[col+ipv][k].c_str(), epv+ipv);
						break;
					case 23:
						for (int ipv = 0; ipv < npv; ipv++)
							wcsncpy(Kilp.pv[epv+ipv].txt, IlmGrid->Cells[col+ipv][k].c_str(), sizeof(Kilp.pv[epv+ipv].txt)/2-1);
						break;
					}
				}
			}
		if (errrivi) {
			LeaveCriticalSection(&tall_CriticalSection);
			Application->MessageBox((UnicodeString(L"Virhe kilpailijan ") + UnicodeString(errrivi)
				+ UnicodeString(L" tiedoissa. Loppuja ei tallennettu.")).c_str(), L"Virhe", MB_OK);
			EnterCriticalSection(&tall_CriticalSection);
			break;
			}
		Kilp.tallenna(d, 0, 0, 0, &entKilp);
		}
	LeaveCriticalSection(&tall_CriticalSection);
	return(0);
}

//---------------------------------------------------------------------------

void __fastcall TFormIlm2::FormShow(TObject *Sender)
{
	int kno = 0;

	Initialisoi();
	if (UusiEmit) {
		FocusControl(EBadge1);
		UusiEmit = false;
		return;
		}
	if (ilmkentat == 2 || ilmkentat == 3) {
		kno = haeSeurVakantti(0);
		SaveEdits();
		if (kno) {
		   Dk = getpos(kno);
		   Kilp.GETREC(Dk);
		   naytaKilpailija();
		   }
		}
	naytaTiedot(kno);
	AktId = (ToimintaTila == 2) ? &EKilp.pv[k_pv].bib : &EKilp.kilpno;
}
//---------------------------------------------------------------------------

int __fastcall TFormIlm2::paivitaMuutos(int col, int row)
{
	int err = 0;
	int k, srj;
	wchar_t line[20];
	kilptietue Kilp;

	k = row;
	srj = haesarja_w(IlmGrid->Cells[1][k].c_str(), false);
	switch (ColIx[col]) {
		case 1:
			srj = haesarja_w(IlmGrid->Cells[col][k].c_str(), false);
			if (srj < 0) {
				err = 1;
				break;
				}
			IlmGrid->Cells[col][k] = UnicodeString(Sarjat[srj].sarjanimi);
			break;
		case 2:
			IlmGrid->Cells[col][k] = UnicodeString(_wtoi(IlmGrid->Cells[col][k].c_str()));
			break;
		case 3:
			IlmGrid->Cells[col][k] = IlmGrid->Cells[col][k].SubString(1, kilpparam.lsnimi);
			break;
		case 4:
			IlmGrid->Cells[col][k] = IlmGrid->Cells[col][k].SubString(1, kilpparam.lenimi);
			break;
		case 5:
			IlmGrid->Cells[col][k] = IlmGrid->Cells[col][k].SubString(1, kilpparam.larvo);
			break;
		case 6:
			IlmGrid->Cells[col][k] = IlmGrid->Cells[col][k].SubString(1, kilpparam.lseura);
			break;
		case 7:
			IlmGrid->Cells[col][k] = IlmGrid->Cells[col][k].SubString(1, kilpparam.lseuralyh);
			break;
		case 8:
			IlmGrid->Cells[col][k] = IlmGrid->Cells[col][k].SubString(1, kilpparam.lyhdistys);
			break;
		case 9:
			IlmGrid->Cells[col][k] = IlmGrid->Cells[col][k].SubString(1, kilpparam.ljoukkue);
			break;
		case 10:
			IlmGrid->Cells[col][k] = IlmGrid->Cells[col][k].SubString(1, 3);
			break;
		case 11:
			IlmGrid->Cells[col][k] = UnicodeString(_wtoi(IlmGrid->Cells[col][k].c_str()));
			break;
		case 12:
			IlmGrid->Cells[col][k] = IlmGrid->Cells[col][k].SubString(1, kilpparam.lwrkoodi);
			break;
		case 13:
			IlmGrid->Cells[col][k] = UnicodeString(_wtoi(IlmGrid->Cells[col][k].c_str()));
			break;
		case 14:
			IlmGrid->Cells[col][k] = UnicodeString(_wtoi(IlmGrid->Cells[col][k].c_str()));
			break;
		case 15:
			for (int ipv = 0; ipv < npv; ipv++)
				IlmGrid->Cells[col+ipv][k] = UnicodeString(_wtoi(IlmGrid->Cells[col+ipv][k].c_str()));
			break;
		case 16:
			for (int ipv = 0; ipv < npv; ipv++)
				IlmGrid->Cells[col+ipv][k] = UnicodeString(_wtoi(IlmGrid->Cells[col+ipv][k].c_str()));
			break;
		case 17:
			for (int ipv = 0; ipv < npv; ipv++) {
				if (towupper(IlmGrid->Cells[col+ipv][k].c_str()[0]) == L'L')
					IlmGrid->Cells[col+ipv][k] = UnicodeString(L"Laina");
				else
					IlmGrid->Cells[col+ipv][k] = UnicodeString(L"");
				}
			break;
		case 18:
			for (int ipv = 0; ipv < npv; ipv++)
				IlmGrid->Cells[col+ipv][k] = UnicodeString(_wtoi(IlmGrid->Cells[col+ipv][k].c_str()));
			break;
		case 19:
			for (int ipv = 0; ipv < npv; ipv++) {
				wchar_t kh;

				switch (towupper(IlmGrid->Cells[col+ipv][k].c_str()[0])) {
					case L'E' :
						if (towupper(IlmGrid->Cells[col+ipv][k].c_str()[1]) != L'S')
							kh = L'E';
						else
							kh = L'I';
						break;
					default :
						kh = towupper(IlmGrid->Cells[col+ipv][k].c_str()[0]);
						break;
					}
				switch (kh) {
					case L'K' :
						wcscpy(line, L"Kesk");
						break;
					case L'O' :
						wcscpy(line, L"Ohit");
						break;
					case L'H' :
						wcscpy(line, L"Hyl");
						break;
					case L'E' :
						wcscpy(line, L"Eiläht.");
						break;
					case L'T' :
						wcscpy(line, L"Tark.");
						break;
					case L'I' :
						wcscpy(line, L"esItys");
						break;
					case L'V' :
						wcscpy(line, L"Vakantti");
						break;
					case L'P' :
						wcscpy(line, L"Poissa");
						break;
					default :
						wcscpy(line, L"Läsnä");
						break;
					}
				IlmGrid->Cells[col+ipv][k] = UnicodeString(line);
				}
			break;
		case 20:
			for (int ipv = 0; ipv < npv; ipv++) {
				int tl;
				wcsncpy(line,IlmGrid->Cells[col+ipv][k].c_str(), 10);
				line[10] = 0;
				elimwbl(line);
				if (line[0] == 0 || line[0] == L':') {
					tl = TMAALI0;
					}
				else
					tl = wstrtoaika_vap(IlmGrid->Cells[col+ipv][k].c_str(), t0);
				IlmGrid->Cells[col+ipv][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, 8);
				}
			break;
		case 21:
			for (int ipv = 0; ipv < npv; ipv++) {
				int tl;
				tl = wstrtoaika_vap(IlmGrid->Cells[col+ipv][k].c_str(), 0);
				IlmGrid->Cells[col+ipv][k] = UnicodeString(AIKATOWSTRS(line, tl, 0)).SubString(1, pvparam[ipv].laika);
				}
			break;
		case 22:
			if (srj < 0)
			   break;
			for (int ipv = 0; ipv < npv; ipv++)
				IlmGrid->Cells[col+ipv][k] = IlmGrid->Cells[col+ipv][k].SubString(1, Sarjat[srj].lsak[ipv]);
			break;
		case 23:
			for (int ipv = 0; ipv < npv; ipv++)
				IlmGrid->Cells[col+ipv][k] = IlmGrid->Cells[col+ipv][k].SubString(1, sizeof(Kilp.pv[0].txt)/2-1);
			break;
		}
	return(err);
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::IlmGridSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (IlmGrid->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktrow = ARow;
	aktcol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::Button1Click(TObject *Sender)
{
	IlmGrid->Options << goEditing;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::Button3Click(TObject *Sender)
{
	IlmGrid->Options >> goEditing;
	naytaTiedot(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::IlmGridExit(TObject *Sender)
{
	if (IlmGrid->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktrow = 0;
	aktcol = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::Button2Click(TObject *Sender)
{
	if (tallennaTiedot() == 0) {
		IlmGrid->Options >> goEditing;
		naytaTiedot(0);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::IlmGridDblClick(TObject *Sender)
{
	Dk = DKilp[aktrow];
	Kilp.GETREC(Dk);
	naytaKilpailija();
	SaveEdits();
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBPaivatChange(TObject *Sender)
{
	if (IlmGrid->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		else
			IlmGrid->Options >> (goEditing);
		}
	naytaTiedot(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::FormClose(TObject *Sender, TCloseAction &Action)

{
	if (IlmGrid->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		else
			IlmGrid->Options >> (goEditing);
		}
	else {

    }
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBJarjestysChange(TObject *Sender)
{
	if (IlmGrid->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		else
			IlmGrid->Options >> (goEditing);
		}
	naytaTiedot(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBSarjaClick(TObject *Sender)
{
	CSarja->Enabled = CBSarja->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBKilpnoClick(TObject *Sender)
{
	EKilpno->Enabled = CBKilpno->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TFormIlm2::CBIdClick(TObject *Sender)
{
	EId->Enabled = CBId->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBLisnoClick(TObject *Sender)
{
	ELisno->Enabled = CBLisno->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBWRKoodiClick(TObject *Sender)
{
	EWRKoodi->Enabled = CBWRKoodi->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBSNimiClick(TObject *Sender)
{
	ESNimi->Enabled = CBSNimi->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBENimiClick(TObject *Sender)
{
	EENimi->Enabled = CBENimi->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBArvoClick(TObject *Sender)
{
	EArvo->Enabled = CBArvo->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBSeuraClick(TObject *Sender)
{
	ESeura->Enabled = CBSeura->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TFormIlm2::CBSeuralyhClick(TObject *Sender)
{
	ESeuralyh->Enabled = CBSeuralyh->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBMaaClick(TObject *Sender)
{
	EMaa->Enabled = CBMaa->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBYhdistysClick(TObject *Sender)
{
	EYhdistys->Enabled = CBYhdistys->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBJoukkueClick(TObject *Sender)
{
	EJoukkue->Enabled = CBJoukkue->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBLasnaClick(TObject *Sender)
{
	ELasna->Enabled = CBLasna->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBBadgeClick(TObject *Sender)
{
	for (int i = 0; i < 5; i++)
		EBadge[i]->Enabled = kilpparam.n_pv_akt > i && CBBadge->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBLAClick(TObject *Sender)
{
	for (int i = 0; i < 5; i++)
		ELA[i]->Enabled = kilpparam.n_pv_akt > i && CBLA->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBTasClick(TObject *Sender)
{
	for (int i = 0; i < 5; i++)
		ETas[i]->Enabled = kilpparam.n_pv_akt > i && CBTas->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBRataClick(TObject *Sender)
{
	for (int i = 0; i < 5; i++)
		ERata[i]->Enabled = kilpparam.n_pv_akt > i && CBRata->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBPiiriClick(TObject *Sender)
{
	EPiiri->Enabled = CBPiiri->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBNimiHThakuClick(TObject *Sender)
{
	if (CBNimiHThaku->Checked) {
		FormTKanta->Show();
		FormTKanta->Caller = FormIlm2;
		FormTKanta->Left = Left+Width;
		FormTKanta->Top = 20;
		if (FormTKanta->WindowState == wsMinimized)
			FormTKanta->WindowState = wsNormal;
		FormTKanta->BringToFront();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::ESNimiChange(TObject *Sender)
{
	if (ESNimi->Text == NULL)
		return;
	if (EdKey == VK_RETURN || EdKey == VK_TAB) {
		ESNimi->SelectAll();
		return;
		}
	if (EdKey == VK_DOWN || EdKey == VK_UP || EdKey == VK_F5 || EdKey == VK_F7)
		return;
	if (NimiKeyHit || ESNimi->Text.Length() < 2) {
	   LSn = ESNimi->Text.Length();
		if (CBNimiHThaku->Checked && FormTKanta->HenkiloHakuOn()) {
			FormTKanta->Caller = FormIlm2;
			FormTKanta->NaytaHenkilot(ESNimi->Text.SubString(1, LSn).c_str(),
				(ESNimi->Text.SubString(1, LSn)+UnicodeString(L"ööö")).c_str(), NULL, NULL, 0);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::EENimiChange(TObject *Sender)
{
	if (EENimi->Text == NULL)
		return;
	if (EdKey == VK_RETURN || EdKey == VK_TAB) {
		EENimi->SelectAll();
		return;
		}
	if (EdKey == VK_DOWN || EdKey == VK_UP || EdKey == VK_F5 || EdKey == VK_F7)
		return;
	if (LSn < ESNimi->Text.Length())
		return;
	if (NimiKeyHit) {
	   LEn = EENimi->Text.Length();
		if (CBNimiHThaku->Checked && FormTKanta->HenkiloHakuOn()) {
			FormTKanta->Caller = FormIlm2;
			FormTKanta->NaytaHenkilot(ESNimi->Text.c_str(), ESNimi->Text.c_str(),
				EENimi->Text.SubString(1, LEn).c_str(), (EENimi->Text.SubString(1, LEn)+UnicodeString(L"ööö")).c_str(), 0);
			}
		}
}
//---------------------------------------------------------------------------

int TFormIlm2::annavakantinbib(kilptietue *kilp, int ipv)
{
	int ibib;
	ibib = kilp->pv[ipv].bib + vakbiblisa;
	if (getpos(ibib) > 0) {
		while (ibib < maxkilpno && getpos(ibib) > 0)
			ibib += 1000;
		if (ibib > maxkilpno) {
			ibib = maxkilpno;
			while (ibib > 0 && getpos(ibib) > 0)
				ibib--;
        	}
		}
	return(ibib);
}
//---------------------------------------------------------------------------
void __fastcall TFormIlm2::PalaaHenkHausta(bool tallenna)
{
	if (tallenna) {
		SiirraHTTiedot(0);
		CSarja->SetFocus();
		}
	BringToFront();
	return;
}
//---------------------------------------------------------------------------
void __fastcall TFormIlm2::SiirraHTTiedot(int haku)
{
	int KHenk;
	wchar_t snimi[LSNIMI+1], enimi[LENIMI+1], seura[LSEURA+1], maa[4], sarja[20];
	wchar_t joukkue[LJK+1], sukup;
	int id, intid, badge, bib, ika, synt, d;

	if (!FormTKanta->HenkiloHakuOn())
		return;
//	RestoreEdits();
	if (haku) {
		FormTKanta->Caller = FormIlm2;
		FormTKanta->BringToFront();
		FormTKanta->Selaa(haku);
		return;
		}
	KHenk =	FormTKanta->THenktiedot(haku, snimi, enimi, seura, maa, sarja, joukkue, &id, &intid,
		&badge, &bib, &sukup, &ika, &synt);
	if (KHenk >= 0) {
		if (CBLisnoTark->Checked && id && (d = etsilisno(id, 0)) > 0) {
			if (_wtoi(Kilp.etunimi) == Kilp.id()) {
				VKilp = Kilp;
				DVKilp = Dk;
				}
			Dk = d;
			Kilp.GETREC(Dk);
			Kilp.pv[k_pv].badge[0] = VKilp.pv[k_pv].badge[0];
			Kilp.pv[k_pv].badge[1] = VKilp.pv[k_pv].badge[1];
			Kilp.pv[k_pv].bib = VKilp.pv[k_pv].bib;
			naytaKilpailija();
			}
		else {
			if (DVKilp > 0) {
				Dk = DVKilp;
				Kilp = VKilp;
				naytaKilpailija();
				}
			DVKilp = 0;
			if (CBHTSarja->Checked && wcslen(sarja) > 0 && (ilmkentat != 3 || CSarja->ItemIndex == -1)) {
				int srj;

				srj = haesarja_w(sarja, false);
				if (srj >= 0) {
					CSarja->ItemIndex = srj;
					}
				}
			if (CBLisno->Checked || CBLisnoTark->Checked)
				ELisno->Text = UnicodeString(id);
			if (CBSNimi->Checked)
				ESNimi->Text = UnicodeString(snimi);
			if (CBENimi->Checked)
				EENimi->Text = UnicodeString(enimi);
			if (CBSeura->Checked && !kilpparam.seuralyh_on)
				ESeura->Text = UnicodeString(seura);
			if (CBSeuralyh->Checked && kilpparam.seuralyh_on)
				ESeuralyh->Text = UnicodeString(seura);
			if (CBMaa->Checked)
				EMaa->Text = UnicodeString(maa);
			if (CBJoukkue->Checked)
				EJoukkue->Text = UnicodeString(joukkue);
			if (CBWRKoodi->Checked)
				EWRKoodi->Text = UnicodeString(intid);
			HTBadge = badge;
			for (int i = k_pv; i < kilpparam.n_pv_akt; i++) {
				if (CBHTBadge->Checked) {
					EBadge[i]->Text = UnicodeString(badge);
					}
				if (ToimintaTila == 2)
					break;
				}
			}
		if (CBAutoLaika->Checked) {
			Kilp.tall_lajat_pv(Sarjat[Kilp.Sarja()].enslahto[k_pv]+
				SEK*Sarjat[Kilp.Sarja()].lahtovali[k_pv]*(Kilp.pv[k_pv].bib-Sarjat[Kilp.Sarja()].bibalku[k_pv]),
				k_pv);
			}
		}
	else {
		if (DVKilp > 0) {
			Dk = DVKilp;
			Kilp = VKilp;
			DVKilp = 0;
/*
			ELisno->Text = UnicodeString(Kilp.lisno[0]);
			EBib[k_pv]->Text = UnicodeString(Kilp.pv[k_pv].bib);
			for (int i = k_pv; i < kilpparam.n_pv_akt; i++) {
				if (CBHTBadge->Checked)
					EBadge[i]->Text = UnicodeString(Kilp.pv[i].badge[0]);
				if (ToimintaTila == 2)
					break;
				}
*/
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::ESNimiKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	NimiKeyHit = true;
	SeuraKeyHit = 0;

	EdKey = Key + (Shift.Contains(ssShift) ? 0x10000 : 0) + (Shift.Contains(ssCtrl) ? 0x20000 : 0)
		+ (Shift.Contains(ssAlt) ? 0x40000 : 0);
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(TabControls[0]);
		return;
		}
	if (CBNimiHThaku->Checked) {
		if (Key == VK_RETURN ||
			(Key == VK_F6 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))) {
			if (Key == VK_RETURN && NextTabControl(Sender))
				FocusControl(NextTabControl(Sender));
			Key = (Key == VK_RETURN) ? VK_TAB : 0;
			NimiKeyHit = false;
			SiirraHTTiedot(0);
			}
		if (Key == VK_UP ||
			Key == VK_F5 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			NimiKeyHit = false;
			SiirraHTTiedot(-1);
			}
		if (Key == VK_DOWN ||
			Key == VK_F7 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			NimiKeyHit = false;
			SiirraHTTiedot(1);
			}
		}
	else if (Key == VK_RETURN && NextTabControl(Sender))
		FocusControl(NextTabControl(Sender));
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::tallennaKilpailija(bool kysy)
{
	int d, kilpno, bdg;

	if (RGLisays->ItemIndex == 0 && (Dk <= 0 || Dk >= nrec)) {
		Application->MessageBoxW(L"Kilpailijaa ei voi lisätä muokkausmoodissa", L"Virhe", MB_OK);
		return;
		}
	kilpno = _wtoi(EId->Text.c_str());
	if (CSarja->ItemIndex < 0) {
		Application->MessageBox(L"Sarja puuttuu", L"Virhe",	MB_OK);
		return;
		}
	if (RGLisays->ItemIndex == 1) {
		if (kilpno == 0)
			kilpno = haeseurvapaa(Sarjat[CSarja->ItemIndex].bibalku[k_pv]);
		kilpno = haeseurvapaa(kilpno);
		}
	if (kilpno < minkilpno || kilpno > maxkilpno) {
		Application->MessageBox(L"Kilpailunumero virheellinen", L"Virhe", MB_OK);
		return;
		}
	if ((d = getpos(kilpno)) > 0 && d != Dk && d != DVKilp) {
		Application->MessageBox(L"Kilpailunumero varattu", L"Virhe", MB_OK);
		return;
		}
	if (ilmkentat > 1 && CBNimiHThaku->Checked) {
		bdg = _wtoi(EBadge1->Text.c_str());
		if (!on_lainakortti(bdg) && HTBadge != 0 && bdg != HTBadge && bdg > 11000 &&
			_wtoi(ELisno->Text.c_str()) != 0) {
			if (Application->MessageBox(L"Vaihdetaanko tietokannan Emit-koodi?", L"Pysyvä muutos?",
				MB_YESNO) != IDYES) {
				Kilp.pv[k_pv].laina[1] = L'L';
				}
			}
		}
	if (kysy) {
		if (Application->MessageBox(L"Vahvista tallennus", L"Vahvista",
			MB_YESNO) != IDYES)
		return;
		}
	if (DVKilp > 0 && d == DVKilp) {
		VKilp.pv[k_pv].bib = annavakantinbib(&VKilp, k_pv);
		VKilp.set_tark(L'V', k_pv);
		VKilp.pv[k_pv].badge[0] = 0;
		EnterCriticalSection(&tall_CriticalSection);
		VKilp.tallenna(DVKilp, 0, 0, 0, 0);
		LeaveCriticalSection(&tall_CriticalSection);
		}
	DVKilp = 0;
	Kilp.setSarja(CSarja->ItemIndex);
	for (int is = -1; is < kilpparam.n_pv_akt; is++) {
		if ((is == -1 || is > k_pv) && Sarjat[Kilp.Sarja(is)].vakanttisarja)
			Kilp.setSarja(CSarja->ItemIndex, is);
		}
	Kilp.setId(kilpno);
	if (CBSukup->Checked || CBSarja->Checked) {
		if (ESukup->Text.Length() > 0)
			Kilp.sukup = towupper(ESukup->Text.c_str()[0]);
		else
			Kilp.sukup = 0;
		}
	if (CBIkasrj->Checked || CBSarja->Checked) {
		Kilp.ikasarja = _wtoi(EIkasrj->Text.c_str());
		if (Kilp.ikasarja == 0) {
			switch (towupper(EIkasrj->Text.c_str()[0])) {
				case L'Y':
				   Kilp.ikasarja = 1;
				   break;
				case L'A':
				   Kilp.ikasarja = 2;
				   break;
				}
			}
		}
	if (CBLisno->Checked || CBLisnoTark->Checked)
		Kilp.lisno[0] = _wtoi(ELisno->Text.c_str());
	if (CBLisno->Checked)
		Kilp.lisno[1] = _wtoi(EWRKoodi->Text.c_str());
//	wcsncpy(Kilp.wrkoodi, EWRKoodi->Text.c_str(), sizeof(Kilp.wrkoodi)/2-1);
	if (CBSNimi->Checked)
		wcsncpy(Kilp.sukunimi, ESNimi->Text.c_str(),kilpparam.lsnimi);
	if (CBENimi->Checked)
		wcsncpy(Kilp.etunimi, EENimi->Text.c_str(),kilpparam.lenimi);
	if (CBArvo->Checked)
		wcsncpy(Kilp.arvo, EArvo->Text.c_str(),kilpparam.larvo);
	if (CBSeura->Checked || CBSeurahaku->Checked)
		wcsncpy(Kilp.seura, ESeura->Text.c_str(),kilpparam.lseura);
	if (CBSeuralyh->Checked || CBSeurahaku->Checked)
		wcsncpy(Kilp.seuralyh, ESeuralyh->Text.c_str(),kilpparam.lseuralyh);
	if (CBYhdistys->Checked)
		wcsncpy(Kilp.yhdistys, EYhdistys->Text.c_str(), kilpparam.lyhdistys);
	if (CBJoukkue->Checked)
		wcsncpy(Kilp.joukkue, EJoukkue->Text.c_str(), kilpparam.ljoukkue);
	if (CBMaa->Checked || CBSeurahaku->Checked)
		wcsncpy(Kilp.maa, EMaa->Text.c_str(),3);
	if (CBPiiri->Checked || CBSeurahaku->Checked)
		Kilp.piiri = _wtoi(EPiiri->Text.c_str());
	for (int i = Sarjat[Kilp.Sarja()].enspv; i <= Sarjat[Kilp.Sarja()].viimpv ; i++) {
		wchar_t st[20];
		if (Sarjat[Kilp.Sarja()].luonne[i] < 0)
			continue;
		if (CBLasna->Checked) {
			if (wcswcind(L'1'+i, ELasna->Text.c_str()) >= 0)
				Kilp.set_tark(L'-', i);
			else
				Kilp.set_tark(L'P', i);
			}
		else if (i == k_pv && CBMerkAvoimeksi->Checked)
			Kilp.set_tark(L'-', i);
		if (CBBib->Checked)
			Kilp.pv[i].bib = _wtoi(EBib[i]->Text.c_str());
		if (CBBadge->Checked || CBHTBadge->Checked)
			Kilp.pv[i].badge[0] = _wtoi(EBadge[i]->Text.c_str());
		if (ilmkentat > 1 && on_lainakortti(Kilp.pv[i].badge[0]))
			Kilp.pv[i].laina[1] = L'L';
		if (CBJIilm->Checked) {
			if (CBLA->Checked) {
				Sarjat[Kilp.Sarja()].ensjlahto[i] -= SEK*Sarjat[Kilp.Sarja()].lahtovali[i];
				Kilp.tall_lajat_pv(Sarjat[Kilp.Sarja()].ensjlahto[i], i);
				}
			if (CBJilm2->Checked)
				Kilp.ilmlista = 999;
			}
		else {
			if (CBLA->Checked)
				Kilp.tall_lajat_pv(wstrtoaika_vap(ELA[i]->Text.c_str(), t0), i);
			else if (i == k_pv && CBAutoLaika->Checked) {
				Kilp.tall_lajat_pv(Sarjat[Kilp.Sarja()].enslahto[i]+
					SEK*Sarjat[Kilp.Sarja()].lahtovali[i]*(Kilp.pv[i].bib-Sarjat[Kilp.Sarja()].bibalku[i]), i);
				}
			}
		if (CBTas->Checked)
			Kilp.pv[i].tasoitus = _wtoi(ETas[i]->Text.c_str());
		if (CBRata->Checked)
			wcsncpy(Kilp.pv[i].rata, ERata[i]->Text.c_str(), sizeof(Kilp.pv[i].rata)/2-1);
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = 0; i < Sarjat[Kilp.Sarja(ipv)].lsak[ipv]; i++) {
			if (Kilp.pv[ipv].asakot[i] == 0)
				Kilp.pv[ipv].asakot[i] = L'-';
			}
		}
	EnterCriticalSection(&tall_CriticalSection);
	if (RGLisays->ItemIndex == 0) {
		Kilp.tallenna(Dk, 0, 0, 0, 0);
		}
	else {
		Kilp.addtall(&Dk, 0);
		}
	LeaveCriticalSection(&tall_CriticalSection);
	naytaTiedot(Kilp.id());
	edkno = Kilp.id();
	if (CBSeuralisa->Checked && CBSeurahaku->Checked && FormSeurat->SeuraHakuOn()) {
		FormSeurat->Caller = FormIlm2;
		FormSeurat->TallennaSeura(Kilp.piiri, Kilp.maa, Kilp.seuralyh, Kilp.seura, 0);
	}
	if (CBJIilm->Checked && CBLA->Checked) {
		kirjEventXml(sarjaxmlfname, true, false, false);
		}
	Kilp.nollaa();
	tyhjennaKentat();
	if (ilmkentat == 2 || ilmkentat == 3) {
		int kno;

		kno = haeSeurVakantti(0);
		SaveEdits();
		if (kno) {
		   Dk = getpos(kno);
		   Kilp.GETREC(Dk);
		   naytaKilpailija();
		   }
		lueSeuraavaEmit();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::tyhjennaKentat(void)
{
	LblMoodi->Caption = RGLisays->ItemIndex == 0 ? L"Muokkausmoodi" : L"Lisäysmoodi";
	CSarja->ItemIndex = -1;
	ESukup->Text = UnicodeString(L"");
	EIkasrj->Text = UnicodeString(L"");
	EId->Text = UnicodeString(L"");
	EKilpno->Text = UnicodeString(L"");
	ELisno->Text = UnicodeString(L"");
	EWRKoodi->Text = UnicodeString(L"");
	ESNimi->Text = UnicodeString(L"");
	EENimi->Text = UnicodeString(L"");
	EArvo->Text = UnicodeString(L"");
	ESeura->Text = UnicodeString(L"");
	ESeuralyh->Text = UnicodeString(L"");
	EYhdistys->Text = UnicodeString(L"");
	EJoukkue->Text = UnicodeString(L"");
	EMaa->Text = UnicodeString(L"");
	EPiiri->Text = UnicodeString(L"");
	ELasna->Text = UnicodeString(L"");
	for (int i = 0; i < 5; i++) {
		EBadge[i]->Text = UnicodeString(L"");
		ELA[i]->Text = UnicodeString(L"");
		ETas[i]->Text = UnicodeString(L"");
		ERata[i]->Text = UnicodeString(L"");
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::naytaKilpailija(void)
{
	int srj;

	if (NimiHTHaku == -2) {
		CBNimiHThaku->Checked = false;
		NimiHTHaku = 0;
		}
	if (seurahaku == -2) {
        CBSeurahaku->Checked = false;
		seurahaku = 0;
		}
	tyhjennaKentat();
	srj = Kilp.Sarja();
	if (srj < 0)
	   srj = 0;
	CSarja->ItemIndex = Kilp.Sarja();
	EId->Text = UnicodeString(Kilp.id());
	ESukup->Text = UnicodeString(Kilp.sukup);
	switch (Kilp.ikasarja) {
		case 0:
			EIkasrj->Text = L"";
			break;
		case 1:
			EIkasrj->Text = L"YL";
			break;
		case 2:
			EIkasrj->Text = L"AV";
			break;
		default:
			EIkasrj->Text = Kilp.ikasarja;
		}
	EKilpno->Text = UnicodeString(Kilp.kilpno);
	ELisno->Text = UnicodeString(Kilp.lisno[0]);
	EWRKoodi->Text = UnicodeString(Kilp.lisno[1]);
	ESNimi->Text = UnicodeString(Kilp.sukunimi);
	EENimi->Text = UnicodeString(Kilp.etunimi);
	EArvo->Text = UnicodeString(Kilp.arvo);
	ESeura->Text = UnicodeString(Kilp.seura);
	ESeuralyh->Text = UnicodeString(Kilp.seuralyh);
	EYhdistys->Text = UnicodeString(Kilp.yhdistys);
	EJoukkue->Text = UnicodeString(Kilp.joukkue);
	EMaa->Text = UnicodeString(Kilp.maa);
	EPiiri->Text = UnicodeString(Kilp.piiri);
	ELasna->Text = UnicodeString(L"");
	for (int i = Sarjat[srj].enspv; i <= Sarjat[srj].viimpv ; i++) {
		wchar_t st[20];
		if (Sarjat[srj].luonne[i] < 0)
			continue;
		if (Kilp.tark(i) != L'P')
			ELasna->Text = ELasna->Text+UnicodeString(i+1);
		EBib[i]->Text = UnicodeString(Kilp.pv[i].bib);
		EBadge[i]->Text = UnicodeString(Kilp.pv[i].badge[0]);
		ELA[i]->Text = UnicodeString(AIKATOWSTRS(st, Kilp.pv[i].tlahto, t0)).SubString(1,8);
		ETas[i]->Text = UnicodeString(Kilp.pv[i].tasoitus);
		ERata[i]->Text = UnicodeString(Kilp.pv[i].rata);
		}
}

void __fastcall TFormIlm2::EIdKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	int d, kno;

	SeuraKeyHit = 0;
	NimiKeyHit = false;
	EdKey = Key + (Shift.Contains(ssShift) ? 0x10000 : 0) + (Shift.Contains(ssCtrl) ? 0x20000 : 0)
		+ (Shift.Contains(ssAlt) ? 0x40000 : 0);
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(TabControls[0]);
		return;
		}
	if (CBKnohaku->Checked) {
		if ((Key == VK_RETURN || Key == VK_F6)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			if (Key == VK_RETURN && NextTabControl(Sender)) {
				try {
					FocusControl(NextTabControl(Sender));
					}
				catch(...) {
					}
				}
			Key = (Key == VK_RETURN) ? VK_TAB : 0;
			kno = _wtoi(EId->Text.c_str());
			if (kno == 0)
				kno = edkno;
			}
		else if ((Key == VK_UP || Key == VK_F5)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			kno = _wtoi(EId->Text.c_str());
			if (kno == 0)
				kno = edkno;
			kno--;
			}
		else if ((Key == VK_DOWN || Key == VK_F7)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			kno = _wtoi(EId->Text.c_str());
			if (kno == 0)
				kno = edkno;
			kno++;
			}
		else {
			if (Key == VK_RETURN && NextTabControl(Sender))
				FocusControl(NextTabControl(Sender));
			return;
			}
		d = getpos(kno);
		naytaTiedot(kno);
		edkno = kno;
		if (d <= 0) {
			Application->MessageBox(L"Pyydetty kilpailijanumero ei ole käytössä", L"Ei löydy",
				MB_OK);
			}
		else {
			Dk = d;
			Kilp.GETREC(Dk);
			DVKilp = 0;
			if (CBMerkAvoimeksi->Checked) {
				if (ToimintaTila == 2) {
						Kilp.set_tark(L'-', k_pv);
					}
				else
					for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
						Kilp.set_tark(L'-', ipv);
                }
			naytaKilpailija();
			SaveEdits();
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	EdKey = Key + (Shift.Contains(ssShift) ? 0x10000 : 0) + (Shift.Contains(ssCtrl) ? 0x20000 : 0)
		+ (Shift.Contains(ssAlt) ? 0x40000 : 0);
	if ((Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) ||
		(Key == VK_F8 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))) {
		tallennaKilpailija(true);
		FocusControl(TabControls[0]);
		}
	else if (Key == VK_RETURN && NextTabControl(Sender))
		FocusControl(NextTabControl(Sender));
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::BtnTallennaClick(TObject *Sender)
{
	tallennaKilpailija(true);
	if (ilmkentat <= 1)
		FocusControl(TabControls[0]);
}
//---------------------------------------------------------------------------


void __fastcall TFormIlm2::CBLisnohakuClick(TObject *Sender)
{
	if (CBLisnohaku->Checked) {
		FormTKanta->Show();
		FormTKanta->Caller = FormIlm2;
		FormTKanta->Left = Left+Width;
		FormTKanta->Top = 20;
		if (FormTKanta->WindowState == wsMinimized)
			FormTKanta->WindowState = wsNormal;
		FormTKanta->BringToFront();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::PalaaSeuraHausta(bool tallenna)
{
	if (tallenna) {
		SiirraSeuraTiedot(0);
		EAktSeura->SetFocus();
//		FocusControl(NextTabControl(this));
		}
	BringToFront();
	return;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::SiirraSeuraTiedot(int haku)
{
	int KSeura;
	wchar_t nimi[LSEURA+1], lyhenne[LLYH+1], maa[4];
	int piiri;

	if (FormSeurat->SeuraHakuOn()) {
		if (haku) {
			FormSeurat->Caller = FormIlm2;
			FormSeurat->BringToFront();
			FormSeurat->Selaa(haku);
			return;
			}
		KSeura = FormSeurat->Seuratiedot(haku, &piiri, maa, lyhenne, nimi);
		if (KSeura >= 0) {
			EPiiri->Text = UnicodeString(piiri);
			ESeura->Text = UnicodeString(nimi);
			ESeuralyh->Text = UnicodeString(lyhenne);
			if (maa[0])
				EMaa->Text = UnicodeString(maa);
			}
		else if (!((ESeura->Focused() || ESeuralyh->Focused()) && (EdKey & 0xffff) == VK_RETURN)) {
			EPiiri->Text = UnicodeString(0);
			if (haku)
				ESeura->Text = L"";
			else
				ESeuralyh->Text = L"";
			EMaa->Text = L"";
			}
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormIlm2::CBSeurahakuClick(TObject *Sender)
{
	if (CBSeurahaku->Checked) {
		FormSeurat->Show();
		FormSeurat->Caller = FormIlm2;
		FormSeurat->Left = Left+Width;
		FormSeurat->Top = 300*Screen->PixelsPerInch/96;
		if (FormSeurat->WindowState == wsMinimized)
			FormSeurat->WindowState = wsNormal;
		FormSeurat->BringToFront();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::ESeuraKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	bool ESeuraFocused, ESeuralyhFocused;

	SeuraKeyHit = 1;
	NimiKeyHit = false;

	EdKey = Key + (Shift.Contains(ssShift) ? 0x10000 : 0) + (Shift.Contains(ssCtrl) ? 0x20000 : 0)
		+ (Shift.Contains(ssAlt) ? 0x40000 : 0);
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(TabControls[0]);
		return;
		}
	ESeuraFocused = ESeura->Focused();
	ESeuralyhFocused = ESeuralyh->Focused();
	if (CBSeurahaku->Checked && ((!kilpparam.seuralyh_on && ESeura->Focused()) ||
		(kilpparam.seuralyh_on && ESeuralyh->Focused()))) {
		if (Key == 83 && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {   // Ctrl-S
			FormSeurat->Caller = FormIlm2;
			FormSeurat->TallennaSeura(_wtoi(EPiiri->Text.c_str()), EMaa->Text.c_str(),
				ESeuralyh->Text.c_str(), ESeura->Text.c_str(), 0);
			Key = 0;
			return;
			}
		if ((Key == VK_RETURN || Key == VK_F6)
//		if ((Key == VK_F6)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			if (Key == VK_RETURN && NextTabControl(Sender))
				FocusControl(NextTabControl(Sender));
			Key = (Key == VK_RETURN) ? VK_TAB : 0;
			SeuraKeyHit = 0;
			SiirraSeuraTiedot(0);
			}
		else if ((Key == VK_UP || Key == VK_F5)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			SeuraKeyHit = 0;
			SiirraSeuraTiedot(-1);
			}
		else if ((Key == VK_DOWN ||	Key == VK_F7)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			SeuraKeyHit = 0;
			SiirraSeuraTiedot(1);
			}
		}
//	else
		{
		if (Key == VK_RETURN && NextTabControl(Sender))
			FocusControl(NextTabControl(Sender));
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::TabSheet1Show(TObject *Sender)
{
	int VTop = 19;
	int VVali = 21;
	int TE = 1, TO = 1, TLisa = 0, ILisa = 5, YSrj = 0;
	TEdit *Edt;
	TLabel *Lbl;

	SheetHeight = 0;
	memset(TabControls, 0, sizeof(TabControls));
	if (!Edits[4].CB->Checked) {
		Edits[5].CB->Checked = false;
		Edits[6].CB->Checked = false;
		}
	if (Edits[5].CB->Checked) {
		TLisa++;
		Edits[5].EOrd->Text = _wtoi(Edits[4].EOrd->Text.c_str()) + TLisa;
		ILisa = _wtoi(Edits[5].EOrd->Text.c_str());
		}
	if (Edits[6].CB->Checked) {
		TLisa++;
		Edits[6].EOrd->Text = _wtoi(Edits[4].EOrd->Text.c_str()) + TLisa;
		ILisa = _wtoi(Edits[6].EOrd->Text.c_str());
		}
	for (int i = 0; i < 21; i++) {
		EPos[i][1] = i+1;
		if (Edits[i].CB->Checked) {
			EPos[i][0] = _wtoi(Edits[i].EOrd->Text.c_str());
			EPos[i][2] = EPos[i][0];
			}
		else {
			EPos[i][0] = 0;
			EPos[i][2] = 0;
			}
		}
	qsortint(EPos, 20, sizeof(EPos[0]), 0, 1);
	for (int i = 0; i < 21; i++) {
		if (EPos[i][0]) {
		   if (EPos[i][1] == 5)
			  YSrj = TE;
		   if (EPos[i][1] == 6 || EPos[i][1] == 7) {
			  EPos[i][0] = YSrj;
			  }
		   else
			  EPos[i][0] = TE++;
		   EPos[i][2] = TO;
		   TO += (Edits[EPos[i][1]].Count > 1) ? kilpparam.n_pv_akt : 1;
			}
		}
	for (int i = 0; i < 21; i++) {
		if (EPos[i][1] == 5) {
			if (EPos[i][0] > 0) {
				CSarja->Top = VTop + (EPos[i][0]-1)*VVali;
				CSarja->Visible = true;
				CSarja->Enabled = true;
				CSarja->TabOrder = EPos[i][2]-1;
				TabControls[EPos[i][2]-1] = CSarja;
				CSarja->TabStop = true;
				LSarja->Top = VTop + (EPos[i][0]-1)*VVali + 3;
				LSarja->Visible = true;
				LSarja->Enabled = true;
				if (SheetHeight < CSarja->Top)
					SheetHeight = CSarja->Top;
				}
			else {
				CSarja->Visible = false;
				CSarja->Enabled = false;
				CSarja->TabStop = false;
				LSarja->Visible = false;
				LSarja->Enabled = false;
				}
			}
		else if (Edits[EPos[i][1]-1].Count > 1) {
			for (int Pv = 0; Pv < Edits[EPos[i][1]-1].Count; Pv++) {
				Edt = ((TEdit **)Edits[EPos[i][1]-1].Edt)[Pv];
				if (Pv < kilpparam.n_pv_akt && EPos[i][0] > 0) {
					Edt->Top = VTop + (EPos[i][0]-1)*VVali;
					Edt->Visible = true;
					Edt->Enabled = true;
					Edt->TabOrder = EPos[i][2]+Pv-1;
					TabControls[EPos[i][2]-1] = Edt;
					Edt->TabStop = true;
					if (SheetHeight < Edt->Top)
						SheetHeight = Edt->Top;
					}
				else {
					Edt->Visible = false;
					Edt->Enabled = false;
					Edt->TabStop = false;
					}
				}
			Lbl = Edits[EPos[i][1]-1].Lbl;
			if (EPos[i][0] > 0) {
				Lbl->Top = VTop + (EPos[i][0]-1)*VVali + 3;
				Lbl->Visible = true;
				Lbl->Enabled = true;
				}
			else {
				Lbl->Visible = false;
				Lbl->Enabled = false;
				}
			}
		else {
			Edt = Edits[EPos[i][1]-1].Edt;
			Lbl = Edits[EPos[i][1]-1].Lbl;
			if (EPos[i][0] > 0) {
				if (Edt) {
					Edt->Top = VTop + (EPos[i][0]-1)*VVali;
					Edt->Visible = true;
					Edt->Enabled = true;
					Edt->TabOrder = EPos[i][2]-1;
					TabControls[EPos[i][2]-1] = Edt;
					Edt->TabStop = true;
					if (SheetHeight < Edt->Top)
						SheetHeight = Edt->Top;
					}
				if (Lbl) {
					Lbl->Top = VTop + (EPos[i][0]-1)*VVali + 3;
					Lbl->Visible = true;
					Lbl->Enabled = true;
					}
				}
			else {
				if (Edt) {
					Edt->Visible = false;
					Edt->Enabled = false;
					Edt->TabStop = false;
					}
				if (Lbl) {
					Lbl->Visible = false;
					Lbl->Enabled = false;
					}
				}
			}
		if (i == 0) {
			EKilpno->Top = VTop + (EPos[i][0]-1)*VVali;
//						EKilpno->Visible = (ToimintaTila == 2);
//						LPysyva->Visible = (ToimintaTila == 2);
//						EKilpno->Enabled = (ToimintaTila == 2);
			}
		}
	BtnEiBadge->Top = EBadge1->Top;
	BtnTallenna->TabOrder = TO-1;
	TabControls[TO-1] = BtnTallenna;
	if (ilmkentat == 2 || ilmkentat == 3)
		EBadge1->TabOrder = 0;
	SheetHeight += 59 * Screen->PixelsPerInch / 96;
	FormSetSize(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::RGLisaysClick(TObject *Sender)
{
	CBJarjestys->ItemIndex = RGLisays->ItemIndex == 1 ? 4 : 0;
	if (RGLisays->ItemIndex == 1) {
		CBKnohaku->Checked = false;
	}
	LblMoodi->Caption = RGLisays->ItemIndex == 0 ? L"Muokkausmoodi" : L"Lisäysmoodi";
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::ELisnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	EdKey = Key + (Shift.Contains(ssShift) ? 0x10000 : 0) + (Shift.Contains(ssCtrl) ? 0x20000 : 0)
		+ (Shift.Contains(ssAlt) ? 0x40000 : 0);
	if ((Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) ||
		(Key == VK_F8 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))) {
		tallennaKilpailija(true);
		FocusControl(TabControls[0]);
		}
	if (CBLisnohaku->Checked) {
		if (Key == VK_RETURN) {
			int lisno;
			if ((lisno = _wtoi(ELisno->Text.c_str())) != 0 && FormTKanta->EtsiHenkilo(lisno)) {
				FormTKanta->Caller = FormIlm2;
				SiirraHTTiedot(0);
				if (NextTabControl(Sender))
					FocusControl(NextTabControl(Sender));
				}
			NimiKeyHit = false;
			}
		}
	else if (Key == VK_RETURN && NextTabControl(Sender))
		FocusControl(NextTabControl(Sender));
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::EnterKeyPress(TObject *Sender, System::WideChar &Key)
{

	if (Key < L' ' && Key != 8) {
	   Key = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CSarjaChange(TObject *Sender)
{
	if (CBAutoLaika->Checked) {
		wchar_t as[20];
		if (CSarja->ItemIndex >= 0) {
			int tl = Sarjat[CSarja->ItemIndex].enslahto[k_pv]+
				SEK*Sarjat[CSarja->ItemIndex].lahtovali[k_pv]*(Kilp.pv[k_pv].bib-Sarjat[CSarja->ItemIndex].bibalku[k_pv]);
			UnicodeString tlStr = UnicodeString(AIKATOWSTRS(as, tl, t0));

			ELA[k_pv]->Text = tlStr;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::ESeuraChange(TObject *Sender)
{
	if ((ESeura->Focused() || ESeuralyh->Focused()) && (EdKey == VK_DOWN || EdKey == VK_UP || EdKey == VK_F5 || EdKey == VK_F7))
		return;
	if (CBSeurahaku->Checked && FormSeurat->SeuraHakuOn()) {
		FormSeurat->Caller = FormIlm2;
		if (!kilpparam.seuralyh_on && ESeura->Focused()) {
			LSe = ESeura->Text.Length();
			FormSeurat->NaytaSeurat(ESeura->Text.SubString(1, LSe).c_str(),
				(ESeura->Text.SubString(1, LSe)+UnicodeString(L"ööö")).c_str());
			}
		if (kilpparam.seuralyh_on && ESeuralyh->Focused()) {
			LSe = ESeuralyh->Text.Length();
			FormSeurat->NaytaSeurat(ESeuralyh->Text.SubString(1, LSe).c_str(),
				(ESeuralyh->Text.SubString(1, LSe)+UnicodeString(L"ööö")).c_str());
			}
		}
}

//---------------------------------------------------------------------------
void __fastcall TFormIlm2::ESeuraEnter(TObject *Sender)
{
	EAktSeura = (TEdit *)Sender;
	if (!kilpparam.seuralyh_on) {
		SeuraKeyHit = -1;
		LSe = ESeura->Text.Length();
		ESeuraChange(Sender);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormIlm2::ESeuralyhEnter(TObject *Sender)
{
	EAktSeura = (TEdit *)Sender;
	if (kilpparam.seuralyh_on) {
		SeuraKeyHit = -1;
		LSe = ESeuralyh->Text.Length();
		ESeuraChange(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::ESeuraExit(TObject *Sender)
{
	SeuraKeyHit = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::BtnPeruutaClick(TObject *Sender)
{
	int kno;

	Dk = 0;
	DVKilp = 0;
	tyhjennaKentat();
	if (ilmkentat == 2 || ilmkentat == 3) {
		kno = haeSeurVakantti(0);
		SaveEdits();
		if (kno) {
		   Dk = getpos(kno);
		   Kilp.GETREC(Dk);
		   naytaKilpailija();
		   }
		lueSeuraavaEmit();
		}
	FocusControl(TabControls[0]);
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ilmoittautumiset");
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::PageControl1Enter(TObject *Sender)
{
	LblMoodi->Caption = RGLisays->ItemIndex == 0 ? L"Muokkausmoodi" : L"Lisäysmoodi";
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::TabSheet2Show(TObject *Sender)
{
	FormSetSize(1);
}
//---------------------------------------------------------------------------
void __fastcall TFormIlm2::haeKilp(int d)
{
	Dk = d;
	Kilp.GETREC(d);
	EId->Text = UnicodeString(Kilp.id());
	naytaTiedot(Kilp.id());
	DVKilp = 0;
	if (CBMerkAvoimeksi->Checked)
		Kilp.set_tark(L'-', k_pv);
	naytaKilpailija();
	SaveEdits();
}
//---------------------------------------------------------------------------
void KirjaaEmit(int badge, int srj)
{
	wchar_t as[20];

	FormIlm2->EBadge1->Text = UnicodeString(badge);
	if (ilmkentat == 4 || ilmkentat == 5) {
		int d;

		if ((d = bdg2kno(badge)) > 0) {
			FormIlm2->haeKilp(d);
			FormIlm2->FocusControl(FormIlm2->ESNimi);
			}
		return;
		}
	if (ilmkentat == 3 && srj < sarjaluku)
		FormIlm2->CSarja->ItemIndex = srj;
	FormIlm2->ELA1->Text = UnicodeString(aikatowstr_cols_n(as, KSEK*T_TIME(biostime(0, 0)), t0, L':', 8));
	if (FormTKanta->HenkiloHakuOn()) {
		FormTKanta->Caller = FormIlm2;
		FormTKanta->NaytaHenkilot(NULL, NULL, NULL, NULL, badge);
		if (FormIlm2->CBSeurahaku->Checked)
			FormSeurat->BringToFront();
		FormTKanta->BringToFront();
		}
	FormIlm2->BringToFront();
	FormIlm2->EBadge1->Color = clLime;
	FormIlm2->ActiveControl = FormIlm2->EBadge1;
	FormIlm2->FocusControl(FormIlm2->EBadge1);
	UusiEmit = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::BtnEiBadgeClick(TObject *Sender)
{
	wchar_t as[20];

//	for (; haebdg(seurVapaaBadge) >= 0; seurVapaaBadge++) ;
	EBadge1->Text = UnicodeString(haeSeurVapaaBadge(0));
	FormIlm2->ELA1->Text = UnicodeString(aikatowstr_cols_n(as, KSEK*T_TIME(biostime(0, 0)), t0, L':', 8));
	FocusControl(ESNimi);
}
//---------------------------------------------------------------------------


void __fastcall TFormIlm2::IlmGridMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	if (IlmGrid->RowCount - IlmGrid->FixedRows >= IlmGrid->VisibleRowCount + IlmGrid->TopRow)
		IlmGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::IlmGridMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	if (IlmGrid->TopRow > IlmGrid->FixedRows)
		IlmGrid->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBSukupClick(TObject *Sender)
{
	ESukup->Enabled = CBSarja->Checked && CBSukup->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::CBIkasrjClick(TObject *Sender)
{
	EIkasrj->Enabled = CBSarja->Checked && CBIkasrj->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormIlm2::EdtVakBibLisaChange(TObject *Sender)
{
	vakbiblisa = _wtoi(EdtVakBibLisa->Text.c_str());
	EdtVakBibLisa->Text = vakbiblisa;
}
//---------------------------------------------------------------------------



void __fastcall TFormIlm2::EditNoEnter(TObject *Sender)
{
	if (_wtoi(((TEdit *)Sender)->Text.c_str()) == 0)
		((TEdit *)Sender)->SelectAll();
}
//---------------------------------------------------------------------------



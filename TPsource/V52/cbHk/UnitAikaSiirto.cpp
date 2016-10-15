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

#include "UnitAikaSiirto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAikaSiirto *FormAikaSiirto;
//---------------------------------------------------------------------------
__fastcall TFormAikaSiirto::TFormAikaSiirto(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormAikaSiirto::LBSarjatDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjat->Items->Strings[LBSarjat->ItemIndex].c_str(), false);
	if (srj >= 0)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------


void __fastcall TFormAikaSiirto::LBSarjaValDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjaVal->Items->Strings[LBSarjaVal->ItemIndex].c_str(), false);
	if (srj >= 0)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::BtnLisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::BtnPoisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::BtnLisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjat->Items->Count; rv++) {
		if (LBSarjat->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjat->Items->Strings[rv].c_str(), false)) >= 0)
				SarjaValinnat[srj] = true;
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::BtnPoisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjaVal->Items->Count; rv++) {
		if (LBSarjaVal->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjaVal->Items->Strings[rv].c_str(), false)) >= 0)
				SarjaValinnat[srj] = false;
			}
		}
	NaytaSarjavalinnat();
}//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::FormResize(TObject *Sender)
{
	LBSarjat->Height = ClientHeight - LBSarjat->Top - 8;
	LBSarjaVal->Height = ClientHeight - LBSarjaVal->Top - 8;
}
//---------------------------------------------------------------------------
void TFormAikaSiirto::NaytaSarjavalinnat(void)
{
	LBSarjat->Clear();
	LBSarjaVal->Clear();
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (SarjaValinnat[srj])
			LBSarjaVal->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		else
			LBSarjat->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::Button1Click(TObject *Sender)
{
	int i_pv, j_pv, kohde, d, epv, vpv, siirto, ero = 0, lkm = 0;
	kilptietue kilp;

	i_pv = CBVaiheet->ItemIndex - 1;
	j_pv = CBKohdeVaihe->ItemIndex;
	kohde = RGKohde->ItemIndex;
	if (kohde < 0)
		return;
	if (ToimintaTila == 2 && kohde == 5 && i_pv < k_pv+1) {
		Application->MessageBoxW(L"Tulospalvelutilassa voidaan siirtää vain myöhempiä numeroita", L"Rajoitus", MB_OK);
		return;
		}
	if (i_pv >= 0 && !pvparam[i_pv].hiihtolahto && (kohde == 2 || kohde == 3)) {
		Application->MessageBoxW(L"Vaihtoehto ei käytössä", L"Rajoitus", MB_OK);
		return;
		}
	if (kohde == 1)
		siirto = wstrtoaika_vap(EdtSiirto->Text.c_str(), t0);
	else
		siirto = _wtoi(EdtSiirto->Text.c_str());
	if (i_pv >= -1 && i_pv < kilpparam.n_pv_akt && kohde <= 5) {
		if (i_pv < 0) {
			epv = 0;
			vpv = kilpparam.n_pv_akt - 1;
			}
		else {
			epv = i_pv;
			vpv = i_pv;
			}
		for (d = 0; d < nrec; d++) {
			kilp.GETREC(d);
			if (kilp.kilpstatus)
				continue;
			if (kohde == 5 && i_pv < 0) {
				if (kilp.kilpno + siirto >= minkilpno && kilp.kilpno + siirto <= maxkilpno) {
					if (!SarjaValinnat[kilp.Sarja()])
						continue;
					kilp.setIdAll(kilp.kilpno + siirto);
					lkm++;
					}
				else
					continue;
				}
			for (i_pv = epv; i_pv <= vpv; i_pv++) {
				if (!SarjaValinnat[kilp.Sarja(i_pv)])
					continue;
				switch (kohde) {
					case 0:    // Arvotut lähtöajat
						if (kilp.TLahto(i_pv, false) != TMAALI0) {
							if (pvparam[i_pv].hiihtolahto &&
								kilp.TLahto(i_pv, false) != kilp.TLahto(i_pv, true)) {
								kilp.tall_lajat_pv(kilp.TLahto(i_pv, false)+siirto*SEK, i_pv, false);
								}
							else {
								kilp.tall_lajat_pv(kilp.TLahto(i_pv, false)+siirto*SEK, i_pv, true);
								if (i_pv == k_pv && RGTulos->ItemIndex == 1) {
									for (int iva = 0; iva <= Sarjat[kilp.Sarja()].valuku[k_pv]; iva++) {
										if (kilp.p_aika(iva) != 0)
											kilp.tall_tulos(iva, kilp.p_aika(iva)-siirto*SEK);
										}
									}
								}
							lkm++;
							}
						break;
					case 1:    // Aseta lähtöajat
						ero = 0;
						if (kilp.TLahto(i_pv) != TMAALI0)
							ero = siirto - kilp.TLahto(i_pv);
						if (pvparam[i_pv].hiihtolahto &&
							kilp.TLahto(i_pv, false) != kilp.TLahto(i_pv, true)) {
							kilp.tall_lajat_pv(siirto, i_pv, false);
							}
						else {
							kilp.tall_lajat_pv(siirto, i_pv, true);
							if (i_pv == k_pv && RGTulos->ItemIndex == 1) {
								for (int iva = 0; iva <= Sarjat[kilp.Sarja()].valuku[k_pv]; iva++) {
									if (kilp.p_aika(iva) != 0)
										kilp.tall_tulos(iva, kilp.p_aika(iva)-ero);
									}
								}
							}
						lkm++;
						break;
					case 2:     // Toteutunut lähtöaika
						if (i_pv == k_pv && kilp.TLahto(i_pv, true) != TMAALI0) {
							kilp.tall_tulos(-1, kilp.TLahto(i_pv, true) + siirto);
							if (RGTulos->ItemIndex == 1) {
								for (int iva = 0; iva <= Sarjat[kilp.Sarja()].valuku[k_pv]; iva++) {
									if (kilp.p_aika(iva) != 0)
										kilp.tall_tulos(iva, kilp.p_aika(iva)-siirto);
									}
								}
							lkm++;
							}
						break;
					case 3:     // Toteutunut lähtöaika arvotuksi
						kilp.tall_lajat_pv(kilp.TLahto(i_pv, false), i_pv, true);
						lkm++;
						break;
					case 4:    // Tulos
						if (kilp.p_aika(0) != 0) {
							kilp.tall_tulos(0, kilp.p_aika(0) + siirto);
							lkm++;
							}
						break;
					case 5:
						if (kilp.id(i_pv) + siirto >= minkilpno && kilp.id(i_pv) + siirto <= maxkilpno) {
							kilp.setId(kilp.id(i_pv) + siirto, i_pv);
							lkm++;
							}
						break;
					}
				}
			kilp.tallenna(d, 0, 0, 0, 0);
			}
		}
	if (kohde == 6 &&
		i_pv >= 0 && j_pv >= 0 && i_pv != j_pv) {
		for (d = 0; d < nrec; d++) {
			kilp.GETREC(d);
			if (kilp.kilpstatus)
				continue;
			if (!SarjaValinnat[kilp.Sarja(i_pv)])
				continue;
			kilp.pv[j_pv] = kilp.pv[i_pv];
			lkm++;
			kilp.tallenna(d, 0, 0, 0, 0);
			}
		}
	Application->MessageBoxW((L"Tehty "+UnicodeString(lkm)+L" siirtoa").c_str(), L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::FormShow(TObject *Sender)
{
	CBVaiheet->Clear();
	CBVaiheet->Items->Add(L"Kaikki vaiheet");
	for (int i = 0;i < kilpparam.n_pv_akt; i++) {
	   CBVaiheet->Items->Add(UnicodeString(i+1)+L". vaihe");
		}
	CBVaiheet->ItemIndex = 0;
	CBKohdeVaihe->Clear();
	for (int i = 0;i < kilpparam.n_pv_akt; i++) {
	   CBKohdeVaihe->Items->Add(UnicodeString(i+1)+L". vaihe");
		}
	CBKohdeVaihe->ItemIndex = 1;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::RGKohdeClick(TObject *Sender)
{
	CBVaiheet->ItemIndex = k_pv+1;
	switch (RGKohde->ItemIndex) {
		case 0:       // Arvotut lähtöajat
			Label3->Visible = true;
			Label3->Caption = L"Siirto sekunteina";
			EdtSiirto->Visible = true;
			EdtSiirto->Text = L"0";
			RGTulos->Visible = true;
			RGTulos->Enabled = true;
			Label4->Visible = false;
			CBKohdeVaihe->Visible = false;
			break;
		case 1:        //  Aseta lähtöajat
			Label3->Caption = L"Uusi lähtöaika kaikille";
			EdtSiirto->Text = L"00:00:00";
			Label3->Visible = true;
			EdtSiirto->Visible = true;
			RGTulos->Visible = true;
			RGTulos->Enabled = true;
			Label4->Visible = false;
			CBVaiheet->Enabled = true;
			CBKohdeVaihe->Visible = false;
			break;
		case 2:        // Toteut. lähtöajat
			Label3->Caption = L"Siirto millisekunteina";
			Label3->Visible = true;
			EdtSiirto->Text = L"0000";
			EdtSiirto->Visible = true;
			RGTulos->Visible = true;
			RGTulos->Enabled = true;
			Label4->Visible = false;
			CBVaiheet->Enabled = false;
			CBKohdeVaihe->Visible = false;
			break;
		case 3:        //  Aseta toteutuneet lähtöajat arvotuiksi
			Label3->Caption = L"Toteutuneet lähtöajat arvotuiksi";
			Label3->Visible = true;
			EdtSiirto->Visible = false;
			RGTulos->Visible = true;
			RGTulos->Enabled = false;
			RGTulos->ItemIndex = 1;
			Label4->Visible = false;
			CBVaiheet->Enabled = true;
			CBKohdeVaihe->Visible = false;
			break;
		case 4:        // Tulokset
			Label3->Caption = L"Siirto millisekunteina";
			Label3->Visible = true;
			EdtSiirto->Visible = true;
			EdtSiirto->Text = L"0000";
			RGTulos->Visible = false;
			Label4->Visible = false;
			CBVaiheet->Enabled = false;
			CBKohdeVaihe->Visible = false;
			break;
		case 5:        // Rintanumerot
			Label3->Caption = L"Siirto";
			Label3->Visible = true;
			EdtSiirto->Visible = true;
			EdtSiirto->Text = L"0";
			RGTulos->Visible = false;
			Label4->Visible = false;
			CBVaiheet->Enabled = true;
			CBKohdeVaihe->Visible = false;
			break;
		case 6:		   // vaiheen kopiointi
			Label3->Visible = false;
			EdtSiirto->Visible = false;
			RGTulos->Visible = false;
			Label4->Visible = true;
			CBVaiheet->Enabled = true;
			CBKohdeVaihe->Visible = true;
			break;
		}
	Selostus();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Aikojen siirrot");
}
//---------------------------------------------------------------------------

void TFormAikaSiirto::Selostus(void)
{
	LblSelostus->Visible = false;
	LblSelostus->Width = 198 * Screen->PixelsPerInch / 96;
	LblSelostus->Height = 128 * Screen->PixelsPerInch / 96;
	if (CBVaiheet->ItemIndex > 0 && pvparam[CBVaiheet->ItemIndex-1].hiihtolahto) {
		if (RGKohde->ItemIndex <= 1) {
			LblSelostus->Caption = L"Valinnan mukaiset muutokset koskevat tuloksen laskennassa"
				L" käytettävää lähtöaikaa, jos se on ennen muutosta sama kuin arvottu.\n"
				L"Jos toteutuneita lähtöjä on jo kirjattu, on toteutuneet lähtöportin lähtöajat"
				L" siirrettävä ensin ja arvotut vasta sen jälkeen";
			LblSelostus->Visible = true;
			}
		}
	if (CBVaiheet->ItemIndex > 0 && !pvparam[CBVaiheet->ItemIndex-1].hiihtolahto) {
		if (RGKohde->ItemIndex == 2 || RGKohde->ItemIndex == 3) {
			LblSelostus->Caption = L"Valinta käytettävissä vain, kun lähtöportti valittu käyttöön."
				L" Muussa tapauksessa arvottua lähtöaikaa käytetään myös toteutuneena.";
			LblSelostus->Visible = true;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAikaSiirto::CBVaiheetClick(TObject *Sender)
{
	Selostus();
}
//---------------------------------------------------------------------------


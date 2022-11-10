#include<iostream>
#include<list>
#include<vector>
#include<climits>
using namespace std;
class G_Postoji {
	friend ostream& operator<<(ostream& it, const  G_Postoji& gr) {
		return it << "Vec postoji element u grafu";
	}
};
class G_Dim_Grafa {
	friend ostream& operator<<(ostream& it, const  G_Dim_Grafa& gr) {
		return it << "Nedozvoljen broj cvorova";
	}
};
class G_Petlja {
	friend ostream& operator<<(ostream& it, const  G_Petlja& gr) {
		return it << "Nije dozvoljeno napraviti petlju";
	}
};
class Grana {
	int od;
	int ka;
	int tezina = 1;
	bool izgradjen = 1;

public:

	

	Grana(int odd, int kaa) {
		od = odd;
		ka = kaa;
	}

	void postavi_izgradjen(bool i) {
		izgradjen = i;
	}

	int dohv_od() {
		return od;
	}

	int dohv_ka() {
		return ka;
	}

	int dohv_tezinu() {
		return tezina;
	}

	void postavi_tezinu(int t) {
		tezina = t;
	}

	friend ostream& operator<<(ostream& it, const Grana& gr) {
		return it << " G: (" << gr.od << "," << gr.ka << ")-T:" << gr.tezina;
	}


};



class Cvor {
	int id;
	
	list<Grana> povezane_grane;

public:
	Cvor(int i) {
		id = i;
	}

	int dohv_id() {
		return id;
	}

	list<Grana> dohv_grane() {
		return povezane_grane;
	}

	Cvor& dodaj_granu(Grana gr) {
		povezane_grane.push_back(gr);
		return *this;
	}

	Cvor& ukloni_granu(int id) {
		for (int i = 0; i < povezane_grane.size(); i++) {
			if (next(povezane_grane.begin(), i)->dohv_ka() == id)
				povezane_grane.erase(next(povezane_grane.begin(), i));
		}
		return *this;
	}

	void obrisi_cvor() {
		povezane_grane.clear();
	}

	friend ostream& operator<<(ostream& it, const Cvor& c) {
		it << "Cvor br." << c.id;
		if (!c.povezane_grane.empty()) {
			for (int i = 0; i < c.povezane_grane.size(); i++) {
				it << *next(c.povezane_grane.begin(), i);
			}

		}
		return it;
	}


};

class Graf {
	vector<Cvor> niz_cvorova;
	int duzina;
	vector<Grana> m_stablo;
	

public:
	Graf(int br_cvorova) {
		duzina = br_cvorova;
		for (int i = 0; i < br_cvorova; i++) {
			Cvor novi(i);
			niz_cvorova.push_back(novi);
		}
	}

	Graf& dodaj_cvor(int id) {
		Cvor novi(id);
		niz_cvorova.push_back(novi);
		duzina++;
		return *this;
	}
	

	Cvor* dohvati_cvor(int id) {
		for (int i = 0; i < niz_cvorova.size(); i++) {
			if (niz_cvorova[i].dohv_id() == id) {
				return &niz_cvorova[i];
			}

		}
	}

	Graf& obrisi_cvor(int id) {
		for (int i = 0; i < duzina; i++) {
			niz_cvorova[i].ukloni_granu(id);
		}

		int it = -1;

		for (int i = 0; i < duzina; i++) {
			if (niz_cvorova[i].dohv_id() == id) {
				it = i;
			}
		}

		niz_cvorova.erase(niz_cvorova.begin() + it);
		duzina--;
		return *this;
	}
	bool postoji_grana(int levi, int desni) {
		for (int i = 0; i < niz_cvorova[levi].dohv_grane().size(); i++) {
			if (next(niz_cvorova[levi].dohv_grane().begin(), i)->dohv_ka() == desni)
				return true;
		}
		return false;
	}

	Graf& dodaj_granu(int id1, int id2, int t=1, bool izgr=1) {
		if (postoji_grana(id1, id2)) throw G_Postoji();
		if (id1 == id2) throw G_Petlja();
		Grana g1(id1, id2);
		g1.postavi_tezinu(t);
		g1.postavi_izgradjen(izgr);

		Grana g2(id2, id1);
		g2.postavi_tezinu(t);
		g2.postavi_izgradjen(izgr);

		for (int i = 0; i < duzina ; i++) {
			if (niz_cvorova[i].dohv_id() == id1)
				niz_cvorova[i].dodaj_granu(g1);
			else if (niz_cvorova[i].dohv_id() == id2)
				niz_cvorova[i].dodaj_granu(g2);

		}
		return *this;
	}

	Graf& ukloni_granu(int id1, int id2) {
		for (int i = 0; i < duzina; i++) {
			if (niz_cvorova[i].dohv_id() == id1)
				niz_cvorova[i].ukloni_granu(id2);
			else if (niz_cvorova[i].dohv_id() == id2)
				niz_cvorova[i].ukloni_granu(id1);
		}
		return *this;
	}

	int min_cena() {
		int cena = 0;

		int r = rand() % duzina;
		vector<Cvor*> uskupu;

		

		uskupu.push_back(dohvati_cvor(r));

		int uskupu_velicina = 1;

		Grana* min_grana = nullptr;
		Grana* prev_min_grana = nullptr;

		while (uskupu.size() != duzina) {
			int uskupu_size = uskupu.size();

			for (int i = 0; i < uskupu_size; i++) {
				int br_grana = uskupu[i]->dohv_grane().size();
				for (int j = 0; j < br_grana; j++) {
					Grana pgr = *next(uskupu[i]->dohv_grane().begin(), j); //potencijalna grana
					min_grana = &pgr;

					bool nije_ciklus = 1;

					for (int k = 0; k < uskupu.size(); k++) {
						if (min_grana->dohv_ka() == uskupu[k]->dohv_id()) {
							nije_ciklus = 0;
						}
					}

					if (nije_ciklus) {
						if (!prev_min_grana) {
							Grana g = *min_grana;
							prev_min_grana = &g;
						}
						else {
							if (min_grana->dohv_tezinu() < prev_min_grana->dohv_tezinu()) {
								Grana g = *min_grana;
								prev_min_grana = &g;
							}
						}
					}




				}
			}
			int rb = prev_min_grana->dohv_ka();
			uskupu.push_back(dohvati_cvor(rb));	//ovde
			m_stablo.push_back(*prev_min_grana);
			cena += prev_min_grana->dohv_tezinu();
			prev_min_grana = nullptr;
		}

		return cena;
	}

	

	bool pripada_skupu(vector<Cvor*> skupcv,int idc) {
		for (int i = 0; i < skupcv.size(); i++) {
			if (skupcv[i]->dohv_id() == idc) return true;
		}
		return false;
	}

	void najkraca_ruta(int pocetni) {
		vector<Cvor*> uskupu;
		vector<int> tezine_puta;

		 
		vector <int> roditelj;
		vector<vector <int> > matrica_roditelja;
		vector<vector <int> > matrica_roditeljakopija;
		vector<vector <int> > matrica_tezina;
		for (int i = 0; i < niz_cvorova.size(); i++) {
			vector<int> temp;
			for (int j = 0; j < niz_cvorova.size(); j++) {
				temp.push_back(INT_MAX);
			}
			matrica_tezina.push_back(temp);
		}
		
		cout << endl;
		for (int i = 0; i < matrica_tezina.size(); i++) {
			for (int j = 0; j < matrica_tezina[i].size(); j++) {
				cout << matrica_tezina[i][j] << " ";
			}
			cout << endl;
		}

		for (int i = 0; i < niz_cvorova.size(); i++) {
			for (int j = 0; j < niz_cvorova[i].dohv_grane().size(); j++) {
				int d = next(niz_cvorova[i].dohv_grane().begin(), j)->dohv_od();
				int k = next(niz_cvorova[i].dohv_grane().begin(), j)->dohv_ka();
				matrica_tezina[d][k] = next(niz_cvorova[i].dohv_grane().begin(), j)->dohv_tezinu();
				matrica_tezina[k][d] = next(niz_cvorova[i].dohv_grane().begin(), j)->dohv_tezinu();
				
				
			}

		}

		cout << endl;
		for (int i = 0; i < matrica_tezina.size(); i++) {
			for (int j = 0; j < matrica_tezina[i].size(); j++) {
				cout << matrica_tezina[i][j]<<" ";
			}
			cout << endl;
		}
		

		cout << endl;

		//INICIJALIZACIJA
		uskupu.push_back(dohvati_cvor(pocetni));

		for (int i = 0; i < duzina; i++) {
			tezine_puta.push_back(INT_MAX);
			roditelj.push_back(0);
		}
		

		for (int i = 0; i < uskupu[0]->dohv_grane().size(); i++) {
			Grana prva_grana = *next(uskupu[0]->dohv_grane().begin(), i);
			tezine_puta[prva_grana.dohv_ka()] = prva_grana.dohv_tezinu();
			roditelj[prva_grana.dohv_ka()] = pocetni + 1;
		}

		//for (int i = 0; i < tezine_puta.size(); i++) {			//samo ispis provera
		//	cout << tezine_puta[i] << " ";

		//}
		//cout << endl;
		//for (int i = 0; i < tezine_puta.size(); i++) {		//samo ispis provera
		//	cout << roditelj[i] << " ";
		//}

		matrica_roditelja.push_back(roditelj);

		for (int k = 0; k < duzina; k++) {
			int min = INT_MAX;
			int pamtiid= -1;
			for (int i = 0; i<tezine_puta.size(); i++) {
				if (!pripada_skupu(uskupu, i) && tezine_puta[i] < min) {
					min = tezine_puta[i];
					pamtiid = i;
				}
			}
			if (pamtiid != -1) {
				uskupu.push_back(dohvati_cvor(pamtiid));

				for (int j = 0; j < tezine_puta.size(); j++) {
					if (!pripada_skupu(uskupu, j) && matrica_tezina[pamtiid][j] != INT_MAX && tezine_puta[pamtiid] + matrica_tezina[pamtiid][j] == tezine_puta[j]) {
						for (int k = 0; k < matrica_roditelja.size(); k++) {
							matrica_roditeljakopija.push_back(matrica_roditelja[k]);
						}

						for (int k = 0; k < matrica_roditelja.size(); k++) {
							matrica_roditelja[k][j] = pamtiid + 1;
							for (int i = 0; i < matrica_roditelja.size(); i++) {
								for (int j = 0; j < matrica_roditelja.size(); j++) {
									cout << matrica_roditelja[i][j] << " ";
								}
								cout << endl;
							}
						}
						for (int k = 0; k < matrica_roditeljakopija.size(); k++) {
							matrica_roditelja.push_back(matrica_roditeljakopija[k]);
						}
						
						for (int i = 0; i < matrica_roditelja.size(); i++) {			//PROVERA STAMPANJE
							for (int j = 0; j < matrica_roditelja[i].size(); j++) {
								cout << matrica_roditelja[i][j] <<"  ";
							}
							cout << endl;
						}
					}
					else if (!pripada_skupu(uskupu, j) && matrica_tezina[pamtiid][j] != INT_MAX && tezine_puta[pamtiid] + matrica_tezina[pamtiid][j] < tezine_puta[j]) {

						for (int k = 0; k < matrica_roditelja.size(); k++) {
							matrica_roditelja[k][j] = pamtiid + 1;
								cout << endl;
						}	
						tezine_puta[j] = tezine_puta[pamtiid] + matrica_tezina[pamtiid][j];
						
					}
					
				}
				
			}
		}

		for (int i = 0; i < tezine_puta.size(); i++) {			//samo ispis provera
			cout << tezine_puta[i] << " ";
		}
		

		cout << endl << endl << endl;
		
		for (int i = 0; i<matrica_roditelja.size(); i++){
			vector<int> nacini = matrica_roditelja[i];
			for (int i = 0; i < duzina; i++) {
				if (pocetni != i) {
					int rod = nacini[i];
					int ind_odjedan = i + 1;
					cout << ind_odjedan << "-";
					cout << rod;

					while (rod != pocetni + 1) {
						rod = nacini[rod - 1];
						cout << "-" << rod;
					}
				}
				cout << endl;
			}
		}
		
		cout << endl << endl << endl;
		for (int i = 0; i < matrica_roditelja.size(); i++) {
			for (int j = 0; j < matrica_roditelja[i].size(); j++) {
				cout << matrica_roditelja[i][j] << " ";
			}
			cout << endl;
		}

	}

	void obrisi_graf() {
		for (int i = 0; i < duzina; i++) {
			niz_cvorova[i].obrisi_cvor();
		}
		niz_cvorova.clear();
	}
	
	
	friend ostream& operator<<(ostream& it, const Graf& g) {
		for (int i = 0; i < g.niz_cvorova.size(); i++) {
			it << g.niz_cvorova[i];
			it << endl;
		}
		return it;
	}


};

Graf kreiraj_graf(int n) {
	try {
		return  Graf(n);
	}
	catch (G_Dim_Grafa& greska) {
		cout << greska;
	}
}


void kreiraj_granu(Graf* g, int levi, int desni,int cena, bool postoji =1) {
	try {
		g->dodaj_granu(levi, desni,cena, postoji);
	}
	catch (G_Postoji& greska) {
		cout << greska;
	}
	catch (G_Petlja& greska) {
		cout << greska;
	}
}

int main() {


	//PROVERA ZA DRUGI DEO ZADATKA
	
	Graf g(6);
	g.dodaj_granu(0, 1, 8);
	g.dodaj_granu(0, 4, 19);
	g.dodaj_granu(1, 4, 34);
	g.dodaj_granu(2, 4, 14);
	g.dodaj_granu(1, 2, 18);
	g.dodaj_granu(1, 5, 3);
	g.dodaj_granu(2, 5, 6);
	g.dodaj_granu(2, 3, 12);
	g.dodaj_granu(3, 5, 13);

	cout << g;
	cout << g.min_cena();

	g.obrisi_cvor(1);

	cout << endl;

	//g.dodaj_cvor(1);
	//g.dodaj_granu(0, 1, 8);
	//g.dodaj_granu(1, 4, 34);
	//g.dodaj_granu(1, 2, 18);
	//g.dodaj_granu(1, 5, 3);

	//cout << g;

	//cout << g.min_cena();
	


	//PROVERA ZA TRECI DEO
	
	Graf g3(5);
	g3.dodaj_granu(0,4,5);
	g3.dodaj_granu(0,2,5);
	g3.dodaj_granu(2,4,11);
	g3.dodaj_granu(4,3,8);
	g3.dodaj_granu(4,1,2);
	g3.dodaj_granu(2,3,3);
	g3.dodaj_granu(1,3,5);

	g3.najkraca_ruta(4);
	
	
	cout << "Unesi broj kljucnih lokacija u gradu: ";
	int n;
	cin >> n;
	while (n <= 0) {
		cout << "Unesi broj kljucnih lokacija u gradu: ";
		cin >> n;
	}

	Graf gg = kreiraj_graf(n);

	bool ucitavajgrane = true;
	int lok1, lok2, cena;

	while (ucitavajgrane) {
		cout << "\n1. Unesi kljucne lokacije izmedju kojih se gradi metro i cenu\n"
			"0. Zavrsi unos podataka\n\n"
			"Unesi izbor!\n";
		int izbor;
		cin >> izbor;

		switch (izbor) {
		case 1:
			cout << "1. Unesi kljucne lokacije izmedju kojih se gradi metro i cenu\n";
			cin >> lok1 >> lok2 >> cena;
			kreiraj_granu(&gg, lok1, lok2, cena); break;
		case 0:
			cout << "0. Zavrsi unos podataka\n";
			ucitavajgrane = false; break;

		default:
			cout << "---Nedozvoljen izbor!---\n"; break;

		}
	}

	cout << gg;

	cout << "Mininimaln cena potrebna za izgradnju metroa je : ";
	
	cout<<gg.min_cena();





	cout << "Unesi broj kljucnih lokacija u gradu: ";
	
	cin >> n;
	while (n <= 0) {
		cout << "Unesi broj kljucnih lokacija u gradu: ";
		cin >> n;
	}




	
	
	
}
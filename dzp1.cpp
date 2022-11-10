#include<iostream>
#include<list>
#include<vector>
#include<climits>
using namespace std;

//klase gresaka

class G_Dim_Grafa {
	friend ostream& operator<<(ostream& it, const  G_Dim_Grafa& gr) {
		return it << "Nedozvoljen broj cvorova";
	}
};

class G_Van_Opsega {
	friend ostream& operator<<(ostream& it, const  G_Van_Opsega& gr) {
		return it << "Van Opsega";
	}
};

class G_Postoji{
	friend ostream& operator<<(ostream& it, const  G_Postoji& gr) {
		return it << "Vec postoji element u grafu";
	}
};

class G_Ne_Postoji {
	friend ostream& operator<<(ostream& it, const  G_Ne_Postoji& gr) {
		return it << "Ne postoji element u grafu";
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

public:

	int dohv_od() {
		return od;
	}

	int dohv_ka() {
		return ka;
	}

	Grana(int odd, int kaa) {
		od = odd;
		ka = kaa;
	}

	friend ostream& operator<<(ostream& it, const Grana& gr) {
		return it << " G: (" << gr.od << "," << gr.ka << ")";
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

	~Cvor() {
		povezane_grane.clear();
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


public:
	Graf(int br_cvorova) {
		if (br_cvorova <= 0) throw G_Dim_Grafa();
		duzina = br_cvorova;
		for (int i = 0; i < br_cvorova; i++) {
			Cvor novi(i);
			niz_cvorova.push_back(novi);
		}
	}

	Graf& dodaj_cvor(int id) {
		if (id < 0) throw G_Van_Opsega();
		if (postoji_cvor(id)) throw G_Postoji();
		Cvor novi(id);
		niz_cvorova.push_back(novi);
		duzina++;
		return *this;
	}

	bool postoji_cvor(int id) {
		for (int i = 0; i < niz_cvorova.size(); i++) {
			if (niz_cvorova[i].dohv_id() == id) return true;
		}
		return false;
	}

	Graf& obrisi_cvor(int id) {
		if (!postoji_cvor(id)) throw G_Ne_Postoji();
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
		return *this;
	}

	Graf& dodaj_granu(int id1, int id2) {
		if (postoji_grana(id1,id2)) throw G_Postoji();
		if (id1 == id2) throw G_Petlja();
		Grana g1(id1, id2);
		Grana g2(id2, id1);

		for (int i = 0; i < duzina; i++) {
			if (niz_cvorova[i].dohv_id() == id1)
				niz_cvorova[i].dodaj_granu(g1);
			else if (niz_cvorova[i].dohv_id() == id2)
				niz_cvorova[i].dodaj_granu(g2);

		}
		return *this;
	}
	
	bool postoji_grana(int levi,int desni) {
		for (int i = 0; i < niz_cvorova[levi].dohv_grane().size(); i++) {
			if (next(niz_cvorova[levi].dohv_grane().begin(), i)->dohv_ka() == desni)
				return true;
		}
		return false;
	}

	Graf& ukloni_granu(int id1, int id2) {
		if (!postoji_grana(id1, id2)) throw G_Ne_Postoji();
		for (int i = 0; i < duzina; i++) {
			if (niz_cvorova[i].dohv_id() == id1)
				niz_cvorova[i].ukloni_granu(id2);
			else if (niz_cvorova[i].dohv_id() == id2)
				niz_cvorova[i].ukloni_granu(id1);
		}
		return *this;
	}

	~Graf() {
		niz_cvorova.clear();
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

void kreiraj_granu(Graf* g, int levi, int desni) {
	try {
		g->dodaj_granu(levi, desni);
	}
	catch (G_Postoji& greska) {
		cout << greska;
	}
	catch (G_Petlja& greska) {
		cout << greska;
	}
}

void ukloni_granu(Graf* g, int levi, int desni) {
	try {
		g->ukloni_granu(levi, desni);
	}
	catch (G_Ne_Postoji& greska) {
		cout << greska;
	}
}

void ispisi_graf(Graf* g) {
	cout << *(g);
}

void dodaj_cvor(Graf* g, int redbr) {
	try {
		g->dodaj_cvor(redbr);
	}
	catch (G_Van_Opsega greska) {
		cout << greska;
	}
	catch (G_Postoji& greska) {
		cout << greska;
	}

}

void ukloni_cvor(Graf* g, int redbr) {
	try {
		g->obrisi_cvor(redbr);
	}
	catch(G_Ne_Postoji& greska) {
		cout << greska;
	}
}

void obrisi_graf(Graf* g) {
	g->obrisi_graf();
}


int main() {
	

	cout << "Kreiraj prazan graf, tako sto unosis dimenzije tj. broj cvorova: ";
	int n;
	cin >> n;
	while (n <= 0) {
		cout << "Kreiraj prazan graf, tako sto unosis dimenzije tj.broj cvorova : ";
		cin >> n;
	}

	Graf g = kreiraj_graf(n);
	
	

	

	bool kraj = false;

	int levi, desni;
	int rbcvora;


	while (!kraj) {
		cout << "\n1. Dodaj granu, tako sto unosis levi, pa desni cvor grafa (npr:0 2)\n"
			"2. Ukloni granu, tako sto unosis njene krajnje cvorove\n"
			"3. Dodaj cvor sa rednim brojem: \n"
			"4. Ukloni cvor sa rednim br: \n"
			"5. Ispisi graf\n"
			"6. Obrisi graf\n"
			"0. Zavrsi\n"
			"Unesi izbor!\n";
		int izbor; 
		cin >> izbor;

		switch (izbor) {
		case 1:
			cout << "1. Dodaj granu, tako sto unosis levi, pa desni cvor grafa (npr:0 2)\n";
			cin >> levi >> desni;
			kreiraj_granu(&g, levi, desni); break;
		case 2:
			cout << " 2.Ukloni granu, tako sto unosis njene krajnje cvorove\n";
			cin >> levi >> desni;
			ukloni_granu(&g, levi, desni); break;

		case 3:
			cout << "3. Dodaj cvor sa rednim brojem: \n";
			cin >> rbcvora;
			dodaj_cvor(&g, rbcvora); break;
			
		case 4:
			cout << "4. Ukloni cvor sa rednim br: \n";
			cin >> rbcvora;
			ukloni_cvor(&g, rbcvora); break;

		case 5:
			cout<<"5. Ispisi graf\n";
			ispisi_graf(&g); break;

		case 6:
			cout << "6. Obrisi graf\n";
			obrisi_graf(&g); break;
		case 0:
			cout << "0. Zavrsi\n";
			kraj = true; break;

		default: 
			cout << "---Nedozvoljen izbor!---\n"; break;

		}

	}

}
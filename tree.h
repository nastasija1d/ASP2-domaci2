#include <string>
#include <iostream>
#include <stack>
#include <queue>
using namespace std;
#ifndef _tree_h_
#define _tree_h_

struct value {
	int p;
	string s;
};

struct Node {
	int num;
	int level;
	value* v[3];
	Node* n[4];
};

Node* createNode(value* v) {
	Node* n = new Node;
	n->v[0] = v;
	n->num = 1;
	n->v[1] = n->v[2] = nullptr;
	n->n[0]=n->n[1]=n->n[2]=n->n[3] = nullptr;
	return n;
}

value* createValue(int i, string s) {
	value* v = new value;
	v->p = i;
	v->s = s;
	return v;
}

class Tree {
	Node* root;	
public:
	Tree() { root = nullptr; }
	void  insert(value* v);
	void delete_value(value* v);
	void print_tree();
	int count(int n);
	void change(value* v, int n);
	void search(value* v);
};

//DEFINICIJE FUNKCIJA
bool is_equal(value* v1, value* v2) {
	if (v1->p == v2->p and v1->s.compare(v2->s) == 0) return true;
	return false;
}

bool is_less(value* v1, value* v2) {
	if (v1->p < v2->p) return true;
	if (v1->p == v2->p and v1->s.compare(v2->s) < 0) return true;
	return false;
}

bool is_bigger(value* v1, value* v2) {
	if (v1->p > v2->p) return true;
	if (v1->p == v2->p and v1->s.compare(v2->s) > 0) return true;
	return false;
}

void Tree::insert(value* key) {
	if (root == nullptr) {
		root = createNode(key);
		return;
	}
	stack<Node*>stek;	//na steku se nalaze svi cvorovi od korena do lista
	Node* p = root, * q;
	int i, j, k, i1;
//PRETRAGA
	while (true){
		if (p == nullptr) break;
		stek.push(p);
		if (is_bigger(key, p->v[p->num - 1])) {
			p = p->n[p->num];
		}
		else {
			for (i = 0; i < p->num; i++) {
				if (is_equal(key, p->v[i])) {
					cout << "Kljuc vec postoji";
					return;
				}
				if (is_less(key, p->v[i])) {
					p = p->n[i];
					break;
				}
			}
		}
	}
//UMETANJE
	Node* a = nullptr, * b = nullptr;
	value* key1 = key;
	while (true) {
		p = stek.top();
		stek.pop();
		value* lista1[4];
		Node* lista2[5];
		j = k = i1 = 0;
	//PRAVLJENJE LISTI VREDNOSTI I POKAZIVACA
		for (i = 0; i < p->num; i++) {
			if (is_less(key1, p->v[i]) and !i1) {
				lista1[j++] = key1;
				lista2[k++] = a;
				lista2[k++] = b;
				i1 = 1;
			}
			lista1[j++] = p->v[i];
			lista2[k++] = p->n[i + i1];
		}
		if (is_bigger(key1, p->v[p->num - 1])) {
			lista1[j] = key1;
			lista2[k++] = a;
			lista2[k] = b;
		}
	//ISPITIVANJE POPUNJENOSTI
		if (p->num < 3) {
			p->num++;
			for (i = 0; i < p->num; i++) {
				p->v[i] = lista1[i];
				p->n[i] = lista2[i];
			}
			p->n[i] = lista2[i];
			break;
		}
		else {
			key1 = lista1[1];
			a = createNode(lista1[0]);
			b = createNode(lista1[2]);
			b->v[1] = lista1[3]; b->num++;
			a->n[0] = lista2[0]; a->n[1] = lista2[1];
			b->n[0] = lista2[2]; b->n[1] = lista2[3]; b->n[2] = lista2[4];

			if (p == root) {
				root = createNode(key1);
				root->n[0] = a; root->n[1] = b;
				break;
			}
		}
	}
}

void Tree::delete_value(value* key){
	if (root == nullptr) {
		cout << "Stablo je prazno";
		return;
	}
	stack<Node*>stek1;
	Node* p = root, * q, * r, *l;
	int i, j, k, i1;
//PRETRAGA
	while (true) {
		if (p == nullptr) break;
		stek1.push(p);
		int found = 0;
		if (is_bigger(key, p->v[p->num - 1])) {
			p = p->n[p->num];
		}
		else {
		for (i = 0; i < p->num; i++) {
			if (is_equal(key, p->v[i])) found = 1;
			if (found) break;
			if (is_less(key, p->v[i])) 	p = p->n[i];
		}
		if (found)break;
		}
		
	}
	if (p == nullptr) {
		cout << "Ne postoji uneti kljuc! Greska!";
		return;
	}
	if (p == root and p->num == 1) {
		cout << "\nStablo je potpuno obrisano\n";
		root = nullptr;
		return;
	}
//Provera da li je nadjeni kljuc u listu
	if (p->n[0] != nullptr) {
		for (i = 0; i < p->num; i++) {
			if (is_equal(key, p->v[i])) break;
		}
		q = p;
		p = p->n[i + 1];
		while (p != nullptr) {
			stek1.push(p);
			p = p->n[0];
		}
		p = stek1.top();
		q->v[i] = p->v[0];
		p->v[0] = key;
	}
//uklanjanje kljuca iz cvora
	j = 0;
	for (i = 0; i < p->num; i++) {
		if (!is_equal(key, p->v[i])) p->v[j++] = p->v[i];
	}
	p->num--;
	stek1.pop();
//GLAVNA VELIKA PETLJA
	while (true) {
		if (p->num > 0) break;
		q = stek1.top();
		stek1.pop();
		for (i = 0; i <= p->num; i++) {
			if (q->n[i] == p) break;
		}
		//POZAJMICA OD DESNOG BRATA
		if (i < q->num) {
			if (q->n[i + 1]->num > 1) {
				r = q->n[i + 1];	//desni brat
				p->v[0] = q->v[i];
				p->n[1] = r->n[0];
				q->v[i] = r->v[0];
				r->v[0] = r->v[1]; r->v[1] = r->v[2]; r->v[2] = r->v[3]; r->v[3] = nullptr;
				r->n[0] = r->n[1]; r->n[1] = r->n[2]; r->n[2] = r->n[3]; r->n[3] = r->n[4]; r->n[4] = nullptr;
				p->num = 1;
				r->num--;
				return;
			}
		}
		//POZAJMICA OD LEVOG BRATA
		if (i > 0) {
			if (q->n[i - 1]->num > 1) {
				l = q->n[i - 1];	//levi brat
				p->v[0] = q->v[i - 1];
				p->n[1] = p->n[0];
				p->n[0] = l->n[l->num];
				q->v[i - 1] = l->v[l->num - 1];
				p->num = 1;
				l->num--;
				return;
			}
		}

		//SPAJANJE SA DESNIM BRATOM
		if (i < q->num) {
			r = q->n[i + 1];
			p->v[0] = q->v[i];
			p->v[1] = r->v[0];
			p->n[1] = r->n[0];
			p->n[2] = r->n[1];
			r->v[0] = q->v[i]; r->v[2] = nullptr;
			if (p->n[0] != nullptr) p->n[0] = p->n[0];
			if (p->n[1] != nullptr) p->n[0] = p->n[1];
			p->num=2;
				for (j = i ; j < q->num ; j++) {
					q->v[j] = q->v[j + 1];
					q->n[j] = q->n[j + 1];
				}
			q->num--;
			if (q->num == 0 and q == root) {
				root = r;
				return;
			}

		}

		//SPAJANJE SA LEVIM BRATOM
		if (i > 0) {
			l = q->n[i - 1];
			l->v[1] = q->v[i - 1];
			l->v[2] = nullptr;
			if (p->n[0] != nullptr) l->n[2] = p->n[0];
			if (p->n[1] != nullptr) l->n[2] = p->n[1];
			l->n[3] = nullptr;
			l->num++;
				for (j = i-1; j < q->num-1; j++) {
					q->v[j] = q->v[j + 1];
					q->n[j] = q->n[j + 1];
				}
			q->v[2] = nullptr;
			q->n[3] = nullptr;
			q->num--;
			if (q->num == 0 and q == root) {
				root = l;
				return;
			}
		}
		p = q;
	}
	

}

void Tree::print_tree(){
	if (root == nullptr) {
		cout << "Stablo je prazno!\n";
		return;
	}
	Node* p = root,*q;
	p->level = 0;
	queue<Node*>red;
	red.push(p);
	int i,l = 0;
	cout << "\n";
	while (!red.empty()) {
		p = red.front();
		red.pop();
		if (p->level > l) {
			cout << "\n\n";
			l++;
		}
		for (i = 0; i < 30 - l * 6; i++) cout << " ";
		cout << "(";
		for (i = 0; i < p->num; i++) {
			cout << p->v[i]->p << " ";
		}
		cout <<")";
		for (i = 0; i < p->num + 1; i++) {
			q = p->n[i];
			if (q != nullptr) {
				red.push(q);
				q->level = p->level + 1;
			}
		}
	}
}

int Tree::count(int n){
	int c = 0,i;
	queue<Node*>red1;
	Node* p = root;
	red1.push(p);
	while (!red1.empty()) {
		p = red1.front();
		red1.pop();
		for (i = 0; i < p->num; i++) {
			if (p->v[i]->p == n) c++;
		}
		for (i = 0; i < p->num + 1; i++) {
			if (p->n[i] != nullptr) red1.push(p->n[i]);
		}
	}
	return c;
}

void Tree::change(value* v, int n){
	this->delete_value(v);
	v->p = n;
	this->insert(v);
}

void Tree::search(value* key){
	Node* p = root;
	while (true) {
		if (p == nullptr) break;
		if (is_bigger(key, p->v[p->num - 1])) {
			p = p->n[p->num];
		}
		else {
			for (int i = 0; i < p->num; i++) {
				if (is_equal(key, p->v[i])) {
					cout << "\nKljuc je pronadjen\n";
					return;
				}
				if (is_less(key, p->v[i])) {
					p = p->n[i];
					break;
				}
			}
		}
	}
	cout << "\nKljuc nije pronadjen\n";
}


#endif
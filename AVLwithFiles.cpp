#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

//struktura drzewa avl
struct AVL
{
	struct AVL* left;
	struct AVL* right;
	vector<string> key;
	struct AVL* par;
	int height;
	struct AVL()
	{
		left = NULL;
		right = NULL;
		par = NULL;
		height = 0;
	}
};



//wypisywanie drzewa AVL wraz z oznaczeniem rodzica
void displayAVL(struct AVL* node, ofstream &out)
{
	//wypisanie angielskiego slowa
	out << node->key[0] << " | ";
	cout << node->key[0] << " | ";
	
	for (int i = 1; i < node->key.size(); i++)
	{
		out << node->key[i] << " ";
		cout << node->key[i] << " ";
	}
	out << "| Rodzic: ";
	cout << "| Rodzic: ";
	if (node->par != NULL)
	{
		out << node->par->key[0];
		cout << node->par->key[0];
		
	}
	else
	{
		out << "NULL";
		cout << "NULL";
	}
	cout << endl;
	out << endl;

	

	//przejście do lewego poddrzewa
	if (node->left != NULL)
	{
		displayAVL(node->left,out);
	}

	// przejście do prawego poddrzewa
	if (node->right != NULL)
	{
		displayAVL(node->right,out);
	}
}

//aktualizowanie wysokości węzła
void height_update(struct AVL* node)
{
	if (node != NULL)
	{
		int value = 1;

		//wysokosc lewego i prawego poddrzewa
		if (node->left != NULL)
			value = node->left->height + 1;

		if (node->right != NULL)
			value = max(value, node->right->height + 1); //interesuje nas znalezienie jak najdluzszej wysokosci

		//aktualizowanie wysokości węzła
		node->height = value;
	}
}

//funkcja do przypadku rotacji LeftLeft
struct AVL* LeftLeft(struct AVL* node)
{
	//utworzenie pomocniczej referencji do dziecka z lewej strony
	struct AVL* pom_node = node->left;

	//lewe dziecko rozwazanego teraz node'a to teraz prawe dziecko lewego dziecka node'a
	node->left = pom_node->right;

	//rodzica prawego dziecka pomocniczej referencji staje sie wierzchołkiem node
	if (pom_node->right != NULL)
		pom_node->right->par = node;

	// prawe dziecko referencji pomocniczej przejmuje własciwosci rozwazanego wierzchołka
	pom_node->right = node;

	// rodzic referencji pomocniczej to teraz rodzic rozwazanego wierzchołka
	pom_node->par = node->par;


	//rodzic rozważanego wierzchołka to teraz wierzchołek z referencji pomocniczej
	node->par = pom_node;




	//jeżeli klucz rozważanego wierzchołka jest mniejszy od klucza rodzica pomocniczej referencji, to
	//lewe dziecko rodzica pomocniczej referencji zyskuje wlasciwosci wierzcholka pom_node
	if (pom_node->par != NULL && node->key[0].compare(pom_node->par->key[0]) < 0)
	{
		pom_node->par->left = pom_node;
	}
	else
	{
		if (pom_node->par != NULL)
			pom_node->par->right = pom_node;
	}

	//przypisanie wierzchołka pomocniczego do node'a
	node = pom_node;

	//aktualizowanie wysokosci
	height_update(node->left);
	height_update(node->right);
	height_update(node);
	height_update(node->par);

	//zwrocenie wierzchołka
	return node;
}

//funkcja do przypadku rotacji RightRight
struct AVL* RightRight(struct AVL* node)
{
	//utworzenie referencji do dziecka z prawej strony
	struct AVL* pom_node = node->right;


	//prawe dziecko rozwazanego teraz node'a to teraz lewe dziecko prawego dziecka node'a
	node->right = pom_node->left;

	//rodzic lewego dziecka referencji pomocniczej zyskuje wlasnosci rozwazanego wierzcholka
	if (pom_node->left != NULL)
		pom_node->left->par = node;


	//lewe dziecko referencji pomocniczej przyjmuje wlasnosci node'a
	pom_node->left = node;

	//rodzic referencji pomocniczej przejmuje wlasnosci rodzica node'a
	pom_node->par = node->par;

	//rodzic rozwazanego wierzcholka przyjmuje wlasnosci referencji pomocniczej 
	node->par = pom_node;

	//jeżeli klucz rozważanego wierzchołka jest mniejszy od klucza rodzica pomocniczej referencji, to
	//lewe dziecko rodzica pomocniczej referencji zyskuje wlasciwosci wierzcholka pom_node
	if (pom_node->par != NULL && node->key[0].compare(pom_node->par->key[0]) < 0)
	{
		pom_node->par->left = pom_node;
	}
	else
	{
		if (pom_node->par != NULL)
			pom_node->par->right = pom_node;
	}

	//przypisanie wierzchołka pomocniczego do node'a
	node = pom_node;

	//aktualizowanie wysokosci
	height_update(node->left);
	height_update(node->right);
	height_update(node);
	height_update(node->par);

	//zwrócenie wierzchołka
	return node;
}

//funkcja do przypadku rotacji LeftRight
struct AVL* LeftRight(struct AVL* node)
{
	node->left = RightRight(node->left);
	return LeftLeft(node);
}

//funkcja do przypadku rotacji RightLeft
struct AVL* RightLeft(struct AVL* node)
{
	node->right = LeftLeft(node->right);
	return RightRight(node);
}

//ewentualne wykonanie rotacji po usunięciu wierzchołka
struct AVL* Balance(struct AVL* node)
{
	//wysokosci lewego i prawego poddrzewa
	int leftHeight = 0;
	int rightHeight = 0;

	if (node->left != NULL)
		leftHeight = node->left->height;

	if (node->right != NULL)
		rightHeight = node->right->height;

	//przypadek w którym węzeł nie jest zbalansowany
	if (abs(leftHeight - rightHeight) == 2)
	{   //wysokosc prawego poddrzewa jest wieksza niż lewego
		if (leftHeight < rightHeight)
		{

			//wysokosci lewego i prawego poddrzewa odnosząca się do prawego poddrzewa
			int rightLeft_height = 0;
			int rightRight_height = 0;

			if (node->right->left != NULL)
				rightLeft_height = node->right->left->height;
			if (node->right->right != NULL)
				rightRight_height = node->right->right->height;

			if (rightLeft_height > rightRight_height)
			{

				//rotacja RightLeft
				node = RightLeft(node);
			}
			else
			{

				//rotacja RightRight
				node = RightRight(node);
			}
		}
		//wysokosc prawego poddrzewa jest mniejsza niż lewego
		else
		{

			//wysokosci lewego i prawego poddrzewa odnosząca się do lewgo poddrzewa
			int leftLeft_height = 0;
			int leftRight_height = 0;

			if (node->left->left != NULL)
				leftLeft_height = node->left->left->height;
			if (node->left->right != NULL)
				leftRight_height = node->left->right->height;

			if (leftLeft_height > leftRight_height)
			{

				//rotacja LeftLeft
				node = LeftLeft(node);
			}
			else
			{

				//rotacja LeftRight
				node = LeftRight(node);
			}
		}
	}

	//zwrócenie wierzchołka
	return node;
}

//dodawanie wierzchołka do drzewa AVL
struct AVL* Add(struct AVL* node, struct AVL* parent, vector<string> key)
{

	if (node == NULL)
	{

		//stworzenie węzła i przypisanie do niego odpowiednich wartości
		node = new struct AVL;

		node->height = 1;
		node->left = NULL;
		node->right = NULL;
		node->par = parent;
		node->key = key;


	}
	//klucz wierzchołka jest większy od klucza wpisanego przez nas
	else if (node->key[0] > key[0])
	{

		//rekurencyjne odniesienie do lewego dziecka wierzchołka w celu dodania do niego wierzcholka o kluczu podanym przez użytkownika
		node->left = Add(node->left, node, key);

		//wysokosci lewego i prawego poddrzewa naszego wierzchołka
		int left_height = 0;
		int right_height = 0;

		if (node->left != NULL)
			left_height = node->left->height;

		if (node->right != NULL)
			right_height = node->right->height;

		//wykonanie odpowiednich rotacji, jezeli wezel jest niezbalansowany
		if (abs(left_height - right_height) == 2)
		{

			if (node->left != NULL && key < node->left->key)
			{

				//rotacja LeftLeft
				node = LeftLeft(node);
			}
			else
			{

				//rotacja LeftRight
				node = LeftRight(node);
			}
		}
	}
	//klucz wierzchołka jest mniejszy od klucza wpisanego przez nas
	else if (node->key[0] < key[0])
	{

		//rekurencyjne odniesienie do prawego dziecka wierzchołka w celu dodania do niego wierzcholka o kluczu podanym przez użytkownika

		node->right = Add(node->right, node, key);

		// wysokosci lewego i prawego poddrzewa
		int left_height = 0;
		int right_height = 0;

		if (node->left != NULL)
			left_height = node->left->height;

		if (node->right != NULL)
			right_height = node->right->height;

		//wykonanie odpowiednich rotacji, jezeli wezel jest niezbalansowany
		if (abs(left_height - right_height) == 2)
		{
			if (node->right != NULL && key[0].compare(node->right->key[0]) < 0)
			{

				//rotacja RightLeft
				node = RightLeft(node);
			}
			else
			{

				//rotacja RightRight
				node = RightRight(node);
			}
		}
	}

	//proba dodania angielskiego słówa, które już jest w drzewie
	else
	{
		cout << "Duplikat!" << endl;
	}

	//aktualizacja wysokosci node'a
	height_update(node);

	//zwrócenie wierzchołka
	return node;
}

//funkcja usuwajaca wierzchołek z drzewa AVL
struct AVL* Delete(struct AVL* node, string key)
{
	if (node != NULL)
	{

		//wierzchołek znaleziono
		if (node->key[0] == key)
		{

			//wierzchołek staje się swoim lewym dzieckiem (gdyz if wykazal ze ma lewe dziecko, a nie ma prawego)
			if (node->right == NULL && node->left != NULL)
			{
				if (node->par != NULL)
				{
					if (node->par->key[0].compare(node->key[0]) < 0)
						node->par->right = node->left;
					else
						node->par->left = node->left;

					//aktualizacja wysokosci rodzica naszego node'a
					height_update(node->par);
				}

				node->left->par = node->par;

				// zbalansowanie węzła po operacji usunięcia
				node->left = Balance(node->left);

				return node->left;
			}

			//wierzchołek staje się swoim prawym dzieckiem (gdyz if wykazal ze ma prawe dziecko, a nie ma lewego)
			else if (node->left == NULL && node->right != NULL)
			{
				if (node->par != NULL)
				{
					if (node->par->key[0].compare(node->key[0]) < 0)
						node->par->right = node->right;
					else
						node->par->left = node->right;

					// aktualizacja wysokosci rodzica node'a
					height_update(node->par);
				}

				node->right->par = node->par;

				// zbalansowanie węzła po operacji usuniecia
				node->right = Balance(node->right);
				return node->right;
			}

			// usunięcie odniesień do usuwanego wierzchołka
			else if (node->left == NULL && node->right == NULL)
			{
				if (node->par->key[0].compare(node->key[0]) < 0)
				{
					node->par->right = NULL;
				}
				else
				{
					node->par->left = NULL;
				}

				if (node->par != NULL)
					height_update(node->par);

				node = NULL;
				return NULL;
			}


			//usuwany wierzchołek jest zastępywany przez swojego następcę, a następnie rekursywne wywołanie funkcji Delete
			else
			{
				struct AVL* pom_node = node;
				pom_node = pom_node->right;
				while (pom_node->left != NULL)
				{
					pom_node = pom_node->left;
				}

				vector<string> value = pom_node->key;

				node->right = Delete(node->right, pom_node->key[0]);

				node->key = value;

				//zbalansowanie węzła po wykonaniu operacji usunięcia
				node = Balance(node);
			}
		}

		//odwolanie rekurencyjne do prawego poddrzewa wierzcholka, gdyz klucz danego wierzcholka jest mniejszy od klucza przez nas szukanego
		else if (node->key[0].compare(key) < 0)
		{
			node->right = Delete(node->right, key);

			node = Balance(node);
		}

		//odwolanie rekurencyjne do lewego poddrzewa wierzcholka, gdyz klucz danego wierzcholka jest większy od klucza przez nas szukanego
		else if (node->key[0].compare(key) > 0)
		{
			node->left = Delete(node->left, key);

			node = Balance(node);
		}

		//aktualizacja wysokosci node'a
		if (node != NULL)
		{
			height_update(node);
		}
	}

	//usuwanie wierzchołka który nie istnieje
	else
	{
		cout << "Nie ma takiego slowa" << endl;
	}

	//zwrocenie wierzchołka
	return node;
}


//funkcja wyszukująca wierzchołka po angielskim słowie
vector<string> AVLsearch(struct AVL* node, string key)
{

	if (node == NULL)
		return vector<string>(0);

	//jeżeli znaleziono, zwraca słowo angielskie wraz z tłumaczeniami
	else if (node->key[0] == key)
		return node->key;

	//przejście do lewego poddrzewa, gdyż obecnie analizowane słowo angielskie jest większe od szukanego przez nas
	else if (node->key[0].compare(key) > 0)
	{
		vector<string> word = AVLsearch(node->left, key);
		return word;
	}

	//w innym przypadku, przejdź do prawego poddrzewa
	else
	{
		vector<string> word = AVLsearch(node->right, key);
		return word;
	}
}



int main()
{
	struct AVL* node;
	node = NULL;
	string slowko;
	int number = 0, n_polish;
	vector<string> found;
	
	ifstream in("words.txt");
	ofstream out("exit.txt");
	if (!in)
	{
		cout << "error in" << endl;
		return 0;
	}
	if (!out)
	{
		cout << "error out" << endl;
	}
	while (getline(in, slowko))
	{
		istringstream line_stream(slowko);
		vector<string> word;

		string word1;
		while(line_stream>>word1)
		word.push_back(word1);
		node = Add(node, NULL, word);
	}
	
	
		

	

	//MENU

	while (number != 5)
	{
		system("CLS");
		cout << "Co chcesz zrobic?" << endl;
		cout << "1. Wypisz stan drzewka do pliku exit.txt." << endl;
		cout << "2. Usun wybrane slowo." << endl;
		cout << "3. Szukaj wybranego slowa" << endl;
		cout << "4. Dodaj wybrane slowko" << endl;
		cout << "5. Wyjdz" << endl;

		cin >> number;
		switch (number)
		{
		case 1:
		{	displayAVL(node, out);
		out << endl;

		break;
		}
		case 2:
		{	cin >> slowko;
		node = Delete(node, slowko);
		break;
		}
		case 3:
		{
			cin >> slowko;
			found = AVLsearch(node, slowko);
			if (found.size() != 0)
				for (int i = 0; i < found.size(); i++)
				{
					if (i == 1)
					{
						cout << "Tlumaczenia: ";
					}
					cout << found[i] << " ";
				}
			else
			{
				cout << "Brak slowa" << endl;
			}
			break;
		}
		case 4:
		{	vector<string> word1;
		cout << "Ile tlumaczen tego slowa chcesz zapisac?" << endl;
		cin >> n_polish;
		for (int i = 0; i < n_polish + 1; i++)
		{

			cin >> slowko;
			word1.push_back(slowko);
		}

		node = Add(node, NULL, word1);

		break;
		}
		case 5:
		{

			break;
		}
		}
		cin.get();
		cin.get();
	}
	return 0;


}

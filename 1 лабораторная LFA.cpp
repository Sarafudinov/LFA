#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

//rules
void word_generator(char state ,string* value) 
{
	switch (state)
	{
	case 'S':
		*value += 'a';
		(rand() % 2 == 1) ? word_generator('A', value) : word_generator('B', value);
		break;

	case 'A':
		*value += 'a';
		word_generator('B', value);
		break;

	case 'B':
		*value += 'b';
		if (rand() % 2 == 1) { word_generator('D', value); }
		break;

	case 'D':
		*value += 'c';
		word_generator('B', value);
		break;

	default:
		break;
	}
}

void text_info() 
{
	cout << "\nVn = {S, B, D, A}\nVt = {a, b, c}\n" << endl;
	cout << "P = {   1. S -> aA\n\t2. S -> aB\n\t3. A -> aB\n\t4. B -> b\n\t5. B -> bD\n\t6. D -> cB   }" << endl;
}


int main()
{
	cout << "Menu: " << endl;
	cout << "1. All information about a given formal language" << endl;
	cout << "2. Get words composed according to the rules" << endl;
	cout << "3. EXIT" << endl;

	int choice = 0;

	while (choice != 3)
	{
		cout << "Your choice: ";
		cin >> choice;

		int count_words;

		vector<string> list_of_words;
		string str;

		switch (choice)
		{
		case 1:
			text_info();
			break;
		case 2:

			cout << "Enter how many words you want:  ";
			cin >> count_words;

			while (list_of_words.size() != count_words)
			{
				str = "";
				word_generator('S', &str);

				auto search = find(list_of_words.begin(), list_of_words.end(), str);

				if (search == list_of_words.end())
				{
					list_of_words.push_back(str);
				}
			}

			for (int i = 0; i < list_of_words.size(); i++)
			{
				cout << i + 1 << ". " << list_of_words.at(i) << endl;
			}

			break;
		default:
			break;
		}
	}  
}


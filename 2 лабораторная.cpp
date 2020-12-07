#include <iostream>

using namespace std;

//обьявление функций
void validation(char , char , char* );
char* decrease_word(char*);
void check(char, char*);

//функция удаляющая первый символ в слове
char* decrease_word(char* source)
{
	char* temp = new char[strlen(source)];

	for (int i = 0; i < strlen(source) - 1; i++)
	{
		temp[i] = source[i + 1];
	}
	temp[strlen(source) - 1] = '\0';

	for (int i = 0; i < strlen(temp); i++)
	{
		source[i] = temp[i];
	}
	source[strlen(temp)] = '\0';

	delete[]temp;
	return source;
}

//функция для запуска рекурсивной функции и укорочения слова 
void check(char state, char* word) {

	decrease_word(word);
	validation(state, word[0], word);
}

//функция проверки состояния 
void validation(char state, char input, char* word){
	
	switch (state)
	{
	case 'A':
		if (input == 'a') { check('B', word); }
		if (input == '\0' && word[0] == '\0') cout << "non admitted" << endl;
		break;

	case 'B':
		if (input == 'a') { check('C', word); }
		if (input == 'b') { check('D', word); }
		if (input == '\0' && word[0] == '\0') cout << "non admitted" << endl;
		break;

	case 'C':
		if (input == 'a') { check('E', word); }
		if (input == 'b') { check('D', word); }
		if (input == '\0') cout << "admitted" << endl;
		break;

	case 'D':
		if (input == 'a') { check('F', word); }
		if (input == 'b') { check('D', word); }
		if (input == '\0' && word[0] == '\0') cout << "non admitted" << endl;
		break;

	case 'E':
		if (input == 'a') { check('E', word); }
		if (input == 'b') { check('D', word); }
		if (input == '\0') cout << "admitted" << endl;
		break;

	case 'F':
		if (input == 'a') { check('G', word); }
		if (input == '\0') cout << "admitted" << endl;
		break;

	case 'G':
		if (input == 'a') { check('D', word); }
		if (input == '\0' && word[0] == '\0') cout << "non admitted" << endl;
		break;

	default:
		break;
	}
	
}

void info() 
{
	cout << "INFO:" << endl;
	cout << " KA = (Q, Eps, Del, q0, F)\n Q = {q0, q1, q2, q3}\n Eps = {a, b},  F = {q2}" << endl;
	cout << " Del(q0, a) = q0" << endl;
	cout << " Del(q0, a) = q1" << endl;
	cout << " Del(q1, a) = q2" << endl;
	cout << " Del(q1, b) = q1" << endl;
	cout << " Del(q2, a) = q3" << endl;
	cout << " Del(q3, a) = q1\n" << endl;
}

int main()
{
	info();
	setlocale(LC_ALL, "ru");

	char* word = new char[30];
	int loop = 0;
	cout << "1. Check new word" << endl;
	cout << "2. EXIT" << endl;

	while (loop != 2)
	{
		cout << "\nChoice(menu) ";
		cin >> loop;
		switch (loop)
		{
		case 1:
			cout << "Enter word: ";
			cin >> word;

			cout << "\nVerification:\n Word '";
			for (int i = 0; i < strlen(word); i++) { cout << word[i]; }
			cout << "' --> ";
			validation('A', word[0], word);
			break;
		case 2:
			break;
		default:
			break;
		}
	}
}
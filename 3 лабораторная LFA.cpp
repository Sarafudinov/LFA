#include <iostream>
#include <math.h>
#include <string>
int length = 10;//количество правил

using namespace std;

//объект - правило
class Rule
{
public:
	Rule() {
		left = "";
		right = "";
	}

	void operator=(Rule& other) {
		this->left = other.left;
		this->right = other.right;
	}

	string left;
	string right;
};

ostream& operator<<(ostream& out, Rule obj) {
	out << " " << obj.left << " -> " << obj.right << endl;
	return out;
};


//УДАЛЕНИЕ ЦЕПНЫХ ПРАВИЛ
//*************************************************************************
int multiplier(char mark, string set) {
	int count = 0;
	for (int i = 0; i < set.length(); i++)
	{
		if (mark != set[i])
		{
			count++;
		}
	}
	return count;
}

int count_rules(char symbol, Rule* rules) {
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		if (symbol == rules[i].left[0]) { count++; }
	}
	return count;
}

int new_len(char symbol, string set, Rule* rules) {
	if (set.length() != 1) { 
		return length + count_rules(set[0], rules) * multiplier(symbol, set); }
	return length;
}

void add(Rule* rules, string set, char mark, int len) {
	
	int k = len;
	for (int i = 0; i < len; i++)
	{
		int j = 1;
		if (rules[i].left[0] == mark)
		{
			while (j < set.length())
			{
				rules[k].left = set[j];
				rules[k].right = rules[i].right;
				k++;
				j++;
			}
		}
	}
}

Rule* delete_chain_rules(Rule* rules) {
	string RS = "S"; string RA = "A";
	string RB = "B"; string RC = "C";
	int count = 0;
	int pos[10] = { 0 };
	//заполнение множеств цепнях правил
	int j = 0;
	for (int i = 0; i < length; i++)
	{
		if (rules[i].left.length() == rules[i].right.length() && (rules[i].right[0] >= 'A' && rules[i].right[0] <= 'Z'))
		{
			cout << " " << i + 1 <<". "<< rules[i]; count++;
			pos[j++] = i;
			if (rules[i].right[0] == RS[0] && RS.find(rules[i].left[0]) == string::npos) { RS += rules[i].left[0]; }
			if (rules[i].right[0] == RA[0] && RA.find(rules[i].left[0]) == string::npos) { RA += rules[i].left[0]; }
			if (rules[i].right[0] == RB[0] && RB.find(rules[i].left[0]) == string::npos) { RB += rules[i].left[0]; }
			if (rules[i].right[0] == RC[0] && RC.find(rules[i].left[0]) == string::npos) { RC += rules[i].left[0]; }
		}
	}

	//вывод результата на экран
	cout << endl;
	cout << "RS = " << RS << "\t RA = " << RA << endl;
	cout << "RB = " << RB << "\t RC = " << RC << endl;

	//расчет новой длины с учётом всех цепных правил
	
	int len = length;
	if (length != new_len('S', RS, rules)) { len = new_len('S', RS, rules); }
	if (length != new_len('A', RA, rules)) { len = new_len('A', RA, rules); }
	if (length != new_len('B', RB, rules)) { len = new_len('B', RB, rules); }
	if (length != new_len('C', RC, rules)) { len = new_len('C', RC, rules); }
	len -= count;
	//выделение новой памяти 
	Rule* new_rules = new Rule[len];
	int k = 0, i = 0; j = 0;
	while (k<length)
	{
		if (k == pos[j]) { k++; j++; }
		new_rules[i] = rules[k];
		i++; k++;
	}

	int temp_len = length - count ;
	length = len;

	//добавление новых правил
	add(new_rules, RS, 'S', temp_len);
	add(new_rules, RA, 'A', temp_len);
	add(new_rules, RB, 'B', temp_len);
	add(new_rules, RC, 'C', temp_len);

	delete[]rules;
	return new_rules;
}
//*************************************************************************


//УДАЛЕНИЕ ЭПСИЛОН ПРОДУКЦИИ
//*************************************************************************
int find_len(char N_eps, string rule) {
	int cout = 0;
	for (int i = 0; i < rule.length(); i++)
	{
		if (N_eps == rule[i])
		{
			cout++;
		}
	}
	return pow(2,cout)-1;
}

void add_rul(char N_eps, Rule* rules, int len) {
	int k = len;
	for (int i = 0; i < len; i++)
	{
		string temp = rules[i].right;
		string save = temp;
		while (temp.find(N_eps) != string::npos)
		{
			if (find_len(N_eps, temp) > 1) {
				rules[k].left = rules[i].left;
				rules[k].right = temp.replace(temp.rfind(N_eps), 1, "");
				k++;
				temp = save;
			}

			rules[k].left = rules[i].left;
			rules[k].right = temp.replace(temp.find(N_eps), 1, "");
			k++;
			
		}
	}
}

Rule* delete_eps(Rule* rules) {
	string N_eps = "";

	//поиск эпсилон продукций
	for (int i = 0; i < length; i++)
	{
		if (rules[i].right == "eps")
		{
			N_eps = rules[i].left;
		}
	}

	//расчёт новой длины правил 
	int new_len = length;
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		count += find_len(N_eps[0], rules[i].right);
	}
	new_len += count-1;

	//выделение памяти
	Rule* new_rule = new Rule[new_len];

	//перезапись существующих правил
	int j = 0;
	for (int i = 0; i < length; i++)
	{
		if (rules[i].right != "eps")
		{
			new_rule[j] = rules[i];
			j++;
		}
	}

	//сохранение и изменение общей длины
	int temp_len = length-1;
	length = new_len;

	add_rul(N_eps[0], new_rule, temp_len);
	
	delete[]rules;
	return new_rule;
}
//*************************************************************************


//УДАЛЕНИЕ НЕ ПРОДУКТИВНЫХ СИМВОЛОВ
//*************************************************************************
bool find_ch(char search, string word) {
	if (word.find(search) != string::npos)
	{
		return false;
	}
	return true;
}

Rule* accessibleness(Rule* rules) {
	string access = "";
	access += rules[0].left;
	int len = count_rules(access[0], rules);
	for (int i = 0; i < len; i++)
	{
		if (access.find(rules[i].left[0]) != string::npos)
		{
			access += rules[i].right;
		}
	}

	int count = 0;
	for (int i = 0; i < access.length(); i++)
	{
		char sign = access[i];
		for (int j = i+1; j < access.length(); j++)
		{
			if (sign == access[j]) {  
				count++;
				for (int k = j; k < access.length()-1; k++)
				{
					access[k] = access[k + 1];
				}

			}
		}
	}
	int word_len = access.length() - count + 1;
	while (word_len < access.length())
	{
		access[word_len++] = '\0';
	}

	string Vt = "ad";	string Vn = "SABC";
	string Vn_and_Vt = Vn + Vt;
	string inaccess;

	for (int i = 0; i < Vn_and_Vt.length(); i++)
	{
		if (find_ch(Vn_and_Vt[i], access) )
		{
			inaccess += Vn_and_Vt[i];
		}
	}

	int k = 0;
	for (int i = 0; i < length - 1; i++)
	{
		if (rules[i].left == inaccess) { k++; }
		rules[i] = rules[k];
		k++;
	}

	Rule* new_rules = new Rule[length - 1];
	for (int i = 0; i < length-1; i++)
	{
		new_rules[i] = rules[i];
	}

	length--;
	delete[]rules;
	return new_rules;
}
//*************************************************************************


//Приведение к нормальной форме Хомского
//*************************************************************************
Rule* resize(Rule* rules, int new_len) {
	Rule* new_rules = new Rule[new_len];
	for (int i = 0; i < length; i++)
	{
		new_rules[i] = rules[i];
	}
	length = new_len;
	delete[]rules;
	return new_rules;
}

Rule* new_char(Rule* rules, string one, char two, string new_name) {

	for (int i = 0; i < length; i++)
	{
		if (rules[i].right.length() > 2)
		{
			int pos = rules[i].right.find(one);
			if (pos != string::npos && rules[i].right[pos+one.length()] == two) { rules[i].right.replace(pos, one.length() + 1, new_name); }
		}
	}

	int temp_len = length;
	rules = resize(rules, length + 1);

	string temp = "";
	temp += one; temp += two;

	rules[temp_len].left = new_name; rules[temp_len].right = temp;
	return rules;
}

Rule* Homsk(Rule* rules) {
	string Vt = "ad";	string Vn = "SABC";
	int pos;
	for (int k = 0; k < Vt.length(); k++)
	{
		for (int i = 0; i < length; i++)
		{
			if (rules[i].right.length() > 1)
			{
				for (int j = 0; j < rules[i].right.length(); j++)
				{
					if (rules[i].right[j] == 'a') { rules[i].right.replace(j, 1, "Y"); }
					if (rules[i].right[j] == 'd') { rules[i].right.replace(j, 1, "X"); }
				}
			}
		}
	}
	
	int temp_len = length;
	rules = resize(rules, length + 2);

	rules[temp_len].left = "X"; rules[temp_len].right = "d";
	rules[temp_len+1].left = "Y"; rules[temp_len+1].right = "a";

	rules = new_char(rules, "Y", 'B', "Z1");
	rules = new_char(rules, "Y", 'X', "Z2");
	rules = new_char(rules, "A", 'B', "Z3");
	rules = new_char(rules, "Z1", 'X', "Z4");

	return rules;
}
//*************************************************************************

void print(Rule* rules) {
	for (int i = 0; i < length; i++)
	{
		cout << " " << i + 1 << "." << rules[i];
	}
}

Rule* clean(Rule* rules) {
	int cout = 0;
	for (int i = 0; i < length; i++)
	{
		Rule temp = rules[i];
		for (int j = i + 1; j < length; j++)
		{
			if (temp.left == rules[j].left && temp.right == rules[j].right)
			{
				cout++;
				for (int k = j; k < length - 1; k++)
				{
					rules[k] = rules[k + 1];
				}
			}
		}
	}
	int clean_pos = length - cout + 1;
	length = clean_pos;

	Rule* new_rules = new Rule[clean_pos];
	for (int i = 0; i < length; i++)
	{
		new_rules[i] = rules[i];
	}

	delete[]rules;
	return new_rules;
}

void init(Rule* rules) {
	rules[0].left = "S";		rules[0].right = "dB";
	rules[1].left = "S";		rules[1].right = "A"; 
	rules[2].left = "A";		rules[2].right = "d";
	rules[3].left = "A";		rules[3].right = "dS";
	rules[4].left = "A";		rules[4].right = "aBdAB";
	rules[5].left = "B";		rules[5].right = "a";
	rules[6].left = "B";		rules[6].right = "dA";
	rules[7].left = "B";		rules[7].right = "A";
	rules[8].left = "B";		rules[8].right = "eps";
	rules[9].left = "C";		rules[9].right = "Aa";
}

int main()
{

	setlocale(LC_ALL, "ru");

	//выделение памяти
	Rule* rules = new Rule[length];
	
	//заполнение массива объектов
	init(rules);

	//вывод на экран
	cout << "Rules:" << endl;
	print(rules);

	rules = delete_eps(rules);

	//вывод на экран
	cout << "\nRules (после удаления эпсилон продукции):" << endl;
	print(rules);

	cout << endl;
	rules = delete_chain_rules(rules);
	rules = clean(rules);

	//вывод на экран
	cout << "\nRules (после удаления цепных правил):" << endl;
	print(rules);

	rules = accessibleness(rules);

	cout << "\nRules (после удаления непродуктивных символов):" << endl;
	print(rules);

	rules = Homsk(rules);

	cout << "\nHomski" << endl;
	print(rules);
}
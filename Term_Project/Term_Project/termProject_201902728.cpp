#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

using namespace std;

static int start = 0;
static int last = 0;

bool is_number(const string& s) {
	// ���ڿ� ���� �ִ��� �˻�
	string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void printLine() {
	for (int i = 1; i < 82; i++) {
		cout << "-";
	}
	cout << endl;
}

void printConsole(string e) {
	printLine();
	cout << " n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
	printLine();
	cout << e << endl;
	printLine();
}

int printPage(int startIndex, vector<string>& vector) {
	//���� ��ġ ����
	start = startIndex;

	// ����: 20 line, ����: 75 byte
	int byteCount = 0;

	for (int lineCount = 1; lineCount <= 20; lineCount++) {
		printf("%2d| ", lineCount);
		while (vector[startIndex] != "\0" && byteCount <= 76) {
			byteCount += vector[startIndex].length();
			if (byteCount <= 76) {
				cout << vector[startIndex];
			}
			else
				startIndex--;
			startIndex++;
		}
		cout << endl;
		byteCount = 0;
	}

	return startIndex;
}

string inputUserMessage() {
	string input;
	cout << " �Է�: ";
	cin >> input;
	return input;
}

void inputFileToVector(fstream& file, vector<string>& vector) {
	// ���� ���� vector�� push_back
	string temp;
	while (file.peek() != EOF) {
		file >> temp;
		vector.push_back(temp + ' ');
	}
	vector.push_back("\0");
}

void checkInput_parameter(string& temp, vector<string>& vector, int& index) {
	// ' (  ,  ) ' �Է� Ȯ��
	if (temp[0] != '(') {
		printPage(start, vector);
		throw string("��ȣ�� �������� �ʽ��ϴ�.");
	}
	if (temp[temp.length() - 1] != ')') {
		printPage(start, vector);
		throw string("��ȣ�� �������� �ʽ��ϴ�.");
	}
	temp = temp.substr(1);
	if ((index = temp.find(",")) == string::npos) {
		printPage(start, vector);
		throw string("���ڰ� �����մϴ�.");
	}
}

void checkInput_length(string s, vector<string>& vector) {
	if (s.length() >= 2) {
		last = printPage(start, vector);
		throw string("���ʿ��� �Է��� �ֽ��ϴ�.");
	}
}

void printHelp(int& index, vector<string>& vector) {
	int byteCount = 0;
	int lastIndex = index;
	for (int i = 0; i < 20; i++) {
		while (byteCount < 76) {
			lastIndex--;
			if (lastIndex < 0) {
				lastIndex = 0;
				break;
			}
			byteCount += vector[lastIndex].length();
			if (byteCount > 76) {
				byteCount = 0;
				lastIndex++;
				break;
			}
			else if (byteCount == 76) {
				byteCount = 0;
				break;
			}

		}
		if (lastIndex < 0) {
			lastIndex = 0;
			break;
		}
	}
	last = printPage(lastIndex, vector);
}

class Strategy {
public:
	virtual void doWork(string s, vector<string>& vector) = 0;
};

class n_nextPage : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {

		// ���ڰ� ���� ����� �Է� ���� Ȯ��
		checkInput_length(s, vector);

		int startIndex = last;
		int byteCount = 0; // ����Ʈ�� �Ѿ���� Ȯ��
		bool lastWord_exist = true; // ������ �ܾ ���ԵǾ����� Ȯ��

		for (int lineCount = 1; lineCount <= 20; lineCount++) {
			while (vector[startIndex] != "\0" && byteCount <= 76) {
				byteCount += vector[startIndex].length();
				if (byteCount <= 76) {
					lastWord_exist = false;
					startIndex++;
				}
			}
			byteCount = 0;
		}

		// ������ �ܾ ����
		if (lastWord_exist) {
			printHelp(startIndex, vector);
			throw string("������ �������Դϴ�.");
		}
		//�������� �ʴ� ���
		else {
			last = printPage(last, vector);
			throw string("���� �������� ��µǾ����ϴ�.");
		}
	}
};

class p_prevPage : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {

		// ���ڰ� ���� ����� �Է� ���� Ȯ��
		checkInput_length(s, vector);

		//ù ���������� Ȯ��
		if (start == 0) {
			last = printPage(start, vector);
			throw string("ù �������Դϴ�.");
		}

		//���� ������ ���
		printHelp(start, vector);
		throw string("���� �������� �ͷµǾ����ϴ�.");
	}
};

class i_insertWord : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {
		char temp_lineNum[4], temp_wordNum[4];
		char temp_word[80];
		int indexCheck;
		int lineNum, wordNum;
		string temp = s.substr(1);
		
		// (  ,  ) �Է� Ȯ��
		checkInput_parameter(temp, vector, indexCheck);

		// ù ��° ���� ���� Ȯ��
		if (indexCheck > 2) {
			printPage(start, vector);
			throw string("�߸��� ������ ��ġ �Է��Դϴ�.");
		}

		// ������ ��ġ ����
		int i = 0;
		while (i < indexCheck) {
			temp_lineNum[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		temp_lineNum[i] = '\0';
		i++;
		temp = temp.substr(1);
		stringstream s1(temp_lineNum);
		s1 >> lineNum;

		// ������ ��ġ�� ���� �� Ȥ�� ���ڰ� �ƴ��� Ȯ��
		if (!is_number(temp_lineNum) || lineNum > 20 || lineNum <= 0) {
			printPage(start, vector);
			throw string("1~20 ������ ������ �Է��ϼž� �մϴ�.");
		}

		// �� ��° ',' üũ
		if ((indexCheck = temp.find(",")) == string::npos) {
			printPage(start, vector);
			throw string("���ڰ� �����մϴ�.");
		}

		// �� ��° ���� ���� Ȯ��
		if (indexCheck > 2) {
			printPage(start, vector);
			throw string("�߸��� ���� �Է��Դϴ�.");
		}

		// �ܾ��� ��ġ ����
		i = 0;
		while (i < indexCheck) {
			temp_wordNum[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		temp_wordNum[i] = '\0';
		stringstream s2(temp_wordNum);
		s2 >> wordNum;

		// �ܾ��� ��ġ�� ���ڰ� �ƴϰų� 0 �������� Ȯ��
		if (!is_number(temp_wordNum) || wordNum <= 0) {
			printPage(start, vector);
			throw string("�߸��� �ܾ��� ��ġ �Է��Դϴ�.");
		}

		// �Է� ���ڸ� ����
		temp = temp.substr(1);
		if (temp.length() > 76 || temp.length() <= 1) {
			printPage(start, vector);
			throw string("�Է��Ϸ��� ���ڰ� ���ų� 75����Ʈ���� ��ϴ�.");
		}
		i = 0;
		while (temp.length() != 1) {
			temp_word[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		temp_word[i] = ' ';
		temp_word[i + 1] = '\0';
		string insertWord(temp_word);

		// ������ ��ġ�� �̵�
		int findIndex = start;
		int byteCount = 0;
		for (int line = 1; line < lineNum; line++) {
			while (vector[findIndex] != "\0" && byteCount <= 76) {
				byteCount += vector[findIndex].length();
				if (byteCount > 76) {
					findIndex--;
				}
				findIndex++;
				if (vector[findIndex] == "\0") {
					printPage(start, vector);
					throw string("�ش� ������ ���������ʽ��ϴ�.");
				}
			}
			byteCount = 0;
		}

		// �ܾ��� ��ġ�� �̵�
		for (int word = 1; word < wordNum; word++) {
			byteCount += vector[findIndex].length();
			findIndex++;
			if (byteCount > 76 || vector[findIndex] == "\0") {
				printPage(start, vector);
				throw string("�ش� ��ġ�� �ܾ �������� �ʽ��ϴ�.");
			}
		}
		// �ش� ��ġ�� ������ �Է� ���ڸ� �Է�
		vector.insert(vector.begin() + findIndex + 1, insertWord);

		// ���� ���� ���
		printPage(start, vector);
		printConsole("�ش� �ܾ �Է��߽��ϴ�.");
	}
};

class d_deleteWord : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {
		char temp_lineNum[4], temp_wordNum[4];
		int indexCheck;
		int lineNum, wordNum;
		string temp = s.substr(1);

		// (  ,  ) �Է� Ȯ��
		checkInput_parameter(temp, vector, indexCheck);

		// ù ��° ���� ���� Ȯ��
		if (indexCheck > 2) {
			printPage(start, vector);
			throw string("�߸��� ������ ��ġ �Է��Դϴ�.");
		}

		// ������ ��ġ�� ����
		int i = 0;
		while (i < indexCheck) {
			temp_lineNum[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		temp_lineNum[i] = '\0';
		i++;
		temp = temp.substr(1);
		stringstream s1(temp_lineNum);
		s1 >> lineNum;

		// ������ ��ġ�� ���� �� Ȥ�� ���ڰ� �ƴ��� Ȯ��
		if (!is_number(temp_lineNum) || lineNum > 20 || lineNum <= 0) {
			printPage(start, vector);
			throw string("1~20 ������ ������ �Է��ϼž� �մϴ�.");
		}

		// �ܾ��� ��ġ�� ����
		i = 0;
		while (temp.length() != 1) {
			temp_wordNum[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		temp_wordNum[i] = '\0';
		stringstream s2(temp_wordNum);
		s2 >> wordNum;

		// �ܾ��� ��ġ�� ���ڰ� �ƴϰų� 0 �������� Ȯ��
		if (!is_number(temp_wordNum) || wordNum <= 0) {
			printPage(start, vector);
			throw string("�߸��� �ܾ��� ��ġ �Է��Դϴ�.");
		}

		// ������ ��ġ�� �̵�
		int findIndex = start;
		int lineByte = 0;
		for (int line = 1; line < lineNum; line++) {
			while (vector[findIndex] != "\0" && lineByte <= 76) {
				lineByte += vector[findIndex].length();
				if (lineByte > 76) {
					findIndex--;
				}
				findIndex++;
				if (vector[findIndex] == "\0") {
					printPage(start, vector);
					throw string("�ش� ������ ���������ʽ��ϴ�.");
				}
			}
			lineByte = 0;
		}

		// �ܾ��� ��ġ�� �̵�
		for (int word = 1; word < wordNum; word++) {
			lineByte += vector[findIndex].length();
			findIndex++;
			if (lineByte > 76 || vector[findIndex] == "\0") {
				printPage(start, vector);
				throw string("�ش� ��ġ�� �ܾ �������� �ʽ��ϴ�.");
			}
		}
		// �ش� ��ġ�� �ܾ� ����
		vector.erase(vector.begin() + findIndex);

		// ���� ���� ���
		printPage(start, vector);
		printConsole("�־��� �ܾ �����߽��ϴ�.");
	}
};

class c_changeWord : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {
		char temp_firstWord[80];
		char temp_secondWord[80];
		int inputIndex;
		string temp = s.substr(1);

		// (  ,  ) �Է� Ȯ��
		checkInput_parameter(temp, vector, inputIndex);

		//ù ��° ���ڰ� 75����Ʈ �̻��� ��� üũ
		if (inputIndex > 75) {
			printPage(start, vector);
			throw string("ù ��° ���ڰ� 75����Ʈ���� ��ϴ�.");
		}

		// , ���� ���� ����
		int i = 0;
		while (i < inputIndex) {
			temp_firstWord[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		temp_firstWord[i] = ' ';
		temp_firstWord[i + 1] = '\0';
		i++;
		temp = temp.substr(1);
		string firstWord(temp_firstWord);

		// , ���� ���� ����
		int j = 0;
		int length = s.length() - 3;
		while (i < length) {
			temp_secondWord[j] = temp[0];
			temp = temp.substr(1);
			if (j >= 75) {
				printPage(start, vector);
				throw string("�� ��° ���ڰ� 75����Ʈ���� ��ϴ�.");
			}
			i++;
			j++;
		}
		temp_secondWord[j] = ' ';
		temp_secondWord[j + 1] = '\0';
		string secondWord(temp_secondWord);

		//ù ��° ���� �˻� �� �� ��° ���� ��ü
		string temp1(firstWord);
		j = 0;
		bool find = false;
		while (vector[j] != "\0") {
			if (vector[j].compare(firstWord) == 0) {
				find = true;
				vector.erase(vector.begin() + j);
				vector.insert(vector.begin() + j, secondWord);
			}
			j++;
		}
		if (find == false) {
			printPage(start, vector);
			throw string("�ش� �ܾ ã�� �� �����ϴ�.");
		}
		// ���� ���� ���
		printPage(start, vector);
		printConsole("ù ��° �ܾ ��� �� ��° �ܾ�� �����Ͽ����ϴ�.");
	}
};

class s_searchWord : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {
		char searchWord[80];
		string temp = s.substr(1);

		//��ȣ�� ���ų� ������ ���̰� �� ��� üũ
		if (temp[0] != '(' || temp.length() < 2) {
			printPage(start, vector);
			throw string("��ȣ�� �������� �ʽ��ϴ�.");
		}
		if (temp[temp.length() - 1] != ')') {
			printPage(start, vector);
			throw string("��ȣ�� �������� �ʽ��ϴ�.");
		}
		if (temp.length() > 77) {
			printPage(start, vector);
			throw string("���ڰ� 75Byte�� �ѽ��ϴ�.");
		}

		// �Էµ� �ܾ� ����
		temp = temp.substr(1);
		unsigned int i = 0;
		while (i < s.length() - 3) {
			searchWord[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		searchWord[i] = ' ';
		searchWord[i + 1] = '\0';
		string temp2(searchWord);

		// �Էµ� �ܾ �ִ��� Ȯ��
		int j = 0;
		while (vector[j].compare(temp2) != 0) {
			j++;
			if (vector[j] == "\0") {
				printPage(start, vector);
				throw string("�ش� �ܾ ã�� �� �����ϴ�.");
			}
		}

		// ã�� ���ڸ� ó������ ���
		printPage(j, vector);
		printConsole("�־��� �ܾ ã�ҽ��ϴ�.");
	}
};

class t_saveAndExit : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {

		// ���ڰ� ���� ����� �Է� ���� üũ
		checkInput_length(s, vector);

		// ���� txt �����ϱ�
		ofstream writeFile;
		writeFile.open("test.txt");
		int i = 0;
		for (; vector[i + 1] != "\0"; i++) {
			writeFile << vector[i];
		}

		//������ ���ڵ��� ������ ������ ����
		vector[i].erase(remove_if(vector[i].begin(), vector[i].end(), isspace), vector[i].end());
		writeFile << vector[i];
		writeFile.close();
		printPage(start, vector);
		printConsole("���� ������ ���Ͽ� �����߽��ϴ�. ��� �Ŀ� ����˴ϴ�.");
	}
};

class Menu {
public:
	Menu(Strategy* strategy) {
		this->strategy = strategy;
	}
	Menu() {
		this->strategy = NULL;
	}
	virtual void executeMenu(string s, vector<string>& vector) {
		this->strategy->doWork(s, vector);
	}
	void setMenu(Strategy* strategy) {
		this->strategy = strategy;
	}
private:
	Strategy* strategy;
};

int main() {
	//���� �Է�
	fstream inputfile;
	inputfile.open("test.txt");
	if (!inputfile.is_open()) {
		cout << "�ش� ������ �ҷ��� �� �����ϴ�." << endl;
		return 0;
	}

	//vector�� file�� string ����
	vector<string> vector;
	inputFileToVector(inputfile, vector);

	//�ʱ�ȭ�� ���
	last = printPage(last, vector);

	printConsole("");

	//�Է��� �޾� �۾� ����
	Strategy* n = new n_nextPage;
	Strategy* p = new p_prevPage;
	Strategy* i = new i_insertWord;
	Strategy* d = new d_deleteWord;
	Strategy* c = new c_changeWord;
	Strategy* s = new s_searchWord;
	Strategy* t = new t_saveAndExit;
	Menu* selectCommand = new Menu;

	while (true) {
		string userInput = inputUserMessage();
		char menu = userInput[0];
		system("cls");
		try {
			switch (menu) {
			case 'n':
				selectCommand->setMenu(n);
				selectCommand->executeMenu(userInput, vector);
				break;
			case 'p':
				selectCommand->setMenu(p);
				selectCommand->executeMenu(userInput, vector);
				break;
			case 'i':
				selectCommand->setMenu(i);
				selectCommand->executeMenu(userInput, vector);
				break;
			case 'd':
				selectCommand->setMenu(d);
				selectCommand->executeMenu(userInput, vector);
				break;
			case 'c':
				selectCommand->setMenu(c);
				selectCommand->executeMenu(userInput, vector);
				break;
			case 's':
				selectCommand->setMenu(s);
				selectCommand->executeMenu(userInput, vector);
				break;
			case 't':
				selectCommand->setMenu(t);
				selectCommand->executeMenu(userInput, vector);
				throw - 1;
				break;
			default:
				last = printPage(start, vector);
				printConsole("�ùٸ� �Է��� �ƴմϴ�.");
				break;
			}
		}
		catch (string e) {
			printConsole(e);
		}
		catch (int) {
			inputfile.close();
			delete n;
			delete p;
			delete i;
			delete d;
			delete c;
			delete s;
			delete t;
			delete selectCommand;
			return 0;
		}
	}
}
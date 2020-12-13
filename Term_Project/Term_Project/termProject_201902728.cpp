#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

using namespace std;

static int start = 0;
static int last = 0;

bool is_number(const string& s) {
	// 문자에 숫자 있는지 검사
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
	cout << " n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
	printLine();
	cout << e << endl;
	printLine();
}

int printPage(int startIndex, vector<string>& vector) {
	//시작 위치 저장
	start = startIndex;

	// 가로: 20 line, 세로: 75 byte
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
	cout << " 입력: ";
	cin >> input;
	return input;
}

void inputFileToVector(fstream& file, vector<string>& vector) {
	// 공백 기준 vector에 push_back
	string temp;
	while (file.peek() != EOF) {
		file >> temp;
		vector.push_back(temp + ' ');
	}
	vector.push_back("\0");
}

void checkInput_parameter(string& temp, vector<string>& vector, int& index) {
	// ' (  ,  ) ' 입력 확인
	if (temp[0] != '(') {
		printPage(start, vector);
		throw string("괄호가 완전하지 않습니다.");
	}
	if (temp[temp.length() - 1] != ')') {
		printPage(start, vector);
		throw string("괄호가 완전하지 않습니다.");
	}
	temp = temp.substr(1);
	if ((index = temp.find(",")) == string::npos) {
		printPage(start, vector);
		throw string("인자가 부족합니다.");
	}
}

void checkInput_length(string s, vector<string>& vector) {
	if (s.length() >= 2) {
		last = printPage(start, vector);
		throw string("불필요한 입력이 있습니다.");
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

		// 인자가 없는 기능의 입력 오류 확인
		checkInput_length(s, vector);

		int startIndex = last;
		int byteCount = 0; // 바이트가 넘어가는지 확인
		bool lastWord_exist = true; // 마지막 단어가 포함되었는지 확인

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

		// 마지막 단어를 포함
		if (lastWord_exist) {
			printHelp(startIndex, vector);
			throw string("마지막 페이지입니다.");
		}
		//포함하지 않는 경우
		else {
			last = printPage(last, vector);
			throw string("다음 페이지가 출력되었습니다.");
		}
	}
};

class p_prevPage : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {

		// 인자가 없는 기능의 입력 오류 확인
		checkInput_length(s, vector);

		//첫 페이지인지 확인
		if (start == 0) {
			last = printPage(start, vector);
			throw string("첫 페이지입니다.");
		}

		//이전 페이지 출력
		printHelp(start, vector);
		throw string("이전 페이지가 촐력되었습니다.");
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
		
		// (  ,  ) 입력 확인
		checkInput_parameter(temp, vector, indexCheck);

		// 첫 번째 인자 길이 확인
		if (indexCheck > 2) {
			printPage(start, vector);
			throw string("잘못된 라인의 위치 입력입니다.");
		}

		// 라인의 위치 저장
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

		// 라인의 위치가 범위 밖 혹은 숫자가 아닌지 확인
		if (!is_number(temp_lineNum) || lineNum > 20 || lineNum <= 0) {
			printPage(start, vector);
			throw string("1~20 사이의 라인을 입력하셔야 합니다.");
		}

		// 두 번째 ',' 체크
		if ((indexCheck = temp.find(",")) == string::npos) {
			printPage(start, vector);
			throw string("인자가 부족합니다.");
		}

		// 두 번째 인자 길이 확인
		if (indexCheck > 2) {
			printPage(start, vector);
			throw string("잘못된 인자 입력입니다.");
		}

		// 단어의 위치 저장
		i = 0;
		while (i < indexCheck) {
			temp_wordNum[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		temp_wordNum[i] = '\0';
		stringstream s2(temp_wordNum);
		s2 >> wordNum;

		// 단어의 위치가 숫자가 아니거나 0 이하인지 확인
		if (!is_number(temp_wordNum) || wordNum <= 0) {
			printPage(start, vector);
			throw string("잘못된 단어의 위치 입력입니다.");
		}

		// 입력 문자를 저장
		temp = temp.substr(1);
		if (temp.length() > 76 || temp.length() <= 1) {
			printPage(start, vector);
			throw string("입력하려는 문자가 없거나 75바이트보다 깁니다.");
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

		// 라인의 위치로 이동
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
					throw string("해당 라인이 존재하지않습니다.");
				}
			}
			byteCount = 0;
		}

		// 단어의 위치로 이동
		for (int word = 1; word < wordNum; word++) {
			byteCount += vector[findIndex].length();
			findIndex++;
			if (byteCount > 76 || vector[findIndex] == "\0") {
				printPage(start, vector);
				throw string("해당 위치에 단어가 존재하지 않습니다.");
			}
		}
		// 해당 위치의 다음에 입력 문자를 입력
		vector.insert(vector.begin() + findIndex + 1, insertWord);

		// 변경 내용 출력
		printPage(start, vector);
		printConsole("해당 단어를 입력했습니다.");
	}
};

class d_deleteWord : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {
		char temp_lineNum[4], temp_wordNum[4];
		int indexCheck;
		int lineNum, wordNum;
		string temp = s.substr(1);

		// (  ,  ) 입력 확인
		checkInput_parameter(temp, vector, indexCheck);

		// 첫 번째 인자 길이 확인
		if (indexCheck > 2) {
			printPage(start, vector);
			throw string("잘못된 라인의 위치 입력입니다.");
		}

		// 라인의 위치를 저장
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

		// 라인의 위치가 범위 밖 혹은 숫자가 아닌지 확인
		if (!is_number(temp_lineNum) || lineNum > 20 || lineNum <= 0) {
			printPage(start, vector);
			throw string("1~20 사이의 라인을 입력하셔야 합니다.");
		}

		// 단어의 위치를 저장
		i = 0;
		while (temp.length() != 1) {
			temp_wordNum[i] = temp[0];
			temp = temp.substr(1);
			i++;
		}
		temp_wordNum[i] = '\0';
		stringstream s2(temp_wordNum);
		s2 >> wordNum;

		// 단어의 위치가 숫자가 아니거나 0 이하인지 확인
		if (!is_number(temp_wordNum) || wordNum <= 0) {
			printPage(start, vector);
			throw string("잘못된 단어의 위치 입력입니다.");
		}

		// 라인의 위치로 이동
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
					throw string("해당 라인이 존재하지않습니다.");
				}
			}
			lineByte = 0;
		}

		// 단어의 위치로 이동
		for (int word = 1; word < wordNum; word++) {
			lineByte += vector[findIndex].length();
			findIndex++;
			if (lineByte > 76 || vector[findIndex] == "\0") {
				printPage(start, vector);
				throw string("해당 위치에 단어가 존재하지 않습니다.");
			}
		}
		// 해당 위치의 단어 삭제
		vector.erase(vector.begin() + findIndex);

		// 변경 내용 출력
		printPage(start, vector);
		printConsole("주어진 단어를 삭제했습니다.");
	}
};

class c_changeWord : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {
		char temp_firstWord[80];
		char temp_secondWord[80];
		int inputIndex;
		string temp = s.substr(1);

		// (  ,  ) 입력 확인
		checkInput_parameter(temp, vector, inputIndex);

		//첫 번째 인자가 75바이트 이상인 경우 체크
		if (inputIndex > 75) {
			printPage(start, vector);
			throw string("첫 번째 인자가 75바이트보다 깁니다.");
		}

		// , 앞의 인자 저장
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

		// , 뒤의 인자 저장
		int j = 0;
		int length = s.length() - 3;
		while (i < length) {
			temp_secondWord[j] = temp[0];
			temp = temp.substr(1);
			if (j >= 75) {
				printPage(start, vector);
				throw string("두 번째 인자가 75바이트보다 깁니다.");
			}
			i++;
			j++;
		}
		temp_secondWord[j] = ' ';
		temp_secondWord[j + 1] = '\0';
		string secondWord(temp_secondWord);

		//첫 번째 인자 검색 후 두 번째 인자 교체
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
			throw string("해당 단어를 찾을 수 없습니다.");
		}
		// 변경 내용 출력
		printPage(start, vector);
		printConsole("첫 번째 단어를 모두 두 번째 단어로 변경하였습니다.");
	}
};

class s_searchWord : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {
		char searchWord[80];
		string temp = s.substr(1);

		//괄호가 없거나 문자의 길이가 긴 경우 체크
		if (temp[0] != '(' || temp.length() < 2) {
			printPage(start, vector);
			throw string("괄호가 완전하지 않습니다.");
		}
		if (temp[temp.length() - 1] != ')') {
			printPage(start, vector);
			throw string("괄호가 완전하지 않습니다.");
		}
		if (temp.length() > 77) {
			printPage(start, vector);
			throw string("문자가 75Byte를 넘습니다.");
		}

		// 입력된 단어 저장
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

		// 입력된 단어가 있는지 확인
		int j = 0;
		while (vector[j].compare(temp2) != 0) {
			j++;
			if (vector[j] == "\0") {
				printPage(start, vector);
				throw string("해당 단어를 찾을 수 없습니다.");
			}
		}

		// 찾은 문자를 처음으로 출력
		printPage(j, vector);
		printConsole("주어진 단어를 찾았습니다.");
	}
};

class t_saveAndExit : public Strategy {
public:
	void doWork(string s, vector<string>& vector) {

		// 인자가 없는 기능의 입력 오류 체크
		checkInput_length(s, vector);

		// 현재 txt 저장하기
		ofstream writeFile;
		writeFile.open("test.txt");
		int i = 0;
		for (; vector[i + 1] != "\0"; i++) {
			writeFile << vector[i];
		}

		//마지막 문자뒤의 공백을 지워서 저장
		vector[i].erase(remove_if(vector[i].begin(), vector[i].end(), isspace), vector[i].end());
		writeFile << vector[i];
		writeFile.close();
		printPage(start, vector);
		printConsole("변경 내용을 파일에 저장했습니다. 잠시 후에 종료됩니다.");
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
	//파일 입력
	fstream inputfile;
	inputfile.open("test.txt");
	if (!inputfile.is_open()) {
		cout << "해당 파일을 불러올 수 없습니다." << endl;
		return 0;
	}

	//vector에 file의 string 저장
	vector<string> vector;
	inputFileToVector(inputfile, vector);

	//초기화면 출력
	last = printPage(last, vector);

	printConsole("");

	//입력을 받아 작업 수행
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
				printConsole("올바른 입력이 아닙니다.");
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
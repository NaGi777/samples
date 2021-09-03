#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "math.h"
#include <conio.h>


using namespace  std;


template<typename T>
class SP
{
	T* pointer;
public:
	SP()
	{
		pointer = NULL;
	}
	SP(T* pointer)
	{
		this->pointer = pointer;
	}
	SP(const SP& other)
	{
		pointer = other.pointer();
	}
	~SP()
	{
		delete pointer;
	}

	SP operator=(const SP& other)
	{
		pointer = other.pointer;
		return *this;
	}

	void set(T* pointer)
	{
		if (this->pointer)
			delete this->pointer;

		this->pointer = pointer;
	}
	T& getObj()
	{
		return *pointer;
	}
	T* getPtr()
	{
		return pointer;
	}
};


class Error : exception    // пользовательский класс исключений
{
	int error;

public:
	Error(int numberError)
	{
		error = numberError;
	}

	const char* what() const
	{
		switch (error)
		{
		case 3: return "Обращение к не существующему элементу";
		case 4: return "ID установлен не был";
		case 5: return "Попытка установить не существующий тип";
		case 6: return "Введен не верный логин или пароль";
		case 7: return "Нет доступа в этим возможностям";
		case 8: return "Такой пользователь уже существует";
		case 9:return "Такой тип уже существует";
		}

	}

	int getNumberError()
	{
		return error;
	}
};



bool operator==(string str1, string str2)
{
	if (str1.size() != str2.size())
		return false;

	for (int i = 0; i < str1.size(); i++)
		if (str1[i] != str2[i])
			return false;

	return true;
}

bool operator<(string str1, string str2)
{
	for (int i = 0; i < str1.size() || i < str2.size(); i++)
		if (str1[i] != str2[i])
		{
			if (str1[i] > str2[i])
				return true;
			else
				return false;
		}

	return false;
}

bool operator>(string str1, string str2)
{
	for (int i = 0; i < str1.size() || i < str2.size(); i++)
		if (str1[i] != str2[i])
		{
			if (str1[i] < str2[i])
				return true;
			else
				return false;
		}

	return false;
}

char* stc(string& s1, char s2[256])
{
	for (int i = 0; i < s1.size(); i++)
		s2[i] = s1[i];

	s2[s1.size()] = '\0';
	return s2;
}




class TypeDoctor
{
	static vector<string> list;
	string type;

public:
	static void addType(string type)
	{
		for (int i = 0; i < list.size(); i++)
			if (type == list[i])
				throw Error(9);

		list.push_back(type);
	}
	static void delType(int number)
	{
		if (number < 0 || number >= list.size())
			throw Error(3);

		list.erase(list.begin() + number);
	}
	static int size()
	{
		return list.size();
	}
	static string viewType(int number)
	{
		if (number < 0 || number >= list.size())
			throw Error(3);

		return list[number];
	}

	TypeDoctor()
	{
		type = "xxx";
	}
	TypeDoctor(int ID)
	{
		if (ID < 0 || ID >= list.size())
			throw Error(5);

		this->type = list[ID];
	}
	TypeDoctor(const TypeDoctor& other)
	{
		type = other.type;
	}
	TypeDoctor(ifstream& file)
	{
		char str[256];
		file.read(str, sizeof(str));
		type = str;
	}

	TypeDoctor& operator=(const TypeDoctor& other)
	{
		type = other.type;
		return *this;
	}

	void setType(int ID)
	{
		if (ID < 0 || ID >= list.size())
			throw Error(4);

		this->type = list[ID];
	}
	string getType()
	{
		if (type == "xxx")
			throw Error(4);

		return type;
	}

	friend ostream& operator<<(ostream& out, const TypeDoctor& other);

	void write(ofstream& file)
	{
		char str[256];
		file.write(stc(type, str), sizeof(str));
	}
	void read(ifstream& file)
	{
		char str[256];
		file.read(str, sizeof(str));
		type = str;
	}
};
vector<string> TypeDoctor::list;
ostream& operator<<(ostream& out, const TypeDoctor& other)
{
	out << other.type;
	return out;
}

class User
{
	string name;
	string password;

	string encrypt(string password)
	{
		for (int i = 0; i < password.size(); i++)
			password[i] = ~password[i];

		return password;
	}
public:
	User()
	{

	}
	User(string name, string password)
	{
		this->name = name;
		this->password = encrypt(password);
	}
	User(const User& other)
	{
		name = other.name;
		password = other.password;

	}
	User& operator=(const User& other)
	{
		name = other.name;
		password = other.password;
		return *this;
	}

	void setName(string name)
	{
		this->name = name;
	}
	string getName()
	{
		return name;
	}

	void setPassword(string password)
	{
		this->password = encrypt(password);
	}
	string getPassword()
	{
		return encrypt(password);
	}

	virtual void write(ofstream& file)
	{
		char str[256];

		file.write(stc(name, str), sizeof(str));
		file.write(stc(password, str), sizeof(str));
	}
	virtual	void read(ifstream& file)
	{
		char str[256];

		file.read(str, sizeof(str));
		name = str;

		file.read(str, sizeof(str));
		password = str;
	}
};

class Doctor : public User
{
	TypeDoctor type;

public:
	Doctor() : User()
	{

	}
	Doctor(string name, string password, TypeDoctor type) : User(name, password)
	{
		this->type = type;
	}
	Doctor(const Doctor& other) : User((User)other)
	{
		type = other.type;
	}
	Doctor& operator=(Doctor& other)
	{
		type = other.type;
		setName(other.getName());
		setPassword(other.getPassword());
		return *this;
	}

	TypeDoctor& getType()
	{
		return type;
	}
	void setType(TypeDoctor type)
	{
		this->type = type;
	}

	void write(ofstream& file)
	{
		User::write(file);
		type.write(file);

	}
	void read(ifstream& file)
	{
		User::read(file);
		type.read(file);
	}
};

class Ticket
{
	string date, time, nameDoctor;

public:
	Ticket()
	{
		date = "non";
		time = "non";
		nameDoctor = "non";
	}
	Ticket(string date, string time, string nameDoctor)
	{
		this->date = date;
		this->time = time;
		this->nameDoctor = nameDoctor;
	}
	Ticket(const Ticket& other)
	{
		date = other.date;
		time = other.time;
		nameDoctor = other.nameDoctor;
	}
	Ticket& operator=(const Ticket& other)
	{
		date = other.date;
		time = other.time;
		nameDoctor = other.nameDoctor;
		return *this;
	}

	string getDate()
	{
		return date;
	}
	string getTime()
	{
		return time;
	}
	string getNameDoctor()
	{
		return nameDoctor;
	}

	void setDate(string date)
	{
		this->date = date;
	}
	void setTime(string time)
	{
		this->time = time;
	}
	void setNameDoctor(string nameDoctor)
	{
		this->nameDoctor = nameDoctor;
	}

	void write(ofstream& file)
	{
		char str[256];
		file.write(stc(date, str), sizeof(str));
		file.write(stc(time, str), sizeof(str));
		file.write(stc(nameDoctor, str), sizeof(str));
	}
	void read(ifstream& file)
	{
		char str[256];
		file.read(str, sizeof(str));
		date = str;
		file.read(str, sizeof(str));
		time = str;
		file.read(str, sizeof(str));
		nameDoctor = str;
	}
};


class Patient : public User
{
	vector<Ticket> list;

public:
	Patient() : User()
	{

	}
	Patient(string name, string password) : User(name, password)
	{

	}
	Patient(const Patient& other) : User((User)other)
	{
		for (int i = 0; i < other.list.size(); i++)
			list.push_back(other.list[i]);
	}
	Patient& operator=(Patient& other)
	{
		for (int i = 0; i < other.list.size(); i++)
			list.push_back(other.list[i]);
		setName(other.getName());
		setPassword(other.getPassword());
		return *this;
	}

	Ticket& operator[](int i)
	{
		if (i < 0 || i >= list.size())
			throw Error(3);

		return list[i];
	}
	void add(Ticket t)
	{
		list.push_back(t);
	}
	void del(int i)
	{
		if (i < 0 || i >= list.size())
			throw Error(3);

		list.erase(list.begin() + i);
	}
	int size()
	{
		return list.size();
	}

	void write(ofstream& file)
	{
		User::write(file);

		int data = list.size();
		file.write(reinterpret_cast<char*>(&data), sizeof(int));
		for (int i = 0; i < data; i++)
			list[i].write(file);
	}
	void read(ifstream& file)
	{
		User::read(file);

		int data = 0;
		file.read(reinterpret_cast<char*>(&data), sizeof(int));
		for (int i = 0; i < data; i++)
		{
			Ticket a;
			a.read(file);
			list.push_back(a);
		}
	}
};


class System
{
	vector<Doctor> docs;
	vector<Patient> pats;

	bool isLog, isDoc;
	int current;

public:
	System()
	{
		isLog = false;
	}

	bool checkLogin()
	{
		return isLog;
	}
	bool checkDoc()
	{
		return isDoc;
	}

	void reg(string name, string password, TypeDoctor type)
	{
		for (int i = 0; i < docs.size(); i++)
			if (docs[i].getName() == name)
				throw Error(8);

		for (int i = 0; i < pats.size(); i++)
			if (pats[i].getName() == name)
				throw Error(8);

		docs.push_back(Doctor(name, password, type));
	}
	void reg(string name, string password)
	{
		for (int i = 0; i < docs.size(); i++)
			if (docs[i].getName() == name)
				throw Error(8);

		for (int i = 0; i < pats.size(); i++)
			if (pats[i].getName() == name)
				throw Error(8);

		pats.push_back(Patient(name, password));
	}
	void log(string name, string password)
	{
		for (int i = 0; i < docs.size(); i++)
			if (docs[i].getName() == name)
			{
				current = i;
				isDoc = true;
				isLog = true;
				return;
			}

		for (int i = 0; i < pats.size(); i++)
			if (pats[i].getName() == name)
			{
				current = i;
				isDoc = false;
				isLog = true;
				return;
			}

		throw Error(6);
	}
	void relog()
	{
		isLog = false;
	}

	Doctor& getDoctor()
	{
		if (!isLog)
			throw Error(7);
		if (!isDoc)
			throw Error(7);

		return docs[current];
	}
	Patient& getPacient()
	{
		if (!isLog)
			throw Error(7);
		if (isDoc)
			throw Error(7);

		return pats[current];
	}

	vector<Doctor> getDoctors()
	{
		return docs;
	}



	void write(ofstream& file)
	{
		int data = docs.size();
		file.write(reinterpret_cast<char*>(&data), sizeof(int));
		for (int i = 0; i < data; i++)
			docs[i].write(file);

		data = pats.size();
		file.write(reinterpret_cast<char*>(&data), sizeof(int));
		for (int i = 0; i < data; i++)
			pats[i].write(file);
	}
	void read(ifstream& file)
	{
		int data = 0;
		file.read(reinterpret_cast<char*>(&data), sizeof(int));
		for (int i = 0; i < data; i++)
		{
			Doctor a;
			a.read(file);
			docs.push_back(a);
		}

		file.read(reinterpret_cast<char*>(&data), sizeof(int));
		for (int i = 0; i < data; i++)
		{
			Patient a;
			a.read(file);
			pats.push_back(a);
		}
	}
};



namespace SAVE
{
	void read(System& s)
	{
		ifstream file("data.txt", ios::binary);
		if (file.eof())
		{

			return;
		}

		int data = 0;
		char str[256];

		file.read(reinterpret_cast<char*>(&data), sizeof(int));
		for (int i = 0; i < data; i++)
		{
			file.read(str, sizeof(char) * 256);
			TypeDoctor::addType(str);
		}

		s.read(file);
		file.close();
	}

	void write(System& s)
	{
		ofstream file("data.txt", ios::binary);

		int data = TypeDoctor::size();
		char str[256];

		file.write(reinterpret_cast<char*>(&data), sizeof(int));
		for (int i = 0; i < data; i++)
		{
			string str2 = TypeDoctor::viewType(i);
			file.write(stc(str2, str), sizeof(char) * 256);
		}

		s.write(file);
		file.close();
	}
}


int main()
{
	setlocale(LC_ALL, "rus");
	system("chcp 1251"); system("cls");

	SP<System> S(new System);
	SAVE::read(S.getObj());


	if (TypeDoctor::size() == 0)
	{
		TypeDoctor::addType("Терапевт");
		TypeDoctor::addType("Хирург");
		TypeDoctor::addType("Лор");
	}

	while (true)
	{
		if (!S.getObj().checkLogin())
		{
			cout << "1. Регистрация врача\n2. Регистрация пациента\n3. Вход\n0. Выход\n";
			int scene;
			cin >> scene;
			system("cls");

			switch (scene)
			{
			case 1:
			{
				cout << "Введите имя" << endl;
				cin.ignore(cin.rdbuf()->in_avail());
				char name[256];
				cin.getline(name, 256);
				system("cls");

				cout << "Введите пароль" << endl;
				cin.ignore(cin.rdbuf()->in_avail());
				char password[256];
				cin.getline(password, 256);
				system("cls");

				for (int i = 0; i < TypeDoctor::size(); i++)
					cout << i + 1 << ". " << TypeDoctor::viewType(i) << endl;

				cout << endl << "Выберите тип" << endl;
				int type;
				cin >> type;
				type--;
				system("cls");

				if (type < 0 || type >= TypeDoctor::size())
				{
					cout << "Неверный выбор" << endl;
					system("pause"); system("cls");
					break;
				}

				try
				{
					S.getObj().reg(name, password, TypeDoctor(type));
					cout << "Регистрация завершена" << endl;
				}
				catch (Error error)
				{
					cout << error.what() << endl;
				}
				system("pause"); system("cls");
				break;
			}
			case 2:
			{
				cout << "Введите имя" << endl;
				cin.ignore(cin.rdbuf()->in_avail());
				char name[256];
				cin.getline(name, 256);
				system("cls");

				cout << "Введите пароль" << endl;
				cin.ignore(cin.rdbuf()->in_avail());
				char password[256];
				cin.getline(password, 256);
				system("cls");

				try
				{
					S.getObj().reg(name, password);
					cout << "Регистрация завершена" << endl;
				}
				catch (Error error)
				{
					cout << error.what() << endl;
				}
				system("pause"); system("cls");
				break;
			}
			case 3:
			{
				cout << "Введите имя" << endl;
				cin.ignore(cin.rdbuf()->in_avail());
				char name[256];
				cin.getline(name, 256);
				system("cls");

				cout << "Введите пароль" << endl;
				cin.ignore(cin.rdbuf()->in_avail());
				char password[256];
				cin.getline(password, 256);
				system("cls");

				try
				{
					S.getObj().log(name, password);
					cout << "Вход выполнен успешно" << endl;
				}
				catch (Error error)
				{
					cout << error.what() << endl;
				}
				system("pause"); system("cls");
				break;
			}
			case 0:
				SAVE::write(S.getObj());
				return 0;
			}
		}
		else
		{
			if (S.getObj().checkDoc())
			{
				cout << "1. Сменить пользователя\n2. Изменить имя\n3. Изменить тип врача\n4. Просмотреть информацию\n0. Выход\n";
				int scene;
				cin >> scene;
				system("cls");

				switch (scene)
				{
				case 1:
					S.getObj().relog();
					break;
				case 2:
				{
					cout << "Введите новое имя" << endl;
					cin.ignore(cin.rdbuf()->in_avail());
					char name[256];
					cin.getline(name, 256);
					system("cls");

					S.getObj().getDoctor().setName(name);
					cout << "Имя было изменено успешно" << endl;
					system("pause"); system("cls");

					break;
				}
				case 3:
				{
					for (int i = 0; i < TypeDoctor::size(); i++)
						cout << i + 1 << ". " << TypeDoctor::viewType(i) << endl;

					cout << endl << "Выберите тип" << endl;
					int type;
					cin >> type;
					type--;
					system("cls");

					if (type < 0 || type >= TypeDoctor::size())
					{
						cout << "Неверный выбор" << endl;
						system("pause"); system("cls");
						break;
					}

					S.getObj().getDoctor().setType(TypeDoctor(type));

					break;
				}
				case 4:
				{
					cout << S.getObj().getDoctor().getName() << endl;
					cout << S.getObj().getDoctor().getType() << endl;
					system("pause"); system("cls");
					break;
				}
				case 0:
					SAVE::write(S.getObj());
					return 0;
				}
			}
			else
			{
				cout << "1. Сменить пользователя\n2. Записаться на прием\n3. Просмотреть записи\n4. Редактировать записи\n5. Посетить врача\n0. Выход\n";
				int scene;
				cin >> scene;
				system("cls");

				switch (scene)
				{
				case 1:
					S.getObj().relog();
					break;
				case 2:
				{
					vector<Doctor> a(S.getObj().getDoctors());

					for (int i = 0; i < a.size(); i++)
						cout << i + 1 << ". " << a[i].getName() << "   " << a[i].getType() << endl;

					if (a.size() == 0)
					{
						cout << "Врачей нет" << endl;
						system("pause"); system("cls");
						break;
					}

					cout << endl << "Выберите врача для посещения" << endl;
					int num;
					cin >> num;
					num--;
					system("cls");

					if (num < 0 || num >= a.size())
					{
						cout << "Неверный выбор" << endl;
						system("pause"); system("cls");
						break;
					}

					cout << "Введите дату" << endl;
					cin.ignore(cin.rdbuf()->in_avail());
					char date[256];
					cin.getline(date, 256);
					system("cls");

					cout << "Введите время" << endl;
					cin.ignore(cin.rdbuf()->in_avail());
					char time[256];
					cin.getline(time, 256);
					system("cls");

					S.getObj().getPacient().add(Ticket(date, time, a[num].getName()));
					break;
				}
				case 3:
				{
					if (S.getObj().getPacient().size() == 0)
					{
						cout << "Записей нет" << endl;
						system("pause"); system("cls");
						break;
					}

					vector<Ticket> a;
					for (int i = 0; i < S.getObj().getPacient().size(); i++)
						a.push_back(S.getObj().getPacient()[i]);

					while (true)
					{
						for (int i = 0; i < a.size(); i++)
							cout << a[i].getNameDoctor() << "   " << a[i].getDate() << "   " << a[i].getTime() << endl;

						cout << endl << "1. Сортировка по имени   2. Сортировка по дате   3. Сортировка по врмени\nДля продолжения нажмите любую клавишу...";

						switch (_getch())
						{
						case '1':
						{
							multimap<string, Ticket> b;
							for (int i = 0; i < a.size(); i++)
								b.emplace(a[i].getNameDoctor(), a[i]);

							a.clear();
							for (auto i = b.begin(); i != b.end(); i++)
								a.push_back(i->second);
							system("cls");
							continue;
						}
						case '2':
						{
							multimap<string, Ticket> b;
							for (int i = 0; i < a.size(); i++)
								b.emplace(a[i].getDate(), a[i]);

							a.clear();
							for (auto i = b.begin(); i != b.end(); i++)
								a.push_back(i->second);
							system("cls");
							continue;
						}
						case '3':
						{
							multimap<string, Ticket> b;
							for (int i = 0; i < a.size(); i++)
								b.emplace(a[i].getTime(), a[i]);

							a.clear();
							for (auto i = b.begin(); i != b.end(); i++)
								a.push_back(i->second);
							system("cls");
							continue;
						}
						}
						system("cls");
						break;
					}
					break;
				}
				case 4:
				{
					if (S.getObj().getPacient().size() == 0)
					{
						cout << "Записей нет" << endl;
						system("pause"); system("cls");
						break;
					}

					for (int i = 0; i < S.getObj().getPacient().size(); i++)
						cout << i + 1 << ". " << S.getObj().getPacient()[i].getNameDoctor() << "   " << S.getObj().getPacient()[i].getDate() << "   " << S.getObj().getPacient()[i].getTime() << endl;

					cout << "Выберите измененямый талон" << endl;
					int num;
					cin >> num;
					num--;
					system("cls");

					if (num < 0 || num >= S.getObj().getPacient().size())
					{
						cout << "Не верный выбор" << endl;
						system("pause"); system("cls");
						break;
					}

					vector<Doctor> a(S.getObj().getDoctors());

					for (int i = 0; i < a.size(); i++)
						cout << i + 1 << ". " << a[i].getName() << "   " << a[i].getType() << endl;

					cout << endl << "Выберите врача для посещения" << endl;
					int type;
					cin >> type;
					type--;
					system("cls");


					if (type < 0 || type >= a.size())
					{
						cout << "Неверный выбор" << endl;
						system("pause"); system("cls");
						break;
					}

					cout << "Введите дату" << endl;
					cin.ignore(cin.rdbuf()->in_avail());
					char date[256];
					cin.getline(date, 256);
					system("cls");

					cout << "Введите время" << endl;
					cin.ignore(cin.rdbuf()->in_avail());
					char time[256];
					cin.getline(time, 256);
					system("cls");

					S.getObj().getPacient()[num].setNameDoctor(a[type].getName());
					S.getObj().getPacient()[num].setDate(date);
					S.getObj().getPacient()[num].setTime(time);

					cout << "Талон измненён" << endl;
					system("pause"); system("cls");
					break;
				}
				case 5:
				{
					if (S.getObj().getPacient().size() == 0)
					{
						cout << "Записей нет" << endl;
						system("pause"); system("cls");
						break;
					}

					for (int i = 0; i < S.getObj().getPacient().size(); i++)
						cout << i + 1 << ". " << S.getObj().getPacient()[i].getNameDoctor() << "   " << S.getObj().getPacient()[i].getDate() << "   " << S.getObj().getPacient()[i].getTime() << endl;

					cout << "Выберите талон" << endl;
					int num;
					cin >> num;
					num--;
					system("cls");

					if (num < 0 || num >= S.getObj().getPacient().size())
					{
						cout << "Не верный выбор" << endl;
						system("pause"); system("cls");
						break;
					}

					S.getObj().getPacient().del(num);
					cout << "Вы посетили врача" << endl;
					system("pause"); system("cls");
					break;
				}
				case 0:
					SAVE::write(S.getObj());
					return 0;
				}
			}
		}
	}
}
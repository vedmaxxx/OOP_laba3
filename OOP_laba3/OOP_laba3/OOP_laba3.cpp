#include <iostream>

class Base {
protected:
	char code = 'B';
	int x = 0, y = 0;
public:

	virtual char getCode() {
		return code;
	}
	virtual void print() {}
	virtual ~Base() {
		printf("\t\t~Base()\n");
	}
};

class Point : public Base {
private:
	int x, y;
public:
	char getCode() override {
		return 'P';
	}
	Point() {
		x = 0;
		y = 0;
	}
	Point(int _x, int _y) {
		x = _x;
		y = _y;
	}
	Point(const Point* p) {
		x = p->x;
		y = p->y;
	}


	void print() override {
		printf(" \t%i %i\n", x, y);
	}
	void setXY(int x, int y) {
		printf("setXY()\n");
		this->x = x;
		this->y = y;
	}
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}

	~Point() {
		printf("\t\t~Point\n");
	}
};
class Segment :public Base {
private:
	//int x1, x2, y1, y2;
	Point* p1;
	Point* p2;

public:
	char getCode() override {
		return 'S';
	}
	Segment() {
		printf("\tSegment() %p\n", this);
		p1 = new Point;
		p2 = new Point;
	}
	Segment(Point* _p1, Point* _p2) {
		printf("\tSegment() %p\n", this);
		p1 = new Point(_p1);
		p2 = new Point(_p2);
	}
	Segment(const Segment* segment) {
		printf("\tSegment() %p\n", this);
		p1 = new Point(segment->p1);
		p2 = new Point(segment->p2);
	}

	void print() override {
		printf("\tsegment {\n");
		//printf("\t");
		p1->print();
		//printf("\t");
		p2->print();
		printf("\t}\n");
	}
	~Segment() {
		printf("\t\t~Segment %p\n", this);
		printf("\t\t{\n");
		delete p1;
		delete p2;
		printf("\t\t}\n");
	}
};
//class Cat :public Base {
//private:
//	char Name[30];
//	char Color[30];
//public:
//	Cat(const Cat* cat) {
//		strcpy(Name, cat->Name);
//		strcpy(Color, cat->Color);
//	}
//
//	void setName(char* string) {
//		strcpy(Name, string);
//	}
//	void setColor(char* string) {
//		strcpy(Color, string);
//	}
//	void print() override {
//		printf("Cat name= %s, the color=%s\n", Name, Color);
//	}
//
//	~Cat() {
//		printf("~Cat() %p\n", this);
//	}
//};

class MyBaseFactory {
public:
	MyBaseFactory() {}
	Base* createBase(Base* p)
	{
		Base* _base = nullptr;
		char code = p->getCode();
		switch (code)
		{
		case 'P':
			_base = new Point((Point*)(p));
			break;

		case 'S':
			_base = new Segment((Segment*)(p));
			break;

		/*case 'C':
			_base = new Cat((Cat*)(p));
			break;*/

		default:;
		}
		return _base;
	}
};

class List {
private:
	class Node {
	public:
		Base* base;
		Node* next; //указатель на следующую ячейку списка

		Node(Base* _base) : next(nullptr) {
			MyBaseFactory factory;
			base = factory.createBase(_base);
		}
		~Node() {
			printf("\t~Node(): %p\n", this);
			printf("\t{\n");
			delete base;
			printf("\t}\n\n");
		}

		bool isEOL() { return (this == nullptr ? 1 : 0); }
	};

	void delete_first() {
		if (isEmpty()) return;

		Node* temp = first;
		first = temp->next;
		delete temp;
	}
	void delete_last() {
		if (isEmpty()) return;
		if (last == first) {
			delete_first();
			return;
		}

		Node* temp = first;
		while (temp->next != last) {
			temp = temp->next;
		}
		delete temp->next;
		temp->next = nullptr;
		last = temp;
	}

public:
	Node* first;
	Node* last;

	void add(Base* _base) {
		Node* another = new Node(_base);
		printf("\tЭлемент добавлен в хранилище\n");

		if (isEmpty()) {
			first = another;
			last = another;
			return;
		}
		last->next = another;
		last = another;
	}
	bool isEmpty() {
		return first == nullptr;
	}
	void deleteObj(Base* _base) {
		if (isEmpty()) {
			printf("\tХранилище пусто, удалить не удалось\n");
			return;
		}
		if (last->base == _base) {
			delete_last();
			printf("\tЭлемент удален\n");
			return;
		}
		if (first->base == _base) {
			delete_first();
			printf("\tЭлемент удален\n");
			return;
		}

		Node* current = first;
		while (current->next != nullptr && current->next->base != _base) {
			current = current->next;
		}
		if (current->next == nullptr) {
			printf("\tТакого элемента нет в списке\n");
			return;
		}
		Node* tmp_next = current->next;
		current->next =
			current->next->next;
		delete tmp_next;
		printf("\tЭлемент удален\n");
	}
	void print() {
		printf("Вывод хранилища:\n[\n");
		Node* current = first;
		while (!(current->isEOL())) {
			if (current) {
				current->base->print();
				current = current->next;
			}
			else {
				printf("\tnullptr\n");
			}
		}
		printf("]\n");
	}
	int getSize() {
		if (isEmpty()) return 0;
		Node* node = first;
		int i = 1;
		while (!node->next->isEOL()) {
			i++;
			node = node->next;
		}
		return i;
	}

	Base* getObj(int i) {
		if (isEmpty()) {
			printf("Хранилище пусто, возвращать нечего\n");
			return nullptr;
		}
		int j = 2;
		Node* current = first;
		while (j < i && !(current->isEOL())) {

			current = current->next;
			j++;
		}
		printf("\tОбъект передан\n");
		return(current->base);
	}
	Base* getObjAndDelete(int i) {
		if (isEmpty()) {
			printf("\tХранилище пусто, возвращать нечего\n");
			return nullptr;
		}
		Base* ret = getObj(i);
		Base* tmp;
		MyBaseFactory factory;
		tmp = factory.createBase(ret);
		deleteObj(ret);
		printf("\tОбъект передан\n");
		return tmp;
	}

	~List() {
		printf("~List()\n");
		if (!(isEmpty())) {
			printf("[______________________________________\n");
			Node* tmp = last;
			while (tmp != first) {
				delete_last();
				tmp = last;
			}
			delete_first();
			printf("________________________________________]\n");
		}
	}
};

int main()
{
	setlocale(LC_ALL, "Rus");

	printf("Создание объектов Point и добавление их в хранилище list\n");

	List list;
	for (int i = 0; i < 5; i++) {
		list.add(new Point(i, i));
	}
	list.print();
	printf("Кол-во элементов: %d\n\n", list.getSize());


	printf("Создание объектов Segment и добавление их в хранилище list\n");
	for (int i = 0; i < 5; i++) {
		Point* p1 = new Point(i + 200, i + 200);
		Point* p2 = new Point(i + 300, i + 300);

		list.add(new Segment(p1, p2));
		delete p1;
		delete p2;
	}
	list.print();
	printf("Кол-во элементов: %d\n\n", list.getSize());




	printf("Изъятие объектов в другое хранилище list2\n");
	{
		List list2;

		list2.print();
		printf("Кол-во элементов: %d\n\n", list2.getSize());

		for (int i = 0; i < 10; i++) {
			list2.add(list.getObj(i));
			printf("\n");
		}
		list2.print();
		printf("Кол-во элементов: %d\n\n", list2.getSize());

	}

	printf("Изъятие объектов с удалением в другое хранилище list2\n");
	{
		List list2;

		printf("list2\n");
		list2.print();
		printf("Кол-во элементов: %d\n\n", list2.getSize());
		int i = 0;

		while (list.getSize() != 0) {

			printf("ЭЛЕМЕНТ %d\n", i);
			i++;
			list2.add(list.getObjAndDelete(1));
			printf("\n");

		}
		printf("list\n");
		list.print();
		printf("Кол-во элементов: %d\n\n", list.getSize());

		printf("list2\n");
		list2.print();
		printf("Кол-во элементов: %d\n\n", list2.getSize());
	}

}
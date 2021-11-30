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
		printf("~Base()\n");
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

	Point(const Point* p) {
		x = p->x;
		y = p->y;
	}

	Point(int _x, int _y) {
		x = _x;
		y = _y;
	}


	void print() override {
		printf(" %i %i\n", x, y);
	}

	void SetXY(int x, int y) {
		printf("SetXY()\n");
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
		printf("~Point\n");
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
		printf("Segment() %p\n", this);
		p1 = new Point;
		p2 = new Point;
	}
	Segment(Point* _p1, Point* _p2) {
		printf("Segment() %p\n", this);
		p1 = new Point(_p1);
		p2 = new Point(_p2);
	}
	Segment(const Segment* segment) {
		printf("Segment() %p\n", this);
		p1 = new Point(segment->p1);
		p2 = new Point(segment->p2);
	}

	void print() override {
		printf("{\n");
		printf("\t");
		p1->print();
		printf("\t");
		p2->print();
		printf("}\n");
	}
	~Segment() {
		printf("\n~Segment %p\n", this);
		printf("{\n");
		delete p1;
		delete p2;
		printf("}\n");
	}
};
class Cat :public Base {
private:
	char Name[30];
	char Color[30];
public:
	Cat(const Cat* cat) {
		strcpy(Name, cat->Name);
		strcpy(Color, cat->Color);
	}

	void setName(char* string) {
		strcpy(Name, string);
	}
	void setColor(char* string) {
		strcpy(Color, string);
	}
	void print() override {
		printf("Cat name= %s, the color=%s\n", Name, Color);
	}

	~Cat() {
		printf("~Cat() %p\n", this);
	}
};

class MyBaseFactory {
public:
	MyBaseFactory() {}
	Base* createBase(char code, Base* p)
	{
		Base* _base = nullptr;
		switch (code)
		{
			case 'P':
				_base = new Point((Point*)(p));
				break;

			case 'S':
				_base = new Segment((Segment*)(p));
				break;

			case 'C':
				_base = new Cat((Cat*)(p));
				break;

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
			base = factory.createBase(_base->getCode(), _base);
		}
		~Node() {
			printf("\n~Node(): %p\n", this);
			printf("{\n");
			delete base;
			printf("\n}\n");
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
	void deleteObj(Base* __base) {
		if (isEmpty()) return;
		if (last->base == __base) {
			delete_last();
			return;
		}
		if (first->base == __base) {
			delete_first();
			return;
		}

		Node* current = first;
		while (current->next != nullptr && current->next->base != __base) {
			current = current->next;
		}
		if (current->next == nullptr) {
			printf("Такого элемента нет в списке\n");
			return;
		}
		Node* tmp_next = current->next;
		current->next = current->next->next;
		delete tmp_next;
	}
	void print() {
		Node* current = first;
		while (!(current->isEOL())) {
			current->base->print();
			current = current->next;
		}
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
		int j = 1;
		Node* current = first;
		while (j < i && !(current->isEOL())) {
			current = current->next;
			j++;
		}
		return(current->base);
	}
	Base* getObjAndDelete(int i) {
		Base* ret = getObj(i);
		Base* tmp;
		MyBaseFactory factory;
		tmp = factory.createBase(ret->getCode(), ret);
		deleteObj(ret);
		return tmp;
	}

	~List() {
		printf("~List()\n");
		if (!(isEmpty())) {
			printf("[______________________________________\n");
			Node* tmp = last;
			while (tmp != first) {
				printf("\t");
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

	List list;
	for (int i = 0; i < 5; i++) {
		list.add(new Point(i, i));
	}
	list.print();
	for (int i = 0; i < 5; i++) {
		Point* p1 = new Point(i + 200, i + 200);
		Point* p2 = new Point(i + 300, i + 300);

		list.add(new Segment(p1, p2));
		delete p1;
		delete p2;
	}
	printf("Кол-во элементов до удаления: %d\n", list.getSize());

	list.getObjAndDelete(5);
	list.print();

	printf("Кол-во элементов после удаления: %d\n", list.getSize());
	system("pause");
}
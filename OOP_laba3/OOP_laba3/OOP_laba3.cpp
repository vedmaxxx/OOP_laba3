#include <iostream>

class Base {
public:
    virtual void print() {}
    ~Base() {
        printf("~Base()\n");
    }
};

class Point : public Base {
private:
    int x = 0;
    int y = 0;
public:
    void print() override {
        printf("(%d %d)\n", x, y);
    }
    void setXY(int x, int y) {
        //printf("SetXY()\n");
        this->x = x;
        this->y = y;
    }
    ~Point() {
        printf("~Point\n");
    }
};
class Segment : public Base {
private:
    Point* p1;
    Point* p2;
public:
    void print() override {
        printf("segment {\n");
        printf("P1");
        p1->print();
        printf("P2");
        p2->print();
        printf("}\n");
    }
    Segment() {
        p1 = new Point;
        p2 = new Point;
    }
    Segment(Point* p1, Point* p2) {
        this->p1 = new Point(*p1);
        this->p2 = new Point(*p2);
    }
    Segment(const Segment& segment) {
        this->p1 = new Point(*segment.p1);
        this->p2 = new Point(*segment.p2);
    }
    ~Segment() {
        printf("~Segment\n");
    }
};

class List {
private:
    void remove_first() {
        if (isEmpty()) return;
        Node* temp = first;
        first = temp->next;
        delete temp;
    }
    void remove_last() {
        if (isEmpty()) return;
        if (last == first) {
            remove_first();
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
    class Node {
    public:
        Base* obj;
        Node* next;

        bool isEOL() { return (this == nullptr ? 1 : 0); }
        Node(Base* _obj) : obj(_obj), next(nullptr) {}
        ~Node() {
            printf("~Node(): %p\n", this);
            delete obj;
        }
    };
public:


    Node* first;
    Node* last;

    List() : first(nullptr), last(nullptr) {}

    bool isEmpty() {
        return first == nullptr;
    }
    void push_back(Base* _obj) {
        //создаем УЗЕЛ со значением _obj
        Node* another = new Node(_obj);
        //один узел в списке
        if (isEmpty()) {
            first = another;
            last = another;
            return;
        }
        last->next = another;
        last = another;
    }
    void remove_node(Base* _obj) {
        if (isEmpty()) return;
        if (last->obj == _obj) {
            remove_last();
            return;
        }
        if (first->obj == _obj) {
            remove_first();
            return;
        }

        Node* current = first;
        while (current->next != nullptr && current->next->obj != _obj) {
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
    Base* getNode(int i) {
        
        int j = 0;
        Node* current = first;
        while (j < i && !(current->isEOL())) {
            current = current->next;
            j++;
        }
        return((current->obj));
    }

    Base* getdelNode(int i) {
        Base* obj = getNode(i);
        remove_node(obj);
        return obj;
    }
    void print_list() {
        Node* current = first;
        while (!(current->isEOL())) {
            current->obj->print();
            current = current->next;
        }
    }

    ~List() {
        if (!(isEmpty())) {
            Node* tmp = last;
            while (tmp != first) {
                remove_last();
                tmp = last;
            }
            remove_first();
        }

    }
};



int main()
{
    setlocale(LC_ALL, "Rus");

    List list1;

    for (int i = 0; i < 10; i++) {
        Point* p = new Point;

        p->setXY(i, i);
        list1.push_back(p);
        list1.getNode(i)->print();
    }

    for (int i = 10; i < 20; i++) {
        
        
        Point* p1 = (Point *)list1.getNode(i - 10);
        Point* p2 = (Point*)list1.getNode(i - 9);
        Segment* s = new Segment(p1, p2);

        list1.push_back(s);
        list1.getNode(i)->print();
    }

    

}
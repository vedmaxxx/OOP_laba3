#include <iostream>

class Storage {
public:
    int count;

    Storage(int _count = 0) : count(_count) {
        printf("Storage(int _count)\n");
    }

    int getCount() {
        return count;
    }
};

class Deriv : public Storage {

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
        temp->next = nullptr;
        delete last;
        last = temp;
    }

public:
    class Node {
    public:
        Storage* obj;
        //int value;      //значение задает пользователь
        Node* next;     //указатель на следующую ячейку списка

        //изменение
        Node() {

        }
        Node(Storage* _obj) : obj(_obj), next(nullptr) {}
        ~Node() {
            printf("~Node(): %p\n", this);
            delete obj;
            delete next;
        }
    };

    Node* first;
    Node* last;

    List() : first(nullptr), last(nullptr) {}

    bool isEmpty() {
        return first == nullptr;
    }

    void push_back(Storage* _obj) {
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

    void remove_node(Storage* _obj) {
        if (isEmpty()) return;
        if (first->obj == _obj) {
            remove_first();
            return;
        }
        if (last->obj == _obj) {
            remove_last();
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
};



int main()
{
    Storage storage;
}
#include <iostream>
#include <unordered_set>
#include <cassert>

using namespace std;


struct Node
{
    int data;
    Node *next;
    Node *prev;

    Node(int data){
        this->data = data;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

class DLinkedList
{
    int length;
    Node *head;
    Node *tail;

public:

    DLinkedList()
    {
        length = 0;
        head = nullptr;
        tail = nullptr;
    }

    ~DLinkedList()
    {
        // clear the memory
        Node *deleted = head;
        while (head) {
            deleted = head->next;
            delete head;
            head = deleted;
        }
    }

    void link(Node* first, Node* second)
    {
        if (first) {
            first->next = second;
        }
        if (second) {
            second->prev = first;
        }
    }

    void insert_end(int data)
    {
        Node* node = new Node(data);
        if (!head) {
            head = tail = node;
        }
        else {
            link(tail, node);
            tail = node;
        }
        length++;
    }

    void insert_front(int data)
    {
        Node* node = new Node(data);
        if (!head) {
            head = tail = node;
        }
        else {
            link(node, head);
            head = node;
        }
        length++;
    }

    void insert_sorted(int value)
    {
        if (!length) {
            insert_end(value);
        }
        else if (value >= tail->data) {
            insert_end(value);
        }
        else if (value <= head->data) {
            insert_front(value);
        }
        else {
            Node *node = new Node(value);
            Node *prev = head;
            Node *greater = head->next;

            while (greater->data < value) {
                prev = prev->next;
                greater = prev->next;
            }
            link(prev, node);
            link(node, greater);

            length++;
        }
    }

    void delete_front()
    {
        if (!head) {
            return;
        }
        Node *node = head->next;
        delete head;
        head = node;
        length--;

        if (head) {
            head->prev = nullptr;
        }
        else {
            tail = head = nullptr;
        }
    }

    void delete_end()
    {
        if (!tail) {
            return;
        }
        Node *node = tail->prev;
        delete tail;
        tail = node;
        length--;

        if (tail) {
            tail->next = nullptr;
        }
        else {
            tail = head = nullptr;
        }
    }

    Node * delete_linked(Node* node)
    {
        Node* prev = node->prev;
        link(node->prev, node->next);
        delete node;
        return prev;
    }

    void delete_node(int key)
    {
        if (!head) {
            return;
        }
        if (head->data == key) {
            delete_front();
            return;
        }
        else if (tail->data == key) {
            delete_end();
            return;
        }
        Node* node = head->next;
        bool found = false;
        while (node) {
            if (node->data == key) {
                found = true;
                break;
            }
            node = node->next;
        }
        if (found) {
            link(node->prev, node->next);
            delete node;
            length--;
        }
        else {
            cout << "not found!!\n";
        }
    }

    void delete_nodes(int key)
    {
        if (!head) {
            return;
        }
        Node* node = head;
        while (node) {
            if (node->data == key) {
                if (node == head) {
                    node = node->next;
                    delete_front();
                }
                else if (node == tail) {
                    node = node->next;
                    delete_end();
                }
                else {
                    node = delete_linked(node);
                    node = node->next;
                    length--;
                }
            }
            else {
                node = node->next;
            }
        }
    }

    void delete_even()
    {
        if (!head || length == 2) {
            delete_end();
            return;
        }
        Node* even = head->next, * odd = even;
        while (even) {
            odd = even->next;
            link(even->prev, even->next);
            delete even;
            even = odd->next;
            length--;
            if (even == tail) {
                delete_end();
                return;
            }
        }
    }

    void delete_odd()
    {
        if (length <= 2) {
            delete_front();
            return;
        }
        Node* even = head->next, * odd = even->next;
        delete_front();
        while (odd) {
            if (odd == tail) {
                delete_end();
                return;
            }
            link(odd->prev, odd->next);
            delete odd;
            length--;
            even = even->next;
            odd = even->next;
        }
    }

    void print()
    {
        if (!head) {
            cout << "Linked List is empty!!!\n";
            return;
        }
        cout<<"\n{ ";
        for (Node* node = head; node; node = node->next) {
            cout << node->data;
            if(node->next)
                cout<<", ";
        }
        cout<<" }\n";
    }

    void print_reverse()
    {
        if (!head) {
            cout << "Linked List is empty!!!\n";
            return;
        }
        for (Node* node = tail; node; node = node->prev) {
            cout << node->data << " ";
        }
        cout << endl;
    }

    bool is_palindrome()
    {
        if (length <= 1) {
            return true;
        }
        Node* prev = tail, * next = head;
        while (prev != next) {
            if (next->data != prev->data) {
                return false;
            }
            next = next->next;
            prev = prev->prev;
        }
        return true;
    }

    void debug_verify_data_integrity() {
        if (length == 0) {
            assert(head == nullptr);
            assert(tail == nullptr);
        }
        else {
            assert(head != nullptr);
            assert(tail != nullptr);
            if (length == 1)
                assert(head == tail);
            else
                assert(head != tail);
            assert(!tail->next);
            assert(!head->prev);
        }
        int len = 0;
        for (Node* cur = head; cur; cur = cur->next, len++)
            assert(len < 10000);	// Consider infinite cycle?
        assert(length == len);
        len = 0;
        for (Node* cur = tail; cur; cur = cur->prev, len++)
            assert(len < 10000);	// Consider infinite cycle?
        assert(length == len);
        //assert(length == (int)debug_data.size());
    }

    int find_the_middle(){
        if(length == 0)
            return -1;

        Node *slow = head, *fast = head;
        while(fast && fast->next){
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow->data;
    }

    void swap_Kth_forward_with_backward(int k){
        Node *forward = head, *backward = tail;
        for(int i = 1; i < k; ++i){
            forward = forward->next;
            backward = backward->prev;
        }
        if(forward == backward)
            return;

        Node *forward_prev = forward->prev, *forward_next = forward->next;
        Node *backward_prev = backward->prev, *backward_next = backward->next;

        if(forward->next == backward){
            link(forward_prev, backward);
            link(forward, backward);
            link(forward, backward_next);
        }
        else{
            link(forward_prev, backward);
            link(forward, backward_next);

            link(backward, forward_next);
            link(backward_prev, forward);
        }
        if(k == 1)
            swap(head, tail);
    }

    void reverse(){
        if (length <= 1) {
            return;
        }

        tail = head;
        Node *prev = head;
        Node *next = head->next;
        head = head->next;
        // prev -> head -> next
        // prev <- head <- next
        while (head) {
            next = head->next; // to keep the next node
            head->next = prev; // link the node with the previous node of it

            prev = head;
            head = next;
        }
        head = prev; // back the head from the null to the new first node
        tail->next = nullptr;
    }

    void merge_2sorted_lists(DLinkedList &other){
        if(!other.head)
            return;
        if(!this->head){
            length = other.length;
            this->head = other.head;
            this->tail = other.tail;
            return;
        }
        this->length = this->length + other.length;
        Node *ocur = other.head, *cur = this->head;
        Node *prev = nullptr, *next = nullptr;

        head = nullptr;
        while(ocur && cur){
            if(cur->data <= ocur->data){
                next = cur;
                cur = cur->next;
            }
            else{
                next = ocur;
                ocur = ocur->next;
            }
            link(prev, next);
            prev = next;
            if(!head)
                head = next;
        }
        if(ocur){
            link(prev, ocur);
            tail = other.tail;
        }
        else if(cur)
            link(prev, cur);
    }
};



int main(){
    DLinkedList list1;
    list1.insert_end(10);
    list1.insert_end(20);
    list1.insert_end(30);
    list1.insert_end(40);
    list1.insert_end(50);

    DLinkedList list2;
    list2.insert_end(15);
    list2.insert_end(17);
    list2.insert_end(22);
    list2.insert_end(24);
    list2.insert_end(35);

    list1.merge_2sorted_lists(list2);
    list1.print();



    list1.debug_verify_data_integrity();

    cout<<"\n\nNO TLE..\n\n";

}

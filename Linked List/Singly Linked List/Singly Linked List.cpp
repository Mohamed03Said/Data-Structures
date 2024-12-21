#include <iostream>
#include <unordered_set>
#include <cassert>

using namespace std;


struct Node
{
    int data;
    Node *next;

    Node(int data){
        this->data = data;
        this->next = nullptr;
    }

    Node(int data, Node *next){
        this->data = data;
        this->next = next;
    }
};

class LinkedList {
private:
    Node *head{};
    Node *tail{};
    long long length;

public:
    LinkedList() {
        this->head = nullptr;
        this->tail = nullptr;
        length = 0;
    }

    ~LinkedList() {
        // clear the memory
        Node *deleted = head;
        while (head) {
            deleted = head->next;
            delete head;
            head = deleted;
        }
    }

    void Print() {
        if (!head) {
            cout << "Empty linked list....\n";
            return;
        }
        cout<<"{ ";
        for (Node *current = head; current != nullptr; current = current->next) {
            cout <<current->data;
            if(current->next)
                cout<<", ";
        }
        cout<<" }\n";
    }

    void InsertEnd(int value) {
        Node *newNode = new Node(value);

        if (!head) { // empty linked list
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        length++;
    }

    void InsertFront(int value) {
        Node *newNode = new Node(value);

        if (!head) { // empty linked list
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }

        length++;
    }

    void DeleteFront(){
        if(head == nullptr){
            return;
        }

        Node* newHead = head->next;

        delete head;
        head = newHead;

        if (!head)
            tail = nullptr;

        length--;
    }

    void InsertSorted(int value){
        if (!length || value >= tail->data) {
            InsertEnd(value);
        }
        else if (value <= head->data) {
            InsertFront(value);
        }
        else {
            Node *newNode = new Node(value);
            Node *prev = head;
            Node *next = head->next;

            while (next->data < value) {
                prev = prev->next;
                next = prev->next;
            }
            newNode->next = next;
            prev->next = newNode;

            length++;
        }
    }

    Node * GetNth(long long n){
        if (n > length || n <= 0)
            return nullptr;

        int i = 0;
        for (Node *node = head; node; node = node->next) {
            if (++i == n) {
                return node;
            }
        }
        return nullptr;
    }

    Node * GetNthBack(long long n){
        return GetNth(length - (n - 1));
    }

    void DeleteLast(){
        if (length <= 1) {
            DeleteFront();
            return;
        }

        Node *node = GetNth(length - 1);

        delete tail;
        tail = node;
        tail->next = nullptr;

        length--;
    }

    void DeleteNth(long long n){
        if (n > length || n <= 0) {
            cout << "InValid Index!!\n";
            return;
        }
        if (n == 1) {
            DeleteFront();
            return;
        }

        Node *prev = GetNth(n - 1);
        Node *node = prev->next;

        bool is_tail = (node == tail);
        prev->next = node->next;

        if (is_tail) {
            tail = prev;
        }
        delete node;

        length--;
    }

    void DeleteNode(int value){
        if (!length) {
            cout << "Empty linked list..\n";
            return;
        }

        if (head->data == value) {
            DeleteFront();
            return;
        }
        else if (tail->data == value) {
            DeleteLast();
            return;
        }

        Node *prev = head;
        Node *node = prev->next;

        bool found = false;
        while (node) {
            if (node->data == value) {
                found = true;
                break;
            }
            prev = node;
            node = node->next;
        }
        if (found) {
            prev->next = node->next;
            delete node;

            length--;
        }
        else {
            cout << "not found!!\n";
        }
    }

    void DeleteEven(){ // delete even Indices
        if (length <= 1) {
            return;
        }
        Node *odd = head;
        Node *even = head->next;

        while (even) {
            length--;

            if (even == tail) {
                DeleteLast();
                break;
            }

            odd->next = even->next;
            delete even;
            odd = odd->next;

            even = odd->next;
        }
    }

    bool IsSame(const LinkedList list){
        if(list.length != length)
            return false;

        Node *listNode = list.head;
        Node *thisNode = head;
        while (thisNode && listNode)
        {
            if (thisNode->data != listNode->data)
                return false;

            thisNode = thisNode->next;
            listNode = listNode->next;
        }
        return !thisNode && !listNode;
    }

    void SwapPairs(){
        if (length <= 1) {
            return;
        }

        Node *first = head;
        Node *second = first->next;

        while (true) {
            swap(first->data, second->data);

            first = second->next;
            if (!first) {
                return;
            }

            second = first->next;
            if (!second) {
                return;
            }
        }
    }

    void Reverse(){
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

    void SwapTailWithHead(){
        if (length <= 1) {
            return;
        }
        else if (length == 2) {
            tail->next = head;
            head->next = nullptr;
            swap(head, tail);
            return;
        }

        Node *prev = head;
        Node *next = head->next;
        while (prev->next->next) {
            prev = prev->next;
        }
        head->next = nullptr;
        prev->next = head;
        tail->next = next;
        swap(head, tail);
    }

    void LeftShift(int k)
    {
        if (length <= 1) {
            return;
        }
        k %= length;
        while (k--) {
            Node *new_head = head->next;
            tail->next = head;
            head->next = nullptr;
            head = new_head;
            tail = tail->next;
        }
    }

    void RemoveDuplicates()
    {
        if (length <= 1) {
            return;
        }

        unordered_set<int> unSet;
        unSet.insert(head->data);
        Node *prev = head, *cur = head->next;
        while (cur) {
            if (!unSet.count(cur->data)) {
                unSet.insert(cur->data);

                prev = cur;
                cur = cur->next;
            }
            else {
                prev->next = cur->next;
                delete cur;
                cur = prev->next;
                length--;
                if (!cur) {
                    tail = prev;
                }
            }
        }
    }

    void RemoveLastAppearance(int key)
    {
        if (!length) {
            return;
        }
        Node *prev = head, *next = head->next, *cur = head;
        while (cur->next) {
            if (cur->next->data == key) {
                prev = cur;
                next = cur->next->next;
            }
            cur = cur->next;
        }
        if (prev == head && head->data == key) {
            delete head;
            head = next;
            length--;

            if (!head) {
                tail = head = nullptr;
            }
        }
        else if (!next) {
            delete tail;
            tail = prev;
            tail->next = nullptr;
            length--;
        }
        else {
            delete prev->next;
            prev->next = next;
            length--;
        }
    }

    void MoveBack(int key)
    {
        if (length <= 1) {
            return;
        }
        long long len = length;
        Node *prev = head, *cur = head, *next = head->next;
        while (len--) {
            if (cur->data == key) {
                if (cur == head) {
                    tail->next = head;
                    head->next = nullptr;
                    tail = head;
                    head = next;

                    prev = head;
                    cur = head;
                    next = head->next;
                }
                else {
                    prev->next = next;
                    tail->next = cur;
                    tail = cur;
                    tail->next = nullptr;
                    cur = next;
                    next = next->next;
                }
            }
            else {
                prev = cur;
                cur = next;
                next = next->next;
            }
        }
    }

    Node * GetHead()
    {
        return head;
    }

    Node * GetTail()
    {
        return tail;
    }

    long long GetLength()
    {
        return length;
    }

    int IndexOf(int value)
    {
        int i = 0;
        for (Node * node = head; node; node = node->next, i++) {
            if (node->data == value) {
                return i;
            }
        }
        return -1;
    }

    int SearchImproved(int value)
    {
        int i = 0;
        for (Node *node = head, *prev = nullptr; node; prev = node, node = node->next) {
            if (node->data == value) {
                if (!prev) {
                    return i;
                }
                swap(node->data, prev->data);
                return i - 1;
            }
            i++;
        }
        return -1;
    }

    void RemoveAllRepeated() { // sorted linked list
        if (length <= 1) {
            return;
        }

        Node *prev = head, *cur = head, *next = head->next;
        while(next){
            int value = cur->data;
            bool is_duplicated = false;
            while(cur && next && next->data == value){
                cout<<"delete cur->next\n";
                is_duplicated = true;
                next = next->next;
                delete cur->next;
                cur->next = nullptr;
                cur->next = next;
                length--;
            }
            if(is_duplicated){
                if(next == nullptr){
                    cout<<"deleted tail\n";
                    prev->next = nullptr;
                    tail = prev;
                    if(head == cur){
                        cout<<"deleted head & tail\n";
                        head = tail = cur = prev = nullptr;
                    }
                    delete cur;
                    cur = nullptr;
                    length--;
                    break;
                }
                if(cur == head){
                    cout<<"deleted head\n";
                    head = next;
                    prev = next;
                }
                else
                    prev->next = next;
                cout<<"deleted cur\n";
                delete cur;
                cur = nullptr;
                cur = next;
                next = next->next;
                length--;
                continue;
            }
            cout<<"move..\n";
            prev = cur;
            cur = next;
            next = next->next;
        }
    }

    Node * AddNodeAfter(Node *cur, int data){
        Node *newNode = new Node(data);
        newNode->next = cur->next;
        cur->next = newNode;
        return newNode;
    }

    void ReverseChains(int k){ // { 1, 2, 3, 4 }, k = 2 => { 2, 1, 4, 3 }

    }

    void AddingHUGENumbers(LinkedList &another){
        if(another.length == 0)
            return;
        if(this->length == 0){
            head = another.head;
            tail = another.tail;
            return;
        }

        Node *cur = this->head, *cur_another = another.head;
        bool carry = false;
        for(; cur && cur_another; cur=cur->next, cur_another=cur_another->next){
            int value = cur->data + cur_another->data;
            value += carry;
            if(value > 9){
                carry = true;
                value %= 10;
            }
            else
                carry = false;
            cur->data = value;
        }
        if(this->length < another.length) {
            this->tail->next = cur_another;
            this->tail = another.tail;
        }
        cur = cur_another;
        for(;cur; cur = cur->next){
            int value = cur->data + carry;
            if(value > 9){
                carry = true;
                value %= 10;
            }
            else
                carry = false;
            cur->data = value;
        }
        this->length = max(this->length, another.length);
        if(carry) {
            this->InsertEnd(carry);
        }
    }

    int GetMax(Node *cur = nullptr, bool firstCall = true){
        if(firstCall){
            return this->GetMax(head, false);
        }
        if(cur == nullptr)
            return 0;

        return max(cur->data, GetMax(cur->next, false));
    }

    void ArrangeOddAndEvenNodes(){ // 1, 2, 3, 4, 5, 6 => 1, 3, 5, 2, 4, 6
        if(this->length <= 2){
            return;
        }
        Node *oddHead = head->next;
        Node *even = head, *odd = head->next;

        while(odd && odd->next){
            cout<<"odd: "<<odd->data << " -1- even: " << even->data<<",  ";
            even->next = odd->next;
            even = even->next;

            odd->next = even->next;
            if(odd->next)
                odd = odd->next;
            cout<<"odd: "<<odd->data << " -1- even: " << even->data<<"\n";
        }
        even->next = oddHead;
        odd->next = nullptr;
        tail = odd;
        cout<<"head: "<<head->data<<" - tail: "<<tail->data;
    }

    void InsertAlternating(LinkedList &another){
        if(another.length == 0)
            return;
        if(this->length == 0){
            head = another.head;
            tail = another.tail;
            return;
        }

        this->length = this->length + another.length;

        Node *cur = head, *curAnother = another.head;
        while(cur && curAnother){
            Node *next = cur->next;
            cur->next = curAnother;
            if(!next){
                break;
            }
            cur = next;

            Node *nextAnother = curAnother->next;
            curAnother->next = cur;
            curAnother = nextAnother;
        }
        tail = (this->length > another.length ? this->tail : another.tail);
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
        }
        int len = 0;
        for (Node *cur = head; cur; cur = cur->next, len++)
            assert(len < 10000);	// Consider infinite cycle?
        assert(length == len);
        //assert(length == (int)debug_data.size());
    }


};

int main(){
    LinkedList ls, lls;
    ls.InsertEnd(0);
    ls.InsertEnd(1);
    ls.InsertEnd(2);
    ls.Print();
    ls.debug_verify_data_integrity();
    cout<<"\n\nNO TLE......\n";
}


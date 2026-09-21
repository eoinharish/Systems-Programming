#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    int data;
    Node* next;
    Node* prev;

    public:
        Node(int val)
        {
            data = val;
            next = nullptr;
            prev = nullptr;
        }
        // IMP
        Node(int val, Node* next1, Node* prev1)
        {
            data = val;
            next = next1;
            prev = prev1;
        }
};

Node* convertArr2DLL(vector<int>& arr)
{
    Node* head = new Node(arr[0]);
    Node* prev = head;

    for(int i=1; i<arr.size(); i++)
    {
        Node* temp = new Node(arr[i], nullptr, prev);
        prev->next = temp;
        prev = prev->next;
    }
    return head;
}

/************************************************************/
// Deletions (deleteHead, deleteTail, deleteKth, deleteNode)

Node* deleteHead(Node* head)
{
    // -------- Edge cases (LL having no node, single node)------
    if (head == nullptr) return nullptr; // empty list

    if (head->next == nullptr){ // LL containing only single node
        delete head;
        return nullptr;
    }
    // ----------------------------------------------------------

    // 1 <-> 2
    Node* prev = head;
    head = head->next;

    head->prev = nullptr;
    prev->next = nullptr;
    delete prev;

    return head;
}

Node* deleteTail(Node* head)
{
    // -------- Edge cases (LL having no node, single node)------
    if (head == nullptr) return nullptr; // empty list

    if (head->next == nullptr){ // LL containing only single node
        delete head;
        return nullptr;
    }
    // ----------------------------------------------------------

    // Go to tail of the LL
    Node* tail = head;
    while(tail->next != nullptr)
    {
        tail = tail->next;
    }

    Node* prev = tail->prev;
    prev->next = nullptr;
    tail->prev = nullptr;
    delete tail;
    return head;
}

// Delete Kth element of the DLL
// 1->2->3->4
Node* deleteKth(Node* head, int k) // k>=1 && k<=length
{
    // Go to kth node
    Node* curr = head;
    int cnt = 0;
    while(curr->next != nullptr)
    {
        curr = curr->next;
        cnt++;
        if (cnt == k) break;
    }
    
    // prevNode <-> curr (Kth) <-> nextNode
    Node* prevNode = curr->prev; //prevNode of curr
    Node* nextNode = curr->next; // nextNode of curr

    // LL containing only 1 node
    if (prevNode == nullptr && nextNode == nullptr)
    {
        delete curr;
        head = nullptr;
    }
    else if (prevNode == nullptr) // standing at head
    {
        head = deleteHead(head);
    }
    else if (nextNode == nullptr) // standing at tail
    {
        head = deleteTail(head);
    }
    else
    {
        // has both prevNode and nextNode
        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        curr->prev = nullptr;
        curr->next = nullptr;
        delete curr;
    }
    return head;
}

void deleteNode(Node* node)
{
    Node* prevNode = node->prev;
    Node* nextNode = node->next;

    if (prevNode == nullptr && nextNode == nullptr)
    {
        delete node;
        return;
    }
    // given: node can't be head, because then we need to return new head
    // else if (prevNode == nullptr) // node is the head
    // {
        
    // }
    else if (nextNode == nullptr) // node is the tail
    {
        prevNode->next = nullptr;
        node->prev = nullptr;

        delete node;
        return;
    }
    else // has both prevNode and nextNode
    {
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
        node->next = nullptr;
        node->prev = nullptr;
        delete node;
        return;
    }

}
/************************************************************/

/************************************************************/
// Insertions (beforeHead, beforeTail, beforeKth, beforeNode)

Node* insertBeforeHead(Node* head, int val)
{
    Node* newHead = new Node(val, head, nullptr);
    head->prev = newHead;

    return newHead;
}

Node* insertBeforeTail(Node* head, int val)
{
    // Go to tail
    Node* tail = head;
    while(tail->next != nullptr)
    {
        tail = tail->next;
    }

    // If tail is pointing to the head (i.e only 1 node in the LL)
    if (tail->prev == nullptr)
    {
        return insertBeforeHead(head, val);
    }

    // 1<->2<->3<->4
    Node* prevTail = tail->prev;

    Node* newNode = new Node(val, tail, prevTail);
    prevTail->next = newNode;
    tail->prev = newNode;

    return head;
}
Node* insertBeforeKth(Node* head, int k, int val)
{
    if (k == 1){
        return insertBeforeHead(head, val);
    }

    Node* temp = head;
    int cnt = 1;
    while(temp->next != nullptr)
    {
        temp = temp->next;
        cnt++;
        if(cnt == k) break;
    }

    if (temp->next == nullptr){
        return insertBeforeTail(head, val);
    }

    Node* prevNode = temp->prev;
    Node* newNode = new Node(val, temp, prevNode);
    prevNode->next = newNode;
    temp->prev = newNode;
    return head;
}

// Given: node != head (no head is given)
void insertBeforeNode(Node* node,int val)
{
    if (node == nulptr){
        return;
    }

    Node* prevNode = node->prev;
    Node* newNode = new Node(val, node, prevNode);
    prevNode->next = newNode;
    node->prev = newNode;
}



/************************************************************/
void print(Node* head)
{
    while(head)
    {
        cout << head->data << " ";
        head = head->next;
    }
    cout << '\n';
}

int main()
{
    vector<int> arr = {12,5,8,7};  // NULL <- 12 <-> 5 <-> 8 <-> 7 -> NULL
    Node* head = convertArr2DLL(arr);

    //head = deleteHead(head); // delete head, LL: 5 <-> 8 <-> 7
    //head = deleteTail(head); // delete tail, LL: 5 <-> 8
    //head = deleteKth(head, 4);
    //deleteNode(head);

    //head = insertBeforeHead(head, 10);
    //head = insertBeforeTail(head, 10);
    print(head);

    return 0;
}
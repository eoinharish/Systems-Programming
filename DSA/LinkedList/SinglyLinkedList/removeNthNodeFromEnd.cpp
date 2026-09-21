#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    int data;
    Node* next;

    public:
        Node(int val)
        {
            data = val;
            next = nullptr;
        }
        // IMP
        Node(int val, Node* next1)
        {
            data = val;
            next = next1;
        }
};

// Ref:Neetcode
Node* removeNthFromEnd(Node* head, int n)
{
    Node* dummy = new Node(-1, head);
    Node* left = dummy;
    Node* right = head;

    // Move right pointer to n steps
    int cnt = 0;
    while(right->next != nullptr)
    {
        cnt++;
        if(cnt == n) break;
        right = right->next;
    }

    // keep moving left and right 1 step ahead
    while(right->next != nullptr)
    {
        right = right->next;
        left = left->next;
    }

    Node* target = left->next; // node to be deleted
    // IMP case when node to be deleted is the head as we need to return the new head
    if(target == head)
    {
        head = head->next;

        target->next = nullptr;
        dummy->next = nullptr;
        delete target;
        delete dummy;

        return head;
    }

    left->next = target->next;
    target->next = nullptr;
    delete target;

    return head;
}

Node* convertArr2SinglyDLL(vector<int>& arr)
{
    Node* head = new Node(arr[0]);
    Node* prev = head;

    for(int i=1; i<arr.size(); i++)
    {
        Node* temp = new Node(arr[i]);
        prev->next = temp;
        prev = temp;
    }
    return head;
}

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
    vector<int> arr = {12,5,8,7};
    Node* head = convertArr2SinglyDLL(arr);

    head = removeNthFromEnd(head, 4);

    print(head);

    return 0;
}
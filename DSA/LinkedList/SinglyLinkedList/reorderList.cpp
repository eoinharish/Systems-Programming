#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    int data;
    Node *next;
    Node() : data(0), next(nullptr) {}
    Node(int x) : data(x), next(nullptr) {}
    Node(int x, Node *next) : data(x), next(next) {}
};

class Solution {
public:

    Node* reverse(Node* head)
    {
        Node* prev = nullptr;
        Node* curr = head;

        while(curr != nullptr)
        {
            Node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }

        return prev; // newHead of the reversed list
    }

    void reorderList(Node* head)
    {
        // Find the middle
        Node* slow = head;
        Node* fast = head->next;

        while(fast != nullptr && fast->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        // Reverse the right half
        Node* rightHead = slow->next;

        slow->next = nullptr;

        rightHead = reverse(rightHead); // head of the right half

        // Merge two lists
        Node* left = head;
        Node* right = rightHead;

        while(left != nullptr && right != nullptr)
        {
            Node* leftNext = left->next;
            Node* rightNext = right->next;

            left->next = right;
            right->next = leftNext;

            left = leftNext;
            right = rightNext;
        }

        if(left != nullptr)
        {
            left->next = nullptr;
        }
        
    }
};

void printList(Node* head)
{
    while(head != nullptr)
    {
        cout << head->data;

        if(head->next != nullptr)
            cout << " -> ";

        head = head->next;
    }

    cout << endl;
}

Node* convertArr2SinglyDLL(const vector<int>& arr)
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

int main()
{
    // Test Case 1: Odd number of nodes
    Node* head1 = convertArr2SinglyDLL({1,2,3,4,5});

    Solution sol;

    cout << "Original List 1: ";
    printList(head1);

    sol.reorderList(head1);

    cout << "Reordered List 1: ";
    printList(head1);

    cout << endl;

    // Test Case 2: Even number of nodes
    Node* head2 = convertArr2SinglyDLL({1, 2, 3, 4, 5, 6});

    cout << "Original List 2: ";
    printList(head2);

    sol.reorderList(head2);

    cout << "Reordered List 2: ";
    printList(head2);

    cout << endl;

    // Test Case 3: Two nodes
    Node* head3 = convertArr2SinglyDLL({1, 2});

    cout << "Original List 3: ";
    printList(head3);

    sol.reorderList(head3);

    cout << "Reordered List 3: ";
    printList(head3);

    return 0;
}
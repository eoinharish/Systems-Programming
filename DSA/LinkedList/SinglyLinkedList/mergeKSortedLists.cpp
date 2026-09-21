
#include <iostream>
#include <queue>

using namespace std;

struct Node
{
    int val;
    Node *next;
    Node() : val(0), next(nullptr) {}
    Node(int x) : val(x), next(nullptr) {}
    Node(int x, Node *next) : val(x), next(next) {}
};

class Solution {
public:
    // 1->2->4
    // 1->3->5
    // 3->6

    // minHeap -> {1, Node*}
    typedef pair<int, Node*> P;

    Node* mergeKLists(vector<Node*>& lists)
    {
        Node* dummy = new Node(-1);
        priority_queue<P, vector<P>, greater<P>> pq; // minHeap

        for(int i=0; i<lists.size(); i++)
        {
            Node* head = lists[i];
            if(head != nullptr)
            {
                pq.push({head->val,head});
            }
        }

        Node* tail = dummy;
        while(!pq.empty())
        {
            int value = pq.top().first;
            Node* node = pq.top().second;
            pq.pop();

            tail->next = node;
            tail = node;

            if(node->next != nullptr){
                node = node->next;
                pq.push({node->val, node});
            }
            tail->next = nullptr;
        }

        return dummy->next;
    }
};


void printList(Node* head)
{
    while(head != nullptr)
    {
        cout << head->val;

        if(head->next != nullptr)
            cout << " -> ";

        head = head->next;
    }

    cout << endl;
}

int main()
{
    // Create three sorted linked lists:
    // 1 -> 2 -> 4
    // 1 -> 3 -> 5
    // 3 -> 6

    Node* head1 = new Node(1);
    head1->next = new Node(2);
    head1->next->next = new Node(4);

    Node* head2 = new Node(1);
    head2->next = new Node(3);
    head2->next->next = new Node(5);

    Node* head3 = new Node(3);
    head3->next = new Node(6);

    vector<Node*> lists = {head1, head2, head3};

    Solution sol;

    Node* mergedHead = sol.mergeKLists(lists);

    cout << "Merged Linked List: ";
    printList(mergedHead);

    return 0;
}
// File: LinkedList.h

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

template <typename E>
class LinkedList {
  private:
    class Node; // Forward delcaration of a nested class Node, that is directly tied to the LinkedList class. Declared here to use typecast below.
    using NodePtr = Node *;

    class Node {
      public:
        NodePtr prev;
        E element;
        NodePtr next;

        Node(NodePtr const prev, const E &element, NodePtr const next)
            : prev(prev), element(element), next(next)
        {}
    };

    /*-------------------------- Methods for node creation --------------------------*/
    // Create a node by providing pointers to the nodes before and after it in the list, as well as its data.
    // Exception handling to prevent stack overflow
    NodePtr createNode(NodePtr const prev, const E &element, NodePtr const next) {
        try {
            return new Node(prev, element, next); // This returns the address of the node so this method can be used:
        }
        catch (std::bad_alloc e) { // before->next = after->prev = createNode(before, element, after);
            std::cerr << "Memory error: " << e.what() << std::endl;
            exit(1);
        }
    }

    // Insert a node into a LinkedList between before and after pointers. No exception handling, createNode() does that.
    void insertNode(NodePtr const before, const E &element, NodePtr const after) {
        before->next = after->prev = createNode(before, element, after);
        numberOfNodes += 1;
    } // end insertNode()

    /*-------------------------- Methods for node deletion --------------------------*/
    // Link a nodes prev & next, removing it from the list.
    // Eliminate hanging pointers, then delete the node.
    void linkNextAndPrevOfNode(NodePtr &node) {
        NodePtr before = node->prev;
        NodePtr after = node->next;
        before->next = after;
        after->prev = before;
    }
    void releaseNode(NodePtr &node) {
        node->prev = node->next = nullptr; // Remember prev & next are pointers belonging to node.
        delete node;
        node = nullptr;
    }

    // Remove node and return value
    E removeNode(NodePtr &node) {
        E popped = node->element;
        linkNextAndPrevOfNode(node);
        releaseNode(node);
        numberOfNodes -= 1;
        return popped;
    }

    // Remove node, no return value
    void deleteNode(NodePtr &node) {
        linkNextAndPrevOfNode(node);
        releaseNode(node);
        numberOfNodes -= 1;
    }

    // Copy the nodes from the given list to this list. Overwrites, doesn't append
    void copyList(const LinkedList &copy) {
        numberOfNodes = copy.numberOfNodes;
        NodePtr iter = copy.head;
        head = tail = createNode(nullptr, iter->element, nullptr);
        iter = iter->next;
        // In the old copy, set the tail to tail->next while it is not null and create a node in the new list with its data
        while (iter != nullptr)
        {
            tail = tail->next = createNode(tail, iter->element, nullptr);
            iter = iter->next;
        }
    }

    // Repeatedly delete the head element in the list until all nodes in the list (including head and tail) are gone.
    void releaseList() {
        NodePtr del = head;
        while (del != nullptr) {
            head = head->next;
            releaseNode(del);
            del = head;
        }
        tail = nullptr;
        numberOfNodes = 0;
    }
    // List fields
    NodePtr head = nullptr;
    NodePtr tail = nullptr;
    int numberOfNodes = 0;

  public:
    /*-------------------------- Big 3 & constructor --------------------------*/

    LinkedList()
    {
        // E() ensures that no matter the type used, its default constructor is called
        head = tail = createNode(nullptr, E(), nullptr);  // head and tail both point to the same node at first, but then on the next line
        head->next = tail = new Node(head, E(), nullptr); // tail gets a new node with head as prev, then heads next pointer is assigned to tail.
        numberOfNodes = 0;
    }
    LinkedList(const LinkedList &copy) { copyNodes(copy); } // end LinkedList()
    ~LinkedList() { releaseList(); }                        // end ~LinkedList()

    const LinkedList &operator=(const LinkedList &rhs) {
        if (this != &rhs) {
            releaseList(); // Clear the receiving list
            copyList(rhs); // Copy the new list into the list on the left of the =
        }
        else {
            std::cerr << "List = List" << std::endl;
        }
        return *this; // Return the object, not the address
    }                 // end operator =()

    /*-------------------------- Position based pushers --------------------------*/
    // Insert a new node with the given element immediately after the head.
    void pushFront(const E &element) { insertNode(head, element, head->next); }
    // Insert a new node with the given element immediately before the tail.
    void pushBack(const E &element) { insertNode(tail->prev, element, tail); }

    // Insert a new node with the given element at the given index.
    void pushAt(const int index, const E &element) {
        if (index < 0 || index >= numberOfNodes) { throw std::logic_error("Error in pushAt(" + std::to_string(index) + "): Invalid index"); }
        if (index == 0)                          { pushFront(element); }
        else if (index == numberOfNodes - 1)     { pushBack(element); }
        else {
            NodePtr after = head->next;
            NodePtr before = nullptr;
            for (int i = 0; i < index; i++)
            {
                after = after->next;
            }
            before = after->prev;
            insertNode(before, element, after);
        }
    }

    /*-------------------------- Value based pusher --------------------------*/
    // Insert the new node into the list immediately after the first element in the list that is greater than it.
    void push(const E &element) {
        NodePtr after = head->next;
        NodePtr before = nullptr;
        while (after != tail && after->element < element) {
            after = after->next;
        }
        before = after->prev;
        insertNode(before, element, after);
    }
    /*-------------------------- Position based poppers --------------------------*/
    // Remove and return the first value in the list.
    E popFront()
    {
        if (isEmpty())
        {
            throw std::logic_error("Error in popFront(): The list is empty");
        }
        NodePtr del = head->next;
        return removeNode(del);
    }

    // Remove and return the last value in the list.
    E popBack() {
        if (isEmpty()) { throw std::logic_error("Error in popBack(): The list is empty"); }
        NodePtr del = tail->prev;
        return removeNode(del);
    }

    // Remove and return the value at the given index.
    E popAt(const int index)  {
        if (numberOfNodes == 0)                  { throw std::logic_error("Error in popAt(" + std::to_string(index) + "): The list is empty");  }
        if (index < 0 || index >= numberOfNodes) { throw std::logic_error("Error in popAt(" + std::to_string(index) + "): Invalid index");  }
        if (index == 0)                          { return popFront();  }
        else if (index == numberOfNodes - 1)     { return popBack(); }
        else {
            NodePtr del = head->next;
            for (int i = 0; i < index; i++) {
                del = del->next;
            }
            return removeNode(del);
        }
    }

    /*-------------------------- Value based popper --------------------------*/
    // Remove and return an element from the lsit given its actual data value.
    E pop(const E &element) {
        NodePtr del = head->next;
        while (del != tail && del->element != element) {
            del = del->next;
        }
        if (del == tail) {
            throw std::logic_error("Error in pop(" + std::to_string(element) + "): Element not found");
        }
        return removeNode(del);
    }

    /*-------------------------- Position based getters --------------------------*/
    // Return the first element in the list
    E getFront() {
        if (isEmpty()) {
            throw std::logic_error("Error in getFront(): List is empty");
        }
        return head->next->element;
    }

    // Return the last element in the list
    E getBack() {
        if (isEmpty()) {
            throw std::logic_error("Error in getBack(): List is empty");
        }
        return tail->prev->element;
    }

    // Return the element at the given index. Use getFront() or getBack() if appropriate.
    // throws logic_error if list is empty, or if the provided index is invalid.
    E getAt(const int index) {
        if (isEmpty())                           { throw std::logic_error("Error in getAt(" + std::to_string(index) + "): The list is empty"); }
        if (index < 0 || index >= numberOfNodes) { throw std::logic_error("Error in getAt(" + std::to_string(index) + "): Invalid index"); }
        if (index == 0)                          { return getFront(); }
        else if (index == numberOfNodes - 1)     { return getBack(); }
        else {
            NodePtr iter = head->next;
            for (int i = 0; i < index; i++) {
                iter = iter->next;
            }
            return iter->element;
        }
    }

    /*-------------------------- Position based setters --------------------------*/
    /* These methods require that a node already exists at the provided index. 
       If the list is empty, they refer to the push methods in exception message */
    // Set the data value of the first node in the list to the given value.
    void setFront(const E &element) {
        if (isEmpty()) {
            throw std::logic_error("Error in setFront(" + std::to_string(element) + "): The list is empty. Use pushFront(arg)");
        }
        head->next->element = element;
    }
    // Set the data value of the last node in the list to the given value.
    void setBack(const E &element) {
        if (isEmpty()) {
            throw std::logic_error("Error in setBack(" + std::to_string(element) + "): The list is empty. Use pushBack(arg)");
        }
        tail->prev->element = element;
    }

    // Set the data value of node at the given index to the given value.
    void setAt(const int index, const E &element) {
        if (isEmpty())                           { throw std::logic_error("Error in setAt(" + std::to_string(index) + ", " + std::to_string(element) + "): The list is empty. Use push(arg) or another push method"); }
        if (index < 0 || index >= numberOfNodes) { throw std::logic_error("Error in setAt(" + std::to_string(index) + ", " + std::to_string(element) + "): Invalid index "); }
        if (index == 0)                          { setFront(element); }
        else if (index == numberOfNodes - 1)     { setBack(element); }
        else {
            NodePtr iter = head->next;
            iter = head->next;
            for (int i = 0; i < index; i++) {
                iter = iter->next;
            }
            iter->element = element;
        }
    }

    /*-------------------------- Position based deleters --------------------------*/
    // Remove the first node from the list.
    void deleteFront() {
        if (isEmpty()) {
            throw std::logic_error("Error in deleteFront(): The list is empty");
        }
        NodePtr del = head->next;
        deleteNode(del);
    }

    // Remove the last node from the list.
    void deleteBack() {
        if (isEmpty()) {
            throw std::logic_error("Error in deleteBack(): The list is empty");
        }
        NodePtr del = tail->prev;
        deleteNode(del);
    }

    // Remove the node at the given index from the list.
    void deleteAt(const int index) {
        if (isEmpty())                           { throw std::logic_error("Error in setAt(" + std::to_string(index) + "): The list is empty"); }
        if (index < 0 || index >= numberOfNodes) { throw std::logic_error("Error in deleteAt(" + std::to_string(index) + "): Invalid index"); }
        if (index == 0)                          { deleteFront(); }
        else if (index == numberOfNodes - 1)     { deleteBack(); }
        else {
            NodePtr del = head->next;
            for (int i = 0; i < index; i++) {
                del = del->next;
            }
            deleteNode(del);
        }
    }

    /*-------------------------- Value based deleter --------------------------*/
    // Delete the first node in the list that contains the given element value.
    void deleteE(const E &element) {
        NodePtr del = head->next;
        while (del != tail && del->element != element) {
            del = del->next;
        }
        if (del == tail) {
            throw std::logic_error("Error in deleteE(" + std::to_string(element) + "): Element not found");
        }
        deleteNode(del);
    }

    /*-------------------------- List utlities --------------------------*/
    // Return true if the list is empty, else return false.
    bool isEmpty() const { return numberOfNodes == 0; }
    // Return the number of nodes in the list.
    int size() const     { return numberOfNodes;      }

    // A selection sort to sort the list
    void sort() {
        int indexSmallest = 0;
        int temp = 0 ; // Temporary variable for swap

        for (int i = 0; i < numberOfNodes - 1; ++i) {
        // Find index of smallest remaining element
            indexSmallest = i;
            for (int j = i + 1; j < numberOfNodes; ++j) {
                if ( getAt(j) < getAt(indexSmallest) ) {
                    indexSmallest = j;
                }
            }
        // Swap at(i) and at(smallestIndex)
        temp = getAt(i);
        setAt(i, getAt(indexSmallest));
        setAt(indexSmallest, temp);
        }
    }

    // Return true if the element given is found in the list.
    bool contains(const E& key, const int first, const int last) {
        int mid = (first + last) / 2; // integer divison no rounding up
        if      (first > last)      { return false;                         } 
        else if (key == getAt(mid)) { return true;                          } 
        else if (key < getAt(mid))  { return contains(key, first, mid - 1); } // go left
        else                        { return contains(key, mid + 1, last);  } // go right
    }

    // Find the index of the first occurence of the element in the list. Returns zero if not found.
    int whereIs(const E& key) {
        for(int i = 0; i < numberOfNodes; i++) {
            if(getAt(i) == key) {
                return i;
            }
        }
        return -1;
    }

    /*-------------------------- Operators --------------------------*/
    // Return a string with format [element, element, element]
    virtual std::string toString() const {
        std::ostringstream oss;
        if (isEmpty()) {
            return "[ ]";
        } else {
            NodePtr iter = head->next;
            oss << "[ ";
            while (iter != tail) {
                oss << iter->element << ", ";
                iter = iter->next;
            }
            // Remove the last comma and space.
            oss.seekp(-2, std::ios::end);
            // Add the space back now.
            oss << " ]";
        }
        return oss.str();
    }
};

// << operator to print the list.
template <typename E>
std::ostream &operator<<(std::ostream &os, const LinkedList<E> &o) {
    os << o.toString();
    return os;
}

#endif
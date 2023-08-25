// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure
            //       to initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }
            {}

            // Description: Allows access to the element at that Node's
            //              position. There are two versions, getElt() and a
            //              dereference operator, use whichever one seems
            //              more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data
            // members of this Node class from within the PairingPQ class.
            // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
            // function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            // TODO: Add one extra pointer (parent or previous) as desired.
            Node *previous;
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        
        root = nullptr;
        numNodes = 0;
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {

        root = nullptr;
        numNodes = 0;

        while(start != end){
            push(*start);
            ++start;
        }
        //numNodes /= 2;
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare } {
        
        root = nullptr;
        numNodes = 0;

        std::deque<Node*> othernodes;

        if(other.root == nullptr){
            return;
        }

        othernodes.push_back(other.root);
        while(!othernodes.empty()){

            if(othernodes.front() -> child != nullptr){
            othernodes.push_back(othernodes.front()->child);
            }

            if(othernodes.front() -> sibling != nullptr){
                othernodes.push_back(othernodes.front()->sibling);
            }

            //pushing into the actual PQ.
            push(othernodes.front()->elt);

            othernodes.pop_front();

            //++numNodes;
        }


    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // TODO: Implement this function.
        // HINT: Use the copy-swap method from the "Arrays and Containers"
        //       lecture.
        PairingPQ temp(rhs);
       
        std::swap(numNodes, temp.numNodes);
        //should swap the entire heap.
        std::swap(root, temp.root);
        //Node *i = rhs.root;
        //pretty sure this is all unneccessary.
        /*while(temp.root != nullptr){
            std::swap(theroot, temp.root);

            if(temp.root->child != nullptr){
            std::swap(theroot->child, temp.root->child);
            }

            if(temp.root->sibling != nullptr){
            std::swap(theroot->sibling, temp.root->sibling);
            }

            theroot = root->child;

            temp.root = temp.root->child;
        }*/
        
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        
        std::deque<Node*> delnodes;
        if(root == nullptr){
            return;
        }else{

        delnodes.push_back(root);

        }

        while(!delnodes.empty()){

            if(numNodes == 0){
                return;
            }

            Node * delly = delnodes.front();

            if(delly == nullptr){
                return;
            }

            if(delly->child != nullptr){
            delnodes.push_back(delly->child);

            }

            if(delly -> sibling != nullptr){
                delnodes.push_back(delly->sibling);
            }

            root = delly;
            //pop(); //already --numNodes
            pop();

            delnodes.pop_front();
        }

    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant. You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        
        std::deque<Node*> updeque;

        if(root != nullptr){
        updeque.push_back(root);
        }else{
            return;
        }
        root = nullptr;

        while(!updeque.empty()){

            Node * front = updeque.front();

            if(front->child != nullptr){
            updeque.push_back(front->child);
            }

            if(front->sibling != nullptr){
                updeque.push_back(front->sibling);
            }

            //cut current node off from family
            front->sibling = nullptr;
            front->child = nullptr;
            front->previous = nullptr;

            //1st time through it will meld w/ itself
            if(root == nullptr){
                root = front;
            } else{
            root = meld(updeque.front(), root);

            }
            updeque.pop_front();
        }

    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely in
    //              the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the pairing heap is empty. Though you are
    //       welcome to if you are familiar with them, you do not need to use
    //       exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        
        Node *victim = root->child;

        if(victim == nullptr){
            delete root;
            root = nullptr;
            --numNodes;

        }else{

        std::deque<Node*> ptrdeque;
        
        while(victim != nullptr){
            ptrdeque.push_back(victim);
            victim = victim->sibling;
        }

        while(!ptrdeque.empty()){
            
            //base case.
            if(ptrdeque.size() == 1){

                delete root;
                root = ptrdeque[0];
                --numNodes;
                return;
            }

            Node * toppy = ptrdeque.front();
            //need to break off ties with other nodes. 
            toppy->sibling = nullptr;
            toppy->previous = nullptr;

            ptrdeque.pop_front();

            Node * toppy2 = ptrdeque.front();
            toppy2->sibling = nullptr;
            toppy2->previous = nullptr;

            ptrdeque.pop_front();

            Node * toppynode = meld(toppy, toppy2);
            //push_back the product of the two nodes
            ptrdeque.push_back(toppynode);
        }
    
        }

    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {

        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        
        return numNodes;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        
        if(numNodes == 0){
            return true;
        }
        
        return false; 
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value. Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //               extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {

        if(node == root){
            root->elt = new_value;
            return;
        }
    
        node->elt = new_value;

        if(node->previous->child == node && this->compare(node->elt, node->previous->elt)){
            return;
        }
        

        if(node->sibling != nullptr){

            if(node->previous->child == node){

                node->previous->child = node->sibling;
                node->sibling->previous = node->previous;

            }else{

            node->previous->sibling = node->sibling;

            node->sibling->previous = node->previous;

            }

        }

        //re-meld this node w/ root.
        node->sibling = nullptr;
        node->previous = nullptr;
        //node->child = nullptr;

        root = meld(node, root);
        
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    // Runtime: O(1)
    Node* addNode(const TYPE &val) {
       
        Node * nodey = new Node(val);

        if(root == nullptr){

            root = nodey;
           
            ++numNodes;
            return nodey;
        }
        
        root = meld(nodey, root);

        ++numNodes;

        return nodey;
    } // addNode()
    


    private:
    // TODO: Add any additional member variables or member functions you
    //       require here.
    Node * root; 
    uint32_t numNodes;

    // TODO: We recommend creating a 'meld' function (see the Pairing Heap
    //       papers).

    Node * meld(Node *pa, Node *pb){ //pa and pb are roots of two individual pairing heaps.
        
        if(this->compare(pa->elt, pb->elt)){
            if(pb->child == nullptr){//maybe erase this case?
                pb->child = pa;
                pa->previous = pb;
                return pb;
            }else{
            pa->sibling = pb->child;
            pb->child->previous = pa;

            pb->child = pa;
            pa->previous = pb;
            
            return pb;
            }

        }else{

        if(pa->child == nullptr){//maybe erase this case?
                pa->child = pb;
                pb->previous = pa;
                return pa;
            }else{
            pb->sibling = pa->child;
            pa->child->previous = pb;

            pa->child = pb;
            pb->previous = pa;
            
            return pa;
            }

        }
    }
    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};


#endif // PAIRINGPQ_H

// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a binary
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function, or verify that it is already done
    } // BinaryPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, data{start, end} {
        
        updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of
    //              order and 'rebuilds' the heap by fixing the heap
    //              invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        for(size_t i = size(); i > 0; --i){
        fixDown(i);
        }
    } // updatePriorities()


    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val) {
        /*std::cout<<"in push?\n";
        //getElement(data.size() + 1) = val;
        std::cout<<"this the issue?\n"*/
        data.push_back(val);
        
        fixUp(size());

    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the PQ is empty. Though you are welcome to if
    //       you are familiar with them, you do not need to use exceptions in
    //       this project.
    // Runtime: O(log(n))
    virtual void pop() {

        getElement(1) = getElement(data.size());

        data.pop_back();
        //getElement(1) = getElement(data.size() - 1);
        fixDown(1);

        } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function.
        //std::cout<<"top = "<<data.front()<<"\n";
        return getElement(1);
        std::cout<<"top successful!!\n";
    } // top()


    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    virtual std::size_t size() const {
        //don't think I have to do anything after all.
        return data.size();
    } // size()


    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        //same here
    return data.empty();
    } // empty()


private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables. You don't need
    //       a "heapSize", since you can call your own size() member
    //       function, or check data.size().

    // TODO: Add any additional member functions you require here. For
    //       instance, you might add fixUp() and fixDown().
        // Translate 1-based indexing into a 0-based vector
    void fixUp(size_t key){
    
        while(key > 1 && (this->compare(getElement(key/2), getElement(key)))){
            std::swap(getElement(key), getElement(key/2));
            key /= 2;//move up to parent.
        }
        
    }

    void fixDown(size_t key){
    
        while(2 * key <= data.size()){
            size_t j = 2 * key;//start w/ left child
            if(j < data.size() && this->compare(getElement(j), getElement(j+1))){
                ++j;
            }

            if(this->compare(getElement(j), getElement(key))){//this->compare(getElement(j), getElement(key))
                break;
            }

            std::swap(getElement(key), getElement(j));
            key = j;
        }

    }



    TYPE &getElement(std::size_t i) {
      return data[i - 1];
    }  // getElement()


    const TYPE &getElement(std::size_t i) const {
      return data[i - 1];
    }  // getElement()

}; // BinaryPQ


#endif // BINARYPQ_H

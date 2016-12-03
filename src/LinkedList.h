#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi {

    template<typename Type>
    class LinkedList {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type *;
        using reference = Type &;
        using const_pointer = const Type *;
        using const_reference = const Type &;

        struct BaseNode;
        struct Node;

        class ConstIterator;

        friend class ConstIterator;

        class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

        LinkedList() {
            head = new BaseNode(nullptr, nullptr);
            tail = new BaseNode(nullptr, nullptr);
            head->next = tail;
            tail->previous = head;
            size = 0;
        }

        LinkedList(std::initializer_list<Type> l) : LinkedList() {
            for (value_type i: l) {
                append(std::forward<decltype(i)>(i));
            }
            size = l.size();
        }

        LinkedList(const LinkedList &other): LinkedList() {
            for (value_type i:other) {
                append(std::forward<decltype(i)>(i));
            }
        }

        LinkedList(LinkedList &&other) : LinkedList() {
            BaseNode *tempHead = head;
            BaseNode *tempTail = tail;
            size = other.size;
            head = other.head;
            tail = other.tail;
            other.head = tempHead;
            other.tail = tempTail;
            other.size = 0;
        }

        virtual ~LinkedList() {
            clear();
            delete head;
            delete tail;
        }

        LinkedList &operator=(const LinkedList &other) {
            clear();
            for (value_type i: other) {
                append(std::forward<decltype(i)>(i));
            }
            return *this;
        }

        LinkedList &operator=(LinkedList &&other) {
            clear();
            BaseNode *tempHead = head;
            BaseNode *tempTail = tail;
            head = other.head;
            tail = other.tail;
            size = other.size;
            other.head = tempHead;
            other.tail = tempTail;
            other.size = 0;
            return *this;
        }

        bool isEmpty() const {
            return !size;
        }

        size_type getSize() const {
            return size;
        }

        void append(const Type &item) {
            BaseNode *newNode = new Node(tail->previous, tail, item);
            tail->previous->next = newNode;
            tail->previous = newNode;
            size++;
        }

        void prepend(const Type &item) {
            BaseNode *newNode = new Node(head, head->next, item);
            head->next->previous = newNode;
            head->next = newNode;
            size++;
        }

        void insert(const const_iterator &insertPosition, const Type &item) {
            Iterator it = begin();
            while(it != end()){
                if(it==insertPosition){
                    BaseNode *found = insertPosition.node;
                    BaseNode *newNode = new Node(found->previous,found,item);
                    found->previous = newNode;
                    newNode->previous->next = newNode;
                    size++;
                    return;
                }
                it++;
            }
            append(item);
        }

        Type popFirst() {
            Iterator it = begin();
            value_type result = *it;
            erase(it);
            return result;
        }

        Type popLast() {
            Iterator it = --end();
            value_type result = *it;
            erase(it);
            return result;
        }

        void erase(const const_iterator &possition) {
            Iterator it = begin();
            while (it != end()) {
                if (it == possition) {
                    BaseNode *toErase = possition.node;
                    toErase->next->previous = toErase->previous;
                    toErase->previous->next = toErase->next;
                    delete toErase;
                    size--;
                    return;
                }
                ++it;
            }
            throw std::out_of_range("Trying to erase invalid node.");
        }

        void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded) {
            ConstIterator it = cbegin();
            bool isFirstValid = false;
            bool isSecondValid = false;
            while(it != end()){
                if(it == firstIncluded) isFirstValid = true;
                if(it == lastExcluded) isSecondValid = true;
                ++it;
            }
            it = firstIncluded;
            if(isFirstValid||isSecondValid){
                while(it != lastExcluded)
                    {
                        it++;
                        BaseNode *toErase = it.node->previous;
                        toErase->next->previous = toErase->previous;
                        toErase->previous->next = toErase->next;
                        delete toErase;
                        size--;
                    }
                return;
            }
            throw std::out_of_range("Trying to erase invalid nodes.");
        }

        iterator begin() {
            return Iterator(this, head->next);
        }

        iterator end() {
            return Iterator(this, tail);
        }

        const_iterator cbegin() const {
            return ConstIterator(this, head->next);
        }

        const_iterator cend() const {
            return ConstIterator(this, tail);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    private:
        BaseNode *head;
        BaseNode *tail;
        size_type size;

        void clear() {
            while(size!=0){erase(begin());}
        }
    };

    template<typename Type>
    class LinkedList<Type>::ConstIterator {
    public:
        friend class LinkedList;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        explicit ConstIterator(const LinkedList *targetList, BaseNode *targetNode) {
            list = targetList;
            node = targetNode;
        }

        ConstIterator(const ConstIterator &target) {
            list = target.list;
            node = target.node;
        }

        reference operator*() const {
            if (node == list->tail || node == list->head)throw std::out_of_range("Dereferencing out of list's scope.");
            else return static_cast<Node *>(node)->item;
        }

        ConstIterator &operator++() {
            if (node == list->tail) throw std::out_of_range("Incrementing end() iterator.");
            node = node->next;
            return *this;
        }

        ConstIterator operator++(int) {
            if (node == list->tail) throw std::out_of_range("Incrementing end() iterator.");
            ConstIterator result(*this);
            node = node->next;
            return result;
        }

        ConstIterator &operator--() {
            if (node == list->head->next) throw std::out_of_range("Decrementing begin() iterator.");
            node = node->previous;
            return *this;
        }

        ConstIterator operator--(int) {
            if (node == list->head->next) throw std::out_of_range("Decrementing begin() iterator.");
            ConstIterator result(*this);
            node = node->previous;
            return result;
        }

        ConstIterator operator+(difference_type d) const {
            if (d < 0) return *this - (-d);
            difference_type counter = 0;
            BaseNode *temp = node;
            while (temp != list->tail) {
                counter++;
                temp = temp->next;
            }
            if (counter < d) throw std::out_of_range("Trying to shift iterator past end() iterator.");
            ConstIterator result(*this);
            while (d != 0) {
                result.node = result.node->next;
                d--;
            }
            return result;
        }

        ConstIterator operator-(difference_type d) const {
            if (d < 0) return *this + (-d);
            difference_type counter = 0;
            BaseNode *temp = node;
            while (temp != list->head->next) {
                counter++;
                temp = temp->previous;
            }
            if (counter < (d)) throw std::out_of_range("Trying to shift iterator past end() iterator.");
            ConstIterator result(*this);
            while (d != 0) {
                result.node = result.node->previous;
                d--;
            }
            return result;
        }

        bool operator==(const ConstIterator &other) const {
            return ((node == other.node) && (list == other.list));
        }

        bool operator!=(const ConstIterator &other) const {
            return !((node == other.node) && (list == other.list));
        }

    protected:
        const LinkedList *list;
        BaseNode *node;
    };

    template<typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator(const LinkedList *targetList, BaseNode *targetNode) : ConstIterator(targetList, targetNode) {}

        Iterator(const ConstIterator &other)
                : ConstIterator(other) {}

        Iterator &operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator &operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const {
            return ConstIterator::operator-(d);
        }

        reference operator*() const {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

    template<typename Type>
    struct LinkedList<Type>::BaseNode {
        BaseNode *next;
        BaseNode *previous;

        BaseNode() {
            next = nullptr;
            previous = nullptr;
        }

        BaseNode(BaseNode *prev, BaseNode *nxt) {
            previous = prev;
            next = nxt;
        }

        virtual ~BaseNode() {
            previous = nullptr;
            next = nullptr;
        }
    };

    template<typename Type>
    struct LinkedList<Type>::Node : public LinkedList<Type>::BaseNode {
        Type item;

        Node() : BaseNode() {
            item = nullptr;
        }

        Node(BaseNode *prev, BaseNode *nxt, const Type &value) : BaseNode(prev, nxt) {
            item = value;
        }

        Node(BaseNode *prev, BaseNode *nxt, Type &&value) : BaseNode(prev, nxt) {
            item = std::move(value);
        }

        ~Node() {}
    };

}


#endif // AISDI_LINEAR_LINKEDLIST_H

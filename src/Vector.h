#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

m.szlenk@elka.pw.edu.pl
w tytule maila
AISDI - Zad 1



#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

namespace aisdi {

    template<typename Type>
    class Vector {
    public:
        friend class ConstIterator;

        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type *;
        using reference = Type &;
        using const_pointer = const Type *;
        using const_reference = const Type &;


        class ConstIterator;

        class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

        std::size_t current_size;
        size_type capacity;


        Vector() {
            data = new value_type[16];
            current_size = 0;
            capacity = 16;
        }

        Vector(std::initializer_list<Type> l) {
            size_type size_of_vec;
            if (!(l.size() % 2)) {
                size_of_vec = l.size();
            } else {
                size_of_vec = l.size() + 1;
            }
            data = new value_type[size_of_vec];
            capacity = size_of_vec;
            current_size = l.size();
            size_type j = 0;
            for (value_type i:l) {
                data[j] = i;
                j++;
            }
        }

        Vector(const Vector &other) {
            size_type size_of_vec;
            if (!(other.getSize() % 2)) {
                size_of_vec = other.getSize();
            } else {
                size_of_vec = other.getSize() + 1;
            }
            data = new value_type[size_of_vec];
            capacity = size_of_vec;
            current_size = other.getSize();
            size_type j = 0;
            for (value_type i :other) {
                data[j] = i;
                j++;
            }
        }

        Vector(Vector &&other) : Vector() {
            delete []data;
            data = other.data;
            current_size = other.getSize();
            capacity = other.capacity;
            other.data = new value_type[16];
            other.current_size = 0;
            other.capacity = 16;
        }

        ~Vector() {
            delete[]data;
        }

        Vector &operator=(const Vector &other) {
            if (capacity != other.capacity) {
                delete[] data;
                capacity = other.capacity;
                data = new Type[capacity];
            }
            size_t j = 0;
            for (value_type i: other) {
                data[j++] = i;
            }
            current_size = other.current_size;
            return *this;
        }

        Vector &operator=(Vector &&other) {
            delete []data;
            data = other.data;
            current_size = other.getSize();
            capacity = other.capacity;
            other.data = new value_type[16];
            other.current_size = 0;
            other.capacity = 16;
            return *this;
        }

        bool isEmpty() const {
            return current_size == 0;
        }

        size_type getSize() const {
            return current_size;
        }

        void append(const Type &item) {
            if (getSize() == capacity) realocate();
            data[getSize()] = item;
            current_size++;
        }

        void prepend(const Type &item) {
            if (getSize() == capacity) realocate();
            for (difference_type i = static_cast<difference_type>(current_size); i > 0; i--) {
                data[i] = data[i - 1];
            }
            data[0] = item;
            current_size++;
        }

        void insert(const const_iterator &insertPosition, const Type &item) {
            insert_at(item, insertPosition.index);
        }

        Type popFirst() {
            if (isEmpty()) throw std::out_of_range("Trying to pop item from empty vector.");
            Type result = data[0];
            erase_at(0);
            return result;
        }

        Type popLast() {
            if (isEmpty()) throw std::out_of_range("Trying to pop item from empty vector.");
            Type result = data[current_size - 1];
            erase_at(current_size - 1);
            return result;
        }

        void erase(const const_iterator &possition) {
            if (isEmpty()) throw std::out_of_range("Trying to erase empty vector.");
            if (possition == end()) throw std::out_of_range("Trying to erase position of end() iterator.");
            erase_at(possition.index);
        }

        void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded) {
            if (isEmpty()) throw std::out_of_range("Trying to erase empty vector.");
            difference_type beg_index = static_cast<difference_type>(firstIncluded.index);
            difference_type end_index = static_cast<difference_type>(lastExcluded.index);
            difference_type difference = end_index - beg_index;
            for (difference_type i = 0; i < difference; i++) {
                erase_at(beg_index);
            }
        }

        iterator begin() {
            return Iterator(this, 0);
        }

        iterator end() {
            return Iterator(this, current_size);
        }

        const_iterator cbegin() const {
            return ConstIterator(this, 0);
        }

        const_iterator cend() const {
            return ConstIterator(this, current_size);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    private:
        pointer data;

        void realocate() {
            pointer temp_data = new value_type[capacity];
            capacity *= 2;
            current_size = getSize();
            for (size_type i = 0; i < getSize(); i++) {
                temp_data[i] = data[i];
            }
            delete[] data;
            data = new value_type[capacity];
            for (size_type i = 0; i < current_size; i++) {
                data[i] = temp_data[i];
            }
            delete[] temp_data;
        }

        void insert_at(const Type &value, size_type position) {
            if (position > capacity)throw std::out_of_range("Trying to insert item outside of scope.");
            if (current_size == capacity) realocate();
            for (difference_type i = static_cast<difference_type>(current_size - 1);
                 i >= static_cast<difference_type>(position);
                 i--) {
                data[i + 1] = data[i];
            }
            data[position] = value;
            current_size++;
        }

        void erase_at(size_type position) {
            current_size--;
            for (difference_type i = static_cast<difference_type>(position);
                 i < static_cast<difference_type>(current_size);
                 i++) {
                data[i] = data[i + 1];
            }
        }
    };

    template<typename Type>
    class Vector<Type>::ConstIterator {
    public:
        friend class Vector<Type>;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Vector::value_type;
        using difference_type = typename Vector::difference_type;
        using pointer = typename Vector::const_pointer;
        using reference = typename Vector::const_reference;


        explicit ConstIterator(const Vector<Type> *target_vector, size_type target_index) {
            vector = target_vector;
            index = target_index;
        }

        reference operator*() const {// end() or not
            if (vector->getSize() == index) throw std::out_of_range("Dereferencing of end() iterator.");
            return vector->data[index];
        }

        ConstIterator &operator++() {
            if (vector->getSize() == index) throw std::out_of_range("Incrementing end() iterator.");
            index++;
            return *this;
        }

        ConstIterator operator++(int) {
            if (vector->getSize() == index) throw std::out_of_range("Incrementing end() iterator.");
            ConstIterator result(vector, index);
            index++;
            return result;
        }

        ConstIterator &operator--() {
            if (index == 0) throw std::out_of_range("Decrementing begin() iterator.");
            index--;
            return *this;
        }

        ConstIterator operator--(int) {
            if (index == 0) throw std::out_of_range("Decrementing begin() iterator.");
            ConstIterator result(vector, index);
            index--;
            return result;
        }

        ConstIterator operator+(difference_type d) const {
            size_type new_index = index + d;
            if (new_index > vector->getSize()) throw std::out_of_range("Trying to shift iterator past end() iterator.");
            ConstIterator result(vector, new_index);
            return result;
        }

        ConstIterator operator-(difference_type d) const {
            size_type new_index = index - d;
            if (new_index < 0) throw std::out_of_range("Trying to shift iterator before begin() iterator.");
            ConstIterator result(vector, new_index);
            return result;
        }

        bool operator==(const ConstIterator &other) const {
            return ((index == other.index) && (vector == other.vector));
        }

        bool operator!=(const ConstIterator &other) const {
            return !((index == other.index) && (vector == other.vector));
        }

    protected:
        size_type index;
        const Vector<Type> *vector;
    };

    template<typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;

        explicit Iterator(const Vector<Type> *target_vector, size_type target_index)
                : ConstIterator(target_vector, target_index) {}

        Iterator(const ConstIterator &other)
                : ConstIterator(other.vector, other.index) {}

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

}

#endif // AISDI_LINEAR_VECTOR_H

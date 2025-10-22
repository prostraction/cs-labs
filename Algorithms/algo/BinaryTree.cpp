#include <iostream>
#include <memory>
#include <stack>
#include <stdexcept>
#include <initializer_list>

template <typename T>
struct TreeNode {
    T value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    
    TreeNode(const T& val) : value(val), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree {
public:
    BinaryTree() = default;
    BinaryTree(std::initializer_list<T> values) {
        for (const auto& val : values) {
            insert(val);
        }
    }
    BinaryTree(BinaryTree&&) = default;
    BinaryTree& operator=(BinaryTree&&) = default;
    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;
    
    void clear() {
        root_.reset();
    }
    
    bool contains(const T& value) const {
        return findNode(root_, value) != nullptr;
    }
    
    void insert(const T& value) {
        insertNode(root_, value);
    }
    
    // Обходы дерева
    void inOrderTraversal() const { inOrder(root_); }
    void postOrderTraversal() const { postOrder(root_); }
    
    // C++11: оператор вывода через дружественную функцию
    friend std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& tree) {
        tree.inOrderTraversal();
        return os;
    }
    
    class Iterator {
    public:
        // C++11: using для псевдонимов типов
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        Iterator() = default;
        
        // Конструктор из списка инициализации (пример)
        Iterator(std::initializer_list<TreeNode<T>*> nodes) {
            for (auto node : nodes) {
                if (node) stack.push(node);
            }
        }
        
        reference operator*() { 
            if (stack.empty()) throw std::out_of_range("Iterator out of range");
            return stack.top()->value; 
        }
        
        pointer operator->() { return &stack.top()->value; }
        
        Iterator& operator++() {
            if (stack.empty()) return *this;
            
            auto current = stack.top();
            stack.pop();
            
            if (current->right) {
                pushLeftBranch(current->right.get());
            }
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const Iterator& other) const {
            return stack.size() == other.stack.size(); // Упрощенное сравнение
        }
        
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
        
    private:
        std::stack<TreeNode<T>*> stack;
        
        void pushLeftBranch(TreeNode<T>* node) {
            while (node) {
                stack.push(node);
                node = node->left.get();
            }
        }
        
        friend class BinaryTree;
        Iterator(TreeNode<T>* root) { pushLeftBranch(root); }
    };
    
    Iterator begin() const { return Iterator(root_.get()); }
    Iterator end() const { return Iterator(); }
    Iterator cbegin() const { return begin(); }
    Iterator cend() const { return end(); }

private:
    std::unique_ptr<TreeNode<T>> root_;
    
    TreeNode<T>* findNode(const std::unique_ptr<TreeNode<T>>& node, const T& value) const {
        if (!node) return nullptr;
        
        if (value == node->value) return node.get();
        if (value < node->value) return findNode(node->left, value);
        return findNode(node->right, value);
    }
    
    void insertNode(std::unique_ptr<TreeNode<T>>& node, const T& value) {
        if (!node) {
            node = std::make_unique<TreeNode<T>>(value); // C++14: make_unique (можно заменить на new)
            return;
        }
        
        if (value <= node->value) {
            insertNode(node->left, value);
        } else {
            insertNode(node->right, value);
        }
    }
    
    void inOrder(const std::unique_ptr<TreeNode<T>>& node) const {
        if (!node) return;
        
        inOrder(node->left);
        std::cout << node->value << '\t';
        inOrder(node->right);
    }
    
    void postOrder(const std::unique_ptr<TreeNode<T>>& node) const {
        if (!node) return;
        
        postOrder(node->left);
        postOrder(node->right);
        std::cout << node->value << '\t';
    }
};

int main() {
    BinaryTree<int> tree{5, 3, 7, 2, 4, 6, 8};
    
    std::cout << "Post-order traversal: ";
    tree.postOrderTraversal();
    std::cout << std::endl;
    
    std::cout << "In-order traversal: ";
    tree.inOrderTraversal();
    std::cout << std::endl;
    
    std::cout << "Contains 5: " << (tree.contains(5) ? "true" : "false") << std::endl;
    std::cout << "Contains 6: " << (tree.contains(6) ? "true" : "false") << std::endl;
    std::cout << "Contains 10: " << (tree.contains(10) ? "true" : "false") << std::endl;
    
    std::cout << "Using operator<<: " << tree << std::endl;
    
    std::cout << "Using iterators: ";
    for (const auto& val : tree) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
    
    std::cout << "Using const iterators: ";
    for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
    
    tree.clear();
    std::cout << "After clear - contains 5: " << (tree.contains(5) ? "true" : "false") << std::endl;
    
    return 0;
}
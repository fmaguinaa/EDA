#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include "types.h"
#include "iterator.h"

template <typename Container>
class matrix_iterator {
public: 
    // TODO: subir al padre  
    typedef typename Container::Node        Node;
    typedef typename Container::value_type        value_type;
    typedef matrix_iterator<Container>      myself;

private:
    Container *m_pContainer;
    Node      *m_pNode;
    size_t     i = 0, j = 0;
public:
    matrix_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
    matrix_iterator(myself &other)
            : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), i(other.i), j(other.j){}
    matrix_iterator(myself &&other) 
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode      = move(other.m_pNode);
        i = move(other.i);
        j = move(other.j);
    }

public:
    bool operator==(matrix_iterator iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(matrix_iterator iter)   { return !(*this == iter);        }
    value_type &operator*()                    { return m_pNode->getDataRef();   }
    matrix_iterator operator=(matrix_iterator &iter)
    {
        m_pContainer = move(iter.m_pContainer);
        m_pNode      = move(iter.m_pNode);
        i = move(iter.i);
        j = move(iter.j);
        return *this; // Pending static_cast?
    }
    matrix_iterator operator++() {
        if(j != m_pContainer->cols() - 1){
            j++;
        } else {
            i++;
            j=0;
        }
        Node** matrix = m_pContainer->matrix();
        m_pNode = *(matrix+i)+j;
        return *this;
    }
};

template <typename T>
class NodeMatrix{
public:
    using value_type = T;
    using Type = T;
private:
    using Node = NodeMatrix<T>;
public:
    value_type m_key;
public:
    NodeMatrix() {}
    NodeMatrix(value_type key): m_key(key){}
    NodeMatrix(const NodeMatrix<T>& other) : m_key(other.m_key){}
    NodeMatrix(NodeMatrix<T>&&      other) : m_key(std::move(other.m_key)){}

    NodeMatrix& operator=(const NodeMatrix& other){
        if(this != &other){
            m_key = other.m_key;
        }
        return *this;
    }
    value_type getData() const {return m_key;}
    value_type& getDataRef() {return m_key;}

    constexpr operator value_type() const noexcept {
        return m_key;
    }
    constexpr value_type operator()() const noexcept { // since C++14
        return m_key;
    }
};

template <typename _K>
struct MatrixTrait
{
    using value_type = _K;
    using Node       = NodeMatrix<_K>;
};

using MatrixTraitInt   = MatrixTrait<int>;
using MatrixTraitFloat = MatrixTrait<float>;

template <typename Traits>
class CMatrix
{public:
    using value_type      = typename Traits::value_type;
    using Node            = typename Traits::Node;
    using myself          = CMatrix<Traits>;
    using iterator        = matrix_iterator<myself>;

    private:
        Node **m_ppMatrix   = nullptr;
        size_t m_rows = 0, m_cols = 0;
public:
    CMatrix(size_t rows, size_t cols)
    {   create(rows, cols); 
    }
    CMatrix(myself &&other){
        m_rows      = std::move(other.m_rows);
        m_cols      = std::move(other.m_cols);
        m_ppMatrix  = std::move(other.m_ppMatrix);
    }
    ~CMatrix(){
        destroy();
    }

    CMatrix operator*(CMatrix const& other){
        CMatrix result = CMatrix(m_rows, other.m_cols);
        result.fill(.0);
        for(auto i = 0; i<m_rows;i++){
            for(auto j = 0; j<other.m_cols; j++){
                for(auto k =0; k<m_cols;k++){
                    result(i,j) += m_ppMatrix[i][k] * other.m_ppMatrix[k][j];
                }
            }
        }
        return result;
    }

    CMatrix &operator=(CMatrix const& other){
        return new CMatrix(other.m_rows, other.m_cols);
    }

    class CRow
    {
        friend class CMatrix;
    private:
        CMatrix &m_parent;
        size_t m_row;
        CRow(CMatrix &parent, size_t row) : m_parent(parent), m_row(row) {}
    public:
        value_type &operator[](size_t col) { return m_parent.m_ppMatrix[m_row][col].getDataRef(); }
    };

    CRow operator[](size_t row){
        return CRow(*this, row);
    }

    value_type &operator()(size_t row, size_t col){
        return m_ppMatrix[row][col].getDataRef();
    }
    
    size_t rows() {return m_rows;}
    size_t cols() {return m_cols;}
    Node** matrix() {return m_ppMatrix;}

    iterator begin() { iterator iter(this, *m_ppMatrix);    return iter;    }
    iterator end()   { iterator iter(this, *(m_ppMatrix+(m_rows-1))+(m_cols-1));    return iter;    }

    void create(size_t rows, size_t cols){
        destroy();
        m_rows = rows;
        m_cols = cols;
        m_ppMatrix = new Node *[m_rows];
        for(auto i = 0 ; i < m_rows ; i++)
            m_ppMatrix[i] = new Node[m_cols];
            // *(res+i) = new TX[m_cols];
            // *(i+res) = new TX[m_cols];
            // i[res]   = new TX[m_cols];
        
    }
    
    void fill(value_type val){
        for(auto y = 0 ; y < m_rows ; y++)
            for(auto x = 0 ; x < m_cols ; x++)
                m_ppMatrix[y][x] = Node(val);
                // *(m_ppMatrix+y)[x] = val;
                // *(*(m_ppMatrix+y)+x) = val;
                // *(y[m_ppMatrix]+x) = val;
                // x[y[m_ppMatrix]] = val;
    }

    void print(ostream &os){
        os << m_rows << " " << m_cols << endl;
        for(auto y = 0 ; y < m_rows ; y++){
            for(auto x = 0 ; x < m_cols ; x++)
                os << m_ppMatrix[y][x].getDataRef() << " ";
            os << endl;
        }
    }
    void destroy(){
        for(auto y = 0 ; y < m_rows ; y++)
            delete [] m_ppMatrix[y];
        delete [] m_ppMatrix;
        m_ppMatrix = nullptr;
        m_rows = m_cols = 0;
    }
};

template <typename Traits>
ostream &operator<<(ostream &os, CMatrix<Traits> &mat){
    mat.print(os);
    return os;
}

#endif
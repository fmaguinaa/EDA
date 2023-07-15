#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include "types.h"
#include "iterator.h"

template <typename Container>
class matrix_forward_iterator 
     : public general_iterator<Container,  class matrix_forward_iterator<Container> > // 
{public: 
    // TODO: subir al padre  
    typedef class general_iterator<Container, matrix_forward_iterator<Container> > Parent; 
    typedef typename Container::Node           Node; // 
    typedef matrix_forward_iterator<Container>  myself;

  public:
    matrix_forward_iterator(Container *pContainer, Node *pNode) 
            : Parent (pContainer,pNode) {}
    matrix_forward_iterator(myself &other)  : Parent (other) {}
    matrix_forward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    matrix_forward_iterator operator++() { Parent::m_pNode++;  
                                          return *this;
                                        }
};

template <typename T
// , typename V
>
class NodeMatrix{
public:
    using value_type = T;
    using Type = T;
    // using LinkedValueType = V;
private:
    // using Node = NodeMatrix<T, V>;
    using Node = NodeMatrix<T>;
public:
    value_type m_key;
    // LinkedValueType m_value;

public:
    NodeMatrix(value_type key
    // , LinkedValueType value
    )
    : m_key(key)
    // ,m_value(value)
    {}
 
    // NodeMatrix(const NodeMatrix<T, V>& other)
    NodeMatrix(const NodeMatrix<T>& other)
    : m_key(other.m_key)
    // , m_value(other.m_value)
    {}

    // NodeMatrix(NodeMatrix<T, V>&& other)
    NodeMatrix(NodeMatrix<T>&& other)
    : m_key(std::move(other.m_key))
    // , m_value(std::move(other.m_value))
    {}

    NodeMatrix() {}

    NodeMatrix& operator=(const NodeMatrix& other){
        if(this != &other){
            m_key = other.m_key;
            // m_value = other.m_value;
        }
        return *this;
    }

    value_type getData() const {return m_key;}
    value_type& getDataRef() {return m_key;}

    // LinkedValueType getValue() const {return m_value;}
    // LinkedValueType& getValueRef() {return m_value;}
};


template <typename _K>
struct MatrixTrait
{
    using  value_type      = _K;
    // using  LinkedValueType = _V;
    using  Node            = NodeMatrix<_K>;
    // using  CompareFn = _CompareFn;
};

using MatrixTraitFloat = MatrixTrait<float>;

template <typename Traits>
class CMatrix
{public:
    using value_type      = typename Traits::value_type;
    //using LinkedValueType = typename Traits::LinkedValueType;
    using Node            = typename Traits::Node;
    //using CompareFn       = typename Traits::CompareFn;
    using myself          = CMatrix<Traits>;
    using iterator        = matrix_forward_iterator<myself>;

    private:
        Node **m_ppMatrix   = nullptr;
        size_t m_rows = 0, m_cols = 0;
public:
    CMatrix(size_t rows, size_t cols)
    {   create(rows, cols); 
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
                    result(i,j) += m_ppMatrix[i][k].getData() * other.m_ppMatrix[k][j].getData();
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

    iterator begin() { iterator iter(this, *m_ppMatrix);    return iter;    }
    iterator end()   { iterator iter(this, *(m_ppMatrix+m_cols)+m_rows);    return iter;    }

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
                m_ppMatrix[y][x] = val;
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
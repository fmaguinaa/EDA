#ifndef ___BTREE_ITERATOR_H__
#define ___BTREE_ITERATOR_H__

template <typename Container>
class btree_forward_iterator
{
public:
  typedef typename Container::Node Node;
  typedef typename Node::Type Type;
  typedef btree_forward_iterator<Container> myself;

private:
  Container *m_pContainer;
  size_t m_index;

public:
  btree_forward_iterator(Container *pContainer, size_t index) : m_pContainer(pContainer), m_index(index) {}
  btree_forward_iterator(myself &other)
  {
    m_pContainer = other.m_pContainer;
    m_index = other.m_index;
  }
  btree_forward_iterator(myself &&other)
  {
    m_pContainer = std::move(other.m_pContainer);
    m_index = std::move(other.m_index);
  }
  btree_forward_iterator operator=(btree_forward_iterator &iter)
  {
    m_pContainer = std::move(iter.m_pContainer);
    m_index = std::move(iter.m_index);
  }
  bool operator==(btree_forward_iterator iter)
  {
    return m_index == iter.m_index;
  }
  bool operator!=(btree_forward_iterator iter)
  {
    return !(*this == iter);
  }
  Node &operator*()
  {
    return *(m_pContainer->getNode(m_index));
  }
  btree_forward_iterator operator++()
  {
    m_index++;
    return *this;
  }
};

template <typename Container>
class btree_backward_iterator
{
public:
  typedef typename Container::Node Node;
  typedef typename Node::Type Type;
  typedef btree_backward_iterator<Container> myself;

private:
  Container *m_pContainer;
  size_t m_index;

public:
  btree_backward_iterator(Container *pContainer, size_t index) : m_pContainer(pContainer), m_index(index) {}
  btree_backward_iterator(myself &other)
  {
    m_pContainer = other.m_pContainer;
    m_index = other.m_index;
  }
  btree_backward_iterator(myself &&other)
  {
    m_pContainer = std::move(other.m_pContainer);
    m_index = std::move(other.m_index);
  }
  btree_backward_iterator operator=(btree_backward_iterator &iter)
  {
    m_pContainer = std::move(iter.m_pContainer);
    m_index = std::move(iter.m_index);
  }
  bool operator==(btree_backward_iterator iter)
  {
    return m_index == iter.m_index;
  }
  bool operator!=(btree_backward_iterator iter)
  {
    return !(*this == iter);
  }
  Node &operator*()
  {
    return *(m_pContainer->getNode(m_index));
  }
  btree_backward_iterator operator++()
  {
    m_index--;
    return *this;
  }
};

#endif
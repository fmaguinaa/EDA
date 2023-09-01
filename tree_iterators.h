#ifndef __TREE_ITERATORS_H__
#define __TREE_ITERATORS_H__

template <typename Container, class Iterator>
class binary_tree_iterator{
public:
  typedef typename Container::Node                        Node;
  typedef class binary_tree_iterator<Container, Iterator> myself;

private:
  Container *m_pContainer;
  Node      *m_pNode;

protected:
  bool m_visited = false;

public:
  binary_tree_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode){
    m_visited = pNode->getVisited();
  }
  binary_tree_iterator(myself &other)
      : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_visited(other.m_visited){}
  binary_tree_iterator(myself &&other){
    m_pContainer  = move(other.m_pContainer);
    m_pNode       = move(other.m_pNode);
    m_visited     = move(other.m_visited);
  }

  Iterator operator=(Iterator &iter){
    m_pContainer  = move(iter.m_pContainer);
    m_pNode       = move(iter.m_pNode);
    m_visited     = move(iter.m_visited);
    return *(Iterator *)this;
  }

  bool operator==(Iterator iter) { return m_pNode == iter.m_pNode; }
  bool operator!=(Iterator iter) { return !(*this == iter); }
  Node &operator*() { return *m_pNode; }

  virtual Node *recursive(Node *pNode, bool &found){
    found = true;
    pNode->setVisited(!m_visited);
    return pNode->getParent();
  }

  myself operator++(){
    bool found = false;
    Node *pNode = m_pNode;
    pNode->setVisited(!m_visited);
    while (!found){
      pNode = recursive(pNode, found);
    }
    m_pNode = pNode;
    return *this;
  }
  Node *self(Node *pNode, bool &founded){
    founded = true;
    pNode->setVisited(!m_visited);
    return pNode;
  }
  bool selfValidation(Node *pNode){
    return pNode->getVisited() == m_visited;
  }
  bool leftValidation(Node *pNode){
    return pNode->getChild(0) != nullptr && pNode->getChild(0)->getVisited() == m_visited;
  }
  bool rightValidation(Node *pNode){
    return pNode->getChild(1) != nullptr && pNode->getChild(1)->getVisited() == m_visited;
  }
};

template <typename Container>
class inorder_iterator
    : public binary_tree_iterator<Container, class inorder_iterator<Container>>{
public:
  typedef class binary_tree_iterator<Container, inorder_iterator<Container>> Parent;
  typedef typename Container::Node          Node;
  typedef class inorder_iterator<Container> myself;

public:
  inorder_iterator(Container *pContainer, Node *pNode) : Parent(pContainer, pNode) {}
  inorder_iterator(myself &other) : Parent(other) {}
  inorder_iterator(myself &&other) : Parent(other) {}

public:
  Node *recursive(Node *pNode, bool &founded){
    if (Parent::leftValidation(pNode)){
      return recursive(pNode->getChild(0), founded);
    }
    if (Parent::selfValidation(pNode)){
      return Parent::self(pNode, founded);
    }
    if (Parent::rightValidation(pNode)){
      return recursive(pNode->getChild(1), founded);
    }
    return pNode->getParent();
  }
};

template <typename Container>
class preorder_iterator
    : public binary_tree_iterator<Container, class preorder_iterator<Container>>
{
public:
  typedef class binary_tree_iterator<Container, preorder_iterator<Container>> Parent;
  typedef typename Container::Node            Node;
  typedef class preorder_iterator<Container>  myself;

public:
  preorder_iterator(Container *pContainer, Node *pNode) : Parent(pContainer, pNode) {}
  preorder_iterator(myself &other) : Parent(other) {}
  preorder_iterator(myself &&other) : Parent(other) {}

public:
  Node *recursive(Node *pNode, bool &founded){
    if (Parent::selfValidation(pNode)){
      return Parent::self(pNode, founded);
    }
    if (Parent::leftValidation(pNode)){
      return recursive(pNode->getChild(0), founded);
    }
    if (Parent::rightValidation(pNode)){
      return recursive(pNode->getChild(1), founded);
    }
    return pNode->getParent();
  }
};

template <typename Container>
class postorder_iterator
    : public binary_tree_iterator<Container, class postorder_iterator<Container>>
{
public:
  typedef class binary_tree_iterator<Container, postorder_iterator<Container>> Parent;
  typedef typename Container::Node            Node;
  typedef class postorder_iterator<Container> myself;

public:
  postorder_iterator(Container *pContainer, Node *pNode) : Parent(pContainer, pNode) {}
  postorder_iterator(myself &other) : Parent(other) {}
  postorder_iterator(myself &&other) : Parent(other) {}

public:
  Node *recursive(Node *pNode, bool &founded){
    if (Parent::leftValidation(pNode)){
      return recursive(pNode->getChild(0), founded);
    }
    if (Parent::rightValidation(pNode)){
      return recursive(pNode->getChild(1), founded);
    }
    if (Parent::selfValidation(pNode)){
      return Parent::self(pNode, founded);
    }
    return pNode->getParent();
  }
};

template <typename Container>
class printer_iterator
    : public binary_tree_iterator<Container, class printer_iterator<Container>>
{
public:
  typedef class binary_tree_iterator<Container, printer_iterator<Container>> Parent;
  typedef typename Container::Node          Node;
  typedef class printer_iterator<Container> myself;

public:
  printer_iterator(Container *pContainer, Node *pNode) : Parent(pContainer, pNode) {}
  printer_iterator(myself &other) : Parent(other) {}
  printer_iterator(myself &&other) : Parent(other) {}
public:
  Node *recursive(Node *pNode, bool &founded){
    if (Parent::rightValidation(pNode)){
      return recursive(pNode->getChild(1), founded);
    }
    if (Parent::selfValidation(pNode)){
      return Parent::self(pNode, founded);
    }
    if (Parent::leftValidation(pNode)){
      return recursive(pNode->getChild(0), founded);
    }
    return pNode->getParent();
  }
};

#endif

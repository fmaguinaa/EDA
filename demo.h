#ifndef __DEMO_H__
#define __DEMO_H__

void DemoBasicPointers();
void DemoSmartPointers();
void DemoDynamicMatrixes();
void DemoPreandPostIncrement();

void DemoArray();
void DemoIterators();
void DemoReverseIterators();
void DemoHeap();

void DemoBinaryTree();
void DemoHash();

template <typename Container>
void demoLinkedList(Container &);
void demoLinkedListSorted();

template <typename Container>
void demoDoubleLinkedList(Container &);
void demoDoubleLinkedListSorted();

#endif // __DEMO_H__
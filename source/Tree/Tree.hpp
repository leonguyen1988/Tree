#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>

template<typename DataType> class Tree;
template<typename DataType> class TreeNode;

/**
* @returns True if the data encapsulated in the left-hand side TreeNode is less than
* the data encapsulated in the right-hand side TreeNode.
*/
template<typename DataType>
inline bool operator<(const TreeNode<DataType>& lhs, const TreeNode<DataType>& rhs)
{
   return lhs.GetData() < rhs.GetData();
}

/**
* @returns True if the data encapsulated in the left-hand side TreeNode is less than or equal to
* the data encapsulated in the right-hand side TreeNode.
*/
template<typename DataType>
inline bool operator<=(const TreeNode<DataType>& lhs, const TreeNode<DataType>& rhs)
{
   return !(lhs.GetData() > rhs.GetData());
}

/**
* @returns True if the data encapsulated in the left-hand side TreeNode is greater than 
* the data encapsulated in the right-hand side TreeNode.
*/
template<typename DataType>
inline bool operator>(const TreeNode<DataType>& lhs, const TreeNode<DataType>& rhs)
{
   return rhs.GetData() < lhs.GetData();
}

/**
* @returns True if the data encapsulated in the left-hand side TreeNode is greater than
* or equal to the data encapsulated in the right-hand side TreeNode.
*/
template<typename DataType>
inline bool operator>=(const TreeNode<DataType>& lhs, const TreeNode<DataType>& rhs)
{
   return !(lhs.GetData() < rhs.GetData());
}

/**
* @returns True if the data encapsulated in the left-hand side TreeNode is equal to
* the data encapsulated in the right-hand side TreeNode.
*/
template<typename DataType>
inline bool operator==(const TreeNode<DataType>& lhs, const TreeNode<DataType>& rhs)
{
   return lhs.GetData() == rhs.GetData();
}

/**
* @returns True if the data encapsulated in the left-hand side TreeNode is not equal
* to the data encapsulated in the right-hand side TreeNode.
*/
template<typename DataType>
inline bool operator!=(const TreeNode<DataType>& lhs, const TreeNode<DataType>& rhs)
{
   return !(lhs.GetData() == rhs.GetData());
}

/**
* The TreeNode class represents the nodes that make up the Tree.
*
* Each node has a pointer to its parent, its first and last child, its previous and next
* sibling, and, of course, to the data it encapsulates.
*/
template<typename DataType>
class TreeNode : public std::enable_shared_from_this<TreeNode<DataType>>
{
public:
   /**
   * @brief TreeNode default constructs a new TreeNode. All outgoing links from this new node will
   * initialized to a nullptr.
   */
   TreeNode() :
      m_parent(nullptr),
      m_firstChild(nullptr),
      m_lastChild(nullptr),
      m_previousSibling(nullptr),
      m_nextSibling(nullptr),
      m_data(),
      m_childCount(0),
      m_visited(false)
   {
   }

   /**
   * @brief TreeNode constructs a new TreeNode encapsulating the specified data. All outgoing links
   * from the node will be initialized to nullptr.
   */
   TreeNode(DataType data) :
      m_parent(nullptr),
      m_firstChild(nullptr),
      m_lastChild(nullptr),
      m_previousSibling(nullptr),
      m_nextSibling(nullptr),
      m_data(data),
      m_childCount(0),
      m_visited(false)
   {
   }

   /**
   * @brief TreeNode performs a shallow copy-construction of the specified TreeNode.
   */
   TreeNode(const TreeNode<DataType>& other) :
      m_parent(other.m_parent),
      m_firstChild(other.m_firstChild),
      m_lastChild(other.m_lastChild),
      m_previousSibling(other.m_previousSibling),
      m_nextSibling(other.m_nextSibling),
      m_data(other.m_data),
      m_childCount(other.m_childCount),
      m_visited(other.m_visited)
   {
   }

   /**
   *
   */
   TreeNode<DataType>& operator=(TreeNode<DataType> other);

   /**
   * @returns The encapsulated data.
   */
   DataType* operator->()
   {
      return &m_data;
   }

   /**
   * @overload
   */
   const DataType* operator->() const
   {
      return &m_data;
   }

   /**
   * @brief MarkVisited sets node visitation status.
   *
   * @param[in] visited             Whether the node should be marked as having been visited.
   */
   inline void MarkVisited(const bool visited = true)
   {
      m_visited = visited;
   }

   /**
   * @returns True if the node has been marked as visited.
   */
   inline bool GetVisited() const
   {
      return m_visited;
   }

   /**
   * @brief PrependChild will prepend the specified TreeNode as the first child of the TreeNode.
   *
   * @param[in] child               The new TreeNode to set as the first child of the TreeNode.
   *
   * @returns A std::shared_ptr to the newly appended child.
   */
   std::shared_ptr<TreeNode<DataType>> PrependChild(std::shared_ptr<TreeNode<DataType>>& child)
   {
      child->m_parent = shared_from_this();

      if (!m_firstChild)
      {
         return AddFirstChild(child);
      }

      assert(m_firstChild);

      m_firstChild->m_previousSibling = child;
      m_firstChild->m_previousSibling->m_nextSibling = m_firstChild;
      m_firstChild = m_firstChild->m_previousSibling;

      m_childCount++;

      return m_firstChild;
   }

   /**
   * @brief PrependChild will construct and prepend a new TreeNode as the first child of the TreeNode.
   *
   * @param[in] data                The underlying data to be stored in the new TreeNode.
   *
   * @returns The newly prepended TreeNode.
   */
   std::shared_ptr<TreeNode<DataType>> PrependChild(DataType& data)
   {
      const auto newNode = std::make_shared<TreeNode<DataType>>(data);
      return PrependChild(newNode);
   }

   /**
   * @overload
   */
   std::shared_ptr<TreeNode<DataType>> PrependChild(const DataType& data)
   {
      auto newNode = std::make_shared<TreeNode<DataType>>(data);
      return AppendChild(newNode);
   }

   /**
   * @overload
   */
   std::shared_ptr<TreeNode<DataType>> PrependChild(DataType&& data)
   {
      auto newNode = std::make_shared<TreeNode<DataType>>(std::forward<DataType>(data));
      return PrependChild(newNode);
   }

   /**
   * @brief AppendChild will append the specified TreeNode as a child of the TreeNode.
   *
   * @param[in] child               The new TreeNode to set as the last child of the TreeNode.
   *
   * @returns A std::shared_ptr to the newly appended child.
   */
   std::shared_ptr<TreeNode<DataType>> AppendChild(std::shared_ptr<TreeNode<DataType>>& child)
   {
      child->m_parent = shared_from_this();

      if (!m_lastChild)
      {
         return AddFirstChild(child);
      }

      assert(m_lastChild);

      m_lastChild->m_nextSibling = child;
      m_lastChild->m_nextSibling->m_previousSibling = m_lastChild;
      m_lastChild = m_lastChild->m_nextSibling;

      m_childCount++;

      return m_lastChild;
   }

   /**
   * @brief AppendChild will construct and append a new TreeNode as the last child of the TreeNode.
   *
   * @param[in] data                The underlying data to be stored in the new TreeNode.
   *
   * @returns The newly appended TreeNode.
   */
   std::shared_ptr<TreeNode<DataType>> AppendChild(DataType& data)
   {
      auto newNode = std::make_shared<TreeNode<DataType>>(data);
      return AppendChild(newNode);
   }

   /**
   * @overload
   */
   std::shared_ptr<TreeNode<DataType>> AppendChild(const DataType& data)
   {
      auto newNode = std::make_shared<TreeNode<DataType>>(data);
      return AppendChild(newNode);
   }

   /**
   * @overload
   */
   std::shared_ptr<TreeNode<DataType>> AppendChild(DataType&& data)
   {
      auto newNode = std::make_shared<TreeNode<DataType>>(std::forward<DataType>(data));
      return AppendChild(newNode);
   }

   /**
   * @returns The underlying data stored in the TreeNode.
   */
   DataType& GetData()
   {
      return m_data;
   }

   /**
   * @overload
   */
   const DataType& GetData() const
   {
      return m_data;
   }

   /**
   * @returns A std::shared_ptr to the TreeNode's parent, if it exists; nullptr otherwise.
   */
   std::shared_ptr<TreeNode<DataType>> GetParent() const
   {
      return m_parent;
   }

   /**
   * @returns A std::shared_ptr to the TreeNode's first child.
   */
   std::shared_ptr<TreeNode<DataType>> GetFirstChild() const
   {
      return m_firstChild;
   }

   /**
   * @returns A std::shared_ptr to the TreeNode's last child.
   */
   std::shared_ptr<TreeNode<DataType>> GetLastChild() const
   {
      return m_lastChild;
   }

   /**
   * @returns A std::shared_ptr to the TreeNode's next sibling.
   */
   std::shared_ptr<TreeNode<DataType>> GetNextSibling() const
   {
      return m_nextSibling;
   }

   /**
   * @returns A std::shared_ptr to the TreeNode's previous sibling.
   */
   std::shared_ptr<TreeNode<DataType>> GetPreviousSibling() const
   {
      return m_previousSibling;
   }

   /**
   * @returns True if this node has direct descendants.
   */
   bool HasChildren() const
   {
      return m_childCount > 0;
   }

   /**
   * @returns The number of direct descendants that this node has.
   *
   * @note This does not include grandchildren.
   */
   unsigned int GetChildCount() const
   {
      return m_childCount;
   }

   /**
   * @returns The total number of descendant nodes belonging to the node.
   */
   unsigned int CountAllDescendants() const
   {
      return Tree<DataType>::Size(*this) - 1;
   }

   /**
   * @brief DetachFromTree detaches the node from the Tree that it is in.
   */
   void DetachFromTree()
   {
      // First, remove all references to this node as parent:
      auto& currentChild = m_firstChild;
      while (currentChild)
      {
         currentChild->m_parent = nullptr;
         currentChild = currentChild->m_nextSibling;
      }

      // Now update all sibling and parent relations:
      if (m_previousSibling && m_nextSibling)
      {
         m_previousSibling->m_nextSibling = m_nextSibling;
         m_nextSibling->m_previousSibling = m_previousSibling;
      }
      else if (m_previousSibling && !m_nextSibling)
      {
         m_previousSibling->m_nextSibling = nullptr;

         if (m_parent)
         {
            m_parent->m_lastChild = m_previousSibling;
         }
      }
      else if (m_nextSibling && !m_previousSibling)
      {
         m_nextSibling->m_previousSibling = nullptr;

         if (m_parent)
         {
            m_parent->m_firstChild = m_nextSibling;
         }
      }

      // Clear references to anything below this node:
      m_firstChild = nullptr;
      m_lastChild = nullptr;

      if (m_parent)
      {
         m_parent->m_childCount--;
      }
   }

   /**
   * @brief SortChildren performs a merge sort of the direct descendants nodes.
   *
   * @param[in] comparator          The function to be used as the basis for the sorting comparison.
   */
   void SortChildren(const std::function<bool(TreeNode<DataType>, TreeNode<DataType>)>& comparator)
   {
      MergeSort(m_firstChild, comparator);
   }

private:

   /**
   * @brief MergeSort is the main entry point into the merge sort implementation.
   *
   * @param[in] list                The first TreeNode in the list to be sorted.
   * @param[in] comparator          The comparator function to be called to figure out which node
   *                                is the lesser of the two.
   */
   void MergeSort(
      std::shared_ptr<TreeNode<DataType>>& list,
      const std::function<bool(TreeNode<DataType>, TreeNode<DataType>)>& comparator)
   {
      if (!list || !list->m_nextSibling)
      {
         return;
      }

      std::shared_ptr<TreeNode<DataType>> head = list;
      std::shared_ptr<TreeNode<DataType>> lhs = nullptr;
      std::shared_ptr<TreeNode<DataType>> rhs = nullptr;

      DivideList(head, lhs, rhs);

      assert(lhs);
      assert(rhs);

      MergeSort(lhs, comparator);
      MergeSort(rhs, comparator);

      list = MergeSortedHalves(lhs, rhs, comparator);
   }

   /**
   * @brief DivideList is a helper function that will divide the specified TreeNode list in two.
   *
   * @param[in] head                The head of the TreeNode list to be divided in two.
   * @param[out] lhs                The first TreeNode of the left hand side list.
   * @param[out] rhs                The first TreeNode of the right hand side list.
   */
   void DivideList(
      std::shared_ptr<TreeNode<DataType>> head,
      std::shared_ptr<TreeNode<DataType>>& lhs,
      std::shared_ptr<TreeNode<DataType>>& rhs)
   {
      if (!head || !head->m_nextSibling)
      {
         return;
      }

      std::shared_ptr<TreeNode<DataType>> tortoise = head;
      std::shared_ptr<TreeNode<DataType>> hare = head->m_nextSibling;

      while (hare)
      {
         hare = hare->m_nextSibling;
         if (hare)
         {
            tortoise = tortoise->m_nextSibling;
            hare = hare->m_nextSibling;
         }
      }

      lhs = head;
      rhs = tortoise->m_nextSibling;

      tortoise->m_nextSibling = nullptr;
   }

   /**
   * @brief MergeSortedHalves is a helper function that will merge the sorted halves.
   *
   * @param[in] lhs                 The first node of the sorted left half.
   * @param[in] rhs                 The first node of the sorted right half.
   *
   * @returns The first node of the merged TreeNode list.
   */
   std::shared_ptr<TreeNode<DataType>> MergeSortedHalves(
      std::shared_ptr<TreeNode<DataType>>& lhs,
      std::shared_ptr<TreeNode<DataType>>& rhs,
      const std::function<bool(TreeNode<DataType>, TreeNode<DataType>)>& comparator)
   {
      std::shared_ptr<TreeNode<DataType>> result = nullptr;
      if (comparator(*lhs.get(), *rhs.get()))
      {
         result = std::shared_ptr<TreeNode<DataType>>(lhs);
         lhs = lhs->m_nextSibling;
      }
      else
      {
         result = std::shared_ptr<TreeNode<DataType>>(rhs);
         rhs = rhs->m_nextSibling;
      }

      std::shared_ptr<TreeNode<DataType>> tail = result;

      while (lhs && rhs)
      {
         if (comparator(*lhs.get(), *rhs.get()))
         {
            tail->m_nextSibling = std::shared_ptr<TreeNode<DataType>>(lhs);
            tail = tail->m_nextSibling;

            lhs = lhs->m_nextSibling;

            if (lhs)
            {
               lhs->m_previousSibling = nullptr;
            }
         }
         else
         {
            tail->m_nextSibling = std::shared_ptr<TreeNode<DataType>>(rhs);
            tail = tail->m_nextSibling;

            rhs = rhs->m_nextSibling;

            if (rhs)
            {
               rhs->m_previousSibling = nullptr;
            }
         }
      }

      while (lhs)
      {
         tail->m_nextSibling = std::shared_ptr<TreeNode<DataType>>(lhs);
         tail = tail->m_nextSibling;

         lhs = lhs->m_nextSibling;

         if (lhs)
         {
            lhs->m_previousSibling = nullptr;
         }
      }

      while (rhs)
      {
         tail->m_nextSibling = std::shared_ptr<TreeNode<DataType>>(rhs);
         tail = tail->m_nextSibling;

         rhs = rhs->m_nextSibling;

         if (rhs)
         {
            rhs->m_previousSibling = nullptr;
         }
      }

      return result;
   }

   /**
   * @brief AddFirstChild is a helper function to make it easier to add the first descendant.
   *
   * @param[in] child               The TreeNode to be added as a child.
   *
   * @returns The newly added node.
   */
   std::shared_ptr<TreeNode<DataType>> AddFirstChild(std::shared_ptr<TreeNode<DataType>>& child)
   {
      assert(m_childCount == 0);

      m_firstChild = child;
      m_lastChild = m_firstChild;

      m_childCount++;

      return m_firstChild;
   }

   std::shared_ptr<TreeNode<DataType>> m_parent;
   std::shared_ptr<TreeNode<DataType>> m_firstChild;
   std::shared_ptr<TreeNode<DataType>> m_lastChild;
   std::shared_ptr<TreeNode<DataType>> m_previousSibling;
   std::shared_ptr<TreeNode<DataType>> m_nextSibling;

   DataType m_data;

   unsigned int m_childCount;

   bool m_visited;
};

/***************************************************************************************************
* Start of TreeNode<T> Class Definitions
**************************************************************************************************/

template<typename DataType>
TreeNode<DataType>& TreeNode<DataType>::operator=(TreeNode<DataType> other)
{
   std::swap(this->m_childCount, other.m_childCount);
   std::swap(this->m_data, other.m_data);
   std::swap(this->m_firstChild, other.m_firstChild);
   std::swap(this->m_lastChild, other.m_lastChild);
   std::swap(this->m_nextSibling, other.m_nextSibling);
   std::swap(this->m_parent, other.m_parent);
   std::swap(this->m_previousSibling, other.m_previousSibling);
   std::swap(this->m_visited, other.m_visited);

   return *this;
}

/**
* The Tree class declares a basic n-ary tree, built on top of templatized TreeNode nodes.
*
* Each tree consists of a simple head TreeNode and nothing else.
*/
template<typename DataType>
class Tree
{
public:
   class Iterator;
   class SiblingIterator;
   class PreOrderIterator;
   class PostOrderIterator;
   class LeafIterator;

   // Typedefs needed for STL compliance:
   typedef TreeNode<DataType>                value_type;
   typedef TreeNode<DataType>&               reference;
   typedef const TreeNode<DataType>&         const_reference;

   /**
   * @brief Tree constructs a new Tree with the provided data encapsulated in a new
   * TreeNode.
   */
   explicit Tree(DataType data) :
      m_head(new TreeNode<DataType>(data))
   {
   }

   /**
   * @brief Tree copy-constructs a shallow-copy of the specified Tree.
   */
   explicit Tree(const Tree<DataType>& other) :
      m_head(other.m_head)
   {
   }

   /**
   * @returns A std::shared_ptr to the head TreeNode.
   */
   std::shared_ptr<TreeNode<DataType>> GetHead() const
   {
      return m_head;
   }

   /**
   * @returns The total number of leaf nodes in the Tree.
   */
   unsigned int CountLeafNodes() const
   {
      unsigned int count = 0;
      for (auto itr = beginLeaf(); itr != endLeaf(); ++itr)
      {
         count++;
      }

      return count;
   }

   /**
   * @brief Computes the size of nodes in the Tree.
   *
   * @complexity Linear in the size of the sub-tree.
   *
   * @returns The total number of nodes in the Tree (both leaf and non-leaf).
   */
   size_t Size() const
   {
      return std::count_if(std::begin(*this), std::end(*this),
         [] (const_reference) { return true; });
   }

   /**
   * @brief Computes the size of the sub-tree that starts at the specified TreeNode.
   *
   * @complexity Linear in the size of the sub-tree.
   *
   * @param[in] node                The node from which to compute the size of the sub-tree.
   *
   * @returns The total number of nodes (both leaf and branching) in the tree, starting at the
   * passed in node.
   */
   size_t Size(const TreeNode<DataType>& node)
   {
      unsigned int count = 0;

      auto itr = PostOrderIterator(std::make_shared<TreeNode<DataType>>(node));
      for (++itr; itr && &*itr != &node; ++itr)
      {
         count++;
      }

      return count;
   }

   /**
   * @returns The zero-indexed depth of the TreeNode in its Tree.
   */
   static unsigned int Depth(TreeNode<DataType> node)
   {
      unsigned int depth = 0;

      TreeNode<DataType>* nodePtr = &node;
      while (nodePtr->GetParent())
      {
         ++depth;
         nodePtr = nodePtr->GetParent().get();
      }

      return depth;
   }

   /**
   * @brief Print outputs the contents of the Tree starting and ending at the specified TreeNode,
   * using a pre-order traversal.
   *
   * @param[in] node                The starting node.
   * @param[in] printer             A function (or lambda) that returns the std::wstrig to be
   *                                printed.
   */
   static void Print(TreeNode<DataType>& node, std::function<std::wstring (const DataType&)> printer)
   {
      auto itr = Tree<DataType>::PreOrderIterator(std::make_shared<TreeNode<DataType>>(node));
      while (&*itr != &node)
      {
         if (&*itr == nullptr)
         {
            break;
         }

         const auto depth = Tree<DataType>::Depth(*itr);
         const auto tabSize = 2;
         const std::wstring padding((depth * tabSize), ' ');

         std::wcout << padding << printer(itr->GetData()) << std::endl;

         ++itr;
      }
   }

   /**
   * @brief beginSibling Creates a sibling iterator starting at the specified node.
   *
   * @param node                The TreeNode at which to start iteration.
   *
   * @returns A sibling iterator that advances over the siblings of the node.
   */
   typename Tree::SiblingIterator beginSibling(const std::shared_ptr<TreeNode<DataType>> node) const
   {
      const auto iterator = Tree<DataType>::SiblingIterator(node);
      return iterator;
   }

   /**
   * @brief endSibling Creates a sibling iterator that points past the last sibling to iterate
   * over.
   *
   * @param[in] node                Any node that is a sibling of the target range.
   * 
   * @returns A sibling iterator pointing past the last sibling.
   */
   typename Tree::SiblingIterator endSibling(const std::shared_ptr<TreeNode<DataType>> node) const
   {
      Tree<DataType>::SiblingIterator siblingIterator(nullptr);
      siblingIterator.m_parent = node->GetParent();

      return siblingIterator;
   }

   /**
   * @returns A pre-order iterator that will iterate over all TreeNodes in the tree.
   */
   typename Tree::PreOrderIterator beginPreOrder() const
   {
      const auto iterator = Tree<DataType>::PreOrderIterator(m_head);
      return iterator;
   }

   /**
   * @returns A pre-order iterator pointing "past" the end of the tree.
   */
   typename Tree::PreOrderIterator endPreOrder() const
   {
      const auto iterator = Tree<DataType>::PreOrderIterator(nullptr);
      return iterator;
   }

   /**
   * @returns A post-order iterator that will iterator over all nodes in the tree, starting
   * with the head of the Tree.
   */
   typename Tree::PostOrderIterator begin() const
   {
      auto iterator = Tree<DataType>::PostOrderIterator(m_head);
      return ++iterator;
   }

   /**
   * @returns A post-order iterator that points past the end of the Tree.
   */
   typename Tree::PostOrderIterator end() const
   {
      const auto iterator = Tree<DataType>::PostOrderIterator(nullptr);
      return iterator;
   }

   /**
   * @returns An iterator that will iterator over all leaf nodes in the Tree, starting with the
   * left-most leaf in the Tree.
   */
   typename Tree::LeafIterator beginLeaf() const
   {
      auto iterator = Tree<DataType>::LeafIterator(m_head);
      return ++iterator;
   }

   /**
   * @return A LeafIterator that points past the last leaf TreeNode in the Tree.
   */
   typename Tree::LeafIterator endLeaf() const
   {
      const auto iterator = Tree<DataType>::LeafIterator(nullptr);
      return iterator;
   }

private:
   std::shared_ptr<TreeNode<DataType>> m_head;
};

/**
* @brief The Iterator class
*
* This is the base iterator class that all other iterators (sibling, post-, pre-, in-order)
* will derive from.
*/
template<typename DataType>
class Tree<DataType>::Iterator
{
public:
   friend class Tree<DataType>;

   // Typedefs needed for STL compliance:
   typedef DataType                             value_type;
   typedef DataType*                            pointer;
   typedef DataType&                            reference;
   typedef const DataType&                      const_reference;
   typedef std::size_t                          size_type;
   typedef std::ptrdiff_t                       difference_type;
   typedef std::bidirectional_iterator_tag      iterator_category;

   /**
   * @returns True if the Tree::Iterator points to a valid TreeNode; false otherwise.
   */
   explicit operator bool() const
   {
      const bool isValid = (m_currentNode != nullptr);
      return isValid;
   }

   /**
   * @returns The TreeNode pointed to by the Tree::Iterator.
   */
   TreeNode<DataType>& operator*()
   {
      return *m_currentNode;
   }

   /**
   * @overload
   */
   const TreeNode<DataType>& operator*() const
   {
      return *m_currentNode;
   }

   /**
   * @returns A pointer to the TreeNode.
   */
   TreeNode<DataType>* operator&()
   {
      return m_currentNode.get();
   }

   /**
   * @overload
   */
   const TreeNode<DataType>* operator&() const
   {
      return m_currentNode.get();
   }

   /**
   * @returns A pointer to the TreeNode pointed to by the Tree:Iterator.
   */
   TreeNode<DataType>* operator->()
   {
      return m_currentNode.get();
   }

   /**
   * @overload
   */
   const TreeNode<DataType>* operator->() const
   {
      return m_currentNode.get();
   }

   /**
   * @returns True if the Iterator points to the same node as the other Iterator.
   */
   bool operator==(const Iterator& other) const
   {
      return m_currentNode == other.m_currentNode;
   }

   /**
   * @returns True if the Iterator points to the same node as the other Iterator.
   */
   bool operator!=(const Iterator& other) const
   {
      return m_currentNode != other.m_currentNode;
   }

protected:

   /**
   * Default constructor.
   */
   explicit Iterator() :
      m_currentNode(nullptr),
      m_startingNode(nullptr),
      m_endingNode(nullptr)
   {
   }
   
   /**
   * Copy constructor.
   */
   explicit Iterator(const Iterator& other) :
      m_currentNode(other.m_currentNode),
      m_startingNode(other.m_startingNode),
      m_endingNode(other.m_endingNode)
   {
   }

   explicit Iterator(std::shared_ptr<TreeNode<DataType>> node) :
      m_currentNode(node),
      m_startingNode(node),
      m_endingNode(nullptr)
   {
   }

   std::shared_ptr<TreeNode<DataType>> m_currentNode;
   std::shared_ptr<TreeNode<DataType>> m_startingNode;
   std::shared_ptr<TreeNode<DataType>> m_endingNode;
};

/**
* @brief The SiblingIterator class
*/
template<typename DataType>
class Tree<DataType>::SiblingIterator : public Tree<DataType>::Iterator
{
public:
   /**
   * Default constructor.
   */
   explicit SiblingIterator() :
      Iterator()
   {
   }

   /**
   * Copy constructor.
   */
   explicit SiblingIterator(const Iterator& other) :
      Iterator(other)
   {
   }

   /**
   *
   */
   explicit SiblingIterator(std::shared_ptr<TreeNode<DataType>> node) :
      Iterator(node)
   {
   }

   /**
   * Prefix increment operator.
   */
   typename Tree::SiblingIterator& operator++()
   {
      if (m_currentNode)
      {
         m_currentNode = m_currentNode->GetNextSibling();
      }

      return *this;
   }

   /**
   * Postfix increment operator.
   */
   typename Tree::SiblingIterator operator++(int)
   {
      const auto result = *this;
      ++(*this);

      return result;
   }

   /**
   * Prefix decrement operator.
   */
   typename Tree::SiblingIterator& operator--()
   {
      if (!m_currentNode)
      {
         if (m_parent)
         {
            m_currentNode = m_parent->GetLastChild();
         }
         else
         {
            // If no parent exists, then the node in question must be the one "past" the head,
            // so decrementing from there should return the head node:
            m_currentNode = m_startingNode;
         }
      }
      else
      {
         m_currentNode = m_currentNode->m_previousSibling;
      }

      return *this;
   }

   /**
   * Postfix decrement operator.
   */
   typename Tree::SiblingIterator operator--(int)
   {
      const auto result = *this;
      --(*this);

      return result;
   }

private:
   std::shared_ptr<TreeNode<DataType>> m_parent;
};

/**
* @brief The PreOrderIterator class
*/
template<typename DataType>
class Tree<DataType>::PreOrderIterator : public Tree<DataType>::Iterator
{
public:
   /**
   * Default constructor.
   */
   explicit PreOrderIterator() :
      Iterator()
   {
   }

   /**
   * Copy constructor.
   */
   explicit PreOrderIterator(const Iterator& other) :
      Iterator(other)
   {
   }

   /**
   * Constructs an iterator that starts and ends at the specified node.
   */
   explicit PreOrderIterator(std::shared_ptr<TreeNode<DataType>> node) :
      Iterator(node)
   {
      m_endingNode = (node && node->GetNextSibling())
         ? node->GetNextSibling()
         : nullptr;
   }

   /**
   * Prefix increment operator.
   */
   typename Tree::PreOrderIterator& operator++()
   {
      assert(m_currentNode);
      auto traversingNode = m_currentNode;

      if (traversingNode->HasChildren())
      {
         traversingNode = traversingNode->GetFirstChild();
      }
      else if (traversingNode->GetNextSibling())
      {
         traversingNode = traversingNode->GetNextSibling();
      }
      else
      {
         while (traversingNode->GetParent() && !traversingNode->GetParent()->GetNextSibling())
         {
            traversingNode = traversingNode->GetParent();
         }

         if (traversingNode->GetParent())
         {
            traversingNode = traversingNode->GetParent()->GetNextSibling();
         }
         else
         {
            traversingNode = nullptr;
         }
      }

      m_currentNode = (traversingNode != m_endingNode) ? traversingNode : nullptr;
      return *this;
   }

   /**
   * Postfix increment operator.
   */
   typename Tree::PreOrderIterator operator++(int)
   {
      const auto result = *this;
      ++(*this);

      return result;
   }

   /**
   * Prefix decrement operator.
   */
   typename Tree::PreOrderIterator operator--()
   {
      auto traversingNode = m_currentNode;

      if (!traversingNode)
      {
         traversingNode = m_startingNode;

         while (traversingNode->GetLastChild())
         {
            traversingNode = traversingNode->GetLastChild();
         }
      }
      else if (traversingNode->GetPreviousSibling())
      {
         traversingNode = traversingNode->GetPreviousSibling();

         while (traversingNode->GetLastChild())
         {
            traversingNode = traversingNode->GetLastChild();
         }
      }
      else if (traversingNode->GetParent())
      {
         traversingNode = traversingNode->GetParent();
      }
      else
      {
         traversingNode = nullptr;
      }

      // @todo Check against ending node.

      m_currentNode = traversingNode;
      return *this;
   }

   /**
   * Postfix decrement operator.
   */
   typename Tree::PreOrderIterator& operator--(int)
   {
      const auto result = *this;
      --(*this);

      return result;
   }
};

/**
* @brief The PostOrderIterator class
*/
template<typename DataType>
class Tree<DataType>::PostOrderIterator : public Tree<DataType>::Iterator
{
public:
   /**
   * Default constructor.
   */
   explicit PostOrderIterator() :
      Iterator(),
      m_traversingUpTheTree(false)
   {
   }

   /**
   * Copy constructor.
   */
   explicit PostOrderIterator(const Iterator& other) :
      Iterator(other),
      m_traversingUpTheTree(false)
   {
   }

   /**
   *
   */
   explicit PostOrderIterator(std::shared_ptr<TreeNode<DataType>> node) :
      Iterator(node),
      m_traversingUpTheTree(false)
   {
   }

   /**
   * Prefix increment operator.
   */
   typename Tree::PostOrderIterator& operator++()
   {
      assert(m_currentNode);
      auto traversingNode = m_currentNode;

      if (traversingNode->HasChildren() && !m_traversingUpTheTree)
      {
         while (traversingNode->GetFirstChild())
         {
            traversingNode = traversingNode->GetFirstChild();
         }
      }
      else if (traversingNode->GetNextSibling())
      {
         m_traversingUpTheTree = false;

         traversingNode = traversingNode->GetNextSibling();
         while (traversingNode->HasChildren())
         {
            traversingNode = traversingNode->GetFirstChild();
         }
      }
      else
      {
         m_traversingUpTheTree = true;

         traversingNode = traversingNode->GetParent();
      }

      m_currentNode = traversingNode;
      return *this;
   }

   /**
   * Postfix increment operator.
   */
   typename Tree::PostOrderIterator operator++(int)
   {
      const auto result = *this;
      ++(*this);

      return result;
   }

   /**
   * Prefix decrement operator.
   */
   typename Tree::PostOrderIterator& operator--()
   {
      auto traversingNode = m_currentNode;

      // When the iterator is at the end(), then the next position should be the head:
      if (!traversingNode)
      {
         assert(traversingNode);

         traversingNode = m_startingNode;
      }
      else if (traversingNode->HasChildren())
      {
         traversingNode = traversingNode->GetLastChild();
      }
      else if (traversingNode->GetPreviousSibling())
      {
         traversingNode = traversingNode->GetPreviousSibling();
      }
      else if (traversingNode->GetParent())
      {
         while (traversingNode->GetParent() && !traversingNode->GetParent()->GetPreviousSibling())
         {
            traversingNode = traversingNode->GetParent();
         }

         traversingNode = traversingNode->GetParent();

         if (traversingNode)
         {
            traversingNode = traversingNode->GetPreviousSibling();
         }
      }

      m_currentNode = traversingNode;
      return *this;
   }

   /**
   * Postfix decrement operator.
   */
   typename Tree::PostOrderIterator operator--(int)
   {
      const auto result = *this;
      --(*this);

      return result;
   }

private:
   bool m_traversingUpTheTree;
};

/**
* @brief The LeafIterator class
*/
template<typename DataType>
class Tree<DataType>::LeafIterator : public Tree<DataType>::Iterator
{
public:
   /**
   * Default constructor.
   */
   explicit LeafIterator() :
      Iterator()
   {
   }

   /**
   * Copy constructor.
   */
   explicit LeafIterator(const Iterator& other) :
      Iterator(other)
   {
   }

   /**
   *
   */
   explicit LeafIterator(std::shared_ptr<TreeNode<DataType>> node) :
      Iterator(node)
   {
   }

   /**
   * Prefix increment operator.
   */
   typename Tree::LeafIterator& operator++()
   {
      assert(m_currentNode);
      auto traversingNode = m_currentNode;

      if (traversingNode->HasChildren())
      {
         while (traversingNode->GetFirstChild())
         {
            traversingNode = traversingNode->GetFirstChild();
         }
      }
      else if (traversingNode->GetNextSibling())
      {
         traversingNode = traversingNode->GetNextSibling();

         while (traversingNode->GetFirstChild())
         {
            traversingNode = traversingNode->GetFirstChild();
         }
      }
      else if (traversingNode->GetParent())
      {
         while (traversingNode->GetParent() && !traversingNode->GetParent()->GetNextSibling())
         {
            traversingNode = traversingNode->GetParent();
         }

         if (traversingNode->GetParent())
         {
            traversingNode = traversingNode->GetParent()->GetNextSibling();

            while (traversingNode->HasChildren())
            {
               traversingNode = m_currentNode->GetFirstChild();
            }

            return *this;
         }

         // Otherwise, the traversal is at the end:
         traversingNode = nullptr;
      }

      m_currentNode = traversingNode;
      return *this;
   }

   /**
   * Postfix increment operator.
   */
   typename Tree::LeafIterator operator++(int)
   {
      const auto result = *this;
      ++(*this);

      return result;
   }

   /**
   * Prefix decrement operator.
   */
   typename Tree::LeafIterator& operator--()
   {
      assert(m_currentNode);
      auto traversingNode = m_currentNode;

      if (!traversingNode)
      {
         traversingNode = m_startingNode;
      }

      if (traversingNode->HasChildren())
      {
         while (traversingNode->GetLastChild())
         {
            traversingNode = traversingNode->GetLastChild();
         }
      }
      else if (traversingNode->GetPreviousSibling())
      {
         traversingNode = traversingNode->GetPreviousSibling();

         while (traversingNode->GetLastChild())
         {
            traversingNode->GetLastChild();
         }
      }
      else if (traversingNode->GetParent())
      {
         while (traversingNode->GetParent() && !traversingNode->GetParent()->GetPreviousSibling())
         {
            traversingNode = traversingNode->GetParent();
         }

         if (traversingNode->GetParent())
         {
            traversingNode = traversingNode->GetParent()->GetPreviousSibling();

            while (traversingNode->HasChildren())
            {
               traversingNode = traversingNode->GetLastChild();
            }

            return *this;
         }

         // Otherwise, the traversal is at the end:
         traversingNode = nullptr;
      }

      m_currentNode = traversingNode;
      return *this;
   }

   /**
   * Postfix decrement operator.
   */
   typename Tree::LeafIterator operator--(int)
   {
      const auto result = *this;
      --(*this);

      return result;
   }
};
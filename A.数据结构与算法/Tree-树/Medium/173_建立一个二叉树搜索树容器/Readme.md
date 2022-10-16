题目地址：https://leetcode.com/problems/binary-search-tree-iterator/  
题目描述：   
Implement an iterator over a binary search tree (BST). Your iterator will be initialized with the root node of a BST.  
  
Calling next() will return the next smallest number in the BST.  
  
Note: next() and hasNext() should run in average O(1) time and uses O(h) memory, where h is the height of the tree.  
  
思路：  
C++:  
1. 用一个栈保存一部分节点。栈的顶部是最小的节点（树的最左下的节点就是最小节点）  
2. hasNext()就是判断栈是否为空，如果空则返回0（没有下一个节点）.  
3. next()取栈定那个。判断栈顶节点pNode是right是否为空，不为空则把right子树的左节点全部压栈。[根据BST的特性，始终把最小元素放栈顶。]  

c语言：  
1. 基本思路和C++一样， 不同的自己写的这个方法用了链表当栈。「利用前插法，可以保证最小元素在前面」  
2. 注意要用一个固定的头节点。  
也就是:不能head = newNode;  
      要head->next = newNode;   

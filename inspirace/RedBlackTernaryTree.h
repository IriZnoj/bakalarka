/* 
 * File:   RedBlackTernaryTree.h
 * Author: Daniel Robenek
 *
 * Created on 22.01.2013
 * Red Black ternary search tree
 * Inspired by: http://en.literateprograms.org/Red-black_tree_(C)
 */

#ifndef REDBLACKTREE_H
#define	REDBLACKTREE_H

//#define ASSERT_RBT(predicate, message, args...) if(!(predicate)) {printf("ASSERT: "); printf(message, ## args); printf("\n"); throw "";}
#define ASSERT_RBT(predicate, message, args...) 

#ifdef TST_POINTER_EQUAL
#define TST_POINTER_EQUAL_P
#else
#define TST_POINTER_EQUAL_P	*
#endif

#include "cHugeArray.h"
#include <stdio.h>
#include <climits>

#define RBT_RED_COLOR 0
#define RBT_BLACK_COLOR 1
#define RBT_UNINITIALIZED_INDEX UINT_MAX


template <class T>
class RedBlackTernaryTree {
protected:
	typedef unsigned char Color;
	typedef unsigned int NodeIndex;
	
	struct Node {
	public:
		Node() { color = 0U; middle = left = right = UNINITIALIZED_INDEX; }
		Node(const Node & node) { key = node.key; color = node.color; middle = node.middle; left = node.left; right = node.right; key = node.key; }
		Color color; // 1st bit is color, 2nd bit is IsFinal node
		T key;
		NodeIndex middle;
		NodeIndex left;
		NodeIndex right;
	};
	
#ifdef COMPRESS_RBTST
	struct Leaf {
	public:
		Leaf() { color = 0U; middle = UNINITIALIZED_INDEX; }
		Leaf(const Node & node) { key = node.key; color = node.color; middle = node.middle; }
		Color color; // 1st bit is color, 2nd bit is IsFinal node
		T key;
		NodeIndex middle;
	};
#endif
	public:
	arg::cHugeArray<Node> nodes;
#ifdef COMPRESS_RBTST
	arg::cHugeArray<Node> findNodes;
	arg::cHugeArray<Leaf> leafs;
	NodeIndex leafBase;
#endif
	
	NodeIndex parents[33]; // 2 * log(n + 1)
	Node newNodeTemp;
	NodeIndex rootIndex;
	
	inline Node * GetNode(const NodeIndex index) { ASSERT_RBT(index == UNINITIALIZED_INDEX || nodes.Count() > index, "Invalid index %u", index) return (index == UNINITIALIZED_INDEX) ? NULL : &nodes[index]; }
	
#ifdef COMPRESS_RBTST
#define GetFindNode(index) ((index < leafBase) ? (&findNodes[index]) : (index == UNINITIALIZED_INDEX ? NULL : (Node*)&leafs[index - leafBase]))
#else
#define GetFindNode(index) ((index == UNINITIALIZED_INDEX) ? NULL : &nodes[index])
#endif
	
	
	inline Node * GetParent(const unsigned int currentStackIndex) { return GetNode(GetParentIndex(currentStackIndex)); }
	inline const NodeIndex GetParentIndex(const unsigned int currentStackIndex) const { return currentStackIndex > 0 ? parents[currentStackIndex - 1] : RBT_UNINITIALIZED_INDEX; }
	inline Node * GetGrandParent(const unsigned int currentStackIndex) { return GetNode(GetGrandParentIndex(currentStackIndex)); }
	inline const NodeIndex GetGrandParentIndex(const unsigned int currentStackIndex) const { return currentStackIndex > 1 ? parents[currentStackIndex - 2] : RBT_UNINITIALIZED_INDEX; }
	inline Node * GetSibling(const unsigned int currentStackIndex, const NodeIndex currentNodeIndex) { const NodeIndex index = GetSiblingIndex(currentStackIndex, currentNodeIndex); return (index == UNINITIALIZED_INDEX) ? NULL : GetNode(index); }
	inline const NodeIndex GetSiblingIndex(const unsigned int currentStackIndex, const NodeIndex currentNodeIndex) { const Node * parent = GetParent(currentStackIndex); NodeIndex index = ((parent->left == currentNodeIndex) ? parent->right : parent->left); return index; }
	inline Node * GetUncle(const unsigned int currentStackIndex, const NodeIndex currentNodeIndex) { return GetSibling(currentStackIndex - 1, GetParentIndex(currentStackIndex)); }
	inline const NodeIndex GetUncleIndex(const unsigned int currentStackIndex, const NodeIndex currentNodeIndex) { return GetSiblingIndex(currentStackIndex - 1, GetParentIndex(currentStackIndex)); }
	
	inline const Color GetColor(const NodeIndex nodeIndex) { return (nodeIndex == UNINITIALIZED_INDEX) ? RBT_BLACK_COLOR : GetNode(nodeIndex)->color & 0x1U; };
	inline const Color GetColor(const Node * node) { return (node == NULL) ? RBT_BLACK_COLOR : node->color & 0x1U; };
	inline void SetColor(NodeIndex nodeIndex, Color color) { ASSERT_RBT(nodeIndex != UNINITIALIZED_INDEX, "Cant set color of leaf") Node * node = GetNode(nodeIndex); node->color = (node->color & 0xFEU) | color; };
	inline void SetColor(Node * node, Color color) { ASSERT_RBT(node != NULL, "Cant set color of leaf") node->color = (node->color & 0xFEU) | color; };
	
	inline const bool IsFinal(const Node * node) { ASSERT_RBT(node != NULL, "Cant IsFinal on NULL") return (node->color & 0x2U) > 0x0U; };
	inline void SetFinal(Node * node, bool isFinal) { ASSERT_RBT(node != NULL, "Cant set final on NULL") node->color = (node->color & 0xFDU) | (isFinal ? 0x2U : 0x0U); };
	
#ifdef COMPRESS_RBTST
	inline const bool IsLeaf(const Node * node) { ASSERT_RBT(node != NULL, "Cant IsLeaf on NULL") return (node->color & 0x4U) > 0x0U; };
	inline const bool IsLeaf(const NodeIndex index) { return index >= leafBase; }
	inline void SetLeaf(Node * node, bool isLeaf) { ASSERT_RBT(node != NULL, "Cant set leaf on NULL") node->color = (node->color & 0xFBU) | (isLeaf ? 0x4U : 0x0U); };
#endif
	
	bool IsProperlyColored(const NodeIndex nodeIndex) { const Node * node = GetNode(nodeIndex); if(GetColor(node) != RED && GetColor(node) != BLACK) { ASSERT_RBT(false, "Node %c is not properly colored!", node->key) return false; } if(node == NULL) return true; return IsProperlyColored(node->left) && IsProperlyColored(node->right); }
	bool IsRootBlack(const NodeIndex rootIndex) { if(GetColor(rootIndex) != BLACK) { ASSERT_RBT(false, "Root node is not black!") return false; } else return true; }
	bool HasRedNodesOnlyBlackChildrens(const NodeIndex nodeIndex) { const Node * node = GetNode(nodeIndex); if(node == NULL) return true; if(GetColor(node) == RED) { if(GetColor(node->left) != BLACK || GetColor(node->right) != BLACK) { ASSERT_RBT(false, "Red node %c has red children!", node->key) return false;} }; return HasRedNodesOnlyBlackChildrens(node->left) && HasRedNodesOnlyBlackChildrens(node->right); }
	bool HasAllPathsSameBlackNodeCount(const NodeIndex rootIndex) { int pathBlackCount = -1; return HasAllPathsSameBlackNodeCount(rootIndex, 0, &pathBlackCount); }
	bool HasAllPathsSameBlackNodeCount(const NodeIndex nodeIndex, int blackCount, int * pathBlackCount) { const Node * node = GetNode(nodeIndex); if(GetColor(node) == BLACK) blackCount++; if(node == NULL) { if(*pathBlackCount == -1) { *pathBlackCount = blackCount; return true; } else { if(*pathBlackCount == blackCount) return true; else { ASSERT_RBT(false, "Different node height %d x %d !", blackCount, *pathBlackCount) } } } return HasAllPathsSameBlackNodeCount(node->left, blackCount, pathBlackCount) && HasAllPathsSameBlackNodeCount(node->right, blackCount, pathBlackCount); }
	

public:
	static const Color RED = RBT_RED_COLOR;
	static const Color BLACK = RBT_BLACK_COLOR;
	static const NodeIndex UNINITIALIZED_INDEX = RBT_UNINITIALIZED_INDEX;
	
	bool IsRedBlackTree(NodeIndex root = UNINITIALIZED_INDEX) { if(root == UNINITIALIZED_INDEX) root = rootIndex; return IsProperlyColored(root) && IsRootBlack(root) && HasRedNodesOnlyBlackChildrens(root) && HasAllPathsSameBlackNodeCount(root); }
	
	void PrintSize() {
#ifdef COMPRESS_RBTST
		printf("Nodes: sizeof: %d, count: %d, size: %d\n", sizeof(Node), findNodes.Count(), sizeof(Node) * findNodes.Count());
		printf("Leaf: sizeof: %d, count: %d, size: %d\n", sizeof(Leaf), leafs.Count(), sizeof(Leaf) * leafs.Count());
#else
		printf("Nodes: sizeof: %d, count: %d, size: %d\n", sizeof(Node), nodes.Count(), sizeof(Node) * nodes.Count());
#endif		
	}
	
	RedBlackTernaryTree() { 
		rootIndex = UNINITIALIZED_INDEX;
#ifdef COMPRESS_RBTST
		leafBase = UINT_MAX / 2;
#endif
	};
	
	NodeIndex Find(const T item[], const T deliminer[], NodeIndex rootIndex) {
		if ((TST_POINTER_EQUAL_P item) == (TST_POINTER_EQUAL_P deliminer) || rootIndex == UNINITIALIZED_INDEX) {
			return 0;
		}
		Node * current = GetFindNode(rootIndex);
		unsigned int currentIndex = rootIndex;
		// loop through item parts
		while ((TST_POINTER_EQUAL_P item) != (TST_POINTER_EQUAL_P deliminer)) {
			// find node in nth row
			while (current->key != (*item)) {
				if (current->key < (*item)) {
					if (current->right != UNINITIALIZED_INDEX) {
						currentIndex = current->right;
#ifdef COMPRESS_RBTST
						if(currentIndex < leafBase) {
							current = &findNodes[currentIndex];
						} else {
							current = (Node*)&leafs[currentIndex - leafBase];
							if(current->key == (*item)) {
								break;
							}
							return UNINITIALIZED_INDEX;
						}
#else
						current = GetFindNode(currentIndex);
#endif
					} else {
						return UNINITIALIZED_INDEX; // not found
					}
				} else if (current->key > (*item)) {
					if (current->left != UNINITIALIZED_INDEX) {
						currentIndex = current->left;
#ifdef COMPRESS_RBTST
						if(currentIndex < leafBase) {
							current = &findNodes[currentIndex];
						} else {
							current = (Node*)&leafs[currentIndex - leafBase];
							if(current->key == (*item)) {
								break;
							}
							return UNINITIALIZED_INDEX;
						}
#else
						current = GetFindNode(currentIndex);
#endif
					} else {
						return UNINITIALIZED_INDEX; // not found
					}
				}
			}

			// next item part
			item++;

			if ((TST_POINTER_EQUAL_P item) != (TST_POINTER_EQUAL_P deliminer)) {
				if (current->middle == UNINITIALIZED_INDEX) {
					return UNINITIALIZED_INDEX;
				} else {
					currentIndex = current->middle;
					current = GetFindNode(currentIndex);
				}
			}
		}
		return IsFinal(current) ? currentIndex : UNINITIALIZED_INDEX;
	}
	
	NodeIndex Find(const T item[], const T deliminer[]) {
		return Find(item, deliminer, rootIndex);
	}
	
#ifdef COMPRESS_RBTST
	NodeIndex Compress(NodeIndex currentIndex = UNINITIALIZED_INDEX) {
		bool isRoot = false;
		if(currentIndex == UNINITIALIZED_INDEX) {
			currentIndex = rootIndex;
			isRoot = true;
		}

		NodeIndex result;
		
		Node * oldNode = GetNode(currentIndex);
		SetLeaf(oldNode, oldNode->left == UNINITIALIZED_INDEX && oldNode->right == UNINITIALIZED_INDEX);
		if(IsLeaf(oldNode)) {
			Leaf leaf = (Leaf)*oldNode;
			leafs.Add(&leaf);
			result = leafs.Count() - 1 + leafBase;
			Leaf * newLeaf = &leafs[result - leafBase];
			if(oldNode->middle != UNINITIALIZED_INDEX) {
				newLeaf->middle = Compress(oldNode->middle);
			}
		} else {
			Node node(*oldNode);
			findNodes.Add(&node);
			result = findNodes.Count() - 1;
			Node * newNode = &findNodes[result];
			if(oldNode->left != UNINITIALIZED_INDEX) {
				newNode->left = Compress(oldNode->left);
			}
			if(oldNode->right != UNINITIALIZED_INDEX) {
				newNode->right = Compress(oldNode->right);
			}
			if(oldNode->middle != UNINITIALIZED_INDEX) {
				newNode->middle = Compress(oldNode->middle);
			}
		}
		
		if(isRoot) {
			rootIndex = result;
		}
		return result;
	}
#endif
	
	private:
	/**
     * @return New root
     */
	NodeIndex ReplaceNode(NodeIndex oldParentIndex, NodeIndex oldNodeIndex, NodeIndex newNodeIndex) {
		if(oldParentIndex == UNINITIALIZED_INDEX) {
			return newNodeIndex;
		} else {
			Node * parent = GetNode(oldParentIndex);
			if(parent->left == oldNodeIndex) {
				parent->left = newNodeIndex;
			} else {
				parent->right = newNodeIndex;
			}
			return newNodeIndex;
		}
	}
	
	/**
     * @return New root
     */
	NodeIndex RotateLeft(NodeIndex index, unsigned int currentStackIndex) {
		Node * node = GetNode(index);
		Node * right = GetNode(node->right);
		NodeIndex newRoot = ReplaceNode(GetParentIndex(currentStackIndex), index, node->right);
		node->right = right->left;
		right->left = index;
		return newRoot;
	}
	
	/**
     * @return New root
     */
	NodeIndex RotateRight(NodeIndex index, unsigned int currentStackIndex) {
		Node * node = GetNode(index);
		Node * left = GetNode(node->left);
		NodeIndex newRoot = ReplaceNode(GetParentIndex(currentStackIndex), index, node->left);
		node->left = left->right;
		left->right = index;
		return newRoot;
	}
	
	NodeIndex CreateNode(T key, Color color) {
		newNodeTemp.color = color & 1;
		newNodeTemp.key = key;
		newNodeTemp.left = newNodeTemp.middle = newNodeTemp.right = UNINITIALIZED_INDEX;
		nodes.Add(&newNodeTemp);
		return nodes.Count() - 1;
	}
	
	public:
	NodeIndex Add(const T item[], const T deliminer[], NodeIndex * rootIndex) {
		if ((TST_POINTER_EQUAL_P item) == (TST_POINTER_EQUAL_P deliminer)) {
			throw "Can't add empty item!";
		}
		// no root element - create
		if ((*rootIndex) == UNINITIALIZED_INDEX) {
			(*rootIndex) = CreateNode(*item, BLACK);
		}
		Node * current;
		NodeIndex currentIndex;
		NodeIndex * parentReference = rootIndex;
		
		
		// loop through item parts
		while ((TST_POINTER_EQUAL_P item) != (TST_POINTER_EQUAL_P deliminer)) {
			// insert n-th item part into binary tree
			currentIndex = AddBinaryTreeNode(parentReference, *item);
			current = GetNode(currentIndex);
			item++;
			// move to new character in ternary tree
			if((*item) != (*deliminer)) {
				if(current->middle == UNINITIALIZED_INDEX) {
					current->middle = CreateNode(*item, BLACK);
					parentReference = &current->middle;
				} else {
					parentReference = &current->middle;
				}
			}
		}
		SetFinal(current, true);
		return currentIndex;
	}
	
	NodeIndex Add(const T item[], const T deliminer[]) {
		return Add(item, deliminer, &rootIndex);
	}
	
	private:
	/**
     * @return Node index
     */
	NodeIndex AddBinaryTreeNode(NodeIndex * rootIndex, T key) {
		NodeIndex index = * rootIndex;

		if(index == UNINITIALIZED_INDEX) {
			return *rootIndex = CreateNode(key, BLACK);
		}
		
		Node * node;
		unsigned int currentStackLevel = -1;
		while(true) {
			parents[++currentStackLevel] = index;
			node = GetNode(index);
			if(key == node->key) {
				return index;
			} else if(key < node->key) {
				if(node->left == UNINITIALIZED_INDEX) {
					index = node->left = parents[++currentStackLevel] = CreateNode(key, RED);
					break;
				} else {
					index = node->left;
				}
			} else {
				if(node->right == UNINITIALIZED_INDEX) {
					index = node->right = parents[++currentStackLevel] = CreateNode(key, RED);
					break;
				} else {
					index = node->right;
				}
			}
		}
		
		TreeRecolor1(currentStackLevel, index);
		*rootIndex = parents[0];
		
		ASSERT_RBT(IsRedBlackTree(*rootIndex), "Not a red black tree")
		return index;
	}
	
	void TreeRecolor5(unsigned int currentStackLevel, NodeIndex index) {
		SetColor(GetParent(currentStackLevel), BLACK);
		SetColor(GetGrandParent(currentStackLevel), RED);
		
		
		Node * grandGrandParent = GetGrandParent(currentStackLevel - 1);
		NodeIndex * grandGrandParentNodeIndex = grandGrandParent == NULL ? NULL : ((GetGrandParentIndex(currentStackLevel) == grandGrandParent->left) ? &grandGrandParent->left : &grandGrandParent->right);
		NodeIndex newRootIndex;
		if(index == GetParent(currentStackLevel)->left && GetParentIndex(currentStackLevel) == GetGrandParent(currentStackLevel)->left) {
			newRootIndex = RotateRight(GetGrandParentIndex(currentStackLevel), currentStackLevel - 2);
		} else {
			newRootIndex = RotateLeft(GetGrandParentIndex(currentStackLevel), currentStackLevel - 2);
		}
		if(grandGrandParentNodeIndex != NULL) {
			*grandGrandParentNodeIndex = newRootIndex;
		}
		parents[currentStackLevel - 2] = parents[currentStackLevel - 1];
		parents[currentStackLevel - 1] = index;
	}
	
	void TreeRecolor4(unsigned int currentStackLevel, NodeIndex index) {
		if(index == GetParent(currentStackLevel)->right && GetParentIndex(currentStackLevel) == GetGrandParent(currentStackLevel)->left) {
			NodeIndex newLeft = RotateLeft(GetParentIndex(currentStackLevel), currentStackLevel - 1);
			Node * grandParent = GetGrandParent(currentStackLevel);
			if(grandParent != NULL) {
				grandParent->left = newLeft;
			}
			
			parents[currentStackLevel] = GetParentIndex(currentStackLevel);
			parents[currentStackLevel - 1] = index;
			index = GetNode(index)->left;
			
		} else if(index == GetParent(currentStackLevel)->left && GetParentIndex(currentStackLevel) == GetGrandParent(currentStackLevel)->right) {
			NodeIndex newRight = RotateRight(GetParentIndex(currentStackLevel), currentStackLevel - 1);
			
			Node * grandParent = GetGrandParent(currentStackLevel);
			if(grandParent != NULL) {
				grandParent->right = newRight;
			}
			
			parents[currentStackLevel] = GetParentIndex(currentStackLevel);
			parents[currentStackLevel - 1] = index;
			index = GetNode(index)->right;
		}
		TreeRecolor5(currentStackLevel, index);
	}
	
	void TreeRecolor3(unsigned int currentStackLevel, NodeIndex index) {
		if(GetColor(GetUncle(currentStackLevel, index)) == RED) {
			SetColor(GetParent(currentStackLevel), BLACK);
			SetColor(GetUncle(currentStackLevel, index), BLACK);
			SetColor(GetGrandParent(currentStackLevel), RED);
			TreeRecolor1(currentStackLevel - 2, GetGrandParentIndex(currentStackLevel));
		} else {
			TreeRecolor4(currentStackLevel, index);
		}
	}
	
	void TreeRecolor2(unsigned int currentStackLevel, NodeIndex index) {
		if(GetColor(GetParent(currentStackLevel)) != BLACK) {
			TreeRecolor3(currentStackLevel, index);
		}
	}
	
	void TreeRecolor1(unsigned int currentStackLevel, NodeIndex index) {
		if(GetParentIndex(currentStackLevel) == UNINITIALIZED_INDEX) {
			SetColor(index, BLACK);
		} else {
			TreeRecolor2(currentStackLevel, index);
		}
	}
	
	void PrintNode(NodeIndex index) { 
		PrintNode(GetNode(index));
	}
	void PrintNode(Node * node) { 
		if(node->left != UNINITIALIZED_INDEX) 
			printf("%c<", GetNode(node->left)->key); 
		printf("(%c%c)", node->key, GetColor(node) == BLACK ? 'B' : 'R');
		if(node->right != UNINITIALIZED_INDEX)
			printf(">%c", GetNode(node->right)->key); 
		printf("\n");
	};
	
	
	public:
	void Print(Node * node = NULL, bool newRow = true, bool binaryTreeOnly = false) {
		bool begin = false;
		if (node == NULL) {
			node = &nodes[rootIndex];
			newRow = true;
			begin = true;
		}

		if (newRow) {
			printf(begin ? "{" : ":{");
		}
		if (node->left != UNINITIALIZED_INDEX) {
			printf("(");
			Print(&nodes[node->left], false, binaryTreeOnly);
			printf(")<");
		}
		printf("%c ", node->key);
//		printf("%c%c ", node->key, GetColor(node) == RED ? 'R' : 'B');
		if (node->middle != UNINITIALIZED_INDEX && !binaryTreeOnly) {
			Print(&nodes[node->middle], true, binaryTreeOnly);
		}
		if (node->right != UNINITIALIZED_INDEX) {
			printf(">(");
			Print(&nodes[node->right], false, binaryTreeOnly);
			printf(")");
		}
		if (newRow) {
			printf("}");
		}

	}
	
};

#endif	/* REDBLACKTREE_H */


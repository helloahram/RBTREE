#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  if (NULL == p->nil)
  {
    free(p);
    return NULL;
  }
  // calloc 으로 메모리 할당 + 초기화 진행

  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;

  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  // nil node 를 제외하고 가장 왼쪽 자식에 있는 자식 노드가
  // 트리에서 가장 작은 값이기 때문에 끝까지 탐색

  // root node 부터 탐색 시작
  node_t *current = t->root;

  // current->left 가 nil node 인 경우 가장 작은 값이므로
  // current->left 가 nil node 일 때까지 탐색
  while (current->left != t->nil)
  {
    current = current->left;
  }

  return current;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  // nil node 를 제외하고 가장 오른쪽 자식에 있는 자식 노드가
  // 트리에서 가장 큰 값이기 때문에 끝까지 탐색

  // root node 부터 탐색 시작
  node_t *current = t->root;

  // current->left 가 nil node 인 경우 가장 작은 값이므로
  // current->left 가 nil node 일 때까지 탐색
  while (current->right != t->nil)
  {
    current = current->right;
  }

  return current;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}

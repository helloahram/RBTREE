#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  // rebtree 구조체에 대한 메모리 할당
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (NULL == p) // p 메모리 할당에 실패할 경우
    return NULL;

  // nil Node 구조체에 대한 메모리 할당
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  if (NULL == p->nil) // p->nil 메모리 할당에 실패할 경우
  {
    free(p); // p 메모리 할당 해제
    return NULL;
  }

  // calloc 이 메모리 할당 + 초기화이기 때문에 색만 지정
  p->nil->color = RBTREE_BLACK;

  // 트리의 Root Node 와 nil Node 초기화
  p->root = p->nil; // 초기 Root 는 nil 로 설정

  // 생성한 트리 반환
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// FixUp 시 좌회전
void rotate_left(rbtree *t, node_t *node)
{
  // 1. right_child 의 left 를 node 의 right 와 연결
  // 2. node 의 parent 와 right_child 를 연결
  // 3. node 와 right_child 를 연결

  // 기준 노드 node 의 right 를
  // right_child 라는 포인터 변수로 선언
  node_t *right_child = node->right;

  // right_child 의 left 는 node 보다 크기 때문에
  // node 의 right 로 연결해 준다
  // right_child 의 left 가 nil node 가 아닌 경우
  // right_child->left 의 부모를 node 로 연결해준다
  node->right = right_child->left;
  if (right_child->left != t->nil)
    right_child->left->parent = node;

  // node 의 parent 와 right_child 를 연결
  // parent 가 nil node 인 경우 r_c 가 root node
  right_child->parent = node->parent;
  if (node->parent == t->nil)
    t->root = right_child;
  else if (node == node->parent->left)
    node->parent->left = right_child;
  else // node 가 부모의 오른쪽 자식일 때
    node->parent->right = right_child;

  // node 와 right_child 연결해주기
  node->parent = right_child;
  right_child->left = node;
}

// FixUp 시 우회전
void rotate_right(rbtree *t, node_t *node)
{
  node_t *left_child = node->left;

  node->left = left_child->right;
  if (left_child->right != t->nil)
    left_child->right->parent = node;

  left_child->parent = node->parent;
  if (node->parent == t->nil)
    t->root = left_child;
  else if (node == node->parent->left)
    node->parent->left = left_child;
  else
    node->parent->right = left_child;

  node->parent = left_child;
  left_child->right = node;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert

  // 최초 노드 생성 시 root 를 만들어주고 초기화
  // root == NULL 로 확인하면 nil 노드를 사용하는 RBT 규칙을 어긴다
  if (t->root == t->nil)
  {
    t->root = (node_t *)calloc(1, sizeof(node_t));
    t->root->color = RBTREE_BLACK;
    t->root->key = key;
    t->root->parent = t->root->left = t->root->right = t->nil;
  }

  // 새로 추가하는 New Node 만들기
  node_t *new = (node_t *)calloc(1, sizeof(node_t));
  // 현재 위치를 찾는 current Node 를 만들어 root 부터 탐색 시작
  node_t *current = t->root;

  // current 로 t->nil 까지 탐색 진행
  // left or right 가 nil 이면 new 할당
  while (current != t->nil)
  {
    // 트리 왼쪽 탐색
    if (current->key > key)
    {
      current = current->left;
      if (current->left == t->nil)
      {
        current->left = new;
        break;
      }
    }
    // 트리 오른쪽 탐색
    if (current->key < key)
    {
      current = current->right;
      if (current->right == t->nil)
      {
        current->right = new;
        break;
      }
    }
    // new Node 초기화 설정
    new->key = key;
    new->parent = current;
    new->left = new->right = t->nil;
    new->color = RBTREE_RED; // 삽입 노드는 빨강
  }

  // insert_fix 함수도 구현해줘야함
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  // root node 부터 탐색 시작
  node_t *current = t->root;

  // nil->node 도달까지 (Key 를 못 찾을 때까지) 탐색 진행
  // 찾는 값 key 과 현재 값 current->key 를 비교
  // Current 와 비교했을 때 Key 가 작다면 왼쪽으로 이동
  // Current 와 비교했을 때 Key 가 크다면 오른쪽으로 이동
  while (current->key != t->nil)
  {
    // 현재 값보다 찾는 값이 클 때 왼쪽으로 이동
    if (current->key > key)
      current = current->left;
    // 현재 값보다 찾는 값이 작을 때 오른쪽으로 이동
    else if (current->key < key)
      current = current->right;
    // Key 를 찾았을 때 해당 노드 current 반환
    else
      return current;
  }

  // nil node 까지 도달했지만 Key 가 없는 경우,
  // node_t * 반환 타입에 맞게 NULL return
  return NULL;
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

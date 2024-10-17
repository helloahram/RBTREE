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

  // 트리가 비어있는 경우 최초 노드 생성
  // root == NULL 대신 root == t->nil 로 비교
  if (t->root == t->nil)
  {
    t->root = (node_t *)calloc(1, sizeof(node_t));
    if (t->root == NULL) // 메모리 할당 실패 처리
      return NULL;
    t->root->color = RBTREE_BLACK; // 루트 노드는 항상 검정
    t->root->key = key;
    t->root->parent = t->root->left = t->root->right = t->nil;

    return t->root; // 트리가 비어있는 경우 t->root 반환 후 종료
  }

  // 새 노드 생성 및 초기화
  node_t *new = (node_t *)calloc(1, sizeof(node_t));
  if (new == NULL) // 메모리 할당 실패 처리
    return NULL;

  new->key = key;
  new->color = RBTREE_RED; // 삽입 노드는 빨강
  new->left = new->right = t->nil;

  // 현재 위치를 찾는 current Node 를 만들어 root 부터 탐색 시작
  node_t *current = t->root;
  // 새로 삽입할 노드의 부모 추적
  node_t *parent = t->nil;

  // current 로 t->nil 까지 탐색 진행
  // left or right 가 nil 이면 new 할당
  // while (current != t->nil)
  // {
  //   // 트리 왼쪽 탐색
  //   if (current->key > key)
  //   {
  //     current = current->left;
  //     if (current->left == t->nil)
  //     {
  //       current->left = new;
  //       break;
  //     }
  //   }
  //   // 트리 오른쪽 탐색
  //   if (current->key < key)
  //   {
  //     current = current->right;
  //     if (current->right == t->nil)
  //     {
  //       current->right = new;
  //       break;
  //     }
  //   }
  // }

  // 새 노드를 삽입할 위치 탐색
  while (current != t->nil)
  {
    parent = current; // 부모 추적
    if (current->key > key)
      current = current->left;
    else if (current->key <= key) // 중복 키 허용
      current = current->right;   // 중복 키도 오른쪽
  }

  // 부모 노드에 새 노드 연결
  new->parent = parent;
  if (parent == t->nil) // 트리가 비어있는 경우
    t->root = new;      // 새 노드가 루트가 됨
  else if (parent->key > key)
    parent->left = new;
  else
    parent->right = new;

  // insert_fixup(t, new);
  // insert_fix 함수도 구현해줘야함
  return t->root;
}

// Insert Fixup 구현
node_t *insert_fixup(rbtree *t, node_t *new)
{
  // 삽입의 경우, 부모가 RED 인 경우만 고려하면 된다 - Case #4 위반
  // 부모가 존재하고, 부모가 RED 인 경우에 while 루프 진행
  while (new->parent &&new->parent->color == RBTREE_RED)
  {
    // 조부모 포인터 변수 생성
    node_t *grand_parent = new->parent->parent;

    // 부모가 조부모의 왼쪽 자식인 경우
    if (grand_parent->left == new->parent)
    {
      // 부모가 조부모의 왼쪽 자식인 경우, 오른쪽 자식을 Uncle 로 선언
      node_t *uncle = grand_parent->right;

      // Case 1, Uncle 이 존재하고, Uncle 이 RED 인 경우
      if (uncle && uncle->color == RBTREE_RED)
      { // 조부모를 RED 로, 부모와 삼촌을 BLACK 으로 변경
        grand_parent->color = RBTREE_RED;
        new->parent->color = uncle->color = RBTREE_BLACK;

        new = grand_parent; // 기준 노드를 조부모로 변경하여 반복 확인
      }

      // Case 2, Uncle 이 BLACK 이고, new 가 오른쪽 자식인 경우
      else if (new->parent->right == new)
      {
        // new 의 부모를 기준으로 왼쪽으로 회전 후 Case 3 에서 해결
        new = new->parent;
        rotate_left(t, new);
      }

      // Case 3, Uncle 이 BLACK 이고, new 가 왼쪽 자식인 경우
      else // if(new->parent->left == new)
      {
        // new 의 부모를 BLACK 으로, 조부모를 RED 로 변경
        new->parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;

        right_rotate(t, grand_parent); // 조부모 기준으로 재귀 우회전 진행
      }
    }
  }
  // 루트 노트의 색상을 BLACK 으로 변경
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  // root node 부터 탐색 시작
  node_t *current = t->root;

  // nil->node 도달까지 (Key 를 못 찾을 때까지) 탐색 진행
  // 찾는 값 key 과 현재 값 current 를 비교
  // Current 와 비교했을 때 Key 가 작다면 왼쪽으로 이동
  // Current 와 비교했을 때 Key 가 크다면 오른쪽으로 이동
  while (current != t->nil)
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

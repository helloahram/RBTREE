#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

// RBTREE 색을 열거형으로 정의하고, color_t 라는 이름으로 새로운 자료형 정의
// Red, Black 를 각각 RBTREE_xxx 으로 정의하여 color_t nodeColor; 로 사용 가능
// ex. nodeColor = RBTREE_RED; or nodeColor = RBTREE_BLACK;
typedef enum
{
  RBTREE_RED,
  RBTREE_BLACK
} color_t;

// RBTREE 에서 사용할 정수형 키 값을 key_t 라는 이름으로 정의
typedef int key_t;

// RBTREE 의 노드를 나타내는 구조체 정의
typedef struct node_t
{
  color_t color;                        // 노드의 색상 RED or BLACK
  key_t key;                            // 노드가 저장하는 키 값
  struct node_t *parent, *left, *right; // 부모, 왼쪽 자식, 오른쪽 자식을 나타내는 포인터
} node_t;

// RBTREE 자체를 나타내는 구조체 정의
typedef struct
{
  node_t *root; // 트리의 루트 노드를 가리키는 포인터
  node_t *nil;  // nil 노드를 가리키는 포인터, Sentinel 로 사용
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif // _RBTREE_H_

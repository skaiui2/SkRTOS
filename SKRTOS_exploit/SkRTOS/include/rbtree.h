//
// Created by el on 2024/10/18.
//
#ifndef RBTREE_H
#define RBTREE_H


struct rb_node
{
    struct rb_node *rb_parent;
    int rb_color;
#define	RB_RED		0
#define	RB_BLACK	1
    struct rb_node *rb_right;
    struct rb_node *rb_left;
};

struct rb_root
{
    struct rb_node *rb_node;
};

#define RB_ROOT	(struct rb_root) { NULL, }
#define	rb_entry(ptr, type, member)					\
	((type *)((char *)(ptr)-(uintptr_t)(&((type *)0)->member)))

void rb_insert_color(struct rb_node *, struct rb_root *);
void rb_erase(struct rb_node *, struct rb_root *);

/* Find logical next and previous nodes in a tree */
struct rb_node *rb_next(struct rb_node *);
struct rb_node *rb_prev(struct rb_node *);
struct rb_node *rb_first(struct rb_root *);
struct rb_node *rb_last(struct rb_root *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
void rb_replace_node(struct rb_node *victim, struct rb_node *new,
                            struct rb_root *root);

struct my_node *search_node(struct rb_root *root, int key);
void delete_node(struct rb_root *root, struct my_node *node);

struct my_node {
    struct rb_node rb_node;
    int key;
    // 其他数据成员
};


void insert_node(struct rb_root *root, struct my_node *new_node);



#endif
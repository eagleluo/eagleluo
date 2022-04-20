/**
 * @Author: Sheng Zhang <XhxngX2>
 * @Date:   20-Jul-2020
 * @Email:  zhangsheng@brisonus.com
 * @Project: Commom
 * @Last modified by:   Sheng Zhang
 * @Last modified time: 2020-09-01T09:08:13+08:00
 * @Copyright: Huayan Huisheng (Suzhou) Electronics Technology Co.Ltd,All Rights Reserved.
 */


/*Copied from Linux Kernel*/
#ifndef __STR2FUN_LIST_H
#define __STR2FUN_LIST_H

#undef offsetof
#define offsetof(TYPE, MEMBER) ((int32_t)&((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ( \
		(type *)((int32_t)ptr - offsetof(type, member)))


struct list_head {
	struct list_head *next, *prev;
};


#define STR_LIST_INIT(x) \
	(x).next = (struct list_head *)&(x); \
	(x).prev = (struct list_head *)&(x); \

#define STR_LIST_HEAD_INIT(name) { &(name), &(name) }

#define STR_LIST_HEAD(name) \
	struct list_head name = STR_LIST_HEAD_INIT(name)

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, whole, member)                          \
	for (pos = list_entry((head)->next, whole, member);      \
	     &pos->member != (head);    \
	     pos = list_entry(pos->member.next, whole, member))

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static int list_empty(const struct list_head *head)
{
	return head->next == head;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *_new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *_new, struct list_head *head)
{
	__list_add(_new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

#endif

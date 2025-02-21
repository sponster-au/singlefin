/*
 *  Duktape public API for Duktape @DUK_VERSION_FORMATTED@.
 *
 *  See the API reference for documentation on call semantics.  The exposed,
 *  supported API is between the "BEGIN PUBLIC API" and "END PUBLIC API"
 *  comments.  Other parts of the header are Duktape internal and related to
 *  e.g. platform/compiler/feature detection.
 *
 *  Git commit 03ef94d43a300ac77443e37fea2591d8cc316442 (03ef94d-dirty).
 *  Git branch master.
 *
 *  See Duktape AUTHORS.rst and LICENSE.txt for copyright and
 *  licensing information.
 */



#if !defined(SINGLEFIN_H_INCLUDED)
#define SINGLEFIN_H_INCLUDED

#define FIN_SINGLE_FILE

#define FIN_MODULE_BG96


/*
 *  Version and Git commit identification
 */

/* Duktape version, (major * 10000) + (minor * 100) + patch.  Allows C code
 * to #if (DUK_VERSION >= NNN) against Duktape API version.  The same value
 * is also available to ECMAScript code in Duktape.version.  Unofficial
 * development snapshots have 99 for patch level (e.g. 0.10.99 would be a
 * development version after 0.10.0 but before the next official release).
 */
#define FIN_VERSION                       10000L

/* Git commit, describe, and branch for Duktape build.  Useful for
 * non-official snapshot builds so that application code can easily log
 * which Duktape snapshot was used.  Not available in the ECMAScript
 * environment.
 */
#define FIN_GIT_COMMIT                    "03ef94d43a300ac77443e37fea2591d8cc316442"
#define FIN_GIT_DESCRIBE                  "03ef94d-dirty"
#define FIN_GIT_BRANCH                    "master"



/* External fin_config.h provides platform/compiler/OS dependent
 * typedefs and macros, and FIN_USE_xxx config options so that
 * the rest of SingleFin doesn't need to do any feature detection.
 * FIN_VERSION is defined before including so that configuration
 * snippets can react to it.
 */
#include "fin_config.h"


/*
 *  Avoid C++ name mangling
 */

#if defined(__cplusplus)
extern "C" {
#endif


/* LIST */

#ifndef container_of
#define container_of(ptr, type, member) \
	({const typeof(((type *)0)->member) *__mptr = (ptr); (type *)((char *)__mptr - offsetof(type,member));})
#endif

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head {
	struct list_head * next, * prev;
};

#define LIST_HEAD(name) \
	struct list_head name = { &(name), &(name) }

static inline void init_list_head(struct list_head * list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head * new,
			      struct list_head * prev,
			      struct list_head * next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head * new, struct list_head * head)
{
	__list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head * new, struct list_head * head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void __list_del_entry(struct list_head * entry)
{
	__list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
	__list_del_entry(entry);
	entry->next = 0;
	entry->prev = 0;
}

/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static inline void list_replace(struct list_head * old,
				struct list_head * new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline void list_replace_init(struct list_head * old,
					struct list_head * new)
{
	list_replace(old, new);
	init_list_head(old);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(struct list_head * entry)
{
	__list_del_entry(entry);
	init_list_head(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(struct list_head * list, struct list_head * head)
{
	__list_del_entry(list);
	list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(struct list_head * list,
				  struct list_head * head)
{
	__list_del_entry(list);
	list_add_tail(list, head);
}

/**
 * list_bulk_move_tail - move a subsection of a list to its tail
 * @head: the head that will follow our entry
 * @first: first entry to move
 * @last: last entry to move, can be the same as first
 *
 * Move all entries between @first and including @last before @head.
 * All three entries must belong to the same linked list.
 */
static inline void list_bulk_move_tail(struct list_head * head,
				       struct list_head * first,
				       struct list_head * last)
{
	first->prev->next = last->next;
	last->next->prev = first->prev;

	head->prev->next = first;
	first->prev = head->prev;

	last->next = head;
	head->prev = last;
}

/**
 * list_is_first - tests whether @list is the first entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int list_is_first(const struct list_head * list,
				const struct list_head * head)
{
	return list->prev == head;
}

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int list_is_last(const struct list_head * list,
				const struct list_head * head)
{
	return list->next == head;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const struct list_head * head)
{
	return head->next == head;
}

/**
 * list_empty_careful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using list_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is list_del_init(). Eg. it cannot be used
 * if another CPU could re-list_add() it.
 */
static inline int list_empty_careful(const struct list_head * head)
{
	struct list_head * next = head->next;
	return (next == head) && (next == head->prev);
}

/**
 * list_rotate_left - rotate the list to the left
 * @head: the head of the list
 */
static inline void list_rotate_left(struct list_head * head)
{
	struct list_head * first;

	if (!list_empty(head))
	{
		first = head->next;
		list_move_tail(first, head);
	}
}

/**
 * list_is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
static inline int list_is_singular(const struct list_head * head)
{
	return !list_empty(head) && (head->next == head->prev);
}

static inline void __list_cut_position(struct list_head * list,
		struct list_head * head, struct list_head * entry)
{
	struct list_head * new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

/**
 * list_cut_position - cut a list into two
 * @list: a new list to add all removed entries
 * @head: a list with entries
 * @entry: an entry within head, could be the head itself
 *	and if so we won't cut the list
 *
 * This helper moves the initial part of @head, up to and
 * including @entry, from @head to @list. You should
 * pass on @entry an element you know is on @head. @list
 * should be an empty list or a list you do not care about
 * losing its data.
 *
 */
static inline void list_cut_position(struct list_head * list,
		struct list_head * head, struct list_head * entry)
{
	if(list_empty(head))
		return;
	if(list_is_singular(head) &&
		(head->next != entry && head != entry))
		return;
	if(entry == head)
		init_list_head(list);
	else
		__list_cut_position(list, head, entry);
}

/**
 * list_cut_before - cut a list into two, before given entry
 * @list: a new list to add all removed entries
 * @head: a list with entries
 * @entry: an entry within head, could be the head itself
 *
 * This helper moves the initial part of @head, up to but
 * excluding @entry, from @head to @list.  You should pass
 * in @entry an element you know is on @head.  @list should
 * be an empty list or a list you do not care about losing
 * its data.
 * If @entry == @head, all entries on @head are moved to
 * @list.
 */
static inline void list_cut_before(struct list_head * list,
				   struct list_head * head,
				   struct list_head * entry)
{
	if (head->next == entry)
	{
		init_list_head(list);
		return;
	}
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry->prev;
	list->prev->next = list;
	head->next = entry;
	entry->prev = head;
}

static inline void __list_splice(const struct list_head * list,
				 struct list_head * prev,
				 struct list_head * next)
{
	struct list_head * first = list->next;
	struct list_head * last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

/**
 * list_splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(const struct list_head * list,
				struct list_head * head)
{
	if(!list_empty(list))
		__list_splice(list, head, head->next);
}

/**
 * list_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice_tail(struct list_head * list,
				struct list_head * head)
{
	if(!list_empty(list))
		__list_splice(list, head->prev, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(struct list_head * list,
				    struct list_head * head)
{
	if(!list_empty(list))
	{
		__list_splice(list, head, head->next);
		init_list_head(list);
	}
}

/**
 * list_splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
static inline void list_splice_tail_init(struct list_head * list,
					 struct list_head * head)
{
	if(!list_empty(list))
	{
		__list_splice(list, head->prev, head);
		init_list_head(list);
	}
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

/**
 * list_last_entry - get the last element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)

/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define list_first_entry_or_null(ptr, type, member) ({ \
	struct list_head * head__ = (ptr); \
	struct list_head * pos__ = head__->next; \
	pos__ != head__ ? list_entry(pos__, type, member) : NULL; \
})

/**
 * list_next_entry - get the next element in list
 * @pos:	the type * to cursor
 * @member:	the name of the list_head within the struct.
 */
#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_prev_entry - get the prev element in list
 * @pos:	the type * to cursor
 * @member:	the name of the list_head within the struct.
 */
#define list_prev_entry(pos, member) \
	list_entry((pos)->member.prev, typeof(*(pos)), member)

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop cursor.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop cursor.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define list_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define list_for_each_entry(pos, head, member) \
	for (pos = list_first_entry(head, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define list_for_each_entry_reverse(pos, head, member) \
	for (pos = list_last_entry(head, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = list_prev_entry(pos, member))

/**
 * list_prepare_entry - prepare a pos entry for use in list_for_each_entry_continue()
 * @pos:	the type * to use as a start point
 * @head:	the head of the list
 * @member:	the name of the list_head within the struct.
 *
 * Prepares a pos entry for use as a start point in list_for_each_entry_continue().
 */
#define list_prepare_entry(pos, head, member) \
	((pos) ? : list_entry(head, typeof(*pos), member))

/**
 * list_for_each_entry_continue - continue iteration over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define list_for_each_entry_continue(pos, head, member) \
	for (pos = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_continue_reverse - iterate backwards from the given point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Start to iterate over list of given type backwards, continuing after
 * the current position.
 */
#define list_for_each_entry_continue_reverse(pos, head, member) \
	for (pos = list_prev_entry(pos, member); \
	     &pos->member != (head); \
	     pos = list_prev_entry(pos, member))

/**
 * list_for_each_entry_from - iterate over list of given type from the current point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define list_for_each_entry_from(pos, head, member) \
	for (; &pos->member != (head); \
	     pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_from_reverse - iterate backwards over list of given type
 *                                    from the current point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Iterate backwards over list of given type, continuing from current position.
 */
#define list_for_each_entry_from_reverse(pos, head, member) \
	for (; &pos->member != (head); \
	     pos = list_prev_entry(pos, member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member) \
	for (pos = list_first_entry(head, typeof(*pos), member), \
		n = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_next_entry(n, member))

/**
 * list_for_each_entry_safe_continue - continue list iteration safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define list_for_each_entry_safe_continue(pos, n, head, member) \
	for (pos = list_next_entry(pos, member), \
		n = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_next_entry(n, member))

/**
 * list_for_each_entry_safe_from - iterate over list from current point safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define list_for_each_entry_safe_from(pos, n, head, member) \
	for (n = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_next_entry(n, member))

/**
 * list_for_each_entry_safe_reverse - iterate backwards over list safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
#define list_for_each_entry_safe_reverse(pos, n, head, member) \
	for (pos = list_last_entry(head, typeof(*pos), member), \
		n = list_prev_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_prev_entry(n, member))

/**
 * list_safe_reset_next - reset a stale list_for_each_entry_safe loop
 * @pos:	the loop cursor used in the list_for_each_entry_safe loop
 * @n:		temporary storage used in list_for_each_entry_safe
 * @member:	the name of the list_head within the struct.
 *
 * list_safe_reset_next is not safe to use in general if the list may be
 * modified concurrently (eg. the lock is dropped in the loop body). An
 * exception to this is if the cursor element (pos) is pinned in the list,
 * and list_safe_reset_next is called after re-taking the lock and before
 * completing the current iteration of the loop body.
 */
#define list_safe_reset_next(pos, n, member) \
	n = list_next_entry(pos, member)

/*
 * Double linked lists with a single pointer list head.
 * Mostly useful for hash tables where the two pointer list head is
 * too wasteful.
 * You lose the ability to access the tail in O(1).
 */
struct hlist_head {
	struct hlist_node * first;
};

struct hlist_node {
	struct hlist_node * next, ** pprev;
};

#define HLIST_HEAD(name) \
	struct hlist_head name = { .first = NULL }

static inline void init_hlist_head(struct hlist_head * hlist)
{
	hlist->first = NULL;
}

static inline void init_hlist_node(struct hlist_node * h)
{
	h->next = NULL;
	h->pprev = NULL;
}

static inline int hlist_unhashed(const struct hlist_node * h)
{
	return !h->pprev;
}

static inline int hlist_empty(const struct hlist_head * h)
{
	return !h->first;
}

static inline void __hlist_del(struct hlist_node * n)
{
	struct hlist_node * next = n->next;
	struct hlist_node ** pprev = n->pprev;

	*pprev = next;
	if(next)
		next->pprev = pprev;
}

static inline void hlist_del(struct hlist_node * n)
{
	__hlist_del(n);
	n->next = 0;
	n->pprev = 0;
}

static inline void hlist_del_init(struct hlist_node * n)
{
	if(!hlist_unhashed(n))
	{
		__hlist_del(n);
		init_hlist_node(n);
	}
}

static inline void hlist_add_head(struct hlist_node * n, struct hlist_head * h)
{
	struct hlist_node * first = h->first;
	n->next = first;
	if(first)
		first->pprev = &n->next;
	h->first = n;
	n->pprev = &h->first;
}

/* next must be != NULL */
static inline void hlist_add_before(struct hlist_node * n,
					struct hlist_node * next)
{
	n->pprev = next->pprev;
	n->next = next;
	next->pprev = &n->next;
	*(n->pprev) = n;
}

static inline void hlist_add_behind(struct hlist_node * n,
				    struct hlist_node * prev)
{
	n->next = prev->next;
	prev->next = n;
	n->pprev = &prev->next;

	if(n->next)
		n->next->pprev = &n->next;
}

/* after that we'll appear to be on some hlist and hlist_del will work */
static inline void hlist_add_fake(struct hlist_node * n)
{
	n->pprev = &n->next;
}

static inline int hlist_fake(struct hlist_node * h)
{
	return h->pprev == &h->next;
}

/*
 * Check whether the node is the only node of the head without
 * accessing head:
 */
static inline int hlist_is_singular_node(struct hlist_node * n, struct hlist_head * h)
{
	return !n->next && n->pprev == &h->first;
}

/*
 * Move a list from one list head to another. Fixup the pprev
 * reference of the first entry if it exists.
 */
static inline void hlist_move_list(struct hlist_head * old,
				   struct hlist_head * new)
{
	new->first = old->first;
	if(new->first)
		new->first->pprev = &new->first;
	old->first = NULL;
}

#define hlist_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define hlist_for_each(pos, head) \
	for (pos = (head)->first; pos; pos = pos->next)

#define hlist_for_each_safe(pos, n, head) \
	for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
	     pos = n)

#define hlist_entry_safe(ptr, type, member) \
	({ typeof(ptr) ____ptr = (ptr); \
	   ____ptr ? hlist_entry(____ptr, type, member) : NULL; \
	})

/**
 * hlist_for_each_entry	- iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry(pos, head, member) \
	for (pos = hlist_entry_safe((head)->first, typeof(*(pos)), member); \
	     pos; \
	     pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * hlist_for_each_entry_continue - iterate over a hlist continuing after current point
 * @pos:	the type * to use as a loop cursor.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_continue(pos, member) \
	for (pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member); \
	     pos; \
	     pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * hlist_for_each_entry_from - iterate over a hlist continuing from current point
 * @pos:	the type * to use as a loop cursor.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_from(pos, member) \
	for (; pos; \
	     pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

/**
 * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another &struct hlist_node to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_safe(pos, n, head, member) \
	for (pos = hlist_entry_safe((head)->first, typeof(*pos), member); \
	     pos && ({ n = pos->member.next; 1; }); \
	     pos = hlist_entry_safe(n, typeof(*pos), member))



/* ADC */

typedef enum fin_adc_channel{	
	ADC0,
	ADC1,
	ADC_MAX_NO
}fin_adc_channel_t;

int fin_adc_config(fin_adc_channel_t adc_num);
uint32_t fin_adc_read_microvolts(fin_adc_channel_t adc_num);
uint32_t fin_adc_read_physical(fin_adc_channel_t adc_num);
uint32_t fin_adc_read_percent(fin_adc_channel_t adc_num);
uint32_t fin_adc_read_raw(fin_adc_channel_t adc_num);


/* CLI */
typedef int (*fin_cli_callback_t)(int args, char *argv[]);

typedef	struct fin_cli_cmd{
	char *name;
	fin_cli_callback_t callback;
	struct fin_cli_cmd *next;
} fin_cli_cmd_t;

typedef struct fin_cli{
	char *in;
	int head;
	int tail;
	int count;
	fin_cli_cmd_t *cmds;
} fin_cli_t;


void fin_cli_free(fin_cli_t *ctx);
fin_cli_t *fin_cli_new(void);
void fin_cli_input(fin_cli_t *ctx, char c);
int fin_cli_register(fin_cli_t *ctx, const char *name, fin_cli_callback_t func);


/* DSS */


#define DSS_ADDR_SIZE 48

/* Number of simultaneously dss data calls supported */
#define DSS_CONCURRENT_NUM	4

typedef const void (*fin_dss_data_call_status_cb_t)(qapi_DSS_Net_Evt_t evt);

typedef struct fin_dss_notify{
	TX_EVENT_FLAGS_GROUP *evt;
	char addr[DSS_ADDR_SIZE];
	fin_dss_data_call_status_cb_t cb;
	bool timed_out;
}fin_dss_notify_t;

typedef struct fin_dss_ctx{ 
	char apn[QAPI_DSS_CALL_INFO_APN_MAX_LEN];
	char username[QAPI_DSS_CALL_INFO_USERNAME_MAX_LEN];
	char password[QAPI_DSS_CALL_INFO_PASSWORD_MAX_LEN];
	TX_EVENT_FLAGS_GROUP *dss_event_handle;
	fin_dss_notify_t *notify;
	TX_THREAD	*thread_handle; 
	TX_BYTE_POOL *thread_byte_pool;
	void *thread_stack;
	qapi_DSS_Hndl_t dss_handle;
	struct{
		TX_EVENT_FLAGS_GROUP *nw_event_handle;
		qapi_Device_Info_Hndl_t device_info_handle;
	};
} fin_dss_ctx_t;


#define fin_dss_set_notify(ctx, tobenotified) do { (ctx)->notify = &(tobenotified)->notify; } while (0) 
#define fin_dss_wait_conn_notify(N,T) \
do { 	\
	uint32_t N##_received_sigs = 0; \
	N->notify->timed_out = tx_event_flags_get(N->notify->evt, QAPI_DSS_EVT_NET_IS_CONN_E, TX_OR_CLEAR, &N##_received_sigs, T); \
} while (0)

fin_dss_ctx_t *fin_dss_new(const char *apn, const char *password, const char *username);
int fin_dss_start(fin_dss_ctx_t *ctx);
int fin_dss_restart(fin_dss_ctx_t *ctx);
int fin_dss_stop(fin_dss_ctx_t *ctx);
int fin_dss_free(fin_dss_ctx_t *ctx);


/* GPIO */
typedef void (*fin_gpio_interrupt_cb_t)(uint32_t arg);

int fin_gpio_pin_on(uint32_t pin, uint32_t trigger, fin_gpio_interrupt_cb_t interrupt_cb) ;
int fin_gpio_pin_trigger(uint32_t pin);
int fin_gpio_pin_disable_irq(uint32_t pin);
int fin_gpio_pin_enable_irq(uint32_t pin);
int fin_gpio_pin_config(uint32_t pin, uint32_t pull, uint32_t drive, uint32_t type);
int fin_gpio_pin_output(uint32_t pin, uint32_t pull, uint32_t drive);
int fin_gpio_pin_input(uint32_t pin, uint32_t pull, uint32_t drive);
int fin_gpio_pin_release(uint32_t pin);
int fin_gpio_pin_write(uint32_t pin, bool val);
int fin_gpio_pin_read(uint32_t pin);
void fin_gpio_config_dump(uint32_t pin);

/* HTTP */

typedef struct http_client_ssl {
	qapi_Net_SSL_Obj_Hdl_t ctx; 
	qapi_Net_SSL_Con_Hdl_t con; 
	qapi_Net_SSL_Config_t config; 
	qapi_Net_SSL_Role_t role;
} http_client_ssl_t;


typedef struct http_client_ctx{
	qapi_Net_HTTPc_handle_t handle;
	TX_BYTE_POOL *byte_pool;
	qapi_Net_HTTPc_Config_t *httpc_cfg;
	struct linger so_linger;
	TX_EVENT_FLAGS_GROUP *evt;
	struct list_head list;
	qapi_HTTPc_CB_t user_callback;
	uint32_t timeout;
	http_client_ssl_t ssl;
	bool use_https;
} fin_http_client_ctx_t;

#define fin_htpp_client_set_user_callback(ctx,cb) do{ ctx->user_callback = cb } while(0)
#define fin_http_client_for_each(p,ctx) list_for_each(p,&ctx->list)
#define fin_htpp_client_has_data(ctx) list_first_entry_or_null(&ctx->list, http_client_entry_t,head)
#define fin_http_client_get_data(iter) list_entry(iter,http_client_entry_t, head )->data
#define fin_http_client_get_data_length(iter) (uint32_t)list_entry(iter,http_client_entry_t, head )->data_len
#define fin_http_client_get_header(iter) list_entry(iter,http_client_entry_t, head )->header
#define fin_http_client_get_header_length(iter) (uint32_t)list_entry(iter,http_client_entry_t, head )->header_len
#define fin_http_client_free_resource(iter,data)	\
do{ \
	list_del(iter); \
	free(data); \
}while (0)

#define fin_http_client_set_timeout(ctx, t) do{ if(ctx){ ctx->timeout = t; } } while(0)


fin_http_client_ctx_t *fin_htpp_client_new(void);
int fin_htpp_client_free(fin_http_client_ctx_t *ctx);
int fin_htpp_client_set_header(fin_http_client_ctx_t *ctx, const char *key, const char *value);
int fin_htpp_client_set_parameter(fin_http_client_ctx_t *ctx, const char *key, const char *value);
int fin_htpp_client_set_body(fin_http_client_ctx_t *ctx, const char *body, uint32_t len);
int fin_htpp_client_get(fin_http_client_ctx_t *ctx, const char *host, int port, const char *path);

/* Hardware Random */
void fin_hwrandombytes(uint8_t *buf, uint64_t size );


/* I2C */

typedef enum fin_i2c_num{
	I2C1,
	I2C2,
	I2C_MAX_NO
} fin_i2c_num_t;


int fin_i2c_transfer(fin_i2c_num_t i2c_num, uint16_t addr, uint8_t *tx_data, size_t tx_data_size, 
	uint8_t *rx_data, size_t rx_data_size, uint32_t delay_us);
int fin_i2c_start(fin_i2c_num_t i2c_num);
int fin_i2c_stop(fin_i2c_num_t i2c_num);
int fin_i2c_config(fin_i2c_num_t i2c_num);
int fin_i2c_deconfig(fin_i2c_num_t i2c_num);
int fin_i2c_set_frequency(fin_i2c_num_t i2c_num, uint32_t frequency);
int fin_i2c_power_down(fin_i2c_num_t i2c_num);
int fin_i2c_power_up(fin_i2c_num_t i2c_num);


/* NET */

#define fin_dump_net_interfaces() \
do{ \
	qapi_Net_Ifnameindex_t itf[10]; \
	int itf_no = qapi_Net_Get_All_Ifnames (&itf); \
	for(int i=0; i < itf_no; i++){ \
		const char * address = 0; \
		uint32_t address_length = 0; \
		printf("Interface = %d, %s, %d\r\n",i,itf[i].interface_Name,itf[i].if_Is_Up); \
       	if(qapi_Net_Interface_Get_Physical_Address(itf[i].interface_Name, &address, &address_length) == 0) \
       		printf("physical address = %c.%c.%c.%c\r\n",address[0],address[1],address[2],address[3]); \
	} \
}while(0); \

#define fin_dump_dns_server_list() \
do { \
	qapi_Net_DNS_Server_List_t svr_list; \
	/* iface_index can be 0,1,2,or 3 */ \
	if (qapi_Net_DNSc_Get_Server_List(&svr_list,0) == 0){ \
		int i; \
		char ip_str[48]; \
		for (i = 0; i < MAX_DNS_SVR_NUM; ++i){ \
			if (svr_list.svr[i].type != AF_UNSPEC){ \
				printf("DNS Server: %s\r\n",	inet_ntop(svr_list.svr[i].type,	&svr_list.svr[i].a, ip_str, sizeof(ip_str))); \
			} \
		} \
	} \
} while(0)

const char *fin_resolve_host_itf(const char *domain, const char *itf, char *buf, size_t buf_len);

#define fin_resolve_host(domain, itf) \
({char itf##_ip_str[48]; \
	fin_resolve_host_itf(domain, #itf, itf##_ip_str, sizeof(itf##_ip_str)); \
    itf##_ip_str;\
})

/* TIMER */

#define fin_sleep(T) qapi_Timer_Sleep(T, QAPI_TIMER_UNIT_MSEC, true)


/* SPI */
typedef enum {
	SPI1,
	SPI2,
	SPI_MAX_NO
} fin_spi_num_t;

typedef enum{
	SPI_CS_DEASSERT,
	SPI_CS_KEEP_ASSERTED,
	SPI_CS_MODE_INVALID = 0x7FFFFFFF
} spi_cs_mode_t;

typedef enum{
	SPI_CS_ACTIVE_LOW,
	SPI_CS_ACTIVE_HIGH
} spi_cs_polarity_t;

typedef enum{
	SPI_BYTE_ORDER_NATIVE = 0,
	SPI_BYTE_ORDER_LITTLE_ENDIAN = 0,
	SPI_BYTE_ORDER_BIG_ENDIAN
}spi_endian_t;

typedef enum{
	SPI_MODE_0, //CPOL = 0, CPHA = 0.
	SPI_MODE_1, //CPOL = 0, CPHA = 1.
	SPI_MODE_2, //CPOL = 1, CPHA = 0.
	SPI_MODE_3 //CPOL = 1, CPHA = 1.
} spi_mode_t;

typedef enum{
	SPI_BPW_3 = 3,
	SPI_BPW_4,
	SPI_BPW_5,
	SPI_BPW_6,
	SPI_BPW_7,
	SPI_BPW_8,
	SPI_BPW_9,
	SPI_BPW_10,
	SPI_BPW_11,
	SPI_BPW_12,
	SPI_BPW_13,
	SPI_BPW_14,
	SPI_BPW_15,
	SPI_BPW_16,
	SPI_BPW_17,
	SPI_BPW_18,
	SPI_BPW_19,
	SPI_BPW_20,
	SPI_BPW_21,
	SPI_BPW_22,
	SPI_BPW_23,
	SPI_BPW_24,
	SPI_BPW_25,
	SPI_BPW_26,
	SPI_BPW_27,
	SPI_BPW_28,
	SPI_BPW_29,
	SPI_BPW_30,
	SPI_BPW_31
}spi_bits_per_word_t;



int fin_spi_power_down(fin_spi_num_t spi_num);
int fin_spi_power_up(fin_spi_num_t spi_num);
int fin_spi_config(fin_spi_num_t spi_num);
int fin_spi_deconfig(fin_spi_num_t spi_num);
int fin_spi_set_frequency(fin_spi_num_t spi_num, uint32_t frequency);
int fin_spi_set_endianess(fin_spi_num_t spi_num, spi_endian_t endian);
int fin_spi_set_bit_per_word(fin_spi_num_t spi_num, spi_bits_per_word_t bpw);
int fin_spi_set_mode(fin_spi_num_t spi_num, spi_mode_t mode);
int fin_spi_set_inter_word_delay(fin_spi_num_t spi_num, uint8_t delay);
int fin_spi_set_loopback(fin_spi_num_t spi_num, bool enable);
int fin_spi_set_chip_select_mode(fin_spi_num_t spi_num, spi_cs_mode_t mode);
int fin_spi_set_chip_select_polarity(fin_spi_num_t spi_num, spi_cs_polarity_t polarity);
int fin_spi_set_chip_select_delay(fin_spi_num_t spi_num, uint8_t delay);
int fin_spi_set_slaves_num(fin_spi_num_t spi_num, uint8_t num);
int fin_spi_send(fin_spi_num_t spi_num, uint8_t *tx_buf, size_t tx_len);
int fin_spi_send_receive(fin_spi_num_t spi_num, uint8_t *tx_buf, uint8_t *rx_buf, size_t rx_len);
void fin_spi_config_dump(fin_spi_num_t spi_num);
const char *fin_spi_get_name(fin_spi_num_t spi_num);

/* UART */

typedef enum fin_uart{
	UART1,
	UART2,
	UART3	
} fin_uart_t;

int fin_uart_config(fin_uart_t uart, uint32_t baud_rate, uint32_t stop_bits, uint32_t bits_per_char, uint32_t parity, uint32_t flow_ctrl);
int fin_uart_write(fin_uart_t uart, uint8_t *buf, size_t nbytes);
int fin_uart_read(fin_uart_t uart, uint8_t *buf, size_t nbytes);
int fin_uart_deconfig(fin_uart_t uart);
int fin_uart_power_up(fin_uart_t uart);
int fin_uart_power_down(fin_uart_t uart);


/*
 *  C++ name mangling
 */

#if defined(__cplusplus)
/* end 'extern "C"' wrapper */
}
#endif

/*
 *  END PUBLIC API
 */

#endif  /* SINGLEFIN_H_INCLUDED */
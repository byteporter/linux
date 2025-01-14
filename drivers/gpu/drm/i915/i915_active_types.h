/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2019 Intel Corporation
 */

#ifndef _I915_ACTIVE_TYPES_H_
#define _I915_ACTIVE_TYPES_H_

#include <linux/atomic.h>
#include <linux/llist.h>
#include <linux/mutex.h>
#include <linux/rbtree.h>
#include <linux/rcupdate.h>

struct drm_i915_private;
struct i915_active_request;
struct i915_request;

typedef void (*i915_active_retire_fn)(struct i915_active_request *,
				      struct i915_request *);

struct i915_active_request {
	struct i915_request __rcu *request;
	struct list_head link;
	i915_active_retire_fn retire;
};

struct active_node;

struct i915_active {
	struct drm_i915_private *i915;

	struct active_node *cache;
	struct rb_root tree;
	struct mutex mutex;
	atomic_t count;

	int (*active)(struct i915_active *ref);
	void (*retire)(struct i915_active *ref);

	struct llist_head barriers;
};

#endif /* _I915_ACTIVE_TYPES_H_ */

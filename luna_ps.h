/* luna_ps.h */
#ifndef LUNA_PS_H
#define LUNA_PS_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef LUNA_ASSERT
#include <assert.h>
#define LUNA_ASSERT		assert
#endif

struct core_ps;

struct core_ps_impl {
	void (*attach)(struct core_ps *ps, void *topic, void const *user);
	void (*detach)(struct core_ps *ps, void *topic, void const *user);
	void (*publish)(struct core_ps *ps, void *topic);
};

struct core_ps {
	struct core_ps_impl impl;
};

#endif

#ifdef LUNA_PS_IMPLEMENTATION

void luna_ps_init(struct core_ps *ps, struct core_ps_impl *impl)
{
	LUNA_ASSERT(ps);
	LUNA_ASSERT(impl);
	ps->impl = *impl;
}

void luna_subscribe(struct core_ps *ps, void *topic, void const *user)
{
	LUNA_ASSERT(ps);
	LUNA_ASSERT(user);
	if (ps->impl.attach) {
		ps->impl.attach(ps, topic, user);
	}
}

void luna_unsubscribe(struct core_ps *ps, void *topic, void const *user)
{
	LUNA_ASSERT(ps);
	LUNA_ASSERT(user);
	if (ps->impl.detach) {
		ps->impl.detach(ps, topic, user);
	}
}

void luna_publish(struct core_ps *ps, void *topic)
{
	LUNA_ASSERT(ps);
	if (ps->impl.publish) {
		ps->impl.publish(ps, topic);
	}
}

#endif


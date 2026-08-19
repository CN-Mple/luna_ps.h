/* luna_ps.c */
#include "luna_ps.h"

void luna_ps_init(struct luna_ps *ps, struct luna_bus *bus)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(bus);
	ps->bus = *bus;
}

void luna_ps_attach(struct luna_ps *ps, const void *topic, const void *user)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(user);
	if (ps->bus.attach) {
		ps->bus.attach(ps, topic, user);
	}
}

void luna_ps_detach(struct luna_ps *ps, const void *topic, const void *user)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(user);
	if (ps->bus.detach) {
		ps->bus.detach(ps, topic, user);
	}
}

void luna_ps_publish(struct luna_ps *ps, const void *topic)
{
	LUNA_PS_ASSERT(ps);
	if (ps->bus.publish) {
		ps->bus.publish(ps, topic);
	}
}

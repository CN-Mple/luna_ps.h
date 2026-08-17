/* luna_ps.h */
#ifndef LUNA_PS_H
#define LUNA_PS_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef LUNA_PS_ASSERT
#include <assert.h>
#define LUNA_PS_ASSERT		assert
#endif

struct luna_ps;

struct luna_bus {
	void (*attach)(struct luna_ps *ps, void *topic, const void *user);
	void (*detach)(struct luna_ps *ps, void *topic, const void *user);
	void (*publish)(struct luna_ps *ps, void *topic);
};

struct luna_ps {
	struct luna_bus bus;
};

void luna_ps_init(struct luna_ps *ps, struct luna_bus *bus);
void luna_ps_subscribe(struct luna_ps *ps, void *topic, const void *user);
void luna_ps_unsubscribe(struct luna_ps *ps, void *topic, const void *user);
void luna_ps_publish(struct luna_ps *ps, void *topic);

#endif

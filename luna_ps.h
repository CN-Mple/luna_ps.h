/* luna_ps.h */
#ifndef LUNA_PS_H
#define LUNA_PS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef LUNA_PS_ASSERT
#define LUNA_PS_ASSERT(expr)	((void)(expr))
#endif

typedef enum {
	LUNA_PS_MSG_UNSPECIFIED,
	LUNA_PS_MSG_UNICAST,
	LUNA_PS_MSG_MULTICAST,
	LUNA_PS_MSG_BROADCAST,
} luna_ps_msg_type_t;

typedef struct {
	luna_ps_msg_type_t msg_type;

	void              *topic;
	void              *sender;
	void              *receiver;

	void              *data;
	size_t             size;
} luna_ps_msg_t;

struct luna_ps_impl {
	void (*attach)(void const *topic, void const *user);
	void (*detach)(void const *topic, void const *user);

	void (*unicast)(void const *user, luna_ps_msg_t const *msg);
	void (*multicast)(void const *topic, luna_ps_msg_t const *msg);
	void (*broadcast)(luna_ps_msg_t const *msg);
};

struct luna_ps_config {
	struct luna_ps_impl impl;
	void (*push)(luna_ps_msg_t const *msg);
	bool (*pop)(luna_ps_msg_t const **msg);
};

struct luna_ps {
	struct luna_ps_impl impl;
	void (*push)(luna_ps_msg_t const *msg);
	bool (*pop)(luna_ps_msg_t const **msg);
};

void luna_ps_init(struct luna_ps *ps, struct luna_ps_config *config);
void luna_ps_attach(struct luna_ps *ps, void const *topic, void const *user);
void luna_ps_detach(struct luna_ps *ps, void const *topic, void const *user);

void luna_ps_process(struct luna_ps *ps);


luna_ps_msg_t *luna_ps_msg_alloc(size_t size);
void luna_ps_msg_free(luna_ps_msg_t *msg);

void luna_ps_unicast(struct luna_ps *ps, void const *sender, void const *target, luna_ps_msg_t *msg);
void luna_ps_multicast(struct luna_ps *ps, void const *sender, void const *topic, luna_ps_msg_t *msg);
void luna_ps_broadcast(struct luna_ps *ps, void const *sender, luna_ps_msg_t *msg);

#endif
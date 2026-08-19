/* luna_ps.c */
#include "luna_ps.h"

void luna_ps_init(struct luna_ps *ps, struct luna_ps_config *config)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(config);
	ps->impl = config->impl;
	ps->push = config->push;
	ps->pop  = config->pop;
}

void luna_ps_attach(struct luna_ps *ps, void const *topic, void const *user)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(topic);
	LUNA_PS_ASSERT(user);
	
	LUNA_PS_ASSERT(ps->impl.attach);
	ps->impl.attach(topic, user);
}

void luna_ps_detach(struct luna_ps *ps, void const *topic, void const *user)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(topic);
	LUNA_PS_ASSERT(user);

	LUNA_PS_ASSERT(ps->impl.detach);
	ps->impl.detach(topic, user);
}

static void luna_ps_push(struct luna_ps *ps, luna_ps_msg_t const *msg)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(ps->push);
	ps->push(msg);
}

static bool luna_ps_pop(struct luna_ps *ps, luna_ps_msg_t const **msg)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(ps->pop);
	return ps->pop(msg);
}

static void luna_ps_dispatch(struct luna_ps *ps, luna_ps_msg_t const *msg)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(msg);

	switch (msg->msg_type) {
	case LUNA_PS_MSG_UNICAST: {
		ps->impl.unicast(msg->receiver, msg);
	}
        break;
    	case LUNA_PS_MSG_MULTICAST: {
		ps->impl.multicast(msg->topic, msg);
	}
        break;
    	case LUNA_PS_MSG_BROADCAST: {
		ps->impl.broadcast(msg);
	}
	break;
	default:

	break;
	}
}

luna_ps_msg_t *luna_ps_msg_alloc(size_t size)
{
	luna_ps_msg_t *msg = malloc(sizeof(luna_ps_msg_t) + size);
	if (!msg) {
		return NULL;
	}
	memset(msg, 0, sizeof(luna_ps_msg_t) + size);
	msg->data = msg + 1;
	msg->size = size;
	return msg;
}

void luna_ps_msg_free(luna_ps_msg_t *msg)
{
	if (!msg) {
		return;
	}
	free(msg);
}

void luna_ps_process(struct luna_ps *ps)
{
	luna_ps_msg_t const *msg = NULL;
	if (luna_ps_pop(ps, &msg)) {
		luna_ps_dispatch(ps, msg);
		msg = NULL;
	}
}

void luna_ps_unicast(struct luna_ps *ps, void const *sender, void const *target, luna_ps_msg_t *msg)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(sender);
	LUNA_PS_ASSERT(target);
	LUNA_PS_ASSERT(msg);

	msg->msg_type = LUNA_PS_MSG_UNICAST;
	msg->sender   = sender;
	msg->receiver = target;
	luna_ps_push(ps, msg);
}

void luna_ps_multicast(struct luna_ps *ps, void const *sender, void const *topic, luna_ps_msg_t *msg)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(sender);
	LUNA_PS_ASSERT(topic);
	LUNA_PS_ASSERT(msg);

	msg->msg_type = LUNA_PS_MSG_MULTICAST;
	msg->sender   = sender;
	msg->topic    = topic;
	luna_ps_push(ps, msg);
}

void luna_ps_broadcast(struct luna_ps *ps, void const *sender, luna_ps_msg_t *msg)
{
	LUNA_PS_ASSERT(ps);
	LUNA_PS_ASSERT(sender);
	LUNA_PS_ASSERT(msg);

	msg->msg_type = LUNA_PS_MSG_BROADCAST;
	msg->sender   = sender;
	luna_ps_push(ps, msg);
}
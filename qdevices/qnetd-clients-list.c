/*
 * Copyright (c) 2015 Red Hat, Inc.
 *
 * All rights reserved.
 *
 * Author: Jan Friesse (jfriesse@redhat.com)
 *
 * This software licensed under BSD license, the text of which follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the Red Hat, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/types.h>
#include <arpa/inet.h>

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "qnetd-clients-list.h"

void
qnetd_clients_list_init(struct qnetd_clients_list *clients_list)
{

	TAILQ_INIT(clients_list);
}

struct qnetd_client *
qnetd_clients_list_add(struct qnetd_clients_list *clients_list, PRFileDesc *sock, PRNetAddr *addr,
	size_t max_receive_size, size_t max_send_size)
{
	struct qnetd_client *client;

	client = (struct qnetd_client *)malloc(sizeof(*client));
	if (client == NULL) {
		return (NULL);
	}

	qnetd_client_init(client, sock, addr, max_receive_size, max_send_size);

	TAILQ_INSERT_TAIL(clients_list, client, entries);

	return (client);
}

void
qnetd_clients_list_free(struct qnetd_clients_list *clients_list)
{
	struct qnetd_client *client;
	struct qnetd_client *client_next;

	client = TAILQ_FIRST(clients_list);

	while (client != NULL) {
		client_next = TAILQ_NEXT(client, entries);

		free(client);

		client = client_next;
	}

	TAILQ_INIT(clients_list);
}

void
qnetd_clients_list_del(struct qnetd_clients_list *clients_list, struct qnetd_client *client)
{

	TAILQ_REMOVE(clients_list, client, entries);
	qnetd_client_destroy(client);
	free(client);
}
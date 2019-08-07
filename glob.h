// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *   Copyright (C) 2016 Namjae Jeon <linkinjeon@gmail.com>
 *   Copyright (C) 2018 Samsung Electronics Co., Ltd.
 */

#ifndef __CIFSD_GLOB_H
#define __CIFSD_GLOB_H

#include <linux/ctype.h>
#include <linux/version.h>

#include "unicode.h"
#include "vfs_cache.h"
#include "smberr.h"

/* @FIXME clean up this code */

extern int cifsd_debugging;
extern int cifsd_caseless_search;
extern bool oplocks_enable;
extern bool lease_enable;
extern bool durable_enable;
extern bool multi_channel_enable;

#define NETLINK_CIFSD_MAX_PAYLOAD	4096

#define DATA_STREAM	1
#define DIR_STREAM	2

#define SYNC 1
#define ASYNC 2

#define	WORK_STATE_CANCELLED	0x2
#define WORK_STATE_CLOSED	0x3

struct cifsd_conn;

/* one of these for every pending CIFS request at the connection */
struct cifsd_work {
	/* Server corresponding to this mid */
	struct cifsd_conn		*conn;
	struct cifsd_session		*sess;
	struct cifsd_tree_connect	*tcon;

	/* Pointer to received SMB header */
	char				*request_buf;
	/* Response buffer */
	char				*response_buf;
	unsigned int			response_sz;

	/* Read data buffer */
	char				*aux_payload_buf;
	/* Read data count */
	unsigned int			aux_payload_sz;
	/* response smb header size */
	unsigned int			resp_hdr_sz;

	/* Next cmd hdr in compound req buf*/
	int				next_smb2_rcv_hdr_off;
	/* Next cmd hdr in compound rsp buf*/
	int				next_smb2_rsp_hdr_off;

	/* Transform header buffer */
	void				*tr_buf;
	int				type;

	/*
	 * Current Local FID assigned compound response if SMB2 CREATE
	 * command is present in compound request
	 */
	unsigned int			compound_fid;
	unsigned int			compound_pfid;
	unsigned int			compound_sid;

	int				state;

	/* Multiple responses for one request e.g. SMB ECHO */
	bool				multiRsp:1;
	/* No response for cancelled request */
	bool				send_no_response:1;
	/* Request is encrypted */
	bool				encrypted:1;

	/* List head at conn->requests */
	struct list_head		request_entry;
	/* List head at conn->async_requests */
	struct list_head		async_request_entry;
	struct work_struct		work;

	/* cancel works */
	int				async_id;
	void				**cancel_argv;
	void				(*cancel_fn)(void **argv);
	struct list_head		fp_entry;
	struct list_head		interim_entry;
};

#define RESPONSE_BUF(w)		(void *)((w)->response_buf)
#define RESPONSE_SZ(w)		((w)->response_sz)

#define REQUEST_BUF(w)		(void *)((w)->request_buf)

#define INIT_AUX_PAYLOAD(w)	((w)->aux_payload_buf = NULL)
#define HAS_AUX_PAYLOAD(w)	((w)->aux_payload_sz != 0)
#define AUX_PAYLOAD(w)		(void *)((w)->aux_payload_buf)
#define AUX_PAYLOAD_SIZE(w)	((w)->aux_payload_sz)
#define RESP_HDR_SIZE(w)	((w)->resp_hdr_sz)

#define HAS_TRANSFORM_BUF(w)	((w)->tr_buf != NULL)
#define TRANSFORM_BUF(w)	(void *)((w)->tr_buf)

#define CIFS_DEFAULT_IOSIZE	(64 * 1024)

#ifndef cifsd_pr_fmt
#ifdef SUBMOD_NAME
#define cifsd_pr_fmt(fmt)	"kcifsd: " SUBMOD_NAME ": " fmt
#else
#define cifsd_pr_fmt(fmt)	"kcifsd: " fmt
#endif
#endif

#define cifsd_debug(fmt, ...)					\
	do {							\
		if (cifsd_debugging)				\
			pr_info(cifsd_pr_fmt("%s:%d: " fmt),	\
				__func__,			\
				__LINE__,			\
				##__VA_ARGS__);			\
	} while (0)

#define cifsd_info(fmt, ...)					\
			pr_info(cifsd_pr_fmt(fmt), ##__VA_ARGS__)

#define cifsd_err(fmt, ...) 					\
			pr_err(cifsd_pr_fmt("%s:%d: " fmt),	\
				__func__,			\
				__LINE__,			\
				##__VA_ARGS__)

#define UNICODE_LEN(x)		((x) * 2)

/* @FIXME clean up this code */
/* @FIXME clean up this code */
/* @FIXME clean up this code */

/* cifsd misc functions */
extern void ntstatus_to_dos(__u32 ntstatus, __u8 *eclass, __u16 *ecode);
#endif /* __CIFSD_GLOB_H */

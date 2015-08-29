/**
 * @file   core.c
 * @author Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 *
 * Copyright (C) 2013 - 2014  Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "core.h"
#include "util.h"

#define TOX_ENABLE_IPV6_DEFAULT true

#include <arpa/inet.h>

#if PY_MAJOR_VERSION < 3
# define BUF_TC "s"
#else
# define BUF_TC "y"
#endif

extern PyObject* ToxOpError;

static void callback_friend_request(Tox* tox, const uint8_t* public_key,
    const uint8_t* data, size_t length, void* self)
{
  uint8_t buf[TOX_PUBLIC_KEY_SIZE * 2 + 1];
  memset(buf, 0, TOX_PUBLIC_KEY_SIZE * 2 + 1);

  bytes_to_hex_string(public_key, TOX_PUBLIC_KEY_SIZE, buf);

  PyObject_CallMethod((PyObject*)self, "on_friend_request", "ss#", buf, data,
      length - (data[length - 1] == 0));
}

static void callback_friend_message(Tox *tox, uint32_t friendnumber, TOX_MESSAGE_TYPE type,
    const uint8_t* message, size_t length, void* self)
{
    PyObject_CallMethod((PyObject*)self, "on_friend_message", "iis#", friendnumber, type,
                        message, length - (message[length - 1] == 0));
}

/*
static void callback_action(Tox *tox, int32_t friendnumber,
    const uint8_t* action, size_t length, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_friend_action", "is#", friendnumber,
      action, length - (action[length - 1] == 0));
}
*/

static void callback_name_change(Tox *tox, uint32_t friendnumber,
    const uint8_t* newname, size_t length, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_name_change", "is#", friendnumber,
      newname, length - (newname[length - 1] == 0));
}

static void callback_status_message(Tox *tox, uint32_t friendnumber,
    const uint8_t *newstatus, size_t length, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_status_message", "is#", friendnumber,
      newstatus, length - (newstatus[length - 1] == 0));
}

static void callback_user_status(Tox *tox, uint32_t friendnumber, TOX_USER_STATUS status,
                                 void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_user_status", "ii", friendnumber,
      status);
}

static void callback_typing_change(Tox *tox, uint32_t friendnumber,
    bool is_typing, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_friend_typing", "iO", friendnumber,
      PyBool_FromLong(is_typing));
}

static void callback_read_receipt(Tox *tox, uint32_t friendnumber,
    uint32_t receipt, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_read_receipt", "ii", friendnumber,
      receipt);
}

static void callback_friend_connection_status(Tox *tox, uint32_t friendnumber,
    TOX_CONNECTION status, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_friend_connection_status", "iO",
      friendnumber, PyBool_FromLong(status));
}

#ifndef LEGACY_GROUPCHATS

static void callback_group_peer_name(Tox *tox, uint32_t group_number, uint32_t peer_number,
                                     const uint8_t *name, size_t length, void *self)
{
  PyObject_CallMethod((PyObject*)self, "on_group_peer_name", "iis#",
                      group_number, peer_number, name, length);
}

static void callback_group_peer_status(Tox *tox, uint32_t group_number, uint32_t peer_number,
                                       TOX_USER_STATUS status, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_peer_name", "iii",
                        group_number, peer_number, status);
}

static void callback_group_topic(Tox *tox, uint32_t group_number, uint32_t peer_number,
                                 const uint8_t *topic, size_t length, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_topic", "iis#",
                        group_number, peer_number, topic, length);
}

static void callback_group_privacy_state(Tox *tox, uint32_t group_number, TOX_GROUP_PRIVACY_STATE privacy_state,
                                         void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_privacy_state", "ii",
                        group_number, privacy_state);
}

static void callback_group_peer_limit(Tox *tox, uint32_t group_number, uint32_t peer_limit,
                                      void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_peer_limit", "ii",
                        group_number, peer_limit);
}

static void callback_group_password(Tox *tox, uint32_t group_number, const uint8_t *password, size_t length,
                                      void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_password", "is#",
                        group_number, password, length);
}

static void callback_group_peerlist_update(Tox *tox, uint32_t group_number, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_peerlist_update", "i", group_number);
}

static void callback_group_message(Tox *tox, uint32_t group_number, uint32_t peer_number, TOX_MESSAGE_TYPE type,
                                   const uint8_t *message, size_t length, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_message", "iiis#",
                        group_number, peer_number, type, message, length);
}

static void callback_group_private_message(Tox *tox, uint32_t group_number, uint32_t peer_number,
                                   const uint8_t *message, size_t length, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_private_message", "iis#",
                        group_number, peer_number, message, length);
}

static void callback_group_invite(Tox *tox, uint32_t group_number, const uint8_t *invite_data, size_t length,
                                  void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_invite", "is#",
                        group_number, invite_data, length);
}

static void callback_group_peer_join(Tox *tox, uint32_t group_number, uint32_t peer_number, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_peer_join", "ii",
                        group_number, peer_number);
}

static void callback_group_peer_exit(Tox *tox, uint32_t group_number, uint32_t peer_number,
                                     const uint8_t *part_message, size_t length, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_peer_exit", "iis#",
                        group_number, peer_number, part_message, length);
}

static void callback_group_self_join(Tox *tox, uint32_t group_number, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_self_join", "i",
                        group_number);
}

static void callback_group_join_fail(Tox *tox, uint32_t group_number, TOX_GROUP_JOIN_FAIL fail_type, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_join_fail", "ii",
                        group_number, fail_type);
}

static void callback_group_moderation(Tox *tox, uint32_t group_number, uint32_t source_peer_number,
                                     uint32_t target_peer_number, TOX_GROUP_MOD_EVENT mod_type, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_group_moderation", "iiii",
                        group_number, source_peer_number, target_peer_number, mod_type);
}


#else
// TODO old api
static void callback_group_invite(Tox *tox, int32_t friendnumber, uint8_t type,
    const uint8_t *data, uint16_t length, void *self)
{
  PyObject_CallMethod((PyObject*)self, "on_group_invite", "ii" BUF_TC "#",
      friendnumber, type, data, length);
}
// TODO old api
static void callback_group_message(Tox *tox, int groupid,
    int friendgroupid, const uint8_t* message, uint16_t length, void *self)
{
  PyObject_CallMethod((PyObject*)self, "on_group_message", "iis#", groupid,
      friendgroupid, message, length - (message[length - 1] == 0));
}
// TODO old api
static void callback_group_action(Tox *tox, int groupid,
    int friendgroupid, const uint8_t* action, uint16_t length, void *self)
{
  PyObject_CallMethod((PyObject*)self, "on_group_action", "iis#", groupid,
      friendgroupid, action, length - (action[length - 1] == 0));
}

static void callback_group_namelist_change(Tox *tox, int groupid,
    int peernumber, uint8_t change, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_group_namelist_change", "iii",
      groupid, peernumber, change);
}
#endif  // -DNEW_GROUPCHATS
static void callback_file_chunk_request(Tox *tox, uint32_t friend_number, uint32_t file_number,
                                        uint64_t position, size_t length, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_file_chunk_request", "iiKi",
                        friend_number, file_number, position, length);
}


static void callback_file_recv(Tox *tox, uint32_t friend_number, uint32_t file_number, uint32_t kind,
                               uint64_t file_size,
                               const uint8_t *filename, size_t filename_length, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_file_recv", "iiiKs#",
                        friend_number, file_number, kind, file_size, filename, filename_length);
}

static void callback_file_recv_control(Tox *tox, uint32_t friend_number, uint32_t file_number,
                                       TOX_FILE_CONTROL control, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_file_recv_control", "iii",
                        friend_number, file_number, control);
}

static void callback_file_recv_chunk(Tox *tox, uint32_t friend_number, uint32_t file_number,
                                     uint64_t position,
                                     const uint8_t *data, size_t length, void *self)
{
    PyObject_CallMethod((PyObject*)self, "on_file_recv_chunk", "iiK" BUF_TC "#",
                        friend_number, file_number, position, data, length);
}

/*
static void callback_file_send_request(Tox *m, uint32_t friendnumber,
    uint8_t filenumber, uint64_t filesize, const uint8_t* filename,
    size_t filename_length, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_file_send_request", "iiKs#",
      friendnumber, filenumber, filesize, filename,
      filename_length - (filename[filename_length - 1] == 0));
}
*/
/*
static void callback_file_control(Tox *m, uint32_t friendnumber,
    uint8_t receive_send, uint8_t filenumber, uint8_t control_type,
    const uint8_t* data, size_t length, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_file_control", "iiii" BUF_TC "#",
      friendnumber, receive_send, filenumber, control_type, data, length);
}
*/
/*
static void callback_file_data(Tox *m, uint32_t friendnumber, uint8_t filenumber,
    const uint8_t* data, size_t length, void* self)
{
  PyObject_CallMethod((PyObject*)self, "on_file_data", "ii" BUF_TC "#",
      friendnumber, filenumber, data, length);
}
*/


static void init_options(PyObject* pyopts, struct Tox_Options* tox_opts)
{
    char *buf = NULL;
    Py_ssize_t sz = 0;
    PyObject *p = NULL;
    
    p = PyObject_GetAttrString(pyopts, "savedata_data");
    PyBytes_AsStringAndSize(p, &buf, &sz);
    if (sz > 0) {
        tox_opts->savedata_data = calloc(1, sz);
        memcpy((void*)tox_opts->savedata_data, buf, sz);
        tox_opts->savedata_length = sz;
        tox_opts->savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
    }

    p = PyObject_GetAttrString(pyopts, "proxy_host");
    PyStringUnicode_AsStringAndSize(p, &buf, &sz);
    if (sz > 0) {
        tox_opts->proxy_host = calloc(1, sz);
        memcpy((void*)tox_opts->proxy_host, buf, sz);
    }
    
    p = PyObject_GetAttrString(pyopts, "proxy_port");
    if (p) {
        tox_opts->proxy_port = PyLong_AsLong(p);
    }

    p = PyObject_GetAttrString(pyopts, "proxy_type");
    if (p) {
        tox_opts->proxy_type = PyLong_AsLong(p);
    }
    
    p = PyObject_GetAttrString(pyopts, "ipv6_enabled");
    if (p) {
        tox_opts->ipv6_enabled = p == Py_True;
    }

    p = PyObject_GetAttrString(pyopts, "udp_enabled");
    if (p) {
        tox_opts->udp_enabled = p == Py_True;
    }

    p = PyObject_GetAttrString(pyopts, "start_port");
    if (p) {
        tox_opts->start_port = PyLong_AsLong(p);
    }

    p = PyObject_GetAttrString(pyopts, "end_port");
    if (p) {
        tox_opts->end_port = PyLong_AsLong(p);
    }

    p = PyObject_GetAttrString(pyopts, "tcp_port");
    if (p) {
        tox_opts->tcp_port = PyLong_AsLong(p);
    }
}

static int init_helper(ToxCore* self, PyObject* args)
{
  if (self->tox != NULL) {
    tox_kill(self->tox);
    self->tox = NULL;
  }

  PyObject *opts = NULL;

  if (args) {
      if (!PyArg_ParseTuple(args, "O", &opts)) {
          PyErr_SetString(PyExc_TypeError, "must supply a ToxOptions param");
          return -1;
      }
  }

  struct Tox_Options options = {0};
  tox_options_default(&options);

  if (opts != NULL) {
      init_options(opts, &options);
  }

  TOX_ERR_NEW err = 0;
  Tox* tox = tox_new(&options, &err);
  
  if (tox == NULL) {
      PyErr_Format(ToxOpError, "failed to initialize toxcore: %d", err);
      return -1;
  }

  tox_callback_friend_request(tox, callback_friend_request, self);
  tox_callback_friend_message(tox, callback_friend_message, self);
  // tox_callback_friend_action(tox, callback_action, self);
  // tox_callback_name_change(tox, callback_name_change, self);
  tox_callback_friend_name(tox, callback_name_change, self);
  // tox_callback_status_message(tox, callback_status_message, self);
  tox_callback_friend_status_message(tox, callback_status_message, self);
  // tox_callback_user_status(tox, callback_user_status, self);
  tox_callback_friend_status(tox, callback_user_status, self);
  // tox_callback_typing_change(tox, callback_typing_change, self);
  tox_callback_friend_typing(tox, callback_typing_change, self);
  // tox_callback_read_receipt(tox, callback_read_receipt, self);
  tox_callback_friend_read_receipt(tox, callback_read_receipt, self);
  // tox_callback_connection_status(tox, callback_connection_status, self);
  tox_callback_friend_connection_status(tox, callback_friend_connection_status, self);

#ifndef LEGACY_GROUPCHATS

  tox_callback_group_peer_name(tox, callback_group_peer_name, self);
  tox_callback_group_peer_status(tox, callback_group_peer_status, self);
  tox_callback_group_topic(tox, callback_group_topic, self);
  tox_callback_group_privacy_state(tox, callback_group_privacy_state, self);
  tox_callback_group_peer_limit(tox, callback_group_peer_limit, self);
  tox_callback_group_password(tox, callback_group_password, self);
  tox_callback_group_peerlist_update(tox, callback_group_peerlist_update, self);
  tox_callback_group_message(tox, callback_group_message, self);
  tox_callback_group_private_message(tox, callback_group_private_message, self);
  tox_callback_group_invite(tox, callback_group_invite, self);
  tox_callback_group_peer_join(tox, callback_group_peer_join, self);
  tox_callback_group_peer_exit(tox, callback_group_peer_exit, self);
  tox_callback_group_self_join(tox, callback_group_self_join, self);
  tox_callback_group_join_fail(tox, callback_group_join_fail, self);
  tox_callback_group_moderation(tox, callback_group_moderation, self);
  
#else
  tox_callback_group_invite(tox, callback_group_invite, self);
  tox_callback_group_message(tox, callback_group_message, self);
  tox_callback_group_action(tox, callback_group_action, self);
  tox_callback_group_namelist_change(tox, callback_group_namelist_change, self);
#endif // -DNEW_GROUPCHATS
  // tox_callback_file_send_request(tox, callback_file_send_request, self);
  // tox_callback_file_control(tox, callback_file_control, self);
  // tox_callback_file_data(tox, callback_file_data, self);
  tox_callback_file_chunk_request(tox, callback_file_chunk_request, self);
  tox_callback_file_recv_control(tox, callback_file_recv_control, self);
  tox_callback_file_recv(tox, callback_file_recv, self);
  tox_callback_file_recv_chunk(tox, callback_file_recv_chunk, self);
  

  self->tox = tox;

  return 0;
}

static PyObject*
ToxCore_new(PyTypeObject *type, PyObject* args, PyObject* kwds)
{
  ToxCore* self = (ToxCore*)type->tp_alloc(type, 0);
  self->tox = NULL;

  // We don't care about subclass's arguments
  if (init_helper(self, NULL) == -1) {
    return NULL;
  }

  return (PyObject*)self;
}

static int ToxCore_init(ToxCore* self, PyObject* args, PyObject* kwds)
{
  // since __init__ in Python is optional(superclass need to call it
  // explicitly), we need to initialize self->tox in ToxCore_new instead of
  // init. If ToxCore_init is called, we re-initialize self->tox and pass
  // the new ipv6enabled setting.
  return init_helper(self, args);
}

static int
ToxCore_dealloc(ToxCore* self)
{
  if (self->tox) {
    tox_kill(self->tox);
    self->tox = NULL;
  }
  return 0;
}

static PyObject*
ToxCore_callback_stub(ToxCore* self, PyObject* args)
{
  Py_RETURN_NONE;
}

static PyObject*
ToxCore_self_get_address(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t address[TOX_ADDRESS_SIZE];
  uint8_t address_hex[TOX_ADDRESS_SIZE * 2 + 1];
  memset(address_hex, 0, TOX_ADDRESS_SIZE * 2 + 1);

  tox_self_get_address(self->tox, address);
  bytes_to_hex_string(address, TOX_ADDRESS_SIZE, address_hex);

  return PYSTRING_FromString((const char*)address_hex);
}

static PyObject*
ToxCore_friend_add(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t* address = NULL;
  int addr_length = 0;
  uint8_t* data = NULL;
  int data_length = 0;

  if (!PyArg_ParseTuple(args, "s#s#", &address, &addr_length, &data,
        &data_length)) {
    return NULL;
  }

  uint8_t pk[TOX_ADDRESS_SIZE];
  hex_string_to_bytes(address, TOX_ADDRESS_SIZE, pk);

  TOX_ERR_FRIEND_ADD err = 0;
  uint32_t friend_number = 0;
  friend_number = tox_friend_add(self->tox, pk, data, data_length, &err);
  int success = friend_number == UINT32_MAX ? 0 : 1;
  
  switch (err) {
  case TOX_ERR_FRIEND_ADD_TOO_LONG:
    PyErr_SetString(ToxOpError, "message too long");
    break;
  case TOX_ERR_FRIEND_ADD_NO_MESSAGE:
    PyErr_SetString(ToxOpError, "no message specified");
    break;
  case TOX_ERR_FRIEND_ADD_OWN_KEY:
    PyErr_SetString(ToxOpError, "user's own key");
    break;
  case TOX_ERR_FRIEND_ADD_ALREADY_SENT:
    PyErr_SetString(ToxOpError, "friend request already sent or already "
        "a friend");
    break;
  case TOX_ERR_FRIEND_ADD_NULL:
    PyErr_SetString(ToxOpError, "unknown error");
    break;
  case TOX_ERR_FRIEND_ADD_BAD_CHECKSUM:
    PyErr_SetString(ToxOpError, "bad checksum in address");
    break;
  case TOX_ERR_FRIEND_ADD_SET_NEW_NOSPAM:
    PyErr_SetString(ToxOpError, "the friend was already there but the "
        "nospam was different");
    break;
  case TOX_ERR_FRIEND_ADD_MALLOC:
    PyErr_SetString(ToxOpError, "increasing the friend list size fails");
    break;
  default:
      // success = 1;
      break;
  }

  if (success) {
    return PyLong_FromLong(friend_number);
  } else {
    return NULL;
  }
}

static PyObject*
ToxCore_friend_add_norequest(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t* address = NULL;
  int addr_length = 0;

  if (!PyArg_ParseTuple(args, "s#", &address, &addr_length)) {
    return NULL;
  }

  uint8_t pk[TOX_ADDRESS_SIZE];
  hex_string_to_bytes(address, TOX_ADDRESS_SIZE, pk);

  TOX_ERR_FRIEND_ADD err = 0;
  int res = tox_friend_add_norequest(self->tox, pk, &err);
  if (res == -1) {
    // PyErr_SetString(ToxOpError, "failed to add friend");
    PyErr_Format(ToxOpError, "failed to add friend: %d", err);
    return NULL;
  }

  return PyLong_FromLong(res);
}

static PyObject*
ToxCore_friend_by_public_key(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t* id = NULL;
  int addr_length = 0;

  if (!PyArg_ParseTuple(args, "s#", &id, &addr_length)) {
    return NULL;
  }

  uint8_t pk[TOX_PUBLIC_KEY_SIZE];
  hex_string_to_bytes(id, TOX_PUBLIC_KEY_SIZE, pk);

  int ret = tox_friend_by_public_key(self->tox, pk, NULL);
  if (ret == -1) {
    PyErr_SetString(ToxOpError, "no such friend");
    return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_friend_get_public_key(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t pk[TOX_PUBLIC_KEY_SIZE + 1];
  pk[TOX_PUBLIC_KEY_SIZE] = 0;

  uint8_t hex[TOX_PUBLIC_KEY_SIZE * 2 + 1];
  memset(hex, 0, TOX_PUBLIC_KEY_SIZE * 2 + 1);

  int friend_num = 0;

  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  tox_friend_get_public_key(self->tox, friend_num, pk, NULL);
  bytes_to_hex_string(pk, TOX_PUBLIC_KEY_SIZE, hex);

  return PYSTRING_FromString((const char*)hex);
}

static PyObject*
ToxCore_friend_delete(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;

  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  if (tox_friend_delete(self->tox, friend_num, NULL) == false) {
    PyErr_SetString(ToxOpError, "failed to delete friend");
    return NULL;
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_friend_get_connection_status(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;

  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  int ret = tox_friend_get_connection_status(self->tox, friend_num, NULL);
  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to get connection status");
    return NULL;
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_friend_exists(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;

  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  int ret = tox_friend_exists(self->tox, friend_num);

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_friend_send_message(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;
  int msg_type = 0;
  int length = 0;
  uint8_t* message = NULL;

  if (!PyArg_ParseTuple(args, "iis#", &friend_num, &msg_type, &message, &length)) {
    return NULL;
  }

  TOX_MESSAGE_TYPE type = msg_type;
  TOX_ERR_FRIEND_SEND_MESSAGE errmsg = 0;
  uint32_t ret = tox_friend_send_message(self->tox, friend_num, type, message, length, &errmsg);
  if (ret == 0) {
     PyErr_SetString(ToxOpError, "failed to send message");
    return NULL;
  }

  return PyLong_FromUnsignedLong(ret);
}

/* 合并到send_message
static PyObject*
ToxCore_send_action(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;
  int length = 0;
  uint8_t* action = NULL;

  if (!PyArg_ParseTuple(args, "is#", &friend_num, &action, &length)) {
    return NULL;
  }

  uint32_t ret = tox_send_action(self->tox, friend_num, action, length);
  if (ret == 0) {
    PyErr_SetString(ToxOpError, "failed to send action");
    return NULL;
  }

  return PyLong_FromUnsignedLong(ret);
}
*/

static PyObject*
ToxCore_self_set_name(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t* name = 0;
  int length = 0;

  if (!PyArg_ParseTuple(args, "s#", &name, &length)) {
    return NULL;
  }

  if (tox_self_set_name(self->tox, name, length, NULL) == false) {
    PyErr_SetString(ToxOpError, "failed to set_name");
    return NULL;
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_self_get_name(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t buf[TOX_MAX_NAME_LENGTH];
  memset(buf, 0, TOX_MAX_NAME_LENGTH);

  tox_self_get_name(self->tox, buf);
  if (buf == 0) {
    PyErr_SetString(ToxOpError, "failed to get self name");
    return NULL;
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_self_get_name_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int ret = tox_self_get_name_size(self->tox);
  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to get self name size");
    return NULL;
  }

  return PyLong_FromUnsignedLong(ret);
}

static PyObject*
ToxCore_friend_get_name(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;
  uint8_t buf[TOX_MAX_NAME_LENGTH];
  memset(buf, 0, TOX_MAX_NAME_LENGTH);

  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  if (tox_friend_get_name(self->tox, friend_num, buf, NULL) == false) {
    PyErr_SetString(ToxOpError, "failed to get name");
    return NULL;
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_friend_get_name_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;

  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  int ret = tox_friend_get_name_size(self->tox, friend_num, NULL);
  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to get name size");
    return NULL;
  }

  return PyLong_FromUnsignedLong(ret);
}

static PyObject*
ToxCore_self_set_status_message(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t* message;
  int length;
  if (!PyArg_ParseTuple(args, "s#", &message, &length)) {
    return NULL;
  }

  if (tox_self_set_status_message(self->tox, message, length, NULL) == false) {
    PyErr_SetString(ToxOpError, "failed to set status_message");
    return NULL;
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_self_set_status(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int status = 0;

  if (!PyArg_ParseTuple(args, "i", &status)) {
    return NULL;
  }

  tox_self_set_status(self->tox, status);
  if (true == false) {
    PyErr_SetString(ToxOpError, "failed to set status");
    return NULL;
  }

  Py_RETURN_NONE;
}


static PyObject*
ToxCore_friend_get_status_message_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;
  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  int ret = tox_friend_get_status_message_size(self->tox, friend_num, NULL);
  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_friend_get_status_message(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t buf[TOX_MAX_STATUS_MESSAGE_LENGTH];
  int friend_num = 0;

  memset(buf, 0, TOX_MAX_STATUS_MESSAGE_LENGTH);

  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  int ret = tox_friend_get_status_message(self->tox, friend_num, buf, NULL);

  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to get status_message");
    return NULL;
  }

  buf[TOX_MAX_STATUS_MESSAGE_LENGTH -1] = 0;

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_self_get_status_message(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t buf[TOX_MAX_STATUS_MESSAGE_LENGTH];
  memset(buf, 0, TOX_MAX_STATUS_MESSAGE_LENGTH);

  int ret = 0;
  tox_self_get_status_message(self->tox, buf);

  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to get self status_message");
    return NULL;
  }

  buf[TOX_MAX_STATUS_MESSAGE_LENGTH -1] = 0;

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_self_get_status_message_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int ret = tox_self_get_status_message_size(self->tox);
  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to get self status_message size");
    return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_friend_get_status(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;
  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  int status = tox_friend_get_status(self->tox, friend_num, NULL);

  return PyLong_FromLong(status);
}

static PyObject*
ToxCore_self_get_status(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int status = tox_self_get_status(self->tox);
  return PyLong_FromLong(status);
}

static PyObject*
ToxCore_friend_get_last_online(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;
  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  uint64_t status = tox_friend_get_last_online(self->tox, friend_num, NULL);

  if (status == 0) {
    Py_RETURN_NONE;
  }

  PyObject* datetime = PyImport_ImportModule("datetime");
  PyObject* datetimeClass = PyObject_GetAttrString(datetime, "datetime");
  PyObject* ret = PyObject_CallMethod(datetimeClass, "fromtimestamp", "K",
      status);

  return ret;
}

static PyObject*
ToxCore_self_set_typing(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;
  int is_typing = 0;

  if (!PyArg_ParseTuple(args, "ii", &friend_num, &is_typing)) {
    return NULL;
  }

  int status = tox_self_set_typing(self->tox, friend_num, is_typing, NULL);
  return PyLong_FromLong(status);
}

static PyObject*
ToxCore_friend_get_typing(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int friend_num = 0;

  if (!PyArg_ParseTuple(args, "i", &friend_num)) {
    return NULL;
  }

  if (tox_friend_get_typing(self->tox, friend_num, NULL)) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}

static PyObject*
ToxCore_self_get_friend_list_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t count = tox_self_get_friend_list_size(self->tox);
  return PyLong_FromUnsignedLong(count);
}

/*
static PyObject*
ToxCore_get_num_online_friends(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t count = tox_get_num_online_friends(self->tox);
  return PyLong_FromUnsignedLong(count);
}
*/

static PyObject*
ToxCore_self_get_friend_list(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  PyObject* plist = NULL;
  uint32_t count = tox_self_get_friend_list_size(self->tox);
  uint32_t* list = (uint32_t*)malloc(count * sizeof(uint32_t));

  uint32_t n = count;
  tox_self_get_friend_list(self->tox, list);

  if (!(plist = PyList_New(0))) {
    return NULL;
  }

  uint32_t i = 0;
  for (i = 0; i < n; ++i) {
    PyList_Append(plist, PyLong_FromLong(list[i]));
  }
  free(list);

  return plist;
}

#ifndef LEGACY_GROUPCHATS

static PyObject*
ToxCore_group_new(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int privacy_state = 0;
  uint8_t* group_name = NULL;
  uint32_t length = 0;
  
  if (!PyArg_ParseTuple(args, "is#", &privacy_state, group_name, &length)) {
    return NULL;
  }

  TOX_ERR_GROUP_NEW err = 0;
  uint32_t group_number = tox_group_new(self->tox, privacy_state, group_name, length, &err);
  if (group_number == UINT32_MAX) {
      PyErr_Format(ToxOpError, "failed to group new: %d", err);
  }

  return PyLong_FromLong(group_number);
}

static PyObject*
ToxCore_group_join(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t *chat_id = NULL;
  uint8_t *password = NULL;
  uint32_t length = 0;
  
  if (!PyArg_ParseTuple(args, "ss#", chat_id, password, &length)) {
    return NULL;
  }

  TOX_ERR_GROUP_JOIN err = 0;
  uint32_t group_number = tox_group_join(self->tox, chat_id, password, length, &err);
  if (group_number == UINT32_MAX) {
      PyErr_Format(ToxOpError, "failed to group join: %d", err);
  }

  return PyLong_FromLong(group_number);
}

static PyObject*
ToxCore_group_reconnect(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;

  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_RECONNECT err = 0;
  bool ret = tox_group_reconnect(self->tox, group_number, &err);
  if (!ret) {
      PyErr_Format(ToxOpError, "failed to group reconnect: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_leave(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint8_t *message = NULL;
  uint32_t length = 0;
  
  if (!PyArg_ParseTuple(args, "is#", &group_number, message, &length)) {
    return NULL;
  }

  TOX_ERR_GROUP_LEAVE err = 0;
  bool ret = tox_group_leave(self->tox, group_number, message, length, &err);
  if (!ret) {
      PyErr_Format(ToxOpError, "failed to group leave: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_self_set_name(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint8_t *name = NULL;
  uint32_t length = 0;
  
  if (!PyArg_ParseTuple(args, "is#", &group_number, name, &length)) {
    return NULL;
  }

  TOX_ERR_GROUP_LEAVE err = 0;
  bool ret = tox_group_self_set_name(self->tox, group_number, name, length, &err);
  if (!ret) {
      PyErr_Format(ToxOpError, "failed to group self set name: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_self_get_name_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_SELF_QUERY err = 0;
  uint32_t ret = tox_group_self_get_name_size(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_SELF_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group self get name size: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_self_get_name(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  uint8_t buf[TOX_GROUP_MAX_GROUP_NAME_LENGTH];
  memset(buf, 0, TOX_GROUP_MAX_GROUP_NAME_LENGTH);
  
  TOX_ERR_GROUP_SELF_QUERY err = 0;
  bool ret = tox_group_self_get_name(self->tox, group_number, buf, &err);
  if (err != TOX_ERR_GROUP_SELF_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group self get name: %d", err);
      return NULL;
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_group_self_set_status(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t status = 0;
  
  if (!PyArg_ParseTuple(args, "ii", &group_number, &status)) {
    return NULL;
  }

  TOX_ERR_GROUP_SELF_STATUS_SET err = 0;
  bool ret = tox_group_self_set_status(self->tox, group_number, status, &err);
  if (!ret) {
      PyErr_Format(ToxOpError, "failed to group self set status: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_self_get_status(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  TOX_USER_STATUS status = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_SELF_QUERY err = 0;
  status = tox_group_self_get_status(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_SELF_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group self get status: %d", err);
  }

  return PyLong_FromLong(status);
}

static PyObject*
ToxCore_group_self_get_role(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  TOX_GROUP_ROLE role = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_SELF_QUERY err = 0;
  role = tox_group_self_get_role(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_SELF_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group self get role: %d", err);
  }

  return PyLong_FromLong(role);
}

static PyObject*
ToxCore_group_peer_get_name_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t peer_number = 0;
  
  if (!PyArg_ParseTuple(args, "ii", &group_number, &peer_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_PEER_QUERY err = 0;
  uint32_t ret = tox_group_peer_get_name_size(self->tox, group_number, peer_number, &err);
  if (err != TOX_ERR_GROUP_PEER_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group peer get name size: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_peer_get_name(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t peer_number = 0;
  
  if (!PyArg_ParseTuple(args, "ii", &group_number, &peer_number)) {
    return NULL;
  }

  uint8_t buf[TOX_MAX_NAME_LENGTH];
  memset(buf, 0, TOX_MAX_NAME_LENGTH);
  
  TOX_ERR_GROUP_PEER_QUERY err = 0;
  bool ret = tox_group_peer_get_name(self->tox, group_number, peer_number, buf, &err);
  if (err != TOX_ERR_GROUP_PEER_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group peer get name: %d", err);
      return NULL;
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_group_peer_get_status(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t peer_number = 0;
  uint32_t status = 0;
  
  if (!PyArg_ParseTuple(args, "ii", &group_number, &peer_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_PEER_QUERY err = 0;
  status = tox_group_peer_get_status(self->tox, group_number, peer_number, &err);
  if (err != TOX_ERR_GROUP_PEER_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group peer get status: %d", err);
      return NULL;
  }

  return PyLong_FromLong(status);
}

static PyObject*
ToxCore_group_peer_get_role(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t peer_number = 0;
  uint32_t role = 0;
  
  if (!PyArg_ParseTuple(args, "ii", &group_number, &peer_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_PEER_QUERY err = 0;
  role = tox_group_peer_get_role(self->tox, group_number, peer_number, &err);
  if (err != TOX_ERR_GROUP_PEER_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group peer get role: %d", err);
      return NULL;
  }

  return PyLong_FromLong(role);
}

static PyObject*
ToxCore_group_set_topic(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint8_t *topic = NULL;
  uint32_t length = 0;
  
  if (!PyArg_ParseTuple(args, "is#", &group_number, topic, &length)) {
    return NULL;
  }

  TOX_ERR_GROUP_TOPIC_SET err = 0;
  bool ret = tox_group_set_topic(self->tox, group_number, topic, length, &err);
  if (err != TOX_ERR_GROUP_TOPIC_SET_OK) {
      PyErr_Format(ToxOpError, "failed to group set topic: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_get_topic_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  uint32_t ret = tox_group_get_topic_size(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get topic size: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_get_topic(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  uint8_t buf[TOX_GROUP_MAX_TOPIC_LENGTH];
  memset(buf, 0, TOX_GROUP_MAX_TOPIC_LENGTH);
  
  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  bool ret = tox_group_get_topic(self->tox, group_number, buf, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get topic: %d", err);
      return NULL;
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_group_get_name_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  uint32_t ret = tox_group_get_name_size(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get name size: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_get_name(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  uint8_t buf[TOX_GROUP_MAX_GROUP_NAME_LENGTH];
  memset(buf, 0, TOX_GROUP_MAX_GROUP_NAME_LENGTH);
  
  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  bool ret = tox_group_get_name(self->tox, group_number, buf, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get name: %d", err);
      return NULL;
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_group_get_chat_id(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  uint8_t buf[TOX_GROUP_CHAT_ID_SIZE];
  memset(buf, 0, TOX_GROUP_CHAT_ID_SIZE);
  
  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  bool ret = tox_group_get_chat_id(self->tox, group_number, buf, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get chat id: %d", err);
      return NULL;
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_group_get_number_peers(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  uint32_t ret = tox_group_get_number_peers(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get number peers: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_get_number_groups(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t ret = tox_group_get_number_groups(self->tox);
  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_get_privacy_state(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  TOX_GROUP_PRIVACY_STATE ret = tox_group_get_number_peers(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get privacy state: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_get_peer_limit(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  uint32_t ret = tox_group_get_peer_limit(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get peer limit: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

size_t tox_group_get_password_size(const Tox *tox, uint32_t groupnumber, TOX_ERR_GROUP_STATE_QUERIES *error);
static PyObject*
ToxCore_group_get_password_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  uint32_t ret = tox_group_get_password_size(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get password size: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_get_password(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  
  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  uint8_t buf[TOX_GROUP_MAX_PASSWORD_SIZE];
  memset(buf, 0, TOX_GROUP_MAX_PASSWORD_SIZE);
  
  TOX_ERR_GROUP_STATE_QUERIES err = 0;
  bool ret = tox_group_get_password(self->tox, group_number, buf, &err);
  if (err != TOX_ERR_GROUP_STATE_QUERIES_OK) {
      PyErr_Format(ToxOpError, "failed to group get password: %d", err);
      return NULL;
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_group_send_message(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t type = 0;
  uint8_t *message = NULL;
  uint32_t length = 0;
  
  if (!PyArg_ParseTuple(args, "iis#", &group_number, &type, message, &length)) {
    return NULL;
  }

  TOX_ERR_GROUP_SEND_MESSAGE err = 0;
  bool ret = tox_group_send_message(self->tox, group_number, type, message, length, &err);
  if (err != TOX_ERR_GROUP_SEND_MESSAGE_OK) {
      PyErr_Format(ToxOpError, "failed to group send message: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_send_private_message(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t peer_number = 0;
  uint8_t *message = NULL;
  uint32_t length = 0;
  
  if (!PyArg_ParseTuple(args, "iis#", &group_number, &peer_number, message, &length)) {
    return NULL;
  }

  TOX_ERR_GROUP_SEND_PRIVATE_MESSAGE err = 0;
  bool ret = tox_group_send_private_message(self->tox, group_number, peer_number, message, length, &err);
  if (err != TOX_ERR_GROUP_SEND_PRIVATE_MESSAGE_OK) {
      PyErr_Format(ToxOpError, "failed to group send private message: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_invite_friend(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t friend_number = 0;
  
  if (!PyArg_ParseTuple(args, "ii", &group_number, &friend_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_INVITE_FRIEND err = 0;
  bool ret = tox_group_send_invite_friend(self->tox, group_number, friend_number, &err);
  if (err != TOX_ERR_GROUP_INVITE_FRIEND_OK) {
      PyErr_Format(ToxOpError, "failed to group invite friend: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_invite_accept(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t *invite_data = NULL;
  uint32_t invite_data_length = 0;
  uint8_t *password = NULL;
  uint32_t password_length = 0;
  
  if (!PyArg_ParseTuple(args, "s#s#", invite_data, &invite_data_length, password, &password_length)) {
    return NULL;
  }

  TOX_ERR_GROUP_INVITE_ACCEPT err = 0;
  bool ret = tox_group_send_invite_accept(self->tox, invite_data, invite_data_length, password, password_length, &err);
  if (err != TOX_ERR_GROUP_INVITE_ACCEPT_OK) {
      PyErr_Format(ToxOpError, "failed to group invite accept: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_founder_set_password(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint8_t *password = NULL;
  uint32_t password_length = 0;
  
  if (!PyArg_ParseTuple(args, "is#", &group_number, password, &password_length)) {
    return NULL;
  }

  TOX_ERR_GROUP_FOUNDER_SET_PASSWORD err = 0;
  bool ret = tox_group_founder_set_password(self->tox, group_number, password, password_length, &err);
  if (err != TOX_ERR_GROUP_FOUNDER_SET_PASSWORD_OK) {
      PyErr_Format(ToxOpError, "failed to group founder set password: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_founder_set_privacy_state(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  TOX_GROUP_PRIVACY_STATE privacy_state = 0;
  
  if (!PyArg_ParseTuple(args, "ii", &group_number, &privacy_state)) {
    return NULL;
  }

  TOX_ERR_GROUP_FOUNDER_SET_PRIVACY_STATE err = 0;
  bool ret = tox_group_founder_set_privacy_state(self->tox, group_number, privacy_state, &err);
  if (err != TOX_ERR_GROUP_FOUNDER_SET_PRIVACY_STATE_OK) {
      PyErr_Format(ToxOpError, "failed to group founder set privacy state: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_founder_set_peer_limit(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t max_peers = 0;
  
  if (!PyArg_ParseTuple(args, "ii", &group_number, &max_peers)) {
    return NULL;
  }

  TOX_ERR_GROUP_FOUNDER_SET_PEER_LIMIT err = 0;
  bool ret = tox_group_founder_set_peer_limit(self->tox, group_number, max_peers, &err);
  if (err != TOX_ERR_GROUP_FOUNDER_SET_PEER_LIMIT_OK) {
      PyErr_Format(ToxOpError, "failed to group founder set privacy state: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_toggle_ignore(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t peer_number = 0;
  uint32_t ignore = 0;

  if (!PyArg_ParseTuple(args, "iii", &group_number, &peer_number, &ignore)) {
    return NULL;
  }

  TOX_ERR_GROUP_TOGGLE_IGNORE err = 0;
  bool ret = tox_group_toggle_ignore(self->tox, group_number, peer_number, ignore, &err);
  if (err != TOX_ERR_GROUP_TOGGLE_IGNORE_OK) {
      PyErr_Format(ToxOpError, "failed to group toggle ignore: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_mod_set_role(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t peer_number = 0;
  uint32_t role = 0;

  if (!PyArg_ParseTuple(args, "iii", &group_number, &peer_number, &role)) {
    return NULL;
  }

  TOX_ERR_GROUP_MOD_SET_ROLE err = 0;
  bool ret = tox_group_mod_set_role(self->tox, group_number, peer_number, role, &err);
  if (err != TOX_ERR_GROUP_MOD_SET_ROLE_OK) {
      PyErr_Format(ToxOpError, "failed to group mod set role: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_mod_remove_peer(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t peer_number = 0;
  uint32_t set_ban = 0;

  if (!PyArg_ParseTuple(args, "iii", &group_number, &peer_number, &set_ban)) {
    return NULL;
  }

  TOX_ERR_GROUP_MOD_REMOVE_PEER err = 0;
  bool ret = tox_group_mod_remove_peer(self->tox, group_number, peer_number, set_ban, &err);
  if (err != TOX_ERR_GROUP_MOD_REMOVE_PEER_OK) {
      PyErr_Format(ToxOpError, "failed to group mod remove peer: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_mod_remove_ban(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t ban_id = 0;

  if (!PyArg_ParseTuple(args, "ii", &group_number, &ban_id)) {
    return NULL;
  }

  TOX_ERR_GROUP_MOD_REMOVE_BAN err = 0;
  bool ret = tox_group_mod_remove_ban(self->tox, group_number, ban_id, &err);
  if (err != TOX_ERR_GROUP_MOD_REMOVE_BAN_OK) {
      PyErr_Format(ToxOpError, "failed to group mod remove peer: %d", err);
  }

  return PyBool_FromLong(ret);
}

static PyObject*
ToxCore_group_ban_get_list_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;

  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_BAN_QUERY err = 0;
  uint32_t ret = tox_group_ban_get_list_size(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_BAN_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group ban get list size: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_ban_get_list(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;

  if (!PyArg_ParseTuple(args, "i", &group_number)) {
    return NULL;
  }

  TOX_ERR_GROUP_BAN_QUERY err = 0;
  
  uint32_t n = tox_group_ban_get_list_size(self->tox, group_number, &err);
  if (err != TOX_ERR_GROUP_BAN_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group ban get list size: %d", err);
      return NULL;
  }
  
  uint32_t* list = (uint32_t*)malloc(n * sizeof(uint32_t));

  bool ret = tox_group_ban_get_list(self->tox, group_number, list, &err);
  if (err != TOX_ERR_GROUP_BAN_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group ban get list: %d", err);
      return NULL;
  }
  
  PyObject* plist = NULL;  
  if (!(plist = PyList_New(0))) {
      return NULL;
  }
  
  uint32_t i = 0;
  for (i = 0; i < n; ++i) {
      PyList_Append(plist, PyLong_FromLong(list[i]));
  }
  free(list);
  
  return plist;
}

static PyObject*
ToxCore_group_ban_get_name_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t ban_id = 0;

  if (!PyArg_ParseTuple(args, "ii", &group_number, &ban_id)) {
    return NULL;
  }

  TOX_ERR_GROUP_BAN_QUERY err = 0;
  uint32_t ret = tox_group_ban_get_name_size(self->tox, group_number, ban_id, &err);
  if (err != TOX_ERR_GROUP_BAN_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group ban get name size: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_ban_get_name(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t ban_id = 0;

  if (!PyArg_ParseTuple(args, "ii", &group_number, &ban_id)) {
    return NULL;
  }
  
  TOX_ERR_GROUP_BAN_QUERY err = 0;
  uint32_t sz = tox_group_ban_get_name_size(self->tox, group_number, ban_id, &err);
  if (err != TOX_ERR_GROUP_BAN_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group ban get name size: %d", err);
      return NULL;
  }

  uint8_t *buf = (uint8_t*)malloc(sz + 1);
  memset(buf, 0, sz + 1);
  
  bool ret = tox_group_ban_get_name(self->tox, group_number, ban_id, buf, &err);
  if (err != TOX_ERR_GROUP_BAN_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group ban get name: %d", err);
      return NULL;
  }

  PyObject *name = PYSTRING_FromString((const char*)buf);
  free(buf);
  return name;
}

static PyObject*
ToxCore_group_ban_get_time_set(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t group_number = 0;
  uint32_t ban_id = 0;

  if (!PyArg_ParseTuple(args, "ii", &group_number, &ban_id)) {
    return NULL;
  }

  TOX_ERR_GROUP_BAN_QUERY err = 0;
  uint64_t ret = tox_group_ban_get_time_set(self->tox, group_number, ban_id, &err);
  if (err != TOX_ERR_GROUP_BAN_QUERY_OK) {
      PyErr_Format(ToxOpError, "failed to group ban get time set: %d", err);
      return NULL;
  }

  return PyLong_FromLong(ret);
}


#else
static PyObject*
ToxCore_add_groupchat(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int ret = tox_add_groupchat(self->tox);
  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to add groupchat");
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_get_title(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int groupid = 0;
  if (!PyArg_ParseTuple(args, "i", &groupid)) {
    return NULL;
  }

  uint8_t buf[TOX_MAX_STATUS_MESSAGE_LENGTH];
  memset(buf, 0, TOX_MAX_STATUS_MESSAGE_LENGTH);

  int ret = tox_group_get_title(self->tox, groupid, buf,
      TOX_MAX_STATUS_MESSAGE_LENGTH);
  if (ret == -1) {
    return PYSTRING_FromString("");  // no title.
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_group_set_title(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int groupid = 0;
  uint8_t* title = NULL;
  uint32_t length = 0;

  if (!PyArg_ParseTuple(args, "is#", &groupid, &title, &length)) {
    return NULL;
  }

  if (tox_group_set_title(self->tox, groupid, title, length) == -1) {
    PyErr_SetString(ToxOpError, "failed to set the group title");
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_group_get_type(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int type = 0;
  if (!PyArg_ParseTuple(args, "i", &type)) {
    return NULL;
  }

  int ret = tox_group_get_type(self->tox, type);
  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to get group type");
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_del_groupchat(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int groupid = 0;

  if (!PyArg_ParseTuple(args, "i", &groupid)) {
    return NULL;
  }

  if (tox_del_groupchat(self->tox, groupid) == -1) {
    PyErr_SetString(ToxOpError, "failed to del groupchat");
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_group_peername(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t buf[TOX_MAX_NAME_LENGTH];
  memset(buf, 0, TOX_MAX_NAME_LENGTH);

  int groupid = 0;
  int peernumber = 0;
  if (!PyArg_ParseTuple(args, "ii", &groupid, &peernumber)) {
    return NULL;
  }

  int ret = tox_group_peername(self->tox, groupid, peernumber, buf);
  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to get group peername");
  }

  return PYSTRING_FromString((const char*)buf);
}

static PyObject*
ToxCore_invite_friend(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int32_t friendnumber = 0;
  int groupid = 0;
  if (!PyArg_ParseTuple(args, "ii", &friendnumber, &groupid)) {
    return NULL;
  }

  if (tox_invite_friend(self->tox, friendnumber, groupid) == -1) {
    PyErr_SetString(ToxOpError, "failed to invite friend");
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_join_groupchat(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t* data = NULL;
  int length = 0;
  int32_t friendnumber = 0;

  if (!PyArg_ParseTuple(args, "is#", &friendnumber, &data, &length)) {
    return NULL;
  }

  int ret = tox_join_groupchat(self->tox, friendnumber, data, length);
  if (ret == -1) {
    PyErr_SetString(ToxOpError, "failed to join group chat");
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_message_send(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int groupid = 0;
  uint8_t* message = NULL;
  uint32_t length = 0;

  if (!PyArg_ParseTuple(args, "is#", &groupid, &message, &length)) {
    return NULL;
  }

  if (tox_group_message_send(self->tox, groupid, message, length) == -1) {
    PyErr_SetString(ToxOpError, "failed to send group message");
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_group_action_send(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int groupid = 0;
  uint8_t* action = NULL;
  uint32_t length = 0;

  if (!PyArg_ParseTuple(args, "is#", &groupid, &action, &length)) {
    return NULL;
  }

  if (tox_group_action_send(self->tox, groupid, action, length) == -1) {
    PyErr_SetString(ToxOpError, "failed to send group action");
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_group_number_peers(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int groupid = 0;

  if (!PyArg_ParseTuple(args, "i", &groupid)) {
    return NULL;
  }

  int ret = tox_group_number_peers(self->tox, groupid);

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_group_get_names(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int groupid = 0;
  if (!PyArg_ParseTuple(args, "i", &groupid)) {
    return NULL;
  }

  int n = tox_group_number_peers(self->tox, groupid);
  uint16_t* lengths = (uint16_t*)malloc(sizeof(uint16_t) * n);
  uint8_t (*names)[TOX_MAX_NAME_LENGTH] = (uint8_t(*)[TOX_MAX_NAME_LENGTH])
    malloc(sizeof(uint8_t) * n * TOX_MAX_NAME_LENGTH);

  int n2 = tox_group_get_names(self->tox, groupid, names, lengths, n);
  if (n2 == -1) {
    PyErr_SetString(ToxOpError, "failed to get group member names");
    return NULL;
  }

  PyObject* list = NULL;
  if (!(list = PyList_New(0))) {
    return NULL;
  }

  int i = 0;
  for (i = 0; i < n2; ++i) {
    PyList_Append(list,
        PYSTRING_FromStringAndSize((const char*)names[i], lengths[i]));
  }

  free(names);
  free(lengths);

  return list;
}

static PyObject*
ToxCore_count_chatlist(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t n = tox_count_chatlist(self->tox);
  return PyLong_FromUnsignedLong(n);
}

static PyObject*
ToxCore_get_chatlist(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  PyObject* plist = NULL;
  uint32_t count = tox_count_chatlist(self->tox);
  int* list = (int*)malloc(count * sizeof(int));

  int n = tox_get_chatlist(self->tox, list, count);

  if (!(plist = PyList_New(0))) {
    return NULL;
  }

  int i = 0;
  for (i = 0; i < n; ++i) {
    PyList_Append(plist, PyLong_FromLong(list[i]));
  }
  free(list);

  return plist;
}
#endif // -DNEW_GROUPCHATS
static PyObject*
ToxCore_file_send(ToxCore* self, PyObject*args)
{
    CHECK_TOX(self);
    uint32_t friend_number = 0;
    uint32_t kind = 0;
    uint64_t file_size = 0;
    uint8_t* file_id = NULL;
    uint8_t* filename = 0;
    int filename_length = 0;

    if (!PyArg_ParseTuple(args, "iiKss#", &friend_number, &kind, &file_size, &file_id,
                          &filename, &filename_length)) {
        return NULL;
    }

    TOX_ERR_FILE_SEND err = 0;
    uint32_t file_number =
        tox_file_send(self->tox, friend_number, kind, file_size, file_id, filename, filename_length, &err);
    if (file_number == UINT32_MAX) {
        PyErr_Format(ToxOpError, "tox_file_send() failed: %d", err);
        return NULL;
    }

    return PyLong_FromLong(file_number);
}

static PyObject*
ToxCore_file_control(ToxCore* self, PyObject* args)
{
    CHECK_TOX(self);
    uint32_t friend_number = 0;
    uint32_t file_number = 0;
    int control = 0;

    if (!PyArg_ParseTuple(args, "iii", &friend_number, &file_number, &control)) {
        return NULL;
    }

    bool ret = tox_file_control(self->tox, friend_number, file_number, control, NULL);

    if (ret) Py_RETURN_TRUE;
    PyErr_SetString(ToxOpError, "tox_file_control() failed");
    Py_RETURN_FALSE;
}

static PyObject*
ToxCore_file_send_chunk(ToxCore* self, PyObject* args)
{
    CHECK_TOX(self);
    uint32_t friend_number = 0;
    uint32_t file_number = 0;
    uint64_t position = 0;
    uint8_t *data = 0;
    size_t length = 0;

    if (!PyArg_ParseTuple(args, "iiKs#", &friend_number, &file_number, &position,
                          &data, &length)) {
        return NULL;
    }

    TOX_ERR_FILE_SEND_CHUNK err = 0;
    bool ret = tox_file_send_chunk(self->tox, friend_number, file_number, position,
                                   data, length, &err);

    if (ret) Py_RETURN_TRUE;

    PyErr_Format(ToxOpError, "tox_file_send_chunk() failed:%d", err);
    Py_RETURN_FALSE;
}

static PyObject*
ToxCore_file_seek(ToxCore* self, PyObject* args)
{
    CHECK_TOX(self);
    uint32_t friend_number = 0;
    uint32_t file_number = 0;
    uint64_t position = 0;

    if (!PyArg_ParseTuple(args, "iiK", &friend_number, &file_number, &position)) {
        return NULL;
    }

    TOX_ERR_FILE_SEEK err = 0;
    bool ret = tox_file_seek(self->tox, friend_number, file_number, position, &err);
   
    if (ret) Py_RETURN_TRUE;
    PyErr_Format(ToxOpError, "tox_file_seek() failed: %d", err);
    Py_RETURN_FALSE;
}

static PyObject*
ToxCore_file_get_file_id(ToxCore* self, PyObject* args)
{
    CHECK_TOX(self);
    uint32_t friend_number = 0;
    uint32_t file_number = 0;
    uint8_t* file_id = NULL;
    
    if (!PyArg_ParseTuple(args, "iis", &friend_number, &file_number, &file_id)) {
        return NULL;
    }

    TOX_ERR_FILE_GET err = 0;
    bool ret = tox_file_get_file_id(self->tox, friend_number, file_number, file_id, &err);
   
    if (ret) Py_RETURN_TRUE;
    PyErr_Format(ToxOpError, "tox_file_get_file_id() failed: %d", err);
    Py_RETURN_FALSE;
}

/*
static PyObject*
ToxCore_new_filesender(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int32_t friendnumber = 0;
  uint64_t filesize = 0;
  uint8_t* filename = 0;
  int filename_length = 0;

  if (!PyArg_ParseTuple(args, "iKs#", &friendnumber, &filesize, &filename,
        &filename_length)) {
    return NULL;
  }

  int ret = tox_new_file_sender(self->tox, friendnumber, filesize, filename,
      filename_length);

  if (ret == -1) {
    PyErr_SetString(ToxOpError, "tox_new_file_sender() failed");
    return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_file_sendcontrol(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int32_t friendnumber = 0;
  uint8_t send_receive = 0;
  int filenumber = 0;
  uint8_t message_id = 0;
  uint8_t* data = NULL;
  int data_length = 0;


  if (!PyArg_ParseTuple(args, "ibib|s#", &friendnumber, &send_receive,
        &filenumber, &message_id, &data, &data_length)) {
    return NULL;
  }

  int ret = tox_file_send_control(self->tox, friendnumber, send_receive,
      filenumber, message_id, data, data_length);

  if (ret == -1) {
    PyErr_SetString(ToxOpError, "tox_file_send_control() failed");
    return NULL;
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_file_senddata(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int32_t friendnumber = 0;
  int filenumber = 0;
  uint8_t* data = NULL;
  uint32_t data_length = 0;


  if (!PyArg_ParseTuple(args, "iis#", &friendnumber, &filenumber, &data,
        &data_length)) {
    return NULL;
  }

  int ret = tox_file_send_data(self->tox, friendnumber, filenumber, data,
      data_length);

  if (ret == -1) {
    PyErr_SetString(ToxOpError, "tox_file_send_data() failed");
    return NULL;
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_file_data_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int32_t friendnumber = 0;
  if (!PyArg_ParseTuple(args, "i", &friendnumber)) {
    return NULL;
  }

  int ret = tox_file_data_size(self->tox, friendnumber);

  if (ret == -1) {
    PyErr_SetString(ToxOpError, "tox_file_data_size() failed");
    return NULL;
  }

  return PyLong_FromLong(ret);
}

static PyObject*
ToxCore_file_data_remaining(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  int32_t friendnumber = 0;
  int filenumber = 0;
  int send_receive = 0;


  if (!PyArg_ParseTuple(args, "iii", &friendnumber, &filenumber,
        &send_receive)) {
    return NULL;
  }

  uint64_t ret = tox_file_data_remaining(self->tox, friendnumber, filenumber,
      send_receive);

  if (!ret) {
    PyErr_SetString(ToxOpError, "tox_file_data_remaining() failed");
    return NULL;
  }

  return PyLong_FromUnsignedLongLong(ret);
}
*/

static PyObject*
ToxCore_self_get_nospam(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t nospam = tox_self_get_nospam(self->tox);

  return PyLong_FromUnsignedLongLong(nospam);
}

static PyObject*
ToxCore_self_set_nospam(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t nospam = 0;

  if (!PyArg_ParseTuple(args, "I", &nospam)) {
    return NULL;
  }

  tox_self_set_nospam(self->tox, nospam);

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_self_get_keys(ToxCore* self, PyObject* args)
{
  uint8_t public_key[TOX_PUBLIC_KEY_SIZE];
  uint8_t secret_key[TOX_PUBLIC_KEY_SIZE];

  uint8_t public_key_hex[TOX_PUBLIC_KEY_SIZE * 2 + 1];
  uint8_t secret_key_hex[TOX_PUBLIC_KEY_SIZE * 2 + 1];

  tox_self_get_public_key(self->tox, public_key);
  tox_self_get_secret_key(self->tox, secret_key);
  bytes_to_hex_string(public_key, TOX_PUBLIC_KEY_SIZE, public_key_hex);
  bytes_to_hex_string(secret_key, TOX_PUBLIC_KEY_SIZE, secret_key_hex);

  PyObject* res = PyTuple_New(2);
  PyTuple_SetItem(res, 0, PYSTRING_FromStringAndSize((char*)public_key_hex,
        TOX_PUBLIC_KEY_SIZE * 2));
  PyTuple_SetItem(res, 1, PYSTRING_FromStringAndSize((char*)secret_key_hex,
        TOX_PUBLIC_KEY_SIZE * 2));
  return res;
}

static PyObject*
ToxCore_bootstrap(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint16_t port = 0;
  uint8_t* public_key = NULL;
  char* address = NULL;
  int addr_length = 0;
  int pk_length = 0;
  uint8_t pk[TOX_PUBLIC_KEY_SIZE];

  if (!PyArg_ParseTuple(args, "s#Hs#", &address, &addr_length, &port,
        &public_key, &pk_length)) {
    return NULL;
  }

  hex_string_to_bytes(public_key, TOX_PUBLIC_KEY_SIZE, pk);
  bool ret = tox_bootstrap(self->tox, address, port, pk, NULL);
  
  if (!ret) {
    PyErr_SetString(ToxOpError, "failed to resolve address");
    return NULL;
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_add_tcp_relay(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint16_t port = 0;
  uint8_t* public_key = NULL;
  char* address = NULL;
  int addr_length = 0;
  int pk_length = 0;
  uint8_t pk[TOX_PUBLIC_KEY_SIZE];

  if (!PyArg_ParseTuple(args, "s#Hs#", &address, &addr_length, &port,
        &public_key, &pk_length)) {
    return NULL;
  }

  hex_string_to_bytes(public_key, TOX_PUBLIC_KEY_SIZE, pk);
  bool ret = tox_add_tcp_relay(self->tox, address, port, pk, NULL);
  
  if (!ret) {
    PyErr_SetString(ToxOpError, "failed to add tcp relay");
    return NULL;
  }

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_self_get_connection_status(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  TOX_CONNECTION conns = tox_self_get_connection_status(self->tox);

  switch (conns) {
  case TOX_CONNECTION_TCP:
  case TOX_CONNECTION_UDP:
    Py_RETURN_TRUE;
  default:
    Py_RETURN_FALSE;
  }
}

static PyObject*
ToxCore_kill(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  tox_kill(self->tox);
  self->tox = NULL;

  Py_RETURN_NONE;
}

static PyObject*
ToxCore_iteration_interval(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t interval = tox_iteration_interval(self->tox);

  return PyLong_FromUnsignedLongLong(interval);
}

static PyObject*
ToxCore_iterate(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  tox_iterate(self->tox);

  if (PyErr_Occurred()) {
    return NULL;
  }
  Py_RETURN_NONE;
}

static PyObject*
ToxCore_get_savedata_size(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t size = tox_get_savedata_size(self->tox);

  return PyLong_FromUnsignedLong(size);
}

static PyObject*
ToxCore_get_savedata(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint32_t size = tox_get_savedata_size(self->tox);
  uint8_t* buf = (uint8_t*)malloc(size);
  tox_get_savedata(self->tox, buf);

  PyObject* res = PYBYTES_FromStringAndSize((const char*)buf, size);
  free(buf);

  return res;
}

static PyObject*
ToxCore_save_to_file(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  char* filename = NULL;

  if (!PyArg_ParseTuple(args, "s", &filename)) {
    return NULL;
  }

  int size = tox_get_savedata_size(self->tox);

  uint8_t* buf = (uint8_t*)malloc(size);

  if (buf == NULL) {
    return PyErr_NoMemory();
  }

  tox_get_savedata(self->tox, buf);

  FILE* fp = fopen(filename, "w");

  if (fp == NULL) {
    PyErr_SetString(ToxOpError, "fopen(): can't open file for saving");
    return NULL;
  }

  fwrite(buf, size, 1, fp);
  fclose(fp);

  free(buf);

  Py_RETURN_NONE;
}

/*
static PyObject*
ToxCore_load(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  uint8_t* data = NULL;
  int length = 0;

  if (!PyArg_ParseTuple(args, "s#", &data, &length)) {
    PyErr_SetString(ToxOpError, "no data specified");
    return NULL;
  }

  if (tox_load(self->tox, data, length) == -1) {
    PyErr_SetString(ToxOpError, "tox_load(): load failed");
    return NULL;
  }

  Py_RETURN_NONE;
}
*/
/*
static PyObject*
ToxCore_load_from_file(ToxCore* self, PyObject* args)
{
  CHECK_TOX(self);

  char* filename = NULL;

  if (!PyArg_ParseTuple(args, "s", &filename)) {
    return NULL;
  }

  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    PyErr_SetString(ToxOpError,
        "tox_load(): failed to open file for reading");
    return NULL;
  }

  size_t length = 0;
  fseek(fp, 0, SEEK_END);
  length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  uint8_t* data = (uint8_t*)malloc(length * sizeof(char));

  if (fread(data, length, 1, fp) != 1) {
    PyErr_SetString(ToxOpError,
        "tox_load(): corrupted data file");
    free(data);
    return NULL;
  }

  if (tox_load(self->tox, data, length) == -1) {
    PyErr_SetString(ToxOpError, "tox_load(): load failed");
    free(data);
    return NULL;
  }

  free(data);
  Py_RETURN_NONE;
}
*/
PyMethodDef Tox_methods[] = {
  {
    "on_friend_request", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_friend_request(address, message)\n"
    "Callback for receiving friend requests, default implementation does "
    "nothing."
  },
  {
    "on_friend_message", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_friend_message(friend_number, message)\n"
    "Callback for receiving friend messages, default implementation does "
    "nothing."
  },
  {
    "on_name_change", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_name_change(friend_number, new_name)\n"
    "Callback for receiving friend name changes, default implementation does "
    "nothing."
  },
  {
    "on_status_message", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_status_message(friend_number, new_status)\n"
    "Callback for receiving friend status message changes, default "
    "implementation does nothing."
  },
  {
    "on_user_status", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_user_status(friend_number, kind)\n"
    "Callback for receiving friend status changes, default implementation "
    "does nothing.\n\n"
    ".. seealso ::\n"
    "    :meth:`.set_user_status`"
  },
  {
    "on_friend_typing", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_friend_typing(friend_number, is_typing)\n"
    "Callback for receiving friend status changes, default implementation "
    "does nothing.\n\n"
  },
  {
    "on_read_receipt", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_read_receipt(friend_number, receipt)\n"
    "Callback for receiving read receipt, default implementation does nothing."
  },
  {
    "on_connection_status", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_connection_status(friend_number, status)\n"
    "Callback for receiving read receipt, default implementation does "
    "nothing.\n\n"
    "*status* is a boolean value which indicates the status of the friend "
    "indicated by *friend_number*. True means online and False means offline "
    "after previously online."
  },
  {
    "on_group_invite", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_group_invite(friend_number, type, group_public_key)\n"
    "Callback for receiving group invitations, default implementation does "
    "nothing.\n\n"
    ".. seealso ::\n"
    "    :meth:`.group_get_type`"
  },
  {
    "on_group_message", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_group_message(group_number, friend_group_number, message)\n"
    "Callback for receiving group messages, default implementation does "
    "nothing."
  },
  {
    "on_group_action", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_group_action(group_number, friend_group_number, action)\n"
    "Callback for receiving group actions, default implementation does "
    "nothing."
  },
  {
    "on_group_namelist_change", (PyCFunction)ToxCore_callback_stub,
    METH_VARARGS,
    "on_group_namelist_change(group_number, peer_number, change)\n"
    "Callback for receiving group messages, default implementation does "
    "nothing.\n\n"
    "There are there possible *change* values:\n\n"
    "+---------------------------+----------------------+\n"
    "| change                    | description          |\n"
    "+===========================+======================+\n"
    "| Tox.CHAT_CHANGE_PEER_ADD  | a peer is added      |\n"
    "+---------------------------+----------------------+\n"
    "| Tox.CHAT_CHANGE_PEER_DEL  | a peer is deleted    |\n"
    "+---------------------------+----------------------+\n"
    "| Tox.CHAT_CHANGE_PEER_NAME | name of peer changed |\n"
    "+---------------------------+----------------------+\n"
  },
  {
    "on_file_recv", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_file_recv(friend_number, file_number,  kind, file_size, filename)\n"
    "Callback for receiving file transfer, default implementation does nothing."
  },
  {
    "on_file_recv_control", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_file_recv_control(friend_number, file_number, control)\n"
    "Callback for receiving file control, default implementation does nothing."
  },
  {
    "on_file_recv_chunk", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_file_recv_chunk(friend_number, file_number, position, data)\n"
    "Callback for receiving file chunk, default implementation does nothing."
  },
  {
    "on_file_chunk_request", (PyCFunction)ToxCore_callback_stub, METH_VARARGS,
    "on_file_chunk_request(friend_number, file_number, position, length)\n"
    "Callback for more file chunk, default implementation does nothing."
  },
  {
    "self_get_address", (PyCFunction)ToxCore_self_get_address, METH_NOARGS,
    "self_get_address()\n"
    "Return address to give to others."
  },
  {
    "friend_add", (PyCFunction)ToxCore_friend_add, METH_VARARGS,
    "friend_add(address, message)\n"
    "Add a friend."
  },
  {
    "friend_add_norequest", (PyCFunction)ToxCore_friend_add_norequest,
    METH_VARARGS,
    "friend_add_norequest(address)\n"
    "Add a friend without sending request."
  },
  {
    "friend_by_public_key", (PyCFunction)ToxCore_friend_by_public_key, METH_VARARGS,
    "friend_by_public_key(friend_id)\n"
    "Return the friend id associated to that client id."
  },
  {
    "friend_get_public_key", (PyCFunction)ToxCore_friend_get_public_key, METH_VARARGS,
    "friend_get_public_key(friend_number)\n"
    "Return the public key associated to that friend number."
  },
  {
    "friend_delete", (PyCFunction)ToxCore_friend_delete, METH_VARARGS,
    "friend_delete(friend_number)\n"
    "Remove a friend."
  },
  {
    "friend_get_connection_status",
    (PyCFunction)ToxCore_friend_get_connection_status, METH_VARARGS,
    "friend_get_connection_status(friend_number)\n"
    "Return True if friend is connected(Online) else False."
  },
  {
    "friend_exists", (PyCFunction)ToxCore_friend_exists, METH_VARARGS,
    "friend_exists(friend_number)\n"
    "Checks if there exists a friend with given friendnumber."
  },
  {
    "friend_send_message", (PyCFunction)ToxCore_friend_send_message, METH_VARARGS,
    "friend_send_message(friend_number, type, message)\n"
    "Send a text chat message to an online friend."
  },
  /*  {
    "send_action", (PyCFunction)ToxCore_send_action, METH_VARARGS,
    "send_action(friend_number, action)\n"
    "Send an action to an online friend."
    },*/
  {
    "self_set_name", (PyCFunction)ToxCore_self_set_name, METH_VARARGS,
    "self_set_name(name)\n"
    "Set our self nickname."
  },
  {
    "self_get_name", (PyCFunction)ToxCore_self_get_name, METH_NOARGS,
    "self_get_name()\n"
    "Get our self nickname."
  },
  {
    "self_get_name_size", (PyCFunction)ToxCore_self_get_name_size, METH_NOARGS,
    "self_get_name_size()\n"
    "Get our self nickname string length"
  },
  {
    "friend_get_name", (PyCFunction)ToxCore_friend_get_name, METH_VARARGS,
    "friend_get_name(friend_number)\n"
    "Get nickname of *friend_number*."
  },
  {
    "friend_get_name_size", (PyCFunction)ToxCore_friend_get_name_size, METH_VARARGS,
    "friend_get_name_size(friend_number)\n"
    "Get nickname length of *friend_number*."
  },
  {
    "self_set_status_message", (PyCFunction)ToxCore_self_set_status_message, METH_VARARGS,
    "self_set_status_message(message)\n"
    "Set our self status message."
  },
  {
    "self_set_status", (PyCFunction)ToxCore_self_set_status, METH_VARARGS,
    "self_set_status(status)\n"
    "Set our user status, status can have following values:\n\n"
    "+------------------------+--------------------+\n"
    "| kind                   | description        |\n"
    "+========================+====================+\n"
    "| Tox.USERSTATUS_NONE    | the user is online |\n"
    "+------------------------+--------------------+\n"
    "| Tox.USERSTATUS_AWAY    | the user is away   |\n"
    "+------------------------+--------------------+\n"
    "| Tox.USERSTATUS_BUSY    | the user is busy   |\n"
    "+------------------------+--------------------+\n"
    "| Tox.USERSTATUS_INVALID | invalid status     |\n"
    "+------------------------+--------------------+\n"
  },
  {
    "friend_get_status_message_size", (PyCFunction)ToxCore_friend_get_status_message_size,
    METH_VARARGS,
    "friend_get_status_message_size(friend_number)\n"
    "Return the length of *friend_number*'s status message."
  },
  {
    "friend_get_status_message", (PyCFunction)ToxCore_friend_get_status_message,
    METH_VARARGS,
    "friend_get_status_message(friend_number)\n"
    "Get status message of a friend."
  },
  {
    "self_get_status_message", (PyCFunction)ToxCore_self_get_status_message,
    METH_NOARGS,
    "self_get_status_message()\n"
    "Get status message of yourself."
  },
  {
    "self_get_status_message_size",
    (PyCFunction)ToxCore_self_get_status_message_size,
    METH_NOARGS,
    "self_get_status_message_size()\n"
    "Get status message string length of yourself."
  },
  {
    "friend_get_status", (PyCFunction)ToxCore_friend_get_status,
    METH_VARARGS,
    "friend_get_status(friend_number)\n"
    "Get friend status.\n\n"
    ".. seealso ::\n"
    "    :meth:`.set_user_status`"
  },
  {
    "self_get_status", (PyCFunction)ToxCore_self_get_status,
    METH_NOARGS,
    "self_get_status()\n"
    "Get user status of youself.\n\n"
    ".. seealso ::\n"
    "    :meth:`.set_user_status`"
  },
  {
    "friend_get_last_online", (PyCFunction)ToxCore_friend_get_last_online,
    METH_VARARGS,
    "friend_get_last_online(friend_number)\n"
    "returns datetime.datetime object representing the last time "
    "*friend_number* was seen online, or None if never seen."
  },
  {
    "self_set_typing", (PyCFunction)ToxCore_self_set_typing,
    METH_VARARGS,
    "self_set_typing(friend_number, is_typing)\n"
    "Set user typing status.\n\n"
  },
  {
    "friend_get_typing", (PyCFunction)ToxCore_friend_get_typing,
    METH_VARARGS,
    "friend_get_typing(friend_number)\n"
    "Return True is user is typing.\n\n"
  },
  {
    "self_get_friend_list_size", (PyCFunction)ToxCore_self_get_friend_list_size,
    METH_NOARGS,
    "self_get_friend_list_size()\n"
    "Return the number of friends."
  },
  /*  {
    "get_num_online_friends", (PyCFunction)ToxCore_get_num_online_friends,
    METH_NOARGS,
    "get_num_online_friends()\n"
    "Return the number of online friends."
    },*/
  {
    "self_get_friend_list", (PyCFunction)ToxCore_self_get_friend_list,
    METH_NOARGS,
    "self_get_friend_list()\n"
    "Get a list of valid friend numbers."
  },
#ifndef LEGACY_GROUPCHATS

  {
    "group_new", (PyCFunction)ToxCore_group_new, METH_VARARGS,
    "group_new(privacy_state, group_name)\n"
    "Create a new group for group_name."
  },
  {
    "group_join", (PyCFunction)ToxCore_group_join, METH_VARARGS,
    "group_join(chat_id, password)\n"
    "Join a group for group_name."
  },
  {
    "group_reconnect", (PyCFunction)ToxCore_group_reconnect, METH_VARARGS,
    "group_reconnect(group_number)\n"
    "Reconnects to a group."
  },
  {
    "group_leave", (PyCFunction)ToxCore_group_leave, METH_VARARGS,
    "group_leave(group_number, message)\n"
    "Reconnects to a group."
  },
  {
    "group_self_set_name", (PyCFunction)ToxCore_group_self_set_name, METH_VARARGS,
    "group_self_set_name(group_number, name)\n"
    "Set the client's nickname for the group instance designated by the given group number."
  },
  {
    "group_self_get_name_size", (PyCFunction)ToxCore_group_self_get_name_size, METH_VARARGS,
    "group_self_get_name_size(group_number)\n"
    "Return the length of the client's current nickname."
  },
  {
    "group_self_get_name", (PyCFunction)ToxCore_group_self_get_name, METH_VARARGS,
    "group_self_get_name(group_number)\n"
    "Return the nickname."
  },
  {
    "group_self_set_status", (PyCFunction)ToxCore_group_self_set_status, METH_VARARGS,
    "group_self_set_status(group_number, status)\n"
    "Set the client's status for the group instance."
  },
  {
    "group_self_get_status", (PyCFunction)ToxCore_group_self_get_status, METH_VARARGS,
    "group_self_get_status(group_number)\n"
    "Get the client's status for the group instance."
  },
  {
    "group_self_get_role", (PyCFunction)ToxCore_group_self_get_role, METH_VARARGS,
    "group_self_get_role(group_number)\n"
    "Get the client's role for the group instance."
  },
  {
    "group_peer_get_name_size", (PyCFunction)ToxCore_group_peer_get_name_size, METH_VARARGS,
    "group_peer_get_name_size(group_number, peer_number)\n"
    "Return the length of the peer's name."
  },
  {
    "group_peer_get_name", (PyCFunction)ToxCore_group_peer_get_name, METH_VARARGS,
    "group_peer_get_name(group_number, peer_number)\n"
    "Return the peer's name."
  },
  {
    "group_peer_get_status", (PyCFunction)ToxCore_group_peer_get_status, METH_VARARGS,
    "group_peer_get_status(group_number, peer_number)\n"
    "Return the peer's status."
  },
  {
    "group_peer_get_role", (PyCFunction)ToxCore_group_peer_get_role, METH_VARARGS,
    "group_peer_get_role(group_number, peer_number)\n"
    "Return the peer's role."
  },
  {
    "group_set_topic", (PyCFunction)ToxCore_group_set_topic, METH_VARARGS,
    "group_set_topic(group_number, topic)\n"
    "Set the group topic and broadcast it to the rest of the group."
  },
  {
    "group_get_topic_size", (PyCFunction)ToxCore_group_get_topic_size, METH_VARARGS,
    "group_get_topic_size(group_number)\n"
    "Get the length group's topic."
  },
  {
    "group_get_topic", (PyCFunction)ToxCore_group_get_topic, METH_VARARGS,
    "group_get_topic(group_number)\n"
    "Get the group's topic."
  },
  {
    "group_get_name_size", (PyCFunction)ToxCore_group_get_name_size, METH_VARARGS,
    "group_get_name_size(group_number)\n"
    "Get the length group's name."
  },
  {
    "group_get_name", (PyCFunction)ToxCore_group_get_name, METH_VARARGS,
    "group_get_name(group_number)\n"
    "Get the group's name."
  },
  {
    "group_get_chat_id", (PyCFunction)ToxCore_group_get_chat_id, METH_VARARGS,
    "group_get_chat_id(group_number)\n"
    "Get the group's chat id."
  },
  {
    "group_get_number_peers", (PyCFunction)ToxCore_group_get_number_peers, METH_VARARGS,
    "group_get_number_peers(group_number)\n"
    "Returns the number of peers in the group."
  },
  {
    "group_get_number_groups", (PyCFunction)ToxCore_group_get_number_groups, METH_VARARGS,
    "group_get_number_groups()\n"
    "Return the number of groups in the Tox chats array."
  },
  {
    "group_get_privacy_state", (PyCFunction)ToxCore_group_get_privacy_state, METH_VARARGS,
    "group_get_privacy_state(group_number)\n"
    "Return the privacy state of the group."
  },
  {
    "group_get_peer_limit", (PyCFunction)ToxCore_group_get_peer_limit, METH_VARARGS,
    "group_get_peer_limit(group_number)\n"
    "Return the maximum number of peers allowed for the group."
  },
  {
    "group_get_password_size", (PyCFunction)ToxCore_group_get_password_size, METH_VARARGS,
    "group_get_password_size(group_number)\n"
    "Return the length of the group password."
  },
  {
    "group_get_password", (PyCFunction)ToxCore_group_get_password, METH_VARARGS,
    "group_get_password(group_number)\n"
    "Return the group password."
  },
  {
    "group_send_message", (PyCFunction)ToxCore_group_send_message, METH_VARARGS,
    "group_send_message(group_number, type, message)\n"
    "Send a text chat message to the entire group."
  },
  {
    "group_send_private_message", (PyCFunction)ToxCore_group_send_private_message, METH_VARARGS,
    "group_send_private_message(group_number, peer_number, message)\n"
    "Send a text chat message to the specified peer in the specified group."
  },
  {
    "group_invite_friend", (PyCFunction)ToxCore_group_invite_friend, METH_VARARGS,
    "group_invite_friend(group_number, friend_number)\n"
    "Invite a friend to a group."
  },
  {
    "group_invite_accept", (PyCFunction)ToxCore_group_invite_accept, METH_VARARGS,
    "group_invite_accept(invite_data, password)\n"
    "Invite a friend to a group."
  },
  {
    "group_founder_set_password", (PyCFunction)ToxCore_group_founder_set_password, METH_VARARGS,
    "group_founder_set_password(group_number, password)\n"
    "Set or unset the group password."
  },
  {
    "group_founder_set_privacy_state", (PyCFunction)ToxCore_group_founder_set_privacy_state, METH_VARARGS,
    "group_founder_set_privacy_state(group_number, privacy_state)\n"
    "Set the group privacy state."
  },
  {
    "group_founder_set_peer_limit", (PyCFunction)ToxCore_group_founder_set_peer_limit, METH_VARARGS,
    "group_founder_set_peer_limit(group_number, max_peers)\n"
    "Set the group peer limit."
  },
  {
    "group_toggle_ignore", (PyCFunction)ToxCore_group_toggle_ignore, METH_VARARGS,
    "group_toggle_ignore(group_number, peer_number, ignore)\n"
    "Ignore or unignore a peer."
  },
  {
    "group_mod_set_role", (PyCFunction)ToxCore_group_mod_set_role, METH_VARARGS,
    "group_mod_set_role(group_number, peer_number, role)\n"
    "Set a peer's role."
  },
  {
    "group_mod_remove_peer", (PyCFunction)ToxCore_group_mod_remove_peer, METH_VARARGS,
    "group_mod_remove_peer(group_number, peer_number, set_ban)\n"
    "Kick/ban a peer."
  },
  {
    "group_mod_remove_ban", (PyCFunction)ToxCore_group_mod_remove_ban, METH_VARARGS,
    "group_mod_remove_ban(group_number, ban_id)\n"
    "Removes a ban."
  },
  {
    "group_ban_get_list_size", (PyCFunction)ToxCore_group_ban_get_list_size, METH_VARARGS,
    "group_ban_get_list_size(group_number)\n"
    "Return the number of entries in the ban list."
  },
  {
    "group_ban_get_list", (PyCFunction)ToxCore_group_ban_get_list, METH_VARARGS,
    "group_ban_get_list(group_number)\n"
    "Return a list of valid ban list ID's"
  },
  {
    "group_ban_get_name_size", (PyCFunction)ToxCore_group_ban_get_name_size, METH_VARARGS,
    "group_ban_get_name_size(group_number, ban_id)\n"
    "Return the length of the name for the ban list entry."
  },
  {
    "group_ban_get_name", (PyCFunction)ToxCore_group_ban_get_name, METH_VARARGS,
    "group_ban_get_name(group_number, ban_id)\n"
    "Return the name of the ban entry designated by ban_id."
  },
  {
    "group_ban_get_time_set", (PyCFunction)ToxCore_group_ban_get_time_set, METH_VARARGS,
    "group_ban_get_time_set(group_number, ban_id)\n"
    "Return a time stamp indicating the time the ban was set."
  },
  
  
#else
  {
    "group_get_title", (PyCFunction)ToxCore_group_get_title, METH_VARARGS,
    "group_get_title(group_number)\n"
    "Returns the title for group_number."
  },
  {
    "group_set_title", (PyCFunction)ToxCore_group_set_title, METH_VARARGS,
    "group_set_title(group_number, title)\n"
    "Sets the title for group."
  },
  {
    "group_get_type", (PyCFunction)ToxCore_group_get_type, METH_VARARGS,
    "group_get_type(group_number)\n"
    "Return the type of group, could be the following value:\n\n"
    "+-------------------------+-------------+\n"
    "| type                    | description |\n"
    "+=========================+=============+\n"
    "| Tox.GROUPCHAT_TYPE_TEXT | text chat   |\n"
    "+-------------------------+-------------+\n"
    "| Tox.GROUPCHAT_TYPE_AV   | video chat  |\n"
    "+-------------------------+-------------+\n"
  },
  {
    "add_groupchat", (PyCFunction)ToxCore_add_groupchat, METH_VARARGS,
    "add_groupchat()\n"
    "Creates a new groupchat and puts it in the chats array."
  },
  {
    "del_groupchat", (PyCFunction)ToxCore_del_groupchat, METH_VARARGS,
    "del_groupchat(group_number)\n"
    "Delete a groupchat from the chats array."
  },
  {
    "group_peername", (PyCFunction)ToxCore_group_peername, METH_VARARGS,
    "group_peername(group_number, peer_number)\n"
    "Get the group peer's name."
  },
  {
    "invite_friend", (PyCFunction)ToxCore_invite_friend, METH_VARARGS,
    "invite_friend(friend_number, group_number)\n"
    "Invite friendnumber to groupnumber."
  },
  {
    "join_groupchat", (PyCFunction)ToxCore_join_groupchat, METH_VARARGS,
    "join_groupchat(friend_number, data)\n"
    "Join a group (you need to have been invited first.). Returns the group "
    "number of success."
  },
  {
    "group_message_send", (PyCFunction)ToxCore_group_message_send, METH_VARARGS,
    "group_message_send(group_number, message)\n"
    "send a group message."
  },
  {
    "group_action_send", (PyCFunction)ToxCore_group_action_send, METH_VARARGS,
    "group_action_send(group_number, action)\n"
    "send a group action."
  },
  {
    "group_number_peers", (PyCFunction)ToxCore_group_number_peers, METH_VARARGS,
    "group_number_peers(group_number)\n"
    "Return the number of peers in the group chat."
  },
  {
    "group_get_names", (PyCFunction)ToxCore_group_get_names, METH_VARARGS,
    "group_get_names(group_number)\n"
    "List all the peers in the group chat."
  },
  {
    "count_chatlist", (PyCFunction)ToxCore_count_chatlist, METH_VARARGS,
    "count_chatlist()\n"
    "Return the number of chats in the current Tox instance."
  },
  {
    "get_chatlist", (PyCFunction)ToxCore_get_chatlist, METH_VARARGS,
    "get_chatlist()\n"
    "Return a list of valid group numbers."
  },
#endif // -DNEW_GROUPCHATS
  {
    "file_send", (PyCFunction)ToxCore_file_send, METH_VARARGS,
    "file_send(friend_number, kind, file_size, file_id, filename)\n"
    "Send a file send request. Returns file number to be sent."
  },
  {
    "file_control", (PyCFunction)ToxCore_file_control, METH_VARARGS,
    "file_control(friend_number, file_number, control)\n"
    "Send a file send request. Returns file number to be sent."
  },
  {
    "file_send_chunk", (PyCFunction)ToxCore_file_send_chunk, METH_VARARGS,
    "file_send_chunk(friend_number, file_number, position, data)\n"
    "Send a file send request. Returns file number to be sent."
  },
  {
    "file_seek", (PyCFunction)ToxCore_file_seek, METH_VARARGS,
    "file_seek(friend_number, file_number, position)\n"
    "Send a file send request. Returns file number to be sent."
  },
  {
    "file_get_file_id", (PyCFunction)ToxCore_file_get_file_id, METH_VARARGS,
    "file_get_file_id(friend_number, file_number, file_id)\n"
    "Send a file send request. Returns file number to be sent."
  },
  {
    "self_get_nospam", (PyCFunction)ToxCore_self_get_nospam,
    METH_NOARGS,
    "self_get_nospam()\n"
    "get nospam part from ID"
  },
  {
    "self_set_nospam", (PyCFunction)ToxCore_self_set_nospam,
    METH_VARARGS,
    "self_set_nospam(nospam)\n"
    "set nospam part of ID. *nospam* should be of type uint32"
  },
  {
    "self_get_keys", (PyCFunction)ToxCore_self_get_keys,
    METH_NOARGS,
    "self_get_keys()\n"
    "Get the public and secret key from the Tox object. Return a tuple "
    "(public_key, secret_key)"
  },
  {
    "bootstrap", (PyCFunction)ToxCore_bootstrap,
    METH_VARARGS,
    "bootstrap(address, port, public_key)\n"
    "Resolves address into an IP address. If successful, sends a 'get nodes'"
    "request to the given node with ip, port."
  },
  {
    "add_tcp_relay", (PyCFunction)ToxCore_add_tcp_relay, METH_VARARGS,
    "add_tcp_relay(address, port, public_key)\n"
    ""
  },
  {
    "self_get_connection_status", (PyCFunction)ToxCore_self_get_connection_status, METH_NOARGS,
    "self_get_connection_status()\n"
    "Return False if we are not connected to the DHT."
  },
  {
    "kill", (PyCFunction)ToxCore_kill, METH_NOARGS,
    "kill()\n"
    "Run this before closing shop."
  },
  {
    "iteration_interval", (PyCFunction)ToxCore_iteration_interval, METH_NOARGS,
    "iteration_interval()\n"
    "returns time (in ms) before the next tox_iterate() needs to be run on success."
  },
  {
    "iterate", (PyCFunction)ToxCore_iterate, METH_NOARGS,
    "iterate()\n"
    "The main loop that needs to be run at least 20 times per second."
  },
  {
    "get_savedata_size", (PyCFunction)ToxCore_get_savedata_size, METH_NOARGS,
    "get_savedata_size()\n"
    "return size of messenger data (for saving)."
  },
  {
    "get_savedata", (PyCFunction)ToxCore_get_savedata, METH_VARARGS,
    "get_savedata()\n"
    "Return messenger blob in str."
  },
  /*
  {
    "save_to_file", (PyCFunction)ToxCore_save_to_file, METH_VARARGS,
    "save_to_file(filename)\n"
    "Save the messenger to a file."
    },*/
  /*{
    "load", (PyCFunction)ToxCore_load, METH_VARARGS,
    "load(blob)\n"
    "Load the messenger from *blob*."
  },
  {
    "load_from_file", (PyCFunction)ToxCore_load_from_file, METH_VARARGS,
    "load_from_file(filename)\n"
    "Load the messenger from file."
    },*/
  {NULL}
};

PyTypeObject ToxCoreType = {
#if PY_MAJOR_VERSION >= 3
  PyVarObject_HEAD_INIT(NULL, 0)
#else
  PyObject_HEAD_INIT(NULL)
  0,                         /*ob_size*/
#endif
  "Tox",                     /*tp_name*/
  sizeof(ToxCore),           /*tp_basicsize*/
  0,                         /*tp_itemsize*/
  (destructor)ToxCore_dealloc, /*tp_dealloc*/
  0,                         /*tp_print*/
  0,                         /*tp_getattr*/
  0,                         /*tp_setattr*/
  0,                         /*tp_compare*/
  0,                         /*tp_repr*/
  0,                         /*tp_as_number*/
  0,                         /*tp_as_sequence*/
  0,                         /*tp_as_mapping*/
  0,                         /*tp_hash */
  0,                         /*tp_call*/
  0,                         /*tp_str*/
  0,                         /*tp_getattro*/
  0,                         /*tp_setattro*/
  0,                         /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
  "ToxCore object",          /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  Tox_methods,               /* tp_methods */
  0,                         /* tp_members */
  0,                         /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)ToxCore_init,    /* tp_init */
  0,                         /* tp_alloc */
  ToxCore_new,               /* tp_new */
};

void ToxCore_install_dict()
{
#define SET(name)                                            \
  PyObject* obj_##name = PyLong_FromLong(TOX_##name);        \
  PyDict_SetItemString(dict, #name, obj_##name);             \
  Py_DECREF(obj_##name);

  PyObject* dict = PyDict_New();
  SET(ERR_FRIEND_ADD_TOO_LONG)
  SET(ERR_FRIEND_ADD_NO_MESSAGE)
  SET(ERR_FRIEND_ADD_OWN_KEY)
  SET(ERR_FRIEND_ADD_ALREADY_SENT)
  SET(ERR_FRIEND_ADD_NULL)
  SET(ERR_FRIEND_ADD_BAD_CHECKSUM)
  SET(ERR_FRIEND_ADD_SET_NEW_NOSPAM)
  SET(ERR_FRIEND_ADD_MALLOC)
      SET(CONNECTION_NONE)
      SET(CONNECTION_TCP)
      SET(CONNECTION_UDP)
      SET(PROXY_TYPE_NONE)
      SET(PROXY_TYPE_HTTP)
      SET(PROXY_TYPE_SOCKS5)
      SET(MESSAGE_TYPE_NORMAL)
      SET(MESSAGE_TYPE_ACTION)
      SET(SAVEDATA_TYPE_NONE)
      SET(SAVEDATA_TYPE_TOX_SAVE)
      SET(SAVEDATA_TYPE_SECRET_KEY)
  SET(USER_STATUS_NONE)
  SET(USER_STATUS_AWAY)
  SET(USER_STATUS_BUSY)
#ifndef LEGACY_GROUPCHATS
#else
  SET(CHAT_CHANGE_PEER_ADD)
  SET(CHAT_CHANGE_PEER_DEL)
  SET(CHAT_CHANGE_PEER_NAME)
  SET(GROUPCHAT_TYPE_TEXT)
  SET(GROUPCHAT_TYPE_AV)
#endif // -DNEW_GROUPCHATS
      SET(FILE_KIND_DATA)
      SET(FILE_KIND_AVATAR)
      SET(FILE_CONTROL_RESUME)
      SET(FILE_CONTROL_PAUSE)
      SET(FILE_CONTROL_CANCEL)
      // SET(FILECONTROL_ACCEPT)
      // SET(FILECONTROL_PAUSE)
      // SET(FILECONTROL_KILL)
      // SET(FILECONTROL_FINISHED)
      // SET(FILECONTROL_RESUME_BROKEN)

#undef SET

  ToxCoreType.tp_dict = dict;
}

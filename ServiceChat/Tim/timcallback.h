#ifndef TIMCALLBACK_H
#define TIMCALLBACK_H
#include "stdafx.h"
#include "tim.h"

void onConnected(void*);
void onDisconnected(void*);

void onLoginSuccess(void*);
void onLoginError(int code, const char *desc, void *data);

void onCommSuccess(void*);
void onCommError(int code, const char *desc, void *data);

void onGetFriendListSuccess(TIMFriendListElemHandle *handles, uint32_t num, void *data);
void onGetFriendListError(int code, const char *desc, void *data);

void onAddFriendSuccess(TIMFriendResultHandle* handles, uint32_t num, void* data);
void onAddFriendError(int code, const char* desc, void* data);

void onGetSelfProfileSuccess(TIMSelfProfileHandle* handles, uint32_t num, void* data);
void onGetSelfProfileError(int code, const char* desc, void* data);

void onGetNewMessage(TIMMessageHandle* handles, uint32_t msg_num, void* data);

void onGetImageFileSuccess(void*data);
void onGetImageFileError(int code, const char *desc, void *data);

void onSetNickNameSuccess(void* data);
void onSetNickNameError(int code, const char* desc, void* data);

#endif // TIMCALLBACK_H

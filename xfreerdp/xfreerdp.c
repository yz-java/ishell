/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * X11 Client
 *
 * Copyright 2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 * Copyright 2012 HP Development Company, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <freerdp/config.h>

#include <winpr/crt.h>
#include <winpr/thread.h>
#include <freerdp/freerdp.h>
#include "xf_client.h"
#include "xfreerdp.h"

void openRDPViewer(char*host,char* name,char* password,int port,ErrorCallBack errorCallback)
{
    LPDWORD dwExitCode;
	HANDLE thread;
	rdpContext* context;
	RDP_CLIENT_ENTRY_POINTS clientEntryPoints = { 0 };

	clientEntryPoints.Size = sizeof(RDP_CLIENT_ENTRY_POINTS);
	clientEntryPoints.Version = RDP_CLIENT_INTERFACE_VERSION;

	RdpClientEntry(&clientEntryPoints);

	context = freerdp_client_context_new(&clientEntryPoints);
    if (!context)
        return;

    context->settings->ServerPort=port;
    char* hn = malloc(strlen(host)+1);
    memset(hn,0,strlen(host)+1);
    memcpy(hn,host,strlen(host));
    context->settings->ServerHostname=hn;
    char* n = malloc(strlen(name)+1);
    memset(n,0,strlen(name)+1);
    memcpy(n,name,strlen(name));
    context->settings->Username=n;
    char* pwd = malloc(strlen(password)+1);
    memset(pwd,0,strlen(password)+1);
    memcpy(pwd,password,strlen(password));
    context->settings->Password=pwd;



    if (freerdp_client_start(context) != 0){
        errorCallback("连接失败");
        goto out;
    }


	thread = freerdp_client_get_thread(context);

    WaitForSingleObject(thread, INFINITE);
    GetExitCodeThread(thread, &dwExitCode);
    xf_exit_code_from_disconnect_reason(dwExitCode);

	freerdp_client_stop(context);

out:
	freerdp_client_context_free(context);

    return;
}

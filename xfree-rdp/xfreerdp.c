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
#include <winpr/synch.h>
#include <winpr/thread.h>

#include <freerdp/client/cmdline.h>
#include <freerdp/freerdp.h>
#include <freerdp/streamdump.h>

#include "xf_client.h"
#include "xfreerdp.h"
DWORD client_cli_verify_certificate_ex(freerdp *instance, const char *host,
                                       UINT16 port, const char *common_name,
                                       const char *subject, const char *issuer,
                                       const char *fingerprint, DWORD flags) {
  const char *type = "RDP-Server";

  WINPR_ASSERT(instance);
  WINPR_ASSERT(instance->context);
  WINPR_ASSERT(instance->context->settings);

  if (flags & VERIFY_CERT_FLAG_GATEWAY)
    type = "RDP-Gateway";

  if (flags & VERIFY_CERT_FLAG_REDIRECT)
    type = "RDP-Redirect";

  printf("Certificate details for %s:%" PRIu16 " (%s):\n", host, port, type);
  printf("\tCommon Name: %s\n", common_name);
  printf("\tSubject:     %s\n", subject);
  printf("\tIssuer:      %s\n", issuer);
  /* Newer versions of FreeRDP allow exposing the whole PEM by setting
   * FreeRDP_CertificateCallbackPreferPEM to TRUE
   */
  if (flags & VERIFY_CERT_FLAG_FP_IS_PEM) {
    printf("\t----------- Certificate --------------\n");
    printf("%s\n", fingerprint);
    printf("\t--------------------------------------\n");
  } else
    printf("\tThumbprint:  %s\n", fingerprint);

  printf("The above X.509 certificate could not be verified, possibly because "
         "you do not have\n"
         "the CA certificate in your certificate store, or the certificate has "
         "expired.\n"
         "Please look at the OpenSSL documentation on how to add a private CA "
         "to the store.\n");
  return 1;
}
void openRDPViewer(char *host, char *name, char *password, int port,
                   ErrorCallBack errorCallback) {
  LPDWORD dwExitCode;
  HANDLE thread;
  rdpContext *context;
  RDP_CLIENT_ENTRY_POINTS clientEntryPoints = {0};

  clientEntryPoints.Size = sizeof(RDP_CLIENT_ENTRY_POINTS);
  clientEntryPoints.Version = RDP_CLIENT_INTERFACE_VERSION;

  RdpClientEntry(&clientEntryPoints);

  context = freerdp_client_context_new(&clientEntryPoints);
  if (!context)
    return;

  context->settings->ServerPort = port;
  char *hn = malloc(strlen(host) + 1);
  memset(hn, 0, strlen(host) + 1);
  memcpy(hn, host, strlen(host));
  context->settings->ServerHostname = hn;
  char *n = malloc(strlen(name) + 1);
  memset(n, 0, strlen(name) + 1);
  memcpy(n, name, strlen(name));
  context->settings->Username = n;
  char *pwd = malloc(strlen(password) + 1);
  memset(pwd, 0, strlen(password) + 1);
  memcpy(pwd, password, strlen(password));
  context->settings->Password = pwd;
//  context->settings->DesktopWidth = 1920;
//  context->settings->DesktopHeight = 1080;
  if (!stream_dump_register_handlers(
          context, CONNECTION_STATE_MCS_CREATE_REQUEST, FALSE))
    goto out;

  if (freerdp_client_start(context) != 0) {
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

// int main(int argc, char *argv[]) {
//   int rc = 1;
//   int status;
//   HANDLE thread;
//   xfContext *xfc;
//   DWORD dwExitCode;
//   rdpContext *context;
//   rdpSettings *settings;
//   RDP_CLIENT_ENTRY_POINTS clientEntryPoints = {0};

//   clientEntryPoints.Size = sizeof(RDP_CLIENT_ENTRY_POINTS);
//   clientEntryPoints.Version = RDP_CLIENT_INTERFACE_VERSION;

//   RdpClientEntry(&clientEntryPoints);

//   context = freerdp_client_context_new(&clientEntryPoints);
//   if (!context)
//     return 1;

//   settings = context->settings;
//   xfc = (xfContext *)context;

//   status = freerdp_client_settings_parse_command_line(context->settings,
//   argc,
//                                                       argv, FALSE);
//   if (status) {
//     rc = freerdp_client_settings_command_line_status_print(settings, status,
//                                                            argc, argv);

//     if (freerdp_settings_get_bool(settings, FreeRDP_ListMonitors))
//       xf_list_monitors(xfc);

//     goto out;
//   }

//   if (!stream_dump_register_handlers(
//           context, CONNECTION_STATE_MCS_CREATE_REQUEST, FALSE))
//     goto out;

//   if (freerdp_client_start(context) != 0)
//     goto out;

//   thread = freerdp_client_get_thread(context);

//   WaitForSingleObject(thread, INFINITE);
//   GetExitCodeThread(thread, &dwExitCode);
//   rc = xf_exit_code_from_disconnect_reason(dwExitCode);

//   freerdp_client_stop(context);

// out:
//   freerdp_client_context_free(context);

//   return rc;
// }

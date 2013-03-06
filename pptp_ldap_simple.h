/* 
 File:   pptp_ldap_simple.c
 Copyright (C) 2013  Arthur Tumanyan <arthurtumanyan@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PPTP_LDAP_SIMPLE_H
#define	PPTP_LDAP_SIMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <pppd/pppd.h>
#include <ldap.h>

#ifdef	__cplusplus
extern "C" {
#endif
#define BUFLEN 1024
#define SUCCESS 1
#define FAILURE -1
#define AUTH_ERR 0
#define LDAP_FILT_MAXSIZ 1024
//
char pppd_version[] = VERSION;
char ident[] = "pptp_ldap_simple";
LDAP *Ldap;
static char ldap_host[BUFLEN] = "127.0.0.1";
static char ldap_dn[BUFLEN];
static char ldap_pw[BUFLEN];
static char userbasedn[BUFLEN];
static int  ldap_port = LDAP_PORT;
static int  ldap_timeout = 15;
static int  ldap_nettimeout = 10;
static int  ldap_usetls = 0;


int rc,ldap_errno;
int version = LDAP_VERSION3;
char searchstr[LDAP_FILT_MAXSIZ];
char userdn[BUFLEN];

LDAP *ldap;


static option_t ldap_options[] = {

	{ "ldap_host", o_string, ldap_host,
	  "LDAP server host name",
	  OPT_PRIV | OPT_STATIC, NULL, (BUFLEN - 1)},

	{ "ldap_dn", o_string, ldap_dn,
	  "DN to bind with to LDAP server",
	  OPT_PRIV | OPT_STATIC, NULL, (BUFLEN - 1)},

	{ "ldap_pwd", o_string, ldap_pw,
	  "DN password",
	  OPT_PRIV | OPT_STATIC, NULL, (BUFLEN - 1)},

	{ "ldap_port", o_int, &ldap_port,
	  "LDAP server port",
	  OPT_PRIV | OPT_STATIC},

	{ "ldap_userbasedn", o_string, userbasedn,
	  "LDAP user base DN",
	  OPT_PRIV | OPT_STATIC, NULL, (BUFLEN - 1)},

	{ "ldap_timeout", o_int, &ldap_timeout,
	  "LDAP search timeout",
	  OPT_PRIV | OPT_STATIC},

	{ "ldap_nettimeout", o_int, &ldap_nettimeout,
	  "LDAP network activity timeout",
	  OPT_PRIV | OPT_STATIC },
#ifdef USE_TLS
	{ "ldap_usetls", o_bool, &ldap_usetls,
	  "Connect to LDAP server using TLS", 1},
#endif
	{ NULL }
};

    static int pap_auth(char *user, char *password, char **msgp, struct wordlist **paddrs, struct wordlist **popts);
    static int pap_check(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PPTP_LDAP_SIMPLE_H */


/* 
 * File:   pptp_ldap_simple.c
 * Author: Arthur Tumanyan <arthurtumanyan@yahoo.com>
 *
 */

#include "pptp_ldap_simple.h"
/*
 * 
 */
int plugin_init() {
    add_options(ldap_options);
    pap_check_hook = pap_check;
    pap_auth_hook = pap_auth;
    info("%s: plugin initialized.", ident);
}

static int pap_check(void) {
    return SUCCESS;
}

static int pap_auth(char *user, char *password, char **msgp, struct wordlist **paddrs, struct wordlist **popts) {


    /* Initiate session and bind to LDAP server */
    ldap = ldap_init(ldap_host, ldap_port); 
    if (NULL == ldap) {
        error("%s: failed to initialize session\n", ident);
        return FAILURE;
    }

    /* Set LDAP specific options */

    if ((rc = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION,
            &version) != LDAP_OPT_SUCCESS)) {
        error("%s: failed to set protocol version\n", ident);
        return FAILURE;
    }

    if ((rc = ldap_set_option(ldap, LDAP_OPT_NETWORK_TIMEOUT,
            &ldap_nettimeout) != LDAP_OPT_SUCCESS)) {
        error("%s: failed to set network timeout version\n", ident);
        return FAILURE;
    }

    if ((rc = ldap_set_option(ldap, LDAP_OPT_TIMELIMIT,
            &ldap_timeout) != LDAP_OPT_SUCCESS)) {
        error("%s: failed to set timeout option\n", ident);
        return FAILURE;
    }

#ifdef USE_TLS

    if ((ldap_port == LDAPS_PORT) && ldap_usetls) {
        int tls_opt = LDAP_OPT_X_TLS_HARD;
        if ((rc = ldap_set_option(ldap, LDAP_OPT_X_TLS,
                (void *) &tls_opt)) != LDAP_SUCCESS) {
            ldap_get_option(ldap, LDAP_OPT_ERROR_NUMBER, &ldap_errno);
            error("%s: failed to set TLS option: %s\n", ident, ldap_err2string(rc));
            return FAILURE;
        }
    }

    if (ldap_usetls) {
        info("%s: Setting TLS option -> ON\n", ident);
        if ((rc = ldap_start_tls_s(ldap, NULL, NULL) != LDAP_SUCCESS)) {
            ldap_get_option(ldap, LDAP_OPT_ERROR_NUMBER, &ldap_errno);
            error("%s: failed to initiate TLS: %s\n", ident, ldap_err2string(ldap_errno));
            return FAILURE;
        }
    };

#endif

    if ((rc = ldap_bind_s(ldap, ldap_dn, ldap_pw, LDAP_AUTH_SIMPLE)) != LDAP_SUCCESS) {
        ldap_get_option(ldap, LDAP_OPT_ERROR_NUMBER, &ldap_errno);
        error("%s: failed to bind: %s\n", ident, ldap_err2string(rc));
        ldap_unbind(ldap);
        return FAILURE;
    }

    if (FAILURE == (rc = snprintf(searchstr, LDAP_FILT_MAXSIZ, "cn=%s,%s", user, userbasedn))) {
        error("%s: LDAP search string too big\n", ident);
        ldap_unbind(ldap);
    }
    info("%s: search string: %s\n", ident, searchstr);

    /* User authentication */
    
    rc = ldap_simple_bind_s(ldap, searchstr, password);
    if (rc != LDAP_SUCCESS) {
        ldap_unbind(ldap);
        error("%s: Username or password incorrect\n", ident);
        *msgp = "Username or password incorrect!";
        return AUTH_ERR;
    } else {
        ldap_unbind(ldap);
        info("%s: Auth success\n", ident);
        *msgp = "Access OK!";
        return SUCCESS;
    }

}
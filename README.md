pptp_ldap_simple
================

A simple plugin for PAP authentication using LDAP

Plugin supports only PAP method for now
Add this to the /etc/ppp/options file

  plugin /etc/ppp/libpptp_ldap_simple.so
  ldap_host localhost
  ldap_userbasedn ou=Customers,dc=example,dc=co,dc=uk
  ldap_dn cn=admin,dc=example,dc=co,dc=uk
  ldap_pwd secret
  ldap_timeout 30
  
  Plz, do not hestitate to write about found bugs and sugesstions

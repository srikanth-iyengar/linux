# DNS

/etc/nsswitch.conf this is way linux and free bsd system provide way for configuring DNS


```conf
hosts: dns [!UNVAIL=return] files
```

This means that if DNS is available but the name is not available then the query should fail instead of tyring 
the next entry


```conf
hosts: files dns
```

This configuration gives precedence to static host file /etc/hosts. DNS is consulted for names that are not 
resolvable through /etc/hosts

## DNS Namespace

DNS Namespace is organized in a tree structure. A name in node in the forward branch and ip in the reverse branch

If 1.2.3.4 is ip address of www.srikanthk.in. Then node for forward branch is www.srikanthk.in. and for reverse 
branch it will be 1.2.3.4.in-addr.arpa.. Both of the name end with a "." meaning that both are FQDN


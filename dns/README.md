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


## How DNS Works

Name servers across the world work together to answer queries

Role of Nameservers
1. Answer queries about site's hostnames and IP addresses
2. Asks about local and remote hosts on behalf of users
3. Caches the answers for queries so that it can answer faster next time
4. It communicates with other name servers for synchronizing the data


### Name server anatomy
| Type of Server    | Description |
| ----------------- | ----------- |
| Authoritative     | Offically represents a zone |
| -- master/primary | The master server for a zone, data from disk |
| -- slave/secondary| Copies its data from master |
| -- stub           | Like slave but copies only nameserver data |
| -- distribution   | A server advertised only within a domain |
| Non Authoritative | Answers a query from cache; doesn't know it is still valid |
| -- caching        | Caches data from previous dont have its own zone |
| -- forwarder      | Perform queries on behalf of many client, builds large cache |


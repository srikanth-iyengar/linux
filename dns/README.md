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


#### Authoritative servers
Master server is truly authoritative for a zone, Master server loads the data from the disk.
Slave server loads the master servers address on boot, and gets the data of master through the zone transfer
operation
A Stub server is a special kind of server that only caches name server information
It's fine for a single machine to act as a master for one zone, and slave for the rest of the zones

A caching only name server loads the address of root server on boot, and caches the rest of the queries
caching servers and not authoritative for any zone except localhost

### Recursive and non-recursive DNS
If a non recursive dns has a answer in the cache then it will return the answer for the query. But if it doesnt
know the answer for the query then it will simply refer you to other authoritative server which might know the 
answer.
Authoritative servers are non recursive servers

For security reasons, a organizations externally accessible name servers should always be non recursive

Caching the queries increases efficiency, since cached queris are almost free

The cached result can be served for TTL (Time to live) which is specified in the master server file

DNS has also implemented negative caching. That is, they remember when a query fails, and they do not repeat the query
until the negative caching TTL value has expired.

## Name server Resource Records

### Zone parser commands
Zone parser commands are used to make zone files more readable and easire to maintain
Once read none of these parser commands are part of zone's data.

Three commands ($ORIGIN, $INCLUDE, $TTL)
As the name server reads the zone file, it adds the default domain ($ORIGIN) for all those entries which are not 
fully qualified domain

### Resource Records
The SOA record
The SOA records begins the beginning of a zone, a group of resouce records located at a same place within DNS namespace
Two zones for a DNS namespace: forward translationand reverse translation

srikanthk.in         IN          SOA            ns1.srikanthk.in.            hostmaster.srikanthk.in. (
                     2024050200 ; Serial Numnber
                     10800      ; Refresh timeout (3 hrs)
                     1200       ; Retry timeout   (20 minutes)
                     36000000   ; Expire          (40+ days)
                     3600       ; Minimum         (1 hr)
                     )

Bloated serial number problem: What if you set the serial number to a very high value

- **Refresh**: How often a slave server should check on the master for a update
- **Retry**: If master doesn't respond after retry slave will again try
- **Expire**: The slaves will serve the domains authoritative in abscence of master
- **Minimum**: Time to live for negative cache

The NS record
This record defines the name server records for a zone
Extra servers in the child are OK until atleast one of the childs address is in the parents zone file

```conf
                    IN          NS              172.130.12.12
                    IN          NS              172.130.2.2
```

The A record
This record maps a given Name to a IP address

```conf
jenkins             IN          A               172.17.0.3
```

The PTR record
This record maps reverse maps a IP address to a domain name

```conf
1                   IN          PTR             ns2.srikanthk.in
```

# Bind Specific things
Bind the Berkeley Internet Name Domain System, is a open source software package by Internet System Consortium (ISC)
for linux, windows and macOS

## Components of BIND
- **Named:** A name server daemon which can answer client queries
- **Resolver LIB:** A resolver library which queries servers on behalf of users
- **CLI**: nslookup, dig, drill
- **RNDC**: A utility program to manager to manage dns remotely

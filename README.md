# Project 1: ELastic Array & Disk Analyzer 

**Author**: Chuxi Wang


## About the project

### What is a elasic array and why do we use it?

### What is da? 

### Program options

### Included files

### Program output

```console
[miasly@dirtmouth P1-mialsy]$ ./da /etc/systemd
da.c:251:main(): Done parsing arguments.
da.c:252:main(): Sorting by: [size], limit: [0]
da.c:255:main(): Directory to analyze: [/etc/systemd]
da.c:258:main(): Fullpath of dir: [/etc/systemd]
elist.c:62:elist_create(): Inittializing new elist: capacity=[10], item_size=[1016], byte=[10160]
da.c:268:main(): Created an elist to hold dirs. 
elist.c:130:elist_add(): Resizing the list, new capacity 20
elist.c:130:elist_add(): Resizing the list, new capacity 40
da.c:297:main(): checking the list 
da.c:298:main(): List size is: 23
da.c:157:calColumn(): Display columns: 125
                                       /etc/systemd/system/getty.target.wants/getty@tty1.service       1.9 KiB    Feb 01 2021
                                                                        /etc/systemd/system.conf       1.7 KiB    Feb 01 2021
                                                                          /etc/systemd/user.conf       1.2 KiB    Feb 18 2021
                                                                        /etc/systemd/logind.conf       1.1 KiB    Feb 01 2021
                                                                      /etc/systemd/journald.conf       1.0 KiB    Feb 01 2021
                                                                      /etc/systemd/resolved.conf      1021.0 B    Feb 01 2021
                                                                         /etc/systemd/sleep.conf       790.0 B    Feb 01 2021
                                                                /etc/systemd/journal-remote.conf       695.0 B    Feb 01 2021
                                                                /etc/systemd/journal-upload.conf       672.0 B    Feb 01 2021
                                                                     /etc/systemd/timesyncd.conf       669.0 B    Feb 01 2021
                                                                      /etc/systemd/coredump.conf       615.0 B    Mar 10 2021
                                                                      /etc/systemd/networkd.conf       609.0 B    Feb 01 2021
                                                                         /etc/systemd/homed.conf       541.0 B    Feb 01 2021
                                    /etc/systemd/system/multi-user.target.wants/remote-fs.target       530.0 B    Feb 01 2021
                                                                        /etc/systemd/pstore.conf       529.0 B    Feb 01 2021
                                     /etc/systemd/user/sockets.target.wants/gpg-agent-ssh.socket       308.0 B    Feb 18 2021
                                 /etc/systemd/user/sockets.target.wants/gpg-agent-browser.socket       298.0 B    Feb 18 2021
                                   /etc/systemd/user/sockets.target.wants/gpg-agent-extra.socket       281.0 B    Feb 18 2021
                                        /etc/systemd/system/multi-user.target.wants/sshd.service       250.0 B    Feb 01 2021
                                         /etc/systemd/user/sockets.target.wants/gpg-agent.socket       234.0 B    Feb 18 2021
                                      /etc/systemd/system/multi-user.target.wants/dhcpcd.service       233.0 B    Feb 01 2021
                                           /etc/systemd/user/sockets.target.wants/dirmngr.socket       204.0 B    Feb 18 2021
                                    /etc/systemd/user/sockets.target.wants/p11-kit-server.socket       147.0 B    Feb 18 2021
```


### Testing

For running the test cases, use ```make test```. For updating test cases, runs ```make testupdate```. 

For more detailed testing, please refer to below:

- Run all tests:
    ```console
    [miasly@dirtmouth P1-mialsy]$ make test
    ```
- Run one test (let's say we want to run test x):
    ```console
    [miasly@dirtmouth P1-mialsy]$ make test run=[x]
    ```

- Run a few tests (let's say we want to run test x, y, z):
    ```console
    [miasly@dirtmouth P1-mialsy]$ make test run='[x] [y] [z]'
    ```

- **Note**: before run the test case, make sure you are in the correct directory with the makefile.
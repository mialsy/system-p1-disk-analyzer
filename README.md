# Project 1: ELastic Array & Disk Analyzer 

**Author**: [Chuxi Wang](https://github.com/mialsy "click here to see my awesome profile, not really awesome but welcome to my space :)") 

**Summmary**: This is the project 1 for CS 521 System Programming. In this project, I implemented an elasic array and a command line utility - ```da```, which helps the user track of the disk space used by each files under the input directory. 

**Updated new feature**: [this branch](https://github.com/usf-cs521-sp21/P1-mialsy/tree/directtory-traverse-feature) has the new feature to print subdirectory and its total size, plase check [addiotnal implementation note] for implementation of this feature. 

## About the project

*Grouwn-ups like numbers.* as said by Antoine De Saint-Exupery. Unfortunately as resonsible adult we have to accept that the fact number matters a lot especially when you are running out of disk space. And this is where the disk analyizer util ```da``` comes into the play. 

### What is da? 

```da``` is the short of disk analyzer. It would take directory as input argument and print out the infomation of files in the directory.

This main purpose of command line util ```da``` built in this project is to analyze the disk usage for each file under given directory and to track of last access time for each file. With the sorting by time or size features, you would be able to have detailed idea about how the disk is consumed, and then make a decision on what needs to go and what stays.

The work flow of ```da``` is shown as below:

<img src="https://github.com/usf-cs521-sp21/P1-mialsy/blob/main/res/Presentation1.gif" alt="demo of the da workflow" width="900">

For input options of the da, please refer to the section [**Program options**](https://github.com/usf-cs521-sp21/P1-mialsy/tree/main#program-options).

For example of the da domentrastion, please refer to the section [**Program output**](https://github.com/usf-cs521-sp21/P1-mialsy/tree/main#program-output).

### What is a elastic array and why do we use it?

An elastic array is a resizable array like ArrayList data structure in Java. The downside of the original array type in C is that it cannot be resized, which would be a big problem considering that we would not be able to tell how many files exists in the directory ahead of time. 

The way that elastic array works in terms of adding and resizing is shown in the following diagram:

<img src="https://github.com/usf-cs521-sp21/P1-mialsy/blob/main/res/Presentation2.gif" alt="demo of the elist adding operation" width="900">

Except adding operations, other opeartions on the elist pretty much mirror what is in a normal array. Note that reset capacity is also allowed in this implementation, which will realloc space and may truncate the array if capacity is set to be smaller than the original size. 


### Other options of data structure for da rather than elist? 

As mentioned above, we consider using elist as we want to be able to dynamically adjust the size of the list. For this purpose, we can consider using a linkedlist structre, which also supports appending and deleting element and has a flexible size. But consider the linkedlist has larger overhead (extra space to store next pointer) and worse locality, while not providing additonal speed advantage, it would not an optimal option. 

Another possible way to implent this is with trees, however consider we do not need the indexing for disk analyzation it would be overkilling for our problem.

## Program options

Command line options can provided to da to personalized the display. Options are listed as below:

```console
[miasly@dirtmouth P1-mialsy]$ ./da -h
Disk Analyzer (da): analyzes disk space usage
Usage: ./da [-ahs] [-l limit] [directory]

If no directory is specified, the current working directory is used.

Options:
    * -a              Sort the files by time of last access (descending)
    * -h              Display help/usage information
    * -l limit        Limit the output to top N files (default=unlimited)
    * -s              Sort the files by size (default, ascending)
```
The display can be sorted by last access time (decending) or by size (ascending), by default, the displayed list is sorted by size. A optional limit can be provided to only list N files, the default is unlimited. 

## Included files

Here is a list of files that is included:

- **Makefile**: Included to compile and run the programs
- **da.c**: The program driver. It includes:
    - ```main(int, char *[])``` main function that takes input from command line arguments, parses command line argument, travese and sort the dirtory element list, and output to terminal.
    - ```struct dir_element``` which holds tempary information of the files under the directory to be used later for sorting and output information.
    - helper function ```traverse(elist *, DIR *, char *, char *))``` which would be called recursively to traverse the current directory and subdirectories.
    - comparators ```compareTime(const void *, const void *)``` and ```compareSize(const void *, const void *)```that are used for sorting the elist.
    - helper function ```calColumn(void)``` that is used to calculate the terminal column for better visualization.
- **elist.c**: This include a elastic list data structure ```elist``` and functions that supports the ```elist``` operations.       
    - ```struct elist``` which stores the element and metadata of the elist.
    - elist opreation functions including:
        - creating, sorting, and destorying the elist;
        - clearing out the memory on elist;
        - getting and resetting the elist capacity;
        - getting the elist size;
        - adding, getting, setting, and searching one element in the elist.
    - helper method ```idx_is_valid(struct elist *, size_t)``` to support checking if a given index is valid.
- **util.c**: This includes utilly methods for converting raw size in byte and time in millosecond to human readble format. 
- **logger.h**: Included for log output. 

Header filess are also included for the c files.

For compiling the program, runs:
```console
[miasly@dirtmouth P1-mialsy]$ make
```

For runing the da util, runs:
```console
[miasly@dirtmouth P1-mialsy]$ ./da [directory_path]
```
If the path is not passed in, a defaullt . (current directory) would be used for da util.

## Program output

Here is an example out put of running da on the etc/systemd directory. 

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
da.c:157:calColumn(): Display columns: 89
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
...ystemd/user/sockets.target.wants/gpg-agent-browser.socket       298.0 B    Feb 18 2021
.../systemd/user/sockets.target.wants/gpg-agent-extra.socket       281.0 B    Feb 18 2021
    /etc/systemd/system/multi-user.target.wants/sshd.service       250.0 B    Feb 01 2021
     /etc/systemd/user/sockets.target.wants/gpg-agent.socket       234.0 B    Feb 18 2021
  /etc/systemd/system/multi-user.target.wants/dhcpcd.service       233.0 B    Feb 01 2021
       /etc/systemd/user/sockets.target.wants/dirmngr.socket       204.0 B    Feb 18 2021
/etc/systemd/user/sockets.target.wants/p11-kit-server.socket       147.0 B    Feb 18 2021
```

## Testing

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

## Addtional implementation note

In order to be able to print the subdirectory, I simply add the subdirectory to the new list as well. 

For calculating the total size of the subdirectories, the travese function return the size of the sub directory rather than the status code in the main branch, in this way we can calculate the total size recursively. 

## Demo of printing directory
```
[miasly@dirtmouth P1-mialsy]$ ./da /etc/ssl/certs
da.c:224:main(): Done parsing arguments.
da.c:225:main(): Sorting by: [size], limit: [0]
da.c:228:main(): Directory to analyze: [/etc/ssl/certs]
da.c:231:main(): Fullpath of dir: [/etc/ssl/certs]
elist.c:62:elist_create(): Inittializing new elist: capacity=[10], item_size=[1016], byte=[10160]
da.c:241:main(): Created an elist to hold dirs. 
elist.c:130:elist_add(): Resizing the list, new capacity 20
elist.c:130:elist_add(): Resizing the list, new capacity 40
elist.c:130:elist_add(): Resizing the list, new capacity 80
elist.c:130:elist_add(): Resizing the list, new capacity 160
elist.c:130:elist_add(): Resizing the list, new capacity 320
elist.c:130:elist_add(): Resizing the list, new capacity 640
da.c:270:main(): checking the list 
da.c:271:main(): List size is: 391
da.c:159:calColumn(): Display columns: 116
                                                     /etc/ssl/certs/ca-certificates.crt     199.5 KiB    Mar 10 2021
                                                                    /etc/ssl/certs/java     149.4 KiB    Mar 10 2021
                                                            /etc/ssl/certs/java/cacerts     145.4 KiB    Feb 01 2021
                                                                    /etc/ssl/certs/edk2       4.0 KiB    Mar 10 2021
                                                           /etc/ssl/certs/ACCVRAIZ1.pem       2.7 KiB    Feb 01 2021
                                                              /etc/ssl/certs/a94d09e5.0       2.7 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3c9a4d3b.0       2.7 KiB    Feb 01 2021
                                    /etc/ssl/certs/Chambers_of_Commerce_Root_-_2008.pem       2.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/1eb37bdf.0       2.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/c47d9980.0       2.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/9f533518.0       2.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/0c4c9b6c.0       2.5 KiB    Feb 01 2021
                                      /etc/ssl/certs/Global_Chambersign_Root_-_2008.pem       2.5 KiB    Feb 01 2021
                                                  /etc/ssl/certs/QuoVadis_Root_CA_3.pem       2.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/9339512a.0       2.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/76faf6c0.0       2.3 KiB    Feb 01 2021
                                                    /etc/ssl/certs/Certigna_Root_CA.pem       2.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/f51bb24c.0       2.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/302904dd.0       2.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/cb156124.0       2.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5e98733a.0       2.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/f0cd152c.0       2.2 KiB    Feb 01 2021
                                     /etc/ssl/certs/E-Tugra_Certification_Authority.pem       2.2 KiB    Feb 01 2021
                           /etc/ssl/certs/Entrust_Root_Certification_Authority_-_G4.pem       2.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5273a94c.0       2.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3e44d2f7.0       2.2 KiB    Feb 01 2021
                                              /etc/ssl/certs/TrustCor_RootCert_CA-2.pem       2.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d0cddf45.0       2.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d16a5865.0       2.1 KiB    Feb 01 2021
           /etc/ssl/certs/Autoridad_de_Certificacion_Firmaprofesional_CIF_A62634068.pem       2.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3bde41ac.0       2.1 KiB    Feb 01 2021
             /etc/ssl/certs/Hellenic_Academic_and_Research_Institutions_RootCA_2015.pem       2.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/dc99f41e.0       2.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/32888f65.0       2.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/48a195d8.0       2.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/cc450945.0       2.1 KiB    Feb 01 2021
                                                          /etc/ssl/certs/Izenpe.com.pem       2.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/06dc52d5.0       2.1 KiB    Feb 01 2021
                      /etc/ssl/certs/SSL.com_EV_Root_Certification_Authority_RSA_R2.pem       2.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/583d0756.0       2.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/6fa5da56.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/33ee480d.0       2.0 KiB    Feb 01 2021
                            /etc/ssl/certs/SSL.com_Root_Certification_Authority_RSA.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/fc5a8f99.0       2.0 KiB    Feb 01 2021
                               /etc/ssl/certs/USERTrust_RSA_Certification_Authority.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/35105088.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/edcbddb5.0       2.0 KiB    Feb 01 2021
                            /etc/ssl/certs/Trustwave_Global_Certification_Authority.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/f249de83.0       2.0 KiB    Feb 01 2021
                                  /etc/ssl/certs/COMODO_RSA_Certification_Authority.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d6325660.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d4c339cb.0       2.0 KiB    Feb 01 2021
                                         /etc/ssl/certs/Certum_Trusted_Network_CA_2.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5cf9d536.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/080911ac.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/cb1c3204.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/40193066.0       2.0 KiB    Feb 01 2021
                                                    /etc/ssl/certs/QuoVadis_Root_CA.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/bd43e1dd.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/68dd7389.0       2.0 KiB    Feb 01 2021
                                             /etc/ssl/certs/Hongkong_Post_Root_CA_3.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5046c355.0       2.0 KiB    Feb 01 2021
                                      /etc/ssl/certs/Actalis_Authentication_Root_CA.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/57bcb2da.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5f47b495.0       2.0 KiB    Feb 01 2021
                                            /etc/ssl/certs/SwissSign_Silver_CA_-_G2.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/930ac5d2.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3c860d51.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/4f316efb.0       2.0 KiB    Feb 01 2021
                                              /etc/ssl/certs/SwissSign_Gold_CA_-_G2.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d7e8dc79.0       2.0 KiB    Feb 01 2021
                                                  /etc/ssl/certs/QuoVadis_Root_CA_2.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/7a819ef2.0       2.0 KiB    Feb 01 2021
                                   /etc/ssl/certs/ePKI_Root_Certification_Authority.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/9d6523ce.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/ca6e4ad9.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/bf53fb88.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/9591a472.0       2.0 KiB    Feb 01 2021
                       /etc/ssl/certs/Microsoft_RSA_Root_Certificate_Authority_2017.pem       2.0 KiB    Feb 01 2021
                           /etc/ssl/certs/NAVER_Global_Root_Certification_Authority.pem       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d39b0a2c.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3fb36b73.0       2.0 KiB    Feb 01 2021
                                                              /etc/ssl/certs/a2c66da8.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/75d1b2ed.0       1.9 KiB    Feb 01 2021
                                            /etc/ssl/certs/DigiCert_Trusted_Root_G4.pem       1.9 KiB    Feb 01 2021
                                                        /etc/ssl/certs/CFCA_EV_ROOT.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/9282e51c.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/0b1b94ef.0       1.9 KiB    Feb 01 2021
                                              /etc/ssl/certs/GDCA_TrustAUTH_R5_ROOT.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/ab59055e.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/0f6fa695.0       1.9 KiB    Feb 01 2021
                                             /etc/ssl/certs/GlobalSign_Root_CA_-_R6.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/985c1f52.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/dc4d6a89.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/cd8c0d63.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b936d1c6.0       1.9 KiB    Feb 01 2021
                                                    /etc/ssl/certs/AC_RAIZ_FNMT-RCM.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5a250ea7.0       1.9 KiB    Feb 01 2021
                                  /etc/ssl/certs/Staat_der_Nederlanden_Root_CA_-_G3.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5a4d6896.0       1.9 KiB    Feb 01 2021
                                    /etc/ssl/certs/Staat_der_Nederlanden_EV_Root_CA.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/03179a64.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3c6676aa.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/6187b673.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/4042bcee.0       1.9 KiB    Feb 01 2021
                                                        /etc/ssl/certs/ISRG_Root_X1.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/9576d26b.0       1.9 KiB    Feb 01 2021
                                                    /etc/ssl/certs/CA_Disig_Root_R2.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/2ae6433e.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/1e08bfd1.0       1.9 KiB    Feb 01 2021
                                   /etc/ssl/certs/IdenTrust_Public_Sector_Root_CA_1.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/4be590e0.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/064e0aa9.0       1.9 KiB    Feb 01 2021
                                               /etc/ssl/certs/QuoVadis_Root_CA_1_G3.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/e442e424.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/e18bfb83.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/1f58a078.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d18e9066.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/749e9e03.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/ef954a4e.0       1.9 KiB    Feb 01 2021
                                      /etc/ssl/certs/IdenTrust_Commercial_Root_CA_1.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/52b525c7.0       1.9 KiB    Feb 01 2021
                                               /etc/ssl/certs/QuoVadis_Root_CA_2_G3.pem       1.9 KiB    Feb 01 2021
                                               /etc/ssl/certs/QuoVadis_Root_CA_3_G3.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/0f5dc4f3.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/da7377f6.0       1.9 KiB    Feb 01 2021
                                                         /etc/ssl/certs/GTS_Root_R2.pem       1.9 KiB    Feb 01 2021
                                                         /etc/ssl/certs/GTS_Root_R1.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/f013ecaf.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/626dceaf.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/1001acf7.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/54657681.0       1.9 KiB    Feb 01 2021
                                        /etc/ssl/certs/UCA_Extended_Validation_Root.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/e8de2f56.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/2d9dafe4.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/c559d742.0       1.9 KiB    Feb 01 2021
                                             /etc/ssl/certs/Buypass_Class_3_Root_CA.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/82223c44.0       1.9 KiB    Feb 01 2021
                                             /etc/ssl/certs/Buypass_Class_2_Root_CA.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/aeb67534.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/349f2832.0       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/EC-ACC.pem       1.9 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b727005e.0       1.8 KiB    Feb 01 2021
                                                  /etc/ssl/certs/UCA_Global_G2_Root.pem       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/dbc54cab.0       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/c01eb047.0       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/a9d40e02.0       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5f618aec.0       1.8 KiB    Feb 01 2021
                                                 /etc/ssl/certs/AffirmTrust_Premium.pem       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/304d27c3.0       1.8 KiB    Feb 01 2021
                                                 /etc/ssl/certs/certSIGN_Root_CA_G2.pem       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b0ed035a.0       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5f15c80c.0       1.8 KiB    Feb 01 2021
                                                    /etc/ssl/certs/Amazon_Root_CA_2.pem       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/6d41d539.0       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/fb5fa911.0       1.8 KiB    Feb 01 2021
                                                 /etc/ssl/certs/TWCA_Global_Root_CA.pem       1.8 KiB    Feb 01 2021
                                              /etc/ssl/certs/TeliaSonera_Root_CA_v1.pem       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5cd81ad7.0       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/63a2c897.0       1.8 KiB    Feb 01 2021
                                                              /etc/ssl/certs/c01cdfa2.0       1.7 KiB    Feb 01 2021
                     /etc/ssl/certs/VeriSign_Universal_Root_Certification_Authority.pem       1.7 KiB    Feb 01 2021
                                                              /etc/ssl/certs/524d9b43.0       1.7 KiB    Feb 01 2021
                                /etc/ssl/certs/Entrust_Root_Certification_Authority.pem       1.6 KiB    Feb 01 2021
                                                              /etc/ssl/certs/bf64f35b.0       1.6 KiB    Feb 01 2021
                                                              /etc/ssl/certs/6b99d060.0       1.6 KiB    Feb 01 2021
                                                              /etc/ssl/certs/31188b5e.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/ff34af3f.0       1.5 KiB    Feb 01 2021
                       /etc/ssl/certs/TUBITAK_Kamu_SM_SSL_Kok_Sertifikasi_-_Surum_1.pem       1.5 KiB    Feb 01 2021
                                   /etc/ssl/certs/D-TRUST_Root_Class_3_CA_2_EV_2009.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d7746a63.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d4dae3dd.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/455f1b52.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/02265526.0       1.5 KiB    Feb 01 2021
                           /etc/ssl/certs/Entrust_Root_Certification_Authority_-_G2.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/1df5a75f.0       1.5 KiB    Feb 01 2021
                                            /etc/ssl/certs/Comodo_AAA_Services_root.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/ee64a828.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/75680d2e.0       1.5 KiB    Feb 01 2021
                                      /etc/ssl/certs/D-TRUST_Root_Class_3_CA_2_2009.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/c28a8a30.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/76579174.0       1.5 KiB    Feb 01 2021
                                                /etc/ssl/certs/XRamp_Global_CA_Root.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/7c302982.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/706f604c.0       1.5 KiB    Feb 01 2021
                                              /etc/ssl/certs/TrustCor_RootCert_CA-1.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/1636090b.0       1.5 KiB    Feb 01 2021
             /etc/ssl/certs/Hellenic_Academic_and_Research_Institutions_RootCA_2011.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5d3033c5.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/40dc992e.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3e7271e8.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/aee5f10d.0       1.5 KiB    Feb 01 2021
                           /etc/ssl/certs/Entrust.net_Premium_2048_Secure_Server_CA.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/c2c1704e.0       1.5 KiB    Feb 01 2021
                                                      /etc/ssl/certs/TrustCor_ECA-1.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/7aaf71c0.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5a3f0ff8.0       1.5 KiB    Feb 01 2021
                                      /etc/ssl/certs/COMODO_Certification_Authority.pem       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/40547a79.0       1.5 KiB    Feb 01 2021
                                                              /etc/ssl/certs/988a38cb.0       1.4 KiB    Feb 01 2021
                        /etc/ssl/certs/NetLock_Arany__Class_Gold__F__tan__s__tv__ny.pem       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/60afe812.0       1.4 KiB    Feb 01 2021
                                                /etc/ssl/certs/Starfield_Class_2_CA.pem       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/f387163d.0       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/23f4c490.0       1.4 KiB    Feb 01 2021
                                      /etc/ssl/certs/Microsec_e-Szigno_Root_CA_2009.pem       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/8160b96c.0       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/e8651083.0       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/219d9499.0       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/f081611a.0       1.4 KiB    Feb 01 2021
                                                 /etc/ssl/certs/Go_Daddy_Class_2_CA.pem       1.4 KiB    Feb 01 2021
                  /etc/ssl/certs/Starfield_Services_Root_Certificate_Authority_-_G2.pem       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/09789157.0       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/10531352.0       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/4304c5e5.0       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/2fa87019.0       1.4 KiB    Feb 01 2021
                             /etc/ssl/certs/Network_Solutions_Certificate_Authority.pem       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/85cde254.0       1.4 KiB    Feb 01 2021
                           /etc/ssl/certs/Starfield_Root_Certificate_Authority_-_G2.pem       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/4bfab552.0       1.4 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5443e9e3.0       1.3 KiB    Feb 01 2021
                            /etc/ssl/certs/Go_Daddy_Root_Certificate_Authority_-_G2.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/81b9768f.0       1.3 KiB    Feb 01 2021
                                        /etc/ssl/certs/T-TeleSec_GlobalRoot_Class_3.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/cbf06781.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/1e1eab7c.0       1.3 KiB    Feb 01 2021
                                  /etc/ssl/certs/DigiCert_High_Assurance_EV_Root_CA.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d06393bb.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/bc3f2570.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/244b5494.0       1.3 KiB    Feb 01 2021
                                        /etc/ssl/certs/T-TeleSec_GlobalRoot_Class_2.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/1e09d511.0       1.3 KiB    Feb 01 2021
                                           /etc/ssl/certs/Certum_Trusted_Network_CA.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/bdacca6f.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/95aff9e3.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/4a6481c9.0       1.3 KiB    Feb 01 2021
                                                    /etc/ssl/certs/Secure_Global_CA.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/48bec511.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b66938e9.0       1.3 KiB    Feb 01 2021
                                             /etc/ssl/certs/GlobalSign_Root_CA_-_R2.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/111e6273.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/cf701eeb.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/69105f4f.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/f39fc864.0       1.3 KiB    Feb 01 2021
                                         /etc/ssl/certs/DigiCert_Assured_ID_Root_CA.pem       1.3 KiB    Feb 01 2021
                                                      /etc/ssl/certs/SecureTrust_CA.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b1159c4c.0       1.3 KiB    Feb 01 2021
                                     /etc/ssl/certs/OISTE_WISeKey_Global_Root_GB_CA.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/dfc0fe80.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/e73d606e.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3513523f.0       1.3 KiB    Feb 01 2021
                                             /etc/ssl/certs/DigiCert_Global_Root_CA.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/399e7759.0       1.3 KiB    Feb 01 2021
                                                            /etc/ssl/certs/Certigna.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/fde84897.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/e113c810.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/76cb8f92.0       1.3 KiB    Feb 01 2021
                                              /etc/ssl/certs/Cybertrust_Global_Root.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/343eb6cb.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/8d6437c3.0       1.3 KiB    Feb 01 2021
                                         /etc/ssl/certs/DigiCert_Assured_ID_Root_G2.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/9d04f354.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/2923b3f9.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/f459871d.0       1.3 KiB    Feb 01 2021
                                                 /etc/ssl/certs/emSign_Root_CA_-_G1.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/607986c7.0       1.3 KiB    Feb 01 2021
                                             /etc/ssl/certs/DigiCert_Global_Root_G2.pem       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/c90bc37d.0       1.3 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b7a5b843.0       1.2 KiB    Feb 01 2021
                                   /etc/ssl/certs/TWCA_Root_Certification_Authority.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b7db1890.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b0f3e76e.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/e36a6752.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/cd58d51e.0       1.2 KiB    Feb 01 2021
                                           /etc/ssl/certs/Baltimore_CyberTrust_Root.pem       1.2 KiB    Feb 01 2021
                                                  /etc/ssl/certs/GlobalSign_Root_CA.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3ad48a91.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d59297b8.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b872f2b4.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/5ad8a5d6.0       1.2 KiB    Feb 01 2021
                                      /etc/ssl/certs/Security_Communication_RootCA2.pem       1.2 KiB    Feb 01 2021
                                               /etc/ssl/certs/Atos_TrustedRoot_2011.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/653b494a.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/fe8a2cd8.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/406c9bb1.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/a81e292b.0       1.2 KiB    Feb 01 2021
                                                     /etc/ssl/certs/SZAFIR_ROOT_CA2.pem       1.2 KiB    Feb 01 2021
                                                 /etc/ssl/certs/emSign_Root_CA_-_C1.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/dbff3a01.0       1.2 KiB    Feb 01 2021
                                                 /etc/ssl/certs/SecureSign_RootCA11.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/ab5346f4.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/18856ac4.0       1.2 KiB    Feb 01 2021
                                                 /etc/ssl/certs/Trustis_FPS_Root_CA.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/c51c224c.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/d853d49e.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/062cdee6.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/1e8e7201.0       1.2 KiB    Feb 01 2021
                                             /etc/ssl/certs/GlobalSign_Root_CA_-_R3.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/a3896b44.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/f3377b1b.0       1.2 KiB    Feb 01 2021
                                      /etc/ssl/certs/Security_Communication_Root_CA.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/93bc0acc.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/e48193cf.0       1.2 KiB    Feb 01 2021
                                              /etc/ssl/certs/AffirmTrust_Commercial.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/86212b19.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/2b349938.0       1.2 KiB    Feb 01 2021
                                              /etc/ssl/certs/AffirmTrust_Networking.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/2e5ac55d.0       1.2 KiB    Feb 01 2021
                                                      /etc/ssl/certs/DST_Root_CA_X3.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/12d55845.0       1.2 KiB    Feb 01 2021
                                                    /etc/ssl/certs/Amazon_Root_CA_1.pem       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/ce5e74ef.0       1.2 KiB    Feb 01 2021
                                                              /etc/ssl/certs/fd08c599.0       1.2 KiB    Feb 01 2021
                                                    /etc/ssl/certs/certSIGN_ROOT_CA.pem       1.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/8d86cdd1.0       1.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/882de061.0       1.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/9685a493.0       1.1 KiB    Feb 01 2021
                                             /etc/ssl/certs/Hongkong_Post_Root_CA_1.pem       1.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/3e45d192.0       1.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/9c2e7d30.0       1.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/a7605362.0       1.1 KiB    Feb 01 2021
                                              /etc/ssl/certs/Sonera_Class_2_Root_CA.pem       1.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/106f3e4d.0       1.1 KiB    Feb 01 2021
                          /etc/ssl/certs/Entrust_Root_Certification_Authority_-_EC1.pem       1.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/b3fb433b.0       1.1 KiB    Feb 01 2021
                                                              /etc/ssl/certs/7719f463.0      1017.0 B    Feb 01 2021
                                                              /etc/ssl/certs/9479c8c3.0      1017.0 B    Feb 01 2021
         /etc/ssl/certs/Hellenic_Academic_and_Research_Institutions_ECC_RootCA_2015.pem      1017.0 B    Feb 01 2021
                       /etc/ssl/certs/GeoTrust_Primary_Certification_Authority_-_G2.pem       989.0 B    Feb 01 2021
                                                              /etc/ssl/certs/27af790d.0       989.0 B    Feb 01 2021
                                                              /etc/ssl/certs/116bf586.0       989.0 B    Feb 01 2021
                   /etc/ssl/certs/Trustwave_Global_ECC_P384_Certification_Authority.pem       969.0 B    Feb 01 2021
                                                              /etc/ssl/certs/9aef356c.0       969.0 B    Feb 01 2021
                                                              /etc/ssl/certs/d887a5bb.0       969.0 B    Feb 01 2021
                                                              /etc/ssl/certs/f0c70a8d.0       956.0 B    Feb 01 2021
                         /etc/ssl/certs/SSL.com_EV_Root_Certification_Authority_ECC.pem       956.0 B    Feb 01 2021
                                                              /etc/ssl/certs/7892ad52.0       956.0 B    Feb 01 2021
                                                              /etc/ssl/certs/04f60c28.0       948.0 B    Feb 01 2021
                                                              /etc/ssl/certs/f30dd6ad.0       948.0 B    Feb 01 2021
                               /etc/ssl/certs/USERTrust_ECC_Certification_Authority.pem       948.0 B    Feb 01 2021
                                                              /etc/ssl/certs/88950faa.0       944.0 B    Feb 01 2021
                            /etc/ssl/certs/SSL.com_Root_Certification_Authority_ECC.pem       944.0 B    Feb 01 2021
                                                              /etc/ssl/certs/0bf05006.0       944.0 B    Feb 01 2021
                                  /etc/ssl/certs/COMODO_ECC_Certification_Authority.pem       940.0 B    Feb 01 2021
                                                              /etc/ssl/certs/eed8c118.0       940.0 B    Feb 01 2021
                                                              /etc/ssl/certs/89c02a45.0       940.0 B    Feb 01 2021
                                     /etc/ssl/certs/OISTE_WISeKey_Global_Root_GC_CA.pem       895.0 B    Feb 01 2021
                                                              /etc/ssl/certs/773e07ad.0       895.0 B    Feb 01 2021
                                                              /etc/ssl/certs/3c899c73.0       895.0 B    Feb 01 2021
                                                              /etc/ssl/certs/1ae85e5e.0       883.0 B    Feb 01 2021
                   /etc/ssl/certs/Trustwave_Global_ECC_P256_Certification_Authority.pem       883.0 B    Feb 01 2021
                                                              /etc/ssl/certs/9b5697b0.0       883.0 B    Feb 01 2021
                                                              /etc/ssl/certs/01419da9.0       875.0 B    Feb 01 2021
                                                              /etc/ssl/certs/8d89cda1.0       875.0 B    Feb 01 2021
                       /etc/ssl/certs/Microsoft_ECC_Root_Certificate_Authority_2017.pem       875.0 B    Feb 01 2021
                                                              /etc/ssl/certs/b74d2bd5.0       859.0 B    Feb 01 2021
                                             /etc/ssl/certs/emSign_ECC_Root_CA_-_G3.pem       859.0 B    Feb 01 2021
                                                              /etc/ssl/certs/14bc7599.0       859.0 B    Feb 01 2021
                                         /etc/ssl/certs/DigiCert_Assured_ID_Root_G3.pem       851.0 B    Feb 01 2021
                                                              /etc/ssl/certs/c491639e.0       851.0 B    Feb 01 2021
                                                              /etc/ssl/certs/7f3d5d1d.0       851.0 B    Feb 01 2021
                                               /etc/ssl/certs/e-Szigno_Root_CA_2017.pem       843.0 B    Feb 01 2021
                                                              /etc/ssl/certs/e868b802.0       843.0 B    Feb 01 2021
                                                              /etc/ssl/certs/83e9984f.0       843.0 B    Feb 01 2021
                                             /etc/ssl/certs/DigiCert_Global_Root_G3.pem       839.0 B    Feb 01 2021
                                                              /etc/ssl/certs/dd8e9d41.0       839.0 B    Feb 01 2021
                                                              /etc/ssl/certs/ed39abd0.0       839.0 B    Feb 01 2021
                                             /etc/ssl/certs/emSign_ECC_Root_CA_-_C3.pem       814.0 B    Feb 01 2021
                                                              /etc/ssl/certs/869fbf79.0       814.0 B    Feb 01 2021
                                                              /etc/ssl/certs/4b718d9b.0       814.0 B    Feb 01 2021
                                                              /etc/ssl/certs/1d3472b9.0       794.0 B    Feb 01 2021
                                         /etc/ssl/certs/GlobalSign_ECC_Root_CA_-_R5.pem       794.0 B    Feb 01 2021
                                                              /etc/ssl/certs/2add47b6.0       794.0 B    Feb 01 2021
                                                         /etc/ssl/certs/GTS_Root_R4.pem       769.0 B    Feb 01 2021
                                                              /etc/ssl/certs/5acf816d.0       769.0 B    Feb 01 2021
                                                         /etc/ssl/certs/GTS_Root_R3.pem       769.0 B    Feb 01 2021
                                                              /etc/ssl/certs/6b03dec0.0       769.0 B    Feb 01 2021
                                                              /etc/ssl/certs/a3418fda.0       769.0 B    Feb 01 2021
                                                              /etc/ssl/certs/0a775a30.0       769.0 B    Feb 01 2021
                                                              /etc/ssl/certs/ccc52f49.0       753.0 B    Feb 01 2021
                                                              /etc/ssl/certs/9c8dfbd4.0       753.0 B    Feb 01 2021
                                             /etc/ssl/certs/AffirmTrust_Premium_ECC.pem       753.0 B    Feb 01 2021
                                                    /etc/ssl/certs/Amazon_Root_CA_4.pem       737.0 B    Feb 01 2021
                                                              /etc/ssl/certs/d41b5e2a.0       737.0 B    Feb 01 2021
                                                              /etc/ssl/certs/de6d66f3.0       737.0 B    Feb 01 2021
                                         /etc/ssl/certs/GlobalSign_ECC_Root_CA_-_R4.pem       713.0 B    Feb 01 2021
                                                              /etc/ssl/certs/b0e59380.0       713.0 B    Feb 01 2021
                                                              /etc/ssl/certs/0d69c7e1.0       713.0 B    Feb 01 2021
                                                              /etc/ssl/certs/8cb5ee0f.0       656.0 B    Feb 01 2021
                                                              /etc/ssl/certs/7a7c655d.0       656.0 B    Feb 01 2021
                                                    /etc/ssl/certs/Amazon_Root_CA_3.pem       656.0 B    Feb 01 2021
```
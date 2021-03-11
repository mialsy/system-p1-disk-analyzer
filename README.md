# Project 1: ELastic Array & Disk Analyzer 

**Author**: [Chuxi Wang](https://github.com/mialsy "click here to see my awesome profile, not really awesome but welcome to my space :)") 

**Summmary**: This is the project 1 for CS 521 System Programming. In this project, I implemented an elasic array and a command line utility - ```da```, which helps the user track of the disk space used by each files under the input directory. 

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

The way thatt elastic array works is shown in the following diagram:


On the ohter hand, we can consider using a linkedlist structre, which also support appending and deleting element and has a flexible size. But consider the linkedlist has larger overhead (extra space to store next pointer) and worse locality, while not providing additonal speed advantage, it would not an optimal option. 

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

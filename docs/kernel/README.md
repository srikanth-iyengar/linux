## All about linux kernel

In this section you will find bunch of knowledge specific to linux kernel { Unfortunately when I am preparing this I am using a windows system :( }

Linux kernel is written in **C** but you cannot use **C LIBs** in linux kernel

Kernel memory is not pageable, means that it cannot swap out a lib if the memory gets full, actually this is a hypothetical scenario since if the kernel is that big then you cannot actually boot into the kernel actually it will kernel panic

**So we should keep in mind that every byte you contribute to kernel is one less byte available to you in physical memory**

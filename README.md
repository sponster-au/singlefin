# singlefin
SingleFin Platform for IoT Enablement

## Compile

Add SingleFin C source and header to your build. The distributable contains an example Makefile using the llvm compiler for reference (clang). In the simplest case:

```
clang.exe -o singlefin.o -c singlefin.c
```

## Build

Open the folder build and from the command line type:

```
make clean all
```

The output directory is out and the binary file is placed inside. Deploy to target by connecting to the USB port MODEM and type:

```
make load
```

## Library

A single include header makes easy to integrate in your app.

```
#include "singlefin.h"

int main (void){
	
 printf("Aloha!\r\n");

 for(;;){
  fin_sleep(1000);
 }

}
```

## Configure

To configure the platform you need to run the configure.py script. For now only BG96 platform is supported.

## Prerequisites

Python27

## Documentation

You can view the online documention here: [SingleFin API](http://noomio.com.au/singlefin/)

There is also a local copy in the folder **website/out**

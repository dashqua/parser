# parser

Cparser - C based function that loads and parse data 

### Installing

Make sure to install the latest version of gcc
Here is a summary of all the libraries used in that project, some may be optionnal:
stdlib.h string.h sys/types.h sys/stat.h fcnt1.h unistd.h stdio.h float.h stdarg.h

Just git clone the file, move into it and make it:
```
git clone https://github.com/dashqua/parser.git
cd parser
make
```
You should end with a Cparser file which is the parser binary.

## Running the tests

I provide in the project directory a data file called 'decathlon.txt'.
In the command prompt, type:

```
./Cparser decathlon.txt
```

You should see how the content of the original data set is displayed, and maybe understand a little bit the structure object behind the C code

## What's next ?

Now comes the problem of handling the built object "datum", the next things to do are:
- improve the print functions
- playing with the data on actual algorithm
  The initial goal was to experiment a PCA algorithm on given data from scratch
- make it parallel, optimized
-improve organisation, installation

Anyway the source code is a good way to learn or remember how to treat some complex or uncommon problems of C or programmation in general. 
